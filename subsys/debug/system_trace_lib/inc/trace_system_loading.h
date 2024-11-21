/**
************************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
************************************************************************************************************
* @file     rtl876x_lib_platform.h
* @brief
* @details
* @author
* @date
* @version
*************************************************************************************************************
*/

#ifndef _TRACE_SYSTEM_LOADING_H_
#define _TRACE_SYSTEM_LOADING_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


void trace_system_loading_init(void);

void get_system_loading(void);

void reset_system_loading(void);


#ifdef __cplusplus
}
#endif

#endif /* _TRACE_SYSTEM_LOADING_H_ */
