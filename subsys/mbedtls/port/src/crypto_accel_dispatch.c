#include "crypto_accel_dispatch.h"
#include <string.h>

extern void hw_pke_clock(bool enable);
extern void hw_pke_init(bool byte_swap_en, bool word_swap_en, uint32_t word_swap_base);
extern void hw_ecc_init(uint32_t key_bits, PKE_MODE mode, bool go_to_end_loop, bool RR_mod_n_ready);
extern void hw_ecc_set_sub_operand(uint32_t operand_addr, uint32_t *operands, uint32_t byte_len);
extern bool hw_ecc_set_all_operands(ECC_GROUP *grp, uint32_t *e, uint32_t e_byte_size);
extern ERR_CODE hw_ecc_compute(void *result, uint32_t output_addr, uint16_t func_id);

//extern void hw_sha256_init(void);
//extern void hw_sha256_start(HW_SHA256_CTX *ctx, uint32_t *iv);
//extern bool hw_sha256_cpu_update(HW_SHA256_CTX *ctx, uint8_t *input, uint32_t byte_len);
//extern bool hw_sha256_finish(HW_SHA256_CTX *ctx, uint32_t *result);

//extern bool aes128_ecb_encrypt(uint8_t *plaintext, const uint8_t *key, uint8_t *ciphertext);
//extern bool aes128_ecb_decrypt(uint8_t *ciphertext, const uint8_t *key, uint8_t *plaintext);
//extern bool aes256_ecb_encrypt(uint8_t *plaintext, const uint8_t *key, uint8_t *ciphertext);
//extern bool aes256_ecb_decrypt(uint8_t *ciphertext, const uint8_t *key, uint8_t *plaintext);

//extern bool get_mbedtls_rsa_private_key_blinding_en(void);
//extern void hw_rsa_init(uint32_t key_bits);
//extern void hw_rsa_set_sub_operand(uint32_t operand_addr, uint32_t *operands, uint32_t byte_len);
//extern bool hw_rsa_set_all_operands(uint32_t *M, uint32_t *e, uint32_t *N, uint32_t exp_byte_size);
//extern ERR_CODE hw_rsa_compute(uint32_t *result, uint32_t output_addr, uint16_t func_id);

static bool aes128_ecb_encrypt_internal(const uint8_t input[16], const uint32_t key[4], uint8_t output[16])
{
    bool ret;

    ret = aes128_ecb_encrypt((uint8_t *)input, (const uint8_t *)key, output);

    return ret;
}

static bool aes128_ecb_decrypt_internal(const uint8_t input[16], const uint32_t key[4], uint8_t output[16])
{
    bool ret;

    ret = aes128_ecb_decrypt((uint8_t *)input, (const uint8_t *)key, output);

    return ret;
}

static bool aes256_ecb_encrypt_internal(const uint8_t input[16], const uint32_t key[8], uint8_t output[16])
{
    bool ret;

    uint8_t key_copy[32] = {0};
    memcpy(key_copy, key + 16, 16);
    memcpy(key_copy + 16, key, 16);

    ret = aes256_ecb_encrypt((uint8_t *)input, (const uint8_t *)key, output);

    return ret;
}

static bool aes256_ecb_decrypt_internal(const uint8_t input[16], const uint32_t key[8], uint8_t output[16])
{
    bool ret;

    uint8_t key_copy[32] = {0};
    memcpy(key_copy, key + 16, 16);
    memcpy(key_copy + 16, key, 16);

    ret = aes256_ecb_decrypt((uint8_t *)input, (const uint8_t *)key, output);

    return ret;
}

static void sha256_init()
{
    hw_sha256_init();
}

static int sha256_starts(void *ctx, int is224)
{
    if (ctx == NULL)
    {
        return -1;
    }

    HW_SHA256_CTX *sha2_ctx = (HW_SHA256_CTX *)ctx;

    sha2_ctx->is224 = is224;
    hw_sha256_start((HW_SHA256_CTX *)sha2_ctx, NULL);


    return 0;
}

static int sha256_update(void *ctx, const unsigned char *input, size_t ilen)
{
    if (ctx == NULL || input == NULL)
    {
        return -1;
    }

    bool ret = hw_sha256_cpu_update((HW_SHA256_CTX *)ctx, (uint8_t *)input, ilen);

    if(ret)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

static int sha256_finish(void *ctx, unsigned char *output)
{
    if (ctx == NULL || output == NULL)
    {
        return -1;
    }

    bool ret = hw_sha256_finish((HW_SHA256_CTX *)ctx, (uint32_t *)output);

    if(ret)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

static const crypto_hw_ops_t hw_ops = {
    .trng = {
        .rng_get32          = platform_random,
    },
    .pke = {
        .set_clock          = hw_pke_clock,
        .init               = hw_pke_init,
        .ecp_init           = hw_ecc_init, 
        .ecp_set_sub_operand= hw_ecc_set_sub_operand,
        .ecp_set_all_operands= hw_ecc_set_all_operands,
        .ecp_compute        = hw_ecc_compute,
    },
    .sha256 = {
        .init   = sha256_init,
        .start  = sha256_starts,
        .update = sha256_update,
        .finish = sha256_finish,
    },
    .aes = {
        .ecb128_encrypt = aes128_ecb_encrypt_internal,
        .ecb128_decrypt = aes128_ecb_decrypt_internal,
        .ecb256_encrypt = aes256_ecb_encrypt_internal,
        .ecb256_decrypt = aes256_ecb_decrypt_internal,
    },
//    .rsa = {
//        .get_rsa_private_key_blinding_en = get_mbedtls_rsa_private_key_blinding_en,
//        .init             = hw_rsa_init,
//        .set_sub_operand  = hw_rsa_set_sub_operand,
//        .set_all_operands = hw_rsa_set_all_operands,
//        .compute          = hw_rsa_compute,
//    },
};

const crypto_hw_ops_t *crypto_ops = &hw_ops;

void crypto_register_ops(const crypto_hw_ops_t *ops)
{
    if (ops) {
        crypto_ops = ops;
    } else {
        crypto_ops = &hw_ops;
    }
}
