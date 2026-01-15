/*
 *  FIPS-197 compliant AES implementation
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */
/*
 *  The AES block cipher was designed by Vincent Rijmen and Joan Daemen.
 *
 *  https://csrc.nist.gov/csrc/media/projects/cryptographic-standards-and-guidelines/documents/aes-development/rijndael-ammended.pdf
 *  http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
 */

#include "common.h"

#if defined(MBEDTLS_AES_C)

#include <string.h>

#include "mbedtls/aes.h"
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#if defined(MBEDTLS_AES_USE_HARDWARE_ONLY)
#if !((defined(MBEDTLS_ARCH_IS_ARMV8_A) && defined(MBEDTLS_AESCE_C)) || \
    (defined(MBEDTLS_ARCH_IS_X64)       && defined(MBEDTLS_AESNI_C)) || \
    (defined(MBEDTLS_ARCH_IS_X86)       && defined(MBEDTLS_AESNI_C)))
#error "MBEDTLS_AES_USE_HARDWARE_ONLY defined, but not all prerequisites"
#endif
#endif

#if defined(MBEDTLS_ARCH_IS_X86)
#if defined(MBEDTLS_PADLOCK_C)
#if !defined(MBEDTLS_HAVE_ASM)
#error "MBEDTLS_PADLOCK_C defined, but not all prerequisites"
#endif
#if defined(MBEDTLS_AES_USE_HARDWARE_ONLY)
#error "MBEDTLS_AES_USE_HARDWARE_ONLY cannot be defined when " \
    "MBEDTLS_PADLOCK_C is set"
#endif
#endif
#endif

#if defined(MBEDTLS_PADLOCK_C)
#include "padlock.h"
#endif
#if defined(MBEDTLS_AESNI_C)
#include "aesni.h"
#endif
#if defined(MBEDTLS_AESCE_C)
#include "aesce.h"
#endif

#include "mbedtls/platform.h"
#include "ctr.h"

#include "crypto_hw_locks.h"
#include "crypto_accel_dispatch.h"

/*
 * AES key schedule (encryption)
 */
#if defined(MBEDTLS_AES_SETKEY_ENC_ALT)
int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits)
{
    uint32_t *RK;

    switch (keybits) {
        case 128: ctx->nr = 10; break;
#if !defined(MBEDTLS_AES_ONLY_128_BIT_KEY_LENGTH)
        case 256: ctx->nr = 14; break;
#endif /* !MBEDTLS_AES_ONLY_128_BIT_KEY_LENGTH */
        default: return MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
    }

    ctx->rk_offset = 0;
    RK = ctx->buf + ctx->rk_offset;

    switch (ctx->nr) {
        case 10:
            memcpy(RK, key, 16);
            break;

#if !defined(MBEDTLS_AES_ONLY_128_BIT_KEY_LENGTH)
        case 14:
            memcpy(RK, key, 32);
            break;
#endif /* !MBEDTLS_AES_ONLY_128_BIT_KEY_LENGTH */
    }

    return 0;
}
#endif /* !MBEDTLS_AES_SETKEY_ENC_ALT */

/*
 * AES key schedule (decryption)
 */
#if defined(MBEDTLS_AES_SETKEY_DEC_ALT) || defined(MBEDTLS_BLOCK_CIPHER_NO_DECRYPT)
int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits)
{
    uint32_t *RK;

    switch (keybits) {
        case 128: ctx->nr = 10; break;
#if !defined(MBEDTLS_AES_ONLY_128_BIT_KEY_LENGTH)
        case 256: ctx->nr = 14; break;
#endif /* !MBEDTLS_AES_ONLY_128_BIT_KEY_LENGTH */
        default: return MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
    }

    ctx->rk_offset = 0;
    RK = ctx->buf + ctx->rk_offset;

    switch (ctx->nr) {
        case 10:
            memcpy(RK, key, 16);
            break;

#if !defined(MBEDTLS_AES_ONLY_128_BIT_KEY_LENGTH)
        case 14:
            memcpy(RK, key, 32);
            break;
#endif /* !MBEDTLS_AES_ONLY_128_BIT_KEY_LENGTH */
    }

    return 0;
}
#endif /* !MBEDTLS_AES_SETKEY_DEC_ALT && !MBEDTLS_BLOCK_CIPHER_NO_DECRYPT */

/*
 * AES-ECB block encryption
 */
#if defined(MBEDTLS_AES_ENCRYPT_ALT)
int mbedtls_internal_aes_encrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{
    bool ret;
    int i;
    uint32_t *RK = ctx->buf + ctx->rk_offset;

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&g_crypto_locks.aes) != 0)
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
#endif

    if(ctx->nr == 10)
    {
        ret = crypto_ops->aes.ecb128_encrypt(input, RK, output);
    }
    else if(ctx->nr == 14)
    {
        ret = crypto_ops->aes.ecb256_encrypt(input, RK, output);
    }
    else
    {
        ret = false;
    }

#if defined(MBEDTLS_THREADING_C)
    mbedtls_mutex_unlock(&g_crypto_locks.aes);
#endif

    if(ret)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
#endif /* !MBEDTLS_AES_ENCRYPT_ALT */

/*
 * AES-ECB block decryption
 */
#if defined(MBEDTLS_AES_DECRYPT_ALT) || defined(MBEDTLS_BLOCK_CIPHER_NO_DECRYPT)
int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{
    bool ret;
    int i;
    uint32_t *RK = ctx->buf + ctx->rk_offset;

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&g_crypto_locks.aes) != 0)
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
#endif

    if(ctx->nr == 10)
    {
        ret = crypto_ops->aes.ecb128_decrypt(input, RK, output);
    }
    else if(ctx->nr == 14)
    {
        ret = crypto_ops->aes.ecb256_decrypt(input, RK, output);
    }
    else
    {
        ret = false;
    }

#if defined(MBEDTLS_THREADING_C)
    mbedtls_mutex_unlock(&g_crypto_locks.aes);
#endif

    if(ret)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
#endif /* !MBEDTLS_AES_DECRYPT_ALT && !MBEDTLS_BLOCK_CIPHER_NO_DECRYPT */


#endif /* MBEDTLS_AES_C */
