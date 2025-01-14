/**
*****************************************************************************************
*     Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     main_ns.c
* @brief    This file provides demo code of rtc comparator.
* @author   Grace_yan
* @date     2023-03-24
* @version  v1.0
*****************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "trace.h"

#include "io_rtc.h"

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

    DBG_DIRECT("Start rtc comparator test!");

    /* Initialize RTC peripheral */
    driver_rtc_init();

    while (1);
}

/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/
