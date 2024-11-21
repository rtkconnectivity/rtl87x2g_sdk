/**
*****************************************************************************************
*     Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     io_rtc.h
* @brief    This file provides demo code of rtc comparator.
* @author
* @date     2023-03-24
* @version  v1.0
*****************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "trace.h"
#include "rtl876x.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define IR_DATA_SIZE_MAX            75

#define IR_TX_PIN                   P2_5
#define IR_TX_FIFO_THR_LEVEL        2

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialization of pinmux settings and pad settings.
  * @param  No parameter.
  * @return void
  */
void board_ir_init(void);

/**
  * @brief  Initialize ir peripheral.
  * @param  No parameter.
  * @return void
  */
void driver_ir_init(uint32_t vFreq);

void ir_demo(void);

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/
