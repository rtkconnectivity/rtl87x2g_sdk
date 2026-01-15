/*
 *  FIPS-180-2 compliant SHA-256 implementation
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */
/*
 *  The SHA-256 Secure Hash Standard was published by NIST in 2002.
 *
 *  http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
 */


#include "common.h"

#if defined(MBEDTLS_SHA256_C) || defined(MBEDTLS_SHA224_C)

#include "mbedtls/sha256.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#include <string.h>

#include "mbedtls/platform.h"

#include "crypto_hw_locks.h"
#include "crypto_accel_dispatch.h"

#if defined(MBEDTLS_SHA256_ALT)

void mbedtls_sha256_init(mbedtls_sha256_context *ctx)
{
    memset(ctx, 0, sizeof(mbedtls_sha256_context));

    crypto_ops->sha256.init();
}

void mbedtls_sha256_free(mbedtls_sha256_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    mbedtls_platform_zeroize(ctx, sizeof(mbedtls_sha256_context));
}

void mbedtls_sha256_clone(mbedtls_sha256_context *dst,
                          const mbedtls_sha256_context *src)
{
    *dst = *src;
}

/*
 * SHA-256 context setup
 */
int mbedtls_sha256_starts(mbedtls_sha256_context *ctx, int is224)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

#if defined(MBEDTLS_SHA224_C) && defined(MBEDTLS_SHA256_C)
    if (is224 != 0 && is224 != 1) {
        return MBEDTLS_ERR_SHA256_BAD_INPUT_DATA;
    }
#elif defined(MBEDTLS_SHA256_C)
    if (is224 != 0) {
        return MBEDTLS_ERR_SHA256_BAD_INPUT_DATA;
    }
#else /* defined MBEDTLS_SHA224_C only */
    if (is224 == 0) {
        return MBEDTLS_ERR_SHA256_BAD_INPUT_DATA;
    }
#endif

#if defined(MBEDTLS_SHA224_C)
    ctx->is224 = is224;
#endif
    
    ret = crypto_ops->sha256.start(ctx, is224);

    return ret;
}


/*
 * SHA-256 process buffer
 */
int mbedtls_sha256_update(mbedtls_sha256_context *ctx,
                          const unsigned char *input,
                          size_t ilen)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    if (ilen == 0) {
        return 0;
    }

    ret = crypto_ops->sha256.update(ctx, input, ilen);

    return ret;
}

/*
 * SHA-256 final digest
 */
int mbedtls_sha256_finish(mbedtls_sha256_context *ctx,
                          unsigned char *output)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    ret = crypto_ops->sha256.finish(ctx, output);

    return ret;
}

#endif /* !MBEDTLS_SHA256_ALT */



#endif /* MBEDTLS_SHA256_C || MBEDTLS_SHA224_C */
