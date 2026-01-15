/*
 *  Elliptic curve DSA
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

/*
 * References:
 *
 * SEC1 https://www.secg.org/sec1-v2.pdf
 */

#include "common.h"

#if defined(MBEDTLS_ECDSA_C)

#if ENABLE_HW_ECC_VERIFY == 1
#include "crypto_accel_dispatch.h"
#include "crypto_hw_locks.h"
#include "ecdsa_alt.h"
#endif

#include "mbedtls/ecdsa.h"
#include "mbedtls/asn1write.h"
#include "bignum_internal.h"

#include <string.h>

#if defined(MBEDTLS_ECDSA_DETERMINISTIC)
#include "mbedtls/hmac_drbg.h"
#endif

#include "mbedtls/platform.h"

#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#if defined(MBEDTLS_ECP_RESTARTABLE)

/*
 * Sub-context for ecdsa_verify()
 */
struct mbedtls_ecdsa_restart_ver {
    mbedtls_mpi u1, u2;     /* intermediate values  */
    enum {                  /* what to do next?     */
        ecdsa_ver_init = 0, /* getting started      */
        ecdsa_ver_muladd,   /* muladd step          */
    } state;
};

/*
 * Sub-context for ecdsa_sign()
 */
struct mbedtls_ecdsa_restart_sig {
    int sign_tries;
    int key_tries;
    mbedtls_mpi k;          /* per-signature random */
    mbedtls_mpi r;          /* r value              */
    enum {                  /* what to do next?     */
        ecdsa_sig_init = 0, /* getting started      */
        ecdsa_sig_mul,      /* doing ecp_mul()      */
        ecdsa_sig_modn,     /* mod N computations   */
    } state;
};

#define ECDSA_RS_ECP    (rs_ctx == NULL ? NULL : &rs_ctx->ecp)

/* Utility macro for checking and updating ops budget */
#define ECDSA_BUDGET(ops)   \
    MBEDTLS_MPI_CHK(mbedtls_ecp_check_budget(grp, ECDSA_RS_ECP, ops));

/* Call this when entering a function that needs its own sub-context */
#define ECDSA_RS_ENTER(SUB)   do {                                 \
        /* reset ops count for this call if top-level */                 \
        if (rs_ctx != NULL && rs_ctx->ecp.depth++ == 0)                 \
        rs_ctx->ecp.ops_done = 0;                                    \
                                                                     \
        /* set up our own sub-context if needed */                       \
        if (mbedtls_ecp_restart_is_enabled() &&                          \
            rs_ctx != NULL && rs_ctx->SUB == NULL)                      \
        {                                                                \
            rs_ctx->SUB = mbedtls_calloc(1, sizeof(*rs_ctx->SUB));   \
            if (rs_ctx->SUB == NULL)                                    \
            return MBEDTLS_ERR_ECP_ALLOC_FAILED;                  \
                                                                   \
            ecdsa_restart_## SUB ##_init(rs_ctx->SUB);                 \
        }                                                                \
} while (0)

/* Call this when leaving a function that needs its own sub-context */
#define ECDSA_RS_LEAVE(SUB)   do {                                 \
        /* clear our sub-context when not in progress (done or error) */ \
        if (rs_ctx != NULL && rs_ctx->SUB != NULL &&                     \
            ret != MBEDTLS_ERR_ECP_IN_PROGRESS)                         \
        {                                                                \
            ecdsa_restart_## SUB ##_free(rs_ctx->SUB);                 \
            mbedtls_free(rs_ctx->SUB);                                 \
            rs_ctx->SUB = NULL;                                          \
        }                                                                \
                                                                     \
        if (rs_ctx != NULL)                                             \
        rs_ctx->ecp.depth--;                                         \
} while (0)

#else /* MBEDTLS_ECP_RESTARTABLE */

#define ECDSA_RS_ECP    NULL

#define ECDSA_BUDGET(ops)     /* no-op; for compatibility */

#define ECDSA_RS_ENTER(SUB)   (void) rs_ctx
#define ECDSA_RS_LEAVE(SUB)   (void) rs_ctx

