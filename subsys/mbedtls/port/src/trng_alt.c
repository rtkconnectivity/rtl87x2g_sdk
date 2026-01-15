#include <string.h>
#include <stdlib.h>
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "trng_alt.h"
#include "crypto_hw_locks.h"
#include "crypto_accel_dispatch.h"

#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT)

int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
    (void) data;
    *olen = 0;

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&g_crypto_locks.trng) != 0)
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
#endif

    while (*olen < len) {
        uint32_t rnd;
        rnd = crypto_ops->trng.rng_get32(0xffffffff);
        size_t n = (len - *olen) >= sizeof(rnd) ? sizeof(rnd) : (len - *olen);
        memcpy(output + *olen, &rnd, n);
        *olen += n;
    }

#if defined(MBEDTLS_THREADING_C)
    mbedtls_mutex_unlock(&g_crypto_locks.trng);
#endif

    return 0;
}

#endif
