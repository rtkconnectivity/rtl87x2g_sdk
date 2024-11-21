/**
*****************************************************************************************
*     Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     io_latch_gpio.h
* @brief    This file provides demo code of pwm.
* @author   Grace_yan
* @date     2023-07-21
* @version  v1.0
*****************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl_pinmux.h"
#include "rtl_gpio.h"
#include "rtl876x.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* P0_0 is connected to LED0 on EVB board */
#define ENHTIMER_NUM                    ENH_TIM0
#define ENHTIMER_IRQn                   Enhanced_Timer0_IRQn

/* Timer timing config */
#define ENHTIM_UINT       40 /* 40M source clock is divided to get unit: 1us */
#define ENHTIM_ING        1000000
#define ENHTIM_PERIOD    (ENHTIM_ING * ENHTIM_UINT - 1)

#define OUTPUT_PIN          P0_0
#define GPIO_PIN            GPIO_GetPin(OUTPUT_PIN)
#define GPIO_PORT           GPIO_GetPort(OUTPUT_PIN)
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialization of pinmux settings and pad settings.
  * @param  None.
  * @return None.
  */
void board_gpio_init(void);

/**
  * @brief  Initialize GPIO peripheral.
  * @param  None.
  * @return None.
  */
void driver_gpio_init(void);

/**
  * @brief  Enhance timer handler.
  * @param  None.
  * @return None.
  */
void driver_enhtim_init(void);


/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/
