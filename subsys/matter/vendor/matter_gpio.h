/**
*********************************************************************************************************
*               Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* \file     matter_gpio.h
* \brief    This file provides key related functions.
* \details
* \author   rock ding
* \date     2024-04-17
* \version  v1.0
*********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef MATTER_GPIO_H_
#define MATTER_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "matter_types.h"

/*============================================================================*
 *                         Constants
 *============================================================================*/
/** \defgroup Matter_GPIO_Exported_Constants Matter GPIO Exported Constants
  * \brief
  * \{
  */

/**
 * \defgroup    Matter_GPIO_Key_State Matter GPIO Key State
 * \{
 * \ingroup     Matter_GPIO_Exported_Constants
 */
#define MATTER_GPIO_KEY_STATE_RELEASE 0
#define MATTER_GPIO_KEY_STATE_PRESS   1

/** End of Matter_GPIO_Key_State
  * \}
  */

/** End of Matter_GPIO_Exported_Constants
  * \}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup MATTER_GPIO_Exported_Types Matter GPIO Exported Types
  * @brief
  * @{
  */

/**
 * \brief Define the Matter GPIO callback prototype.
 * \param[in]   index  Key index.
 * \param[in]   state  Key state \ref Matter_GPIO_Key_State.
 *
 * \return  None.
 *
 * \ingroup MATTER_GPIO_Exported_Types
 */
typedef void (*P_MATTER_GPIO_CBACK)(uint8_t index, uint8_t state);

/** End of MATTER_GPIO_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** \defgroup Matter_GPIO_Exported_Functions Matter GPIO Exported Functions
  * \brief
  * \{
  */

/**
  * @brief  Initialize the matter GPIO module.
  * @param[in]  func  Callback function.
  *
  * @return None.
  */
void matter_gpio_init(P_MATTER_GPIO_CBACK func);

/** End of Matter_GPIO_Exported_Functions
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif // MATTER_GPIO_H_

/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/