#endif /* MBEDTLS_ECP_RESTARTABLE */

/*
 * Derive a suitable integer for group grp from a buffer of length len
 * SEC1 4.1.3 step 5 aka SEC1 4.1.4 step 3
 */
static int derive_mpi(const mbedtls_ecp_group *grp, mbedtls_mpi *x,
                      const unsigned char *buf, size_t blen)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t n_size = (grp->nbits + 7) / 8;
    size_t use_size = blen > n_size ? n_size : blen;

    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(x, buf, use_size));
    if (use_size * 8 > grp->nbits) {
        MBEDTLS_MPI_CHK(mbedtls_mpi_shift_r(x, use_size * 8 - grp->nbits));
    }

    /* While at it, reduce modulo N */
    if (mbedtls_mpi_cmp_mpi(x, &grp->N) >= 0) {
        MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mpi(x, x, &grp->N));
    }

cleanup:
    return ret;
}

#if defined(MBEDTLS_ECDSA_SIGN_ALT)
/*
 * Compute ECDSA signature of a hashed message (SEC1 4.1.3)
 * Obviously, compared to SEC1 4.1.3, we skip step 4 (hash message)
 */
int mbedtls_ecdsa_sign_restartable(mbedtls_ecp_group *grp,
                                   mbedtls_mpi *r, mbedtls_mpi *s,
                                   const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
                                   int (*f_rng)(void *, unsigned char *, size_t), void *p_rng,
                                   int (*f_rng_blind)(void *, unsigned char *, size_t),
                                   void *p_rng_blind,
                                   mbedtls_ecdsa_restart_ctx *rs_ctx)
{
    int ret, key_tries, sign_tries;
    int *p_sign_tries = &sign_tries, *p_key_tries = &key_tries;
    mbedtls_ecp_point R;
    mbedtls_mpi k, e;
    mbedtls_mpi *pk = &k, *pr = r;

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if (!mbedtls_ecdsa_can_do(grp->id) || grp->N.p == NULL) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    /* Make sure d is in range 1..n-1 */
    if (mbedtls_mpi_cmp_int(d, 1) < 0 || mbedtls_mpi_cmp_mpi(d, &grp->N) >= 0) {
        return MBEDTLS_ERR_ECP_INVALID_KEY;
    }

    mbedtls_ecp_point_init(&R);
    mbedtls_mpi_init(&k); mbedtls_mpi_init(&e);

    ECDSA_RS_ENTER(sig);

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if (rs_ctx != NULL && rs_ctx->sig != NULL) {
        /* redirect to our context */
        p_sign_tries = &rs_ctx->sig->sign_tries;
        p_key_tries = &rs_ctx->sig->key_tries;
        pk = &rs_ctx->sig->k;
        pr = &rs_ctx->sig->r;

        /* jump to current step */
        if (rs_ctx->sig->state == ecdsa_sig_mul) {
            goto mul;
        }
        if (rs_ctx->sig->state == ecdsa_sig_modn) {
            goto modn;
        }
    }
#endif /* MBEDTLS_ECP_RESTARTABLE */

    *p_sign_tries = 0;
    do {
        if ((*p_sign_tries)++ > 10) {
            ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
            goto cleanup;
        }

        /*
         * Steps 1-3: generate a suitable ephemeral keypair
         * and set r = xR mod n
         */
        *p_key_tries = 0;
        do {
            if ((*p_key_tries)++ > 10) {
                ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
                goto cleanup;
            }

            MBEDTLS_MPI_CHK(mbedtls_ecp_gen_privkey(grp, pk, f_rng, p_rng));

#if defined(MBEDTLS_ECP_RESTARTABLE)
            if (rs_ctx != NULL && rs_ctx->sig != NULL) {
                rs_ctx->sig->state = ecdsa_sig_mul;
            }

mul:
#endif
            MBEDTLS_MPI_CHK(mbedtls_ecp_mul_restartable(grp, &R, pk, &grp->G,
                                                        f_rng_blind,
                                                        p_rng_blind,
                                                        ECDSA_RS_ECP));
#if ENABLE_HW_ECC_VERIFY == 1

#if defined(MBEDTLS_THREADING_C)
            mbedtls_mutex_lock( &g_crypto_locks.pke );
#endif

            crypto_ops->pke.ecp_init(mbedtls_mpi_bitlen(&grp->N), ECC_PRIME_MODE, GO_TO_END_LOOP, !RR_MOD_N_READY);

            crypto_ops->pke.ecp_set_sub_operand(ECC_N_ADDR, grp->N.p, mbedtls_mpi_size(&grp->N));

            mbedtls_mpi_grow(pr, grp->N.n);
            MBEDTLS_MPI_CHK(crypto_ops->pke.ecp_compute(pr->p, ECC_X_RESULT_ADDR, RTK_PUBKEY_X_MOD_N_ENTRY));

            crypto_ops->pke.ecp_set_sub_operand(ECC_N_ADDR, grp->N.p, mbedtls_mpi_size(&grp->N));

#if defined(MBEDTLS_THREADING_C)
            mbedtls_mutex_unlock( &g_crypto_locks.pke );
#endif

#else
            MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(pr, &R.X, &grp->N));
#endif
        } while (mbedtls_mpi_cmp_int(pr, 0) == 0);

#if defined(MBEDTLS_ECP_RESTARTABLE)
        if (rs_ctx != NULL && rs_ctx->sig != NULL) {
            rs_ctx->sig->state = ecdsa_sig_modn;
        }

modn:
#endif
        /*
         * Accounting for everything up to the end of the loop
         * (step 6, but checking now avoids saving e and t)
         */
        ECDSA_BUDGET(MBEDTLS_ECP_OPS_INV + 4);

        /*
         * Step 5: derive MPI from hashed message
         */
        MBEDTLS_MPI_CHK(derive_mpi(grp, &e, buf, blen));
#if ENABLE_HW_ECC_VERIFY == 1

#if defined(MBEDTLS_THREADING_C)
    mbedtls_mutex_lock( &g_crypto_locks.pke );
#endif

        uint32_t temp[16];

        // precomute R^2 mod N and N^-1 mod R
        crypto_ops->pke.ecp_init(mbedtls_mpi_bitlen(&grp->N), ECC_PRIME_MODE, GO_TO_END_LOOP, !RR_MOD_N_READY);
        MBEDTLS_MPI_CHK(crypto_ops->pke.ecp_compute(temp, (ECC_X_RESULT_ADDR), RTK_PUBKEY_N_INV_ENTRY));

        crypto_ops->pke.ecp_init(mbedtls_mpi_bitlen(&grp->N), ECC_PRIME_MODE, GO_TO_END_LOOP, !RR_MOD_N_READY);
        MBEDTLS_MPI_CHK(crypto_ops->pke.ecp_compute(temp, (ECC_X_RESULT_ADDR), RTK_PUBKEY_R_SQAR_ENTRY));

        // (r * da) mod N
        crypto_ops->pke.ecp_init(mbedtls_mpi_bitlen(&grp->N), ECC_PRIME_MODE, GO_TO_END_LOOP, RR_MOD_N_READY);

        crypto_ops->pke.ecp_set_sub_operand((ECC_CURVE_A_ADDR), pr->p, mbedtls_mpi_size(pr));
        crypto_ops->pke.ecp_set_sub_operand((ECC_CURVE_B_ADDR), d->p, mbedtls_mpi_size(d));

        MBEDTLS_MPI_CHK(crypto_ops->pke.ecp_compute(temp, (ECC_CURVE_A_ADDR), RTK_PUBKEY_MOD_MUL_ENTRY));

        // e + (r * da) mod N
        crypto_ops->pke.ecp_init(mbedtls_mpi_bitlen(&grp->N), ECC_PRIME_MODE, GO_TO_END_LOOP, RR_MOD_N_READY);

        crypto_ops->pke.ecp_set_sub_operand((ECC_CURVE_B_ADDR), e.p, mbedtls_mpi_size(&e));

        MBEDTLS_MPI_CHK(crypto_ops->pke.ecp_compute(temp, (ECC_CURVE_A_ADDR), RTK_PUBKEY_MOD_ADD_ENTRY));

        // k^-1 mod N
        crypto_ops->pke.ecp_init(mbedtls_mpi_bitlen(&grp->N), ECC_PRIME_MODE, GO_TO_END_LOOP, RR_MOD_N_READY);

        crypto_ops->pke.ecp_set_sub_operand((ECC_CURVE_B_ADDR), pk->p, mbedtls_mpi_size(pk));

        MBEDTLS_MPI_CHK(crypto_ops->pke.ecp_compute(temp, (ECC_CURVE_B_ADDR), RTK_PUBKEY_K_INV_ENTRY));

        // s = (k^-1) * (z + (r * da)) mod N
        crypto_ops->pke.ecp_init(mbedtls_mpi_bitlen(&grp->N), ECC_PRIME_MODE, GO_TO_END_LOOP, RR_MOD_N_READY);

        mbedtls_mpi_grow(s, grp->N.n);
        MBEDTLS_MPI_CHK(crypto_ops->pke.ecp_compute(s->p, (ECC_CURVE_A_ADDR), RTK_PUBKEY_MOD_MUL_ENTRY));

#if defined(MBEDTLS_THREADING_C)
    mbedtls_mutex_unlock( &g_crypto_locks.pke );
#endif

#else
        /*
         * Step 6: compute s = (e + r * d) / k
         */
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(s, pr, d));
        MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(&e, &e, s));
        MBEDTLS_MPI_CHK(mbedtls_mpi_gcd_modinv_odd(NULL, s, pk, &grp->N));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(s, s, &e));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(s, s, &grp->N));
