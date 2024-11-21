/**
*****************************************************************************************
*     Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     main_ns.c
* @brief    This file provides demo code of pwm.
* @author   Grace_yan
* @date     2024-07-24
* @version  v1.0
*****************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "trace.h"
#include "io_pwm_dma.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None.
  * @retval None.
  */
int main(void)
{
    /* Enable Interrupt */
    __enable_irq();

    DBG_DIRECT("Start pwm_dma test!");

    /* Configure PAD and pinmux firstly! */
    board_enhance_pwm_init();

    /* Initialize gdma peripheral */
    driver_gdma_init();

    /* Initialize timer peripheral */
    enhance_timer_init();

    while (1);

}

/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/
