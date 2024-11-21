/**
*****************************************************************************************
*     Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     main.c
* @brief    This file provides demo code of spi flash.
* @author
* @date     2023-03-24
* @version  v1.0
*****************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "trace.h"
#include "rtl876x.h"

#include "io_spi.h"

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

    DBG_DIRECT("Start spi polling test!");

    /* Configure pad and pinmux firstly! */
    board_spi_init();

    /* Initialize gpio peripheral */
    driver_spi_init();

#if (SPI_CONFIG_GPIO_SIM_CS == 1)
    driver_gpio_init();
#endif

    /* Start spi demo */
    spi_demo();

    while (1);
}

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/