#endif
    } while (mbedtls_mpi_cmp_int(s, 0) == 0);

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if (rs_ctx != NULL && rs_ctx->sig != NULL) {
        MBEDTLS_MPI_CHK(mbedtls_mpi_copy(r, pr));
    }
#endif

cleanup:
    mbedtls_ecp_point_free(&R);
    mbedtls_mpi_free(&k); mbedtls_mpi_free(&e);

    ECDSA_RS_LEAVE(sig);

#if ENABLE_HW_ECC_VERIFY == 1
#if defined(MBEDTLS_THREADING_C)
    mbedtls_mutex_unlock( &g_crypto_locks.pke  );
#endif
#endif

    return ret;
}

/*
 * Compute ECDSA signature of a hashed message
 */
int mbedtls_ecdsa_sign(mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
                       const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
                       int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
    /* Use the same RNG for both blinding and ephemeral key generation */
    return mbedtls_ecdsa_sign_restartable(grp, r, s, d, buf, blen,
                                          f_rng, p_rng, f_rng, p_rng, NULL);
}
#endif /* MBEDTLS_ECDSA_SIGN_ALT */

#if defined(MBEDTLS_ECDSA_VERIFY_ALT)
/*
 * Verify ECDSA signature of hashed message (SEC1 4.1.4)
 * Obviously, compared to SEC1 4.1.3, we skip step 2 (hash message)
 */
