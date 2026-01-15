/**
 * @file platform_alt.h
 * @brief Header for mbedtls platform alternative initialization.
 */

#ifndef MBEDTLS_PLATFORM_ALT_H
#define MBEDTLS_PLATFORM_ALT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Initialize custom mbedtls platform functions.
 *         Should be called before any other mbedtls API.
 */
void platform_alt_init(void);

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_PLATFORM_ALT_H */
