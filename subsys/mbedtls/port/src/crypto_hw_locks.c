// crypto_hw_locks.c

#include "crypto_hw_locks.h"

#if defined(MBEDTLS_THREADING_C)

crypto_hw_locks_t g_crypto_locks;


void crypto_hw_locks_init(void)
{
    mbedtls_mutex_init(&g_crypto_locks.trng);
    mbedtls_mutex_init(&g_crypto_locks.aes);
    mbedtls_mutex_init(&g_crypto_locks.pke);
    mbedtls_mutex_init(&g_crypto_locks.sha);
}

void crypto_hw_locks_free(void)
{
    mbedtls_mutex_free(&g_crypto_locks.trng);
    mbedtls_mutex_free(&g_crypto_locks.aes);
    mbedtls_mutex_free(&g_crypto_locks.pke);
    mbedtls_mutex_free(&g_crypto_locks.sha);
}

#endif
