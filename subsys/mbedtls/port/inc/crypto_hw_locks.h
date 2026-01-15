// crypto_hw_locks.h
#pragma once
#include "mbedtls/threading.h"

#if defined(MBEDTLS_THREADING_C)

typedef struct {
    mbedtls_threading_mutex_t trng;
    mbedtls_threading_mutex_t aes;
    mbedtls_threading_mutex_t pke;  
    mbedtls_threading_mutex_t sha;  
} crypto_hw_locks_t;

extern crypto_hw_locks_t g_crypto_locks;

void crypto_hw_locks_init(void);
void crypto_hw_locks_free(void);

#endif
