#ifndef MBEDTLS_ACCEL_DISPATCH_H
#define MBEDTLS_ACCEL_DISPATCH_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "crypto_engine_nsc.h"
#include "aes_api.h"

typedef struct {
    uint32_t (*rng_get32)(uint32_t max);
} trng_ops_t;

typedef struct {
    void (*set_clock)(bool enable);
    void (*init)(bool byte_swap_en, bool word_swap_en, uint32_t word_swap_base);
    void (*ecp_init)(uint32_t key_bits, PKE_MODE mode, bool go_to_end_loop, bool RR_mod_n_ready);
    void (*ecp_set_sub_operand)(uint32_t operand_addr, uint32_t *operands, uint32_t byte_len);
    bool (*ecp_set_all_operands)(ECC_GROUP *grp, uint32_t *e, uint32_t e_byte_size);
    ERR_CODE (*ecp_compute)(void *result, uint32_t output_addr, uint16_t func_id);
} pke_ops_t;

typedef struct {
    void (*init)();
    int (*start)(void *ctx, int is224);
    int (*update)(void *ctx, const unsigned char *input, size_t ilen);
    int (*finish)(void *ctx, unsigned char *output);
} sha256_ops_t;

typedef struct {
    bool (*ecb128_encrypt)(const uint8_t *plaintext, const uint32_t *key, uint8_t *ciphertext);
    bool (*ecb128_decrypt)(const uint8_t *ciphertext, const uint32_t *key, uint8_t *plaintext);
    bool (*ecb256_encrypt)(const uint8_t *plaintext, const uint32_t *key, uint8_t *ciphertext);
    bool (*ecb256_decrypt)(const uint8_t *ciphertext, const uint32_t *key, uint8_t *plaintext);
} aes_ops_t;

typedef struct {
    bool (*get_rsa_private_key_blinding_en)(void);
    void (*init)(uint32_t key_bits);
    void (*set_sub_operand)(uint32_t operand_addr, uint32_t *operands, uint32_t byte_len);
    bool (*set_all_operands)(uint32_t *M, uint32_t *e, uint32_t *N, uint32_t exp_byte_size);
    ERR_CODE (*compute)(uint32_t *result, uint32_t output_addr, uint16_t func_id);
} rsa_ops_t;

typedef struct {
    trng_ops_t  trng;
    pke_ops_t   pke;
    sha256_ops_t sha256;
    aes_ops_t    aes;
    rsa_ops_t    rsa;
} crypto_hw_ops_t;

extern const crypto_hw_ops_t *crypto_ops;

void crypto_register_ops(const crypto_hw_ops_t *ops);

#endif /* MBEDTLS_ACCEL_DISPATCH_H */
