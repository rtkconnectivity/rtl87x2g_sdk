/**
 * \file threading_alt.h
 *
 * \brief Threading abstraction layer
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */
#ifndef MBEDTLS_THREADING_ALT_H
#define MBEDTLS_THREADING_ALT_H
#include "mbedtls/private_access.h"

#include "mbedtls/build_info.h"

#include <stdlib.h>
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct mbedtls_threading_mutex_t {
    void * MBEDTLS_PRIVATE(mutex);
    int32_t MBEDTLS_PRIVATE(count);
    bool MBEDTLS_PRIVATE(is_valid);
} mbedtls_threading_mutex_t;

void mbedtls_threading_alt_init(void);

#ifdef __cplusplus
}
#endif

#endif /* threading.h */
