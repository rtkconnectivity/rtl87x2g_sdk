/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file     io_gdma.h
* @brief
* @details
* @author   yuan
* @date     2019-01-11
* @version  v1.0
*********************************************************************************************************
*/

#include "rtl_gdma.h"

#ifndef __IO_GDMA_H
#define __IO_GDMA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Defines ------------------------------------------------------------------*/
#define GDMA_CHANNEL_NUM                GDMA_CH_NUM0
#define GDMA_Channel                    GDMA_Channel0
#define GDMA_Channel_IRQn               GDMA_Channel0_IRQn
#define GDMA_Channel_Handler            GDMA_Channel0_Handler

/* The maximum number of GDMA single block is 65535. */
#define GDMA_TRANSFER_SIZE              200

void driver_gdma_init(void);


#ifdef __cplusplus
}
#endif

#endif

