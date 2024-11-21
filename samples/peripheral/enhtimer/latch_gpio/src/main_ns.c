/**
*****************************************************************************************
*     Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     main.c
* @brief    This file provides demo code of pwm.
* @author   Grace_yan
* @date     2023-07-21
* @version  v1.0
*****************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "trace.h"
#include "rtl_gpio.h"
#include "io_latch_gpio.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None.
  * @return None.
  */
int main(void)
{
    /* Enable Interrupt */
    __enable_irq();

    DBG_DIRECT("Start latch_gpio test!");

    /* Initialize timer peripheral */
    driver_enhance_timer_init();

    /* Configure PAD and pinmux firstly! */
    board_gpio_init();

    /* Initialize gpio peripheral */
    driver_gpio_init();

    while (1)
    {
        /* Simulate GPIO trigger signal */
        for (volatile uint32_t i = 0; i < 1000; i++);
        GPIO_WriteBit(GPIO_PORT_OUTPUT, GPIO_PIN_OUTPUT, (BitAction)(1));
        for (volatile uint32_t i = 0; i < 1000; i++);
        GPIO_WriteBit(GPIO_PORT_OUTPUT, GPIO_PIN_OUTPUT, (BitAction)(0));
    }
}

/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/
