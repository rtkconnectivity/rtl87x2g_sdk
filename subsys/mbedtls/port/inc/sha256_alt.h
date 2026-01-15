/**
 * \file sha256.h
 *
 * \brief This file contains SHA-224 and SHA-256 definitions and functions.
 *
 * The Secure Hash Algorithms 224 and 256 (SHA-224 and SHA-256) cryptographic
 * hash functions are defined in <em>FIPS 180-4: Secure Hash Standard (SHS)</em>.
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */
#ifndef MBEDTLS_SHA256_ALT_H
#define MBEDTLS_SHA256_ALT_H
#include "mbedtls/private_access.h"

#include "mbedtls/build_info.h"

#include <stddef.h>
#include <stdint.h>

/** SHA-256 input data was malformed. */
#define MBEDTLS_ERR_SHA256_BAD_INPUT_DATA                 -0x0074

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_SHA256_ALT)
// Regular implementation
//

typedef enum
{
    SHA224_ALGO,
    SHA256_ALGO,
    SHA2_ALGO_MAX,
} SHA2_ALGO;

/**
 * \brief          The SHA-256 context structure.
 *
 *                 The structure is used both for SHA-256 and for SHA-224
 *                 checksum calculations. The choice between these two is
 *                 made in the call to mbedtls_sha256_starts().
 */
typedef struct mbedtls_sha256_context {
    uint32_t MBEDTLS_PRIVATE(total)[2];          /*!< The number of Bytes processed.  */
    uint32_t MBEDTLS_PRIVATE(state)[8];          /*!< The intermediate digest state.  */
    unsigned char MBEDTLS_PRIVATE(buffer)[64];   /*!< The data block being processed. */
#if defined(MBEDTLS_SHA224_C)
    int MBEDTLS_PRIVATE(is224);                  /*!< Determines which function to use:
//                                                    0: Use SHA-256, or 1: Use SHA-224. */
#endif
}
mbedtls_sha256_context;

#endif /* MBEDTLS_SHA256_ALT */



#ifdef __cplusplus
}
#endif

#endif /* sha256_alt.h */
