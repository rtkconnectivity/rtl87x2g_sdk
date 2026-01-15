/**
 * \file trng_alt.h
 *
 * \brief Trng abstraction layer
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */
#ifndef MBEDTLS_TRNG_ALT_H
#define MBEDTLS_TRNG_ALT_H

#include <stdlib.h>
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen);

#ifdef __cplusplus
}
#endif

#endif /* trng_alt.h */
