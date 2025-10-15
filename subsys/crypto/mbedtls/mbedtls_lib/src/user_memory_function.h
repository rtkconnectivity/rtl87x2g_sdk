#ifndef _USER_MEMORY_FUNCTION_H
#define _USER_MEMORY_FUNCTION_H

#include <string.h>
#include <stdint.h>

extern void *(*mbedtls_malloc)(size_t size);
extern void (*mbedtls_free)(void *ptr);

static __inline void *mbedtls_calloc(uint32_t cnt, uint32_t size)
{
    void *p;
    if ((p = mbedtls_malloc(cnt * size)) == NULL)
    {
        return NULL;
    }

    memset(p, 0, cnt * size);

    return p;
}

#endif //_USER_MEMORY_FUNCTION_H