int mbedtls_ecdsa_verify_restartable(mbedtls_ecp_group *grp,
                                     const unsigned char *buf, size_t blen,
                                     const mbedtls_ecp_point *Q,
                                     const mbedtls_mpi *r,
                                     const mbedtls_mpi *s,
                                     mbedtls_ecdsa_restart_ctx *rs_ctx)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_mpi e, s_inv, u1, u2;
    mbedtls_ecp_point R;
    mbedtls_mpi *pu1 = &u1, *pu2 = &u2;

    mbedtls_ecp_point_init(&R);
    mbedtls_mpi_init(&e); mbedtls_mpi_init(&s_inv);
    mbedtls_mpi_init(&u1); mbedtls_mpi_init(&u2);

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if (!mbedtls_ecdsa_can_do(grp->id) || grp->N.p == NULL) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    ECDSA_RS_ENTER(ver);

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if (rs_ctx != NULL && rs_ctx->ver != NULL) {
        /* redirect to our context */
        pu1 = &rs_ctx->ver->u1;
        pu2 = &rs_ctx->ver->u2;

        /* jump to current step */
        if (rs_ctx->ver->state == ecdsa_ver_muladd) {
            goto muladd;
        }
    }
#endif /* MBEDTLS_ECP_RESTARTABLE */

    /*
     * Step 1: make sure r and s are in range 1..n-1
     */
    if (mbedtls_mpi_cmp_int(r, 1) < 0 || mbedtls_mpi_cmp_mpi(r, &grp->N) >= 0 ||
        mbedtls_mpi_cmp_int(s, 1) < 0 || mbedtls_mpi_cmp_mpi(s, &grp->N) >= 0) {
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
        goto cleanup;
    }

    /*
     * Step 3: derive MPI from hashed message
     */
    MBEDTLS_MPI_CHK(derive_mpi(grp, &e, buf, blen));

    /*
     * Step 4: u1 = e / s mod n, u2 = r / s mod n
     */
