/**
*****************************************************************************************
*     Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     io_timer.c
* @brief    This file provides demo code of timer interrupt.
* @author   Grace_yan
* @date     2023-03-24
* @version  v1.0
*****************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "rtl_rcc.h"
#include "rtl_nvic.h"
#include "rtl_gpio.h"
#include "rtl_pinmux.h"
#include "app_task.h"

#include "io_wdt.h"
#include "io_gpio.h"
#include "trace.h"

/* Private define ------------------------------------------------------------*/
#define INPUT_PIN           P4_0
#define GPIO_PIN            GPIO_GetPin(INPUT_PIN)
#define GPIO_PORT           GPIO_GetPort(INPUT_PIN)
#define GPIO_PIN_IRQN       GPIOB5_IRQn
#define GPIO_Pin_Handler    GPIOB5_Handler
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialization of pinmux settings and pad settings.
  * @param  No parameter.
  * @return void
  */
void board_gpio_init(void)
{
    Pad_Config(INPUT_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_HIGH);

    Pinmux_Config(INPUT_PIN, DWGPIO);
}

void driver_gpio_init(void)
{
    /* GPIO peripheral clock enable */
    RCC_PeriphClockCmd(APBPeriph_GPIOB, APBPeriph_GPIOB_CLOCK, ENABLE);

    /* Configure GPIO_PIN_OUTPUT as output mode */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin        = GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode       = GPIO_DIR_IN;
    GPIO_InitStruct.GPIO_ITCmd      = ENABLE;
    GPIO_InitStruct.GPIO_ITTrigger  = GPIO_INT_Trigger_EDGE;
    GPIO_InitStruct.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;

    /* Enable GPIO doubonce function, Set doubonce time is 1ms,
     * debounce time = (CntLimit + 1) * DEB_CLK = 32 / 32000 = 1 */
    GPIO_InitStruct.GPIO_ITDebounce        = GPIO_INT_DEBOUNCE_ENABLE;
    GPIO_InitStruct.GPIO_DebounceClkSource = GPIO_DEBOUNCE_32K;
    GPIO_InitStruct.GPIO_DebounceClkDiv    = GPIO_DEBOUNCE_DIVIDER_1;
    GPIO_InitStruct.GPIO_DebounceCntLimit  = 32 - 1;
    GPIO_Init(GPIO_PORT, &GPIO_InitStruct);

    /* Enable NVIC*/
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = GPIO_PIN_IRQN;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    /* Enable interrupt */
    GPIO_MaskINTConfig(GPIO_PORT, GPIO_PIN, ENABLE);
    GPIO_INTConfig(GPIO_PORT, GPIO_PIN, ENABLE);
    GPIO_ClearINTPendingBit(GPIO_PORT, GPIO_PIN);
    GPIO_MaskINTConfig(GPIO_PORT, GPIO_PIN, DISABLE);
}

/**
  * @brief  GPIO interrupt handler function.
  * @param  None.
  * @return None.
  */
void GPIO_Pin_Handler(void)
{
    /* Mask and disable interrupt */
    GPIO_INTConfig(GPIO_PORT, GPIO_PIN, DISABLE);
    GPIO_MaskINTConfig(GPIO_PORT, GPIO_PIN, ENABLE);

    uint32_t send_val = IO_MSG_TYPE_GPIO;
    if (false == app_send_msg_to_apptask(&send_val))
    {
        APP_PRINT_ERROR0("[io_gpio] GPIO_Input_Handler: Send int_gpio_msg failed!");
        //Add user code here!
        GPIO_ClearINTPendingBit(GPIO_PORT, GPIO_PIN);
        return;
    }

    /* Clear int flag, unmask and enable interrupt */
    GPIO_ClearINTPendingBit(GPIO_PORT, GPIO_PIN);
    GPIO_MaskINTConfig(GPIO_PORT, GPIO_PIN, DISABLE);
    GPIO_INTConfig(GPIO_PORT, GPIO_PIN, ENABLE);
}

/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/