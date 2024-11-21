/**
*****************************************************************************************
*     Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     io_gpio.h
* @brief    This file provides demo code of gpio input interrupt.
* @author
* @date     2023-03-24
* @version  v1.0
*****************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl_codec.h"
#include "rtl_gdma.h"
#include "rtl_i2s.h"
#include "rtl_nvic.h"
#include "rtl_pinmux.h"
#include "rtl_rcc.h"
#include "rtl_uart.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* GDMA RX defines */
#define GDMA_Channel_AMIC_NUM           0
#define GDMA_Channel_AMIC               GDMA_Channel0
#define GDMA_Channel_AMIC_IRQn          GDMA0_Channel0_IRQn
#define GDMA_Channel_AMIC_Handler       GDMA0_Channel0_Handler

#define GDMA_TX_BURST_SIZE              ((uint32_t)1)
#define GDMA_RX_BURST_SIZE              ((uint32_t)1)
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

void codec_demo(void);

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/