#if ENABLE_HW_ECC_VERIFY == 1

#if defined(MBEDTLS_THREADING_C)
    mbedtls_mutex_lock( &g_crypto_locks.pke );
#endif

    uint32_t temp[16];

    // precomute R^2 mod N and N^-1 mod R
    crypto_ops->pke.ecp_init(mbedtls_mpi_bitlen(&grp->N), ECC_PRIME_MODE, GO_TO_END_LOOP, !RR_MOD_N_READY);
    crypto_ops->pke.ecp_set_sub_operand((ECC_N_ADDR), grp->N.p, mbedtls_mpi_size(&grp->N));
    MBEDTLS_MPI_CHK(crypto_ops->pke.ecp_compute(temp, (ECC_X_RESULT_ADDR), RTK_PUBKEY_N_INV_ENTRY));

    crypto_ops->pke.ecp_init(mbedtls_mpi_bitlen(&grp->N), ECC_PRIME_MODE, GO_TO_END_LOOP, !RR_MOD_N_READY);
    MBEDTLS_MPI_CHK(crypto_ops->pke.ecp_compute(temp, (ECC_X_RESULT_ADDR), RTK_PUBKEY_R_SQAR_ENTRY));

    // s^-1 mod N
    crypto_ops->pke.ecp_init(mbedtls_mpi_bitlen(&grp->N), ECC_PRIME_MODE, GO_TO_END_LOOP, RR_MOD_N_READY);

    crypto_ops->pke.ecp_set_sub_operand((ECC_CURVE_B_ADDR), s->p, mbedtls_mpi_size(s));

    mbedtls_mpi_grow(&s_inv, grp->N.n);
    MBEDTLS_MPI_CHK(crypto_ops->pke.ecp_compute(s_inv.p, (ECC_CURVE_B_ADDR), RTK_PUBKEY_K_INV_ENTRY));

    // u1 = s^-1 * z mod N
    crypto_ops->pke.ecp_init(mbedtls_mpi_bitlen(&grp->N), ECC_PRIME_MODE, GO_TO_END_LOOP, RR_MOD_N_READY);

    crypto_ops->pke.ecp_set_sub_operand((ECC_CURVE_A_ADDR), e.p, mbedtls_mpi_size(&e));

    mbedtls_mpi_grow(pu1, grp->N.n);
    MBEDTLS_MPI_CHK(crypto_ops->pke.ecp_compute(pu1->p, (ECC_CURVE_A_ADDR), RTK_PUBKEY_MOD_MUL_ENTRY));

    // u2 = s^-1 * r mod N
    crypto_ops->pke.ecp_init(mbedtls_mpi_bitlen(&grp->N), ECC_PRIME_MODE, GO_TO_END_LOOP, RR_MOD_N_READY);

    crypto_ops->pke.ecp_set_sub_operand((ECC_CURVE_A_ADDR), r->p, mbedtls_mpi_size(r));

    mbedtls_mpi_grow(pu2, grp->N.n);
    MBEDTLS_MPI_CHK(crypto_ops->pke.ecp_compute(pu2->p, (ECC_CURVE_A_ADDR), RTK_PUBKEY_MOD_MUL_ENTRY));

