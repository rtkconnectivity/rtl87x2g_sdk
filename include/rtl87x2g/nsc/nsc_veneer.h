/*
 * FreeRTOS V202107.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef __NSC_VENEER_H__
#define __NSC_VENEER_H__

#include <stdint.h>
#include "rtl876x.h"

typedef void (*NonSecure_func)(void);

typedef enum
{
    SECURE_APP_FUNCTION_OPCODE_INIT = 0,
} SECURE_APP_FUNCTION_OPCODE;

typedef enum
{
    SECURE_FUNCTION_OPCODE_INIT = 0,
    EFUSE_ACCESS  = 1,
    EFUSE_RAM_ACCESS  = 2,
    SECURE_REGION_ACCESS = 3,
    DISABLE_SWD_PIN = 4,
    SPIC_SET_INTERFACE = 5,
    XTAL_40M_AAC_CAL = 6,
    PSRAM_LMP_CTRL = 7,
    WDT_GET_TIMEOUT_MS = 8,
    WDT_GET_MODE = 9,
    HW_AES_SET = 10,
    AON_WDT_GET_TIMEOUT_MS = 11,
    AON_WDT_GET_MODE = 12,
    AON_WDT_IS_ENABLE = 13,
} SECURE_FUNCTION_OPCODE;

typedef enum
{
    SEC_FPTR_OPCODE_OS_AES_SEM_TAKE = 0,
    SEC_FPTR_OPCODE_OS_AES_SEM_GIVE,
    SEC_FPTR_OPCODE_PRINT_LOG_BUFF,
    SEC_FPTR_OPCODE_APP_CB_WDG_RESET,
    SEC_FPTR_OPCODE_OS_LOCK,
    SEC_FPTR_OPCODE_OS_UNLOCK,
    SEC_FPTR_OPCODE_OS_TASK_HDL_GET,
    SEC_FPTR_OPCODE_OS_TASK_PRI_GET,
    SEC_FPTR_OPCODE_WRITE_INFO_TO_FLASH_BEFORE_RESET,
    SEC_FPTR_OPCODE_SAVE_SYSTEM_STATUS_TO_FLASH,
    SEC_FPTR_OPCODE_OS_MEM_ALLOC,
} SECURE_FPTR;

void secure_app_function_call(SECURE_APP_FUNCTION_OPCODE opcode, void *param);
void secure_function_call(SECURE_FUNCTION_OPCODE opcode, void *param);
void set_secure_func_ptr(SECURE_FPTR opcode, NonSecure_func func);



#endif /* __NSC_VENEER_H__ */
