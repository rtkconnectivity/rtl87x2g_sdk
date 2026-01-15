/**
 * \file ecdsa.h
 *
 * \brief This file contains ECDSA definitions and functions.
 *
 * The Elliptic Curve Digital Signature Algorithm (ECDSA) is defined in
 * <em>Standards for Efficient Cryptography Group (SECG):
 * SEC1 Elliptic Curve Cryptography</em>.
 * The use of ECDSA for TLS is defined in <em>RFC-4492: Elliptic Curve
 * Cryptography (ECC) Cipher Suites for Transport Layer Security (TLS)</em>.
 *
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

#ifndef MBEDTLS_ECDSA_ALT_H
#define MBEDTLS_ECDSA_ALT_H
#include "mbedtls/private_access.h"

#include "mbedtls/build_info.h"

#include "mbedtls/ecp.h"
#include "mbedtls/md.h"
#include "mbedtls/ecdsa.h"

#ifdef __cplusplus
extern "C" {
#endif


#if defined(MBEDTLS_ECDSA_SIGN_ALT)
/**
 * \brief               This function computes the ECDSA signature of a
 *                      previously-hashed message, in a restartable way.
 *
 * \note                The deterministic version implemented in
 *                      mbedtls_ecdsa_sign_det_restartable() is usually
 *                      preferred.
 *
 * \note                This function is like \c mbedtls_ecdsa_sign() but
 *                      it can return early and restart according to the
 *                      limit set with \c mbedtls_ecp_set_max_ops() to
 *                      reduce blocking.
 *
 * \note                If the bitlength of the message hash is larger
 *                      than the bitlength of the group order, then the
 *                      hash is truncated as defined in <em>Standards for
 *                      Efficient Cryptography Group (SECG): SEC1 Elliptic
 *                      Curve Cryptography</em>, section 4.1.3, step 5.
 *
 * \see                 ecp.h
 *
 * \param grp           The context for the elliptic curve to use.
 *                      This must be initialized and have group parameters
 *                      set, for example through mbedtls_ecp_group_load().
 * \param r             The MPI context in which to store the first part
 *                      the signature. This must be initialized.
 * \param s             The MPI context in which to store the second part
 *                      the signature. This must be initialized.
 * \param d             The private signing key. This must be initialized
 *                      and setup, for example through
 *                      mbedtls_ecp_gen_privkey().
 * \param buf           The hashed content to be signed. This must be a readable
 *                      buffer of length \p blen Bytes. It may be \c NULL if
 *                      \p blen is zero.
 * \param blen          The length of \p buf in Bytes.
 * \param f_rng         The RNG function used to generate the ECDSA nonce.
 *                      This must not be \c NULL.
 * \param p_rng         The RNG context to be passed to \p f_rng. This may be
 *                      \c NULL if \p f_rng doesn't need a context parameter.
 * \param f_rng_blind   The RNG function used for blinding. This must not be
 *                      \c NULL.
 * \param p_rng_blind   The RNG context to be passed to \p f_rng. This may be
 *                      \c NULL if \p f_rng doesn't need a context parameter.
 * \param rs_ctx        The restart context to use. This may be \c NULL
 *                      to disable restarting. If it is not \c NULL, it
 *                      must point to an initialized restart context.
 *
 * \return              \c 0 on success.
 * \return              #MBEDTLS_ERR_ECP_IN_PROGRESS if maximum number of
 *                      operations was reached: see \c
 *                      mbedtls_ecp_set_max_ops().
 * \return              Another \c MBEDTLS_ERR_ECP_XXX, \c
 *                      MBEDTLS_ERR_MPI_XXX or \c MBEDTLS_ERR_ASN1_XXX
 *                      error code on failure.
 */
int mbedtls_ecdsa_sign_restartable(
    mbedtls_ecp_group *grp,
    mbedtls_mpi *r, mbedtls_mpi *s,
    const mbedtls_mpi *d,
    const unsigned char *buf, size_t blen,
    mbedtls_f_rng_t *f_rng,
    void *p_rng,
    mbedtls_f_rng_t *f_rng_blind,
    void *p_rng_blind,
    mbedtls_ecdsa_restart_ctx *rs_ctx);

#endif /* !MBEDTLS_ECDSA_SIGN_ALT */


#if defined(MBEDTLS_ECDSA_VERIFY_ALT)
/**
 * \brief           This function verifies the ECDSA signature of a
 *                  previously-hashed message, in a restartable manner
 *
 * \note            If the bitlength of the message hash is larger than the
 *                  bitlength of the group order, then the hash is truncated as
 *                  defined in <em>Standards for Efficient Cryptography Group
 *                  (SECG): SEC1 Elliptic Curve Cryptography</em>, section
 *                  4.1.4, step 3.
 *
 * \see             ecp.h
 *
 * \param grp       The ECP group to use.
 *                  This must be initialized and have group parameters
 *                  set, for example through mbedtls_ecp_group_load().
 * \param buf       The hashed content that was signed. This must be a readable
 *                  buffer of length \p blen Bytes. It may be \c NULL if
 *                  \p blen is zero.
 * \param blen      The length of \p buf in Bytes.
 * \param Q         The public key to use for verification. This must be
 *                  initialized and setup.
 * \param r         The first integer of the signature.
 *                  This must be initialized.
 * \param s         The second integer of the signature.
 *                  This must be initialized.
 * \param rs_ctx    The restart context to use. This may be \c NULL to disable
 *                  restarting. If it is not \c NULL, it must point to an
 *                  initialized restart context.
 *
 * \return          \c 0 on success.
 * \return          #MBEDTLS_ERR_ECP_IN_PROGRESS if maximum number of
 *                  operations was reached: see \c mbedtls_ecp_set_max_ops().
 * \return          An \c MBEDTLS_ERR_ECP_XXX or \c MBEDTLS_MPI_XXX
 *                  error code on failure.
 */
int mbedtls_ecdsa_verify_restartable(mbedtls_ecp_group *grp,
                                     const unsigned char *buf, size_t blen,
                                     const mbedtls_ecp_point *Q,
                                     const mbedtls_mpi *r,
                                     const mbedtls_mpi *s,
                                     mbedtls_ecdsa_restart_ctx *rs_ctx);

#endif /* !MBEDTLS_ECDSA_VERIFY_ALT */

#ifdef __cplusplus
}
#endif

#endif /* ecdsa_alt.h */
