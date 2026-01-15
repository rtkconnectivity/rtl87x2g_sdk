// threading_alt.c


#include "common.h"
#include "threading_alt.h"
#include "mbedtls/threading.h"
#include "os_sync.h"

#if defined(MBEDTLS_THREADING_ALT)

static void alt_mutex_init(mbedtls_threading_mutex_t *mutex)
{
    if (!mutex) return;
    os_mutex_create(&mutex->mutex);
    mutex->is_valid = (mutex->mutex != NULL);
    mutex->count = 0;
}

static void alt_mutex_free(mbedtls_threading_mutex_t *mutex)
{
    if (!mutex || !mutex->is_valid) return;
    os_mutex_delete(mutex->mutex);
    mutex->mutex = NULL;
    mutex->is_valid = 0;
    mutex->count = 0;
}

static int alt_mutex_lock(mbedtls_threading_mutex_t *mutex)
{
    if (!mutex || !mutex->is_valid)
        return MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;

    if (!os_mutex_take(mutex->mutex, 0xFFFFFFFF))
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;

    // Enter critical section to update the count (mutex is held now)
    uint32_t flag = os_lock();
    mutex->count--;          // 0 -> -1 (owned)
    os_unlock(flag);

    return 0;
}

static int alt_mutex_unlock(mbedtls_threading_mutex_t *mutex)
{
    if (!mutex || !mutex->is_valid)
        return MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;

    // First, verify and update the count in a critical section
    uint32_t flag = os_lock();
    if (mutex->count >= 0) {
        // Unlock requested while not owning the mutex -> error
        os_unlock(flag);
        return MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;
    }
    mutex->count++;          // -1 -> 0 (about to release)
    os_unlock(flag);

    // Then release the OS-level mutex
    if (!os_mutex_give(mutex->mutex)) {
        // If give fails, roll back the count to keep state consistent
        flag = os_lock();
        mutex->count--;
        os_unlock(flag);
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }

    return 0;
}

void mbedtls_threading_alt_init(void)
{
    mbedtls_threading_set_alt(alt_mutex_init, alt_mutex_free, alt_mutex_lock, alt_mutex_unlock);
}

#endif
