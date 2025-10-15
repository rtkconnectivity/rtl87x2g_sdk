#include <string.h>
#include <stdlib.h>
#include "trace.h"
#include "common.h"
#include "private_access.h"
#include "ecp.h"
#include "ecdsa.h"
#include "sha256.h"

static int myrand(void *rng_state, unsigned char *output, size_t len)
{
    size_t i;

    if (rng_state != NULL)
    {
        rng_state  = NULL;
    }

    srand(* ((volatile uint32_t *) 0xe000e018) + len); ////system tick current value
    for (i = 0; i < len; ++i)
    {
        output[i] = rand();
    }

    return (0);
}

void mbedtls_lib_test(void)
{
    DBG_DIRECT("mbedtls_lib_test");
}

const char *ecdsa_test_curve_list[] =
{
    "secp192k1", "secp192r1",
    "secp224k1", "secp224r1",
    "secp256k1", "secp256r1",
    "secp384r1",
    "brainpoolP256r1", "brainpoolP384r1", "brainpoolP512r1"
};

int mbedtls_ecdsa_self_test(void)
{
    int ret = 0;
    mbedtls_ecdsa_context ctx_sign, ctx_verify;
    mbedtls_sha256_context sha256_ctx = {0};
    unsigned char message[100];
    unsigned char hash[32];
    unsigned char sig[MBEDTLS_ECDSA_MAX_LEN];
    size_t sig_len;

    mbedtls_ecdsa_init(&ctx_sign);
    mbedtls_ecdsa_init(&ctx_verify);

    mbedtls_sha256_starts(&sha256_ctx, false);

    memset(sig, 0, sizeof(sig));
    memset(message, 0x25, sizeof(message));

    for (uint8_t i = 0; i < sizeof(ecdsa_test_curve_list) / sizeof(const char *); i++)
    {
        const mbedtls_ecp_curve_info *curve_info = mbedtls_ecp_curve_info_from_name(
                                                       ecdsa_test_curve_list[i]);

        if (curve_info == NULL)
        {
            continue;
        }

        mbedtls_printf("mbedtls_ecp_group_load %d\n", curve_info->grp_id);

        ret = mbedtls_ecp_group_load(&ctx_sign.grp, curve_info->grp_id);
        if (ret != 0)
        {
            mbedtls_printf("mbedtls_ecp_group_load ret: %d\n", ret);
            goto exit;
        }

        /*
         * Generate a key pair for signing
         */
        mbedtls_printf("Generating key pair...");

        ret = mbedtls_ecp_gen_keypair(&ctx_sign.grp, &ctx_sign.d, &ctx_sign.Q, myrand, NULL);
        if (ret != 0)
        {
            mbedtls_printf("mbedtls_ecp_gen_keypair ret: %d\n", ret);
            goto exit;
        }

        mbedtls_printf("Check Public key...");

        ret = mbedtls_ecp_check_pubkey(&ctx_sign.grp, &ctx_sign.Q);
        if (ret != 0)
        {
            mbedtls_printf("mbedtls_ecp_check_pubkey ret: %d\n", ret);
            goto exit;
        }

        mbedtls_printf(" ok (key size: %d bits)\n", (int) ctx_sign.grp.pbits);

        /*
         * Compute message hash
         */
        mbedtls_printf("Computing message hash...");

        ret = mbedtls_sha256_update(&sha256_ctx, message, sizeof(message));
        if (ret != 0)
        {
            mbedtls_printf("mbedtls_sha256_update failed");
        }
        ret = mbedtls_sha256_finish(&sha256_ctx, (unsigned char *)hash);
        if (ret != 0)
        {
            mbedtls_printf("mbedtls_sha256_finish failed");
        }

        mbedtls_printf(" ok\n");

        /*
         * Sign message hash
         */
        mbedtls_printf("Signing message hash...");

        if ((ret = mbedtls_ecdsa_write_signature(&ctx_sign, MBEDTLS_MD_SHA256,
                                                 hash, sizeof(hash),
                                                 sig, sizeof(sig), &sig_len,
                                                 myrand, NULL)) != 0)
        {
            mbedtls_printf(" failed\n  ! mbedtls_ecdsa_write_signature returned %d\n", ret);
            goto exit;
        }
        mbedtls_printf(" ok (signature length = %d)\n", (int) sig_len);

        /*
         * Transfer public information to verifying context
         *
         * We could use the same context for verification and signatures, but we
         * chose to use a new one in order to make it clear that the verifying
         * context only needs the public key (Q), and not the private key (d).
         */
        mbedtls_printf("Preparing verification context...");

        if ((ret = mbedtls_ecp_group_copy(&ctx_verify.grp, &ctx_sign.grp)) != 0)
        {
            mbedtls_printf(" failed\n  ! mbedtls_ecp_group_copy returned %d\n", ret);
            goto exit;
        }

        if ((ret = mbedtls_ecp_copy(&ctx_verify.Q, &ctx_sign.Q)) != 0)
        {
            mbedtls_printf(" failed\n  ! mbedtls_ecp_copy returned %d\n", ret);
            goto exit;
        }

        ret = 0;

        /*
         * Verify signature
         */
        mbedtls_printf(" ok\n  . Verifying signature...");

        if ((ret = mbedtls_ecdsa_read_signature(&ctx_verify,
                                                hash, sizeof(hash),
                                                sig, sig_len)) != 0)
        {
            mbedtls_printf(" failed\n  ! mbedtls_ecdsa_read_signature returned %d\n", ret);
            goto exit;
        }

        mbedtls_printf(" pass\n");
        mbedtls_printf(" \n");
    }

exit:

    mbedtls_ecdsa_free(&ctx_verify);
    mbedtls_ecdsa_free(&ctx_sign);

    return (ret);
}