#if defined(MBEDTLS_THREADING_C)
    mbedtls_mutex_unlock( &g_crypto_locks.pke );
#endif

#else
    ECDSA_BUDGET(MBEDTLS_ECP_OPS_CHK + MBEDTLS_ECP_OPS_INV + 2);

    MBEDTLS_MPI_CHK(mbedtls_mpi_gcd_modinv_odd(NULL, &s_inv, s, &grp->N));

    MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(pu1, &e, &s_inv));
    MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(pu1, pu1, &grp->N));

    MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(pu2, r, &s_inv));
    MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(pu2, pu2, &grp->N));
#endif

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if (rs_ctx != NULL && rs_ctx->ver != NULL) {
        rs_ctx->ver->state = ecdsa_ver_muladd;
    }

muladd:
#endif
    /*
     * Step 5: R = u1 G + u2 Q
     */
    MBEDTLS_MPI_CHK(mbedtls_ecp_muladd_restartable(grp,
                                                   &R, pu1, &grp->G, pu2, Q, ECDSA_RS_ECP));

    if (mbedtls_ecp_is_zero(&R)) {
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
        goto cleanup;
    }

    /*
     * Step 6: convert xR to an integer (no-op)
     * Step 7: reduce xR mod n (gives v)
     */
#if ENABLE_HW_ECC_VERIFY == 1

#if defined(MBEDTLS_THREADING_C)
    mbedtls_mutex_lock( &g_crypto_locks.pke );
#endif

    crypto_ops->pke.ecp_init(mbedtls_mpi_bitlen(&grp->N), ECC_PRIME_MODE, GO_TO_END_LOOP, !RR_MOD_N_READY);

    crypto_ops->pke.ecp_set_sub_operand((ECC_N_ADDR), grp->N.p, mbedtls_mpi_size(&grp->N));
    crypto_ops->pke.ecp_set_sub_operand((ECC_X_RESULT_ADDR), R.X.p, mbedtls_mpi_size(&R.X));

    MBEDTLS_MPI_CHK(crypto_ops->pke.ecp_compute(R.X.p, (ECC_X_RESULT_ADDR), RTK_PUBKEY_X_MOD_N_ENTRY));

    crypto_ops->pke.ecp_set_sub_operand(ECC_N_ADDR, grp->N.p, mbedtls_mpi_size(&grp->N));

#if defined(MBEDTLS_THREADING_C)
    mbedtls_mutex_unlock( &g_crypto_locks.pke );
#endif

#else
    MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&R.X, &R.X, &grp->N));
#endif

    /*
     * Step 8: check if v (that is, R.X) is equal to r
     */
    if (mbedtls_mpi_cmp_mpi(&R.X, r) != 0) {
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
        goto cleanup;
    }

cleanup:
    mbedtls_ecp_point_free(&R);
    mbedtls_mpi_free(&e); mbedtls_mpi_free(&s_inv);
    mbedtls_mpi_free(&u1); mbedtls_mpi_free(&u2);

    ECDSA_RS_LEAVE(ver);

#if ENABLE_HW_ECC_VERIFY == 1
#if defined(MBEDTLS_THREADING_C)
    mbedtls_mutex_unlock( &g_crypto_locks.pke  );
#endif
#endif

    return ret;
}

/*
 * Verify ECDSA signature of hashed message
 */
int mbedtls_ecdsa_verify(mbedtls_ecp_group *grp,
                         const unsigned char *buf, size_t blen,
                         const mbedtls_ecp_point *Q,
                         const mbedtls_mpi *r,
                         const mbedtls_mpi *s)
{
    return mbedtls_ecdsa_verify_restartable(grp, buf, blen, Q, r, s, NULL);
}
#endif /* MBEDTLS_ECDSA_VERIFY_ALT */

#endif /* MBEDTLS_ECDSA_C */
