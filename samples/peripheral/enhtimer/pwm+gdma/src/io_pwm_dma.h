/**
*****************************************************************************************
*     Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     io_enhtim_pwm.h
* @brief    This file provides demo code of pwm.
* @author   Grace_yan
* @date     2023-03-24
* @version  v1.0
*****************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialization of pinmux settings and pad settings.
  * @param  None.
  * @retval None.
  */
void board_enhance_pwm_init(void);

/**
  * @brief  Initialize ENHTIM peripheral.
  * @param  None.
  * @retval None.
  */
void enhance_timer_init(void);

/**
  * @brief  Initialize GDMA peripheral.
  * @param  None.
  * @retval None.
  */
void driver_gdma_init(void);

/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/
