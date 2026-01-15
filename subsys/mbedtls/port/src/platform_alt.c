/**
 * @file platform_alt.c
 * @brief Implementation of mbedtls platform alternative functions.
 */

#include <stdio.h>
#include <stdarg.h>
#include "log_core.h"
#include "mbedtls/platform.h"
#include "platform_alt.h"

/* 
 * Assuming log_direct prototype is:
 * void log_direct(const char *fmt, ...);
 */

#ifdef MBEDTLS_PLATFORM_PRINTF_ALT

typedef int (*MBEDTLS_PRINTF)(const char *format, ...);
    
/**
 * @brief  Initialize the mbedtls platform layer.
 *         Registers the custom printf function via mbedtls API.
 */
void platform_alt_init(void)
{
    /* Set the custom printf function pointer */
    mbedtls_platform_set_printf((MBEDTLS_PRINTF)log_direct);
}
    
#endif /* MBEDTLS_PLATFORM_PRINTF_ALT */
