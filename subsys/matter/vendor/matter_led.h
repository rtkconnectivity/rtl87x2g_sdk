/**
*********************************************************************************************************
*               Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* \file     matter_led.h
* \brief    This file provides led related functions.
* \details
* \author   rock ding
* \date     2024-04-17
* \version  v1.0
*********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef MATTER_LED_H_
#define MATTER_LED_H_

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "matter_types.h"

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup MATTER_LED_Exported_Types Matter LED Exported Types
  * @brief
  * @{
  */

/**
 * \brief Define the Matter LED instance handle.
 *
 * \ingroup MATTER_LED_Exported_Types
 */
typedef void *T_MATTER_LED_HANDLE;

/** End of MATTER_LED_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** \defgroup Matter_LED_Exported_Functions Matter LED Exported Functions
  * \brief
  * \{
  */

/**
  * @brief  Initialize the matter LED module.
  *
  * @retval true  On success.
  * @retval false On failure.
  */
bool matter_led_init(void);

/**
  * @brief  Create a Matter LED instance.
  * @param[in]  index  The LED index, switch start from 0.
  *
  * \return  The instance handle of Matter LED. If returned handle is NULL, the Matter LED
  *          instance was failed to create.
  */
T_MATTER_LED_HANDLE matter_led_create(uint8_t index, bool dimmable, uint16_t brightness);

/**
  * @brief  Release the Matter LED instance.
  * @param[in]  handle  The Matter LED instance handle \ref T_MATTER_LED_HANDLE.
  *
  * @retval true  On success.
  * @retval false On failure.
  */
bool matter_led_release(T_MATTER_LED_HANDLE handle);

/**
  * @brief  Set Matter LED state to on or off.
  * @param[in]  handle  The Matter LED instance handle \ref T_MATTER_LED_HANDLE.
  * @param[in]  state   Onoff state of LED.
  *     \arg \c true    LED state on.
 *      \arg \c false   LED state off.
  *
  * @retval true  On success.
  * @retval false On failure.
  */
bool matter_led_state_set(T_MATTER_LED_HANDLE handle, bool state);

/**
 *
 * \brief Start LED blink.
 *
 * \param[in]  handle        The Matter LED instance handle \ref T_MATTER_LED_HANDLE.
 * \param[in]  on_time_ms    The LED on time in milliseconds.
 * \param[in]  off_time_ms   The LED off time in milliseconds.
 * \param[in]  count         The target blink count.
 *      \arg \c 0xFFFF       Blink infinitely until timer is stopped.
 *      \arg \c others       The target blink count.
 *
 * \retval true     LED blink was started successfully.
 * \retval false    LED was failed to blink.
 */
bool matter_led_blink_start(T_MATTER_LED_HANDLE handle,
                            uint32_t            on_time_ms,
                            uint32_t            off_time_ms,
                            uint16_t            count);

/**
  * @brief  Stop LED blink.
  * \param[in]  handle        The Matter LED instance handle \ref T_MATTER_LED_HANDLE.
  *
  * \retval true     LED blink was stopped successfully.
  * \retval false    LED blink was failed to stop.
  */
bool matter_led_blink_stop(T_MATTER_LED_HANDLE handle);

bool matter_led_brightness_get(T_MATTER_LED_HANDLE handle, uint16_t *brightness);

bool matter_led_brightness_set(T_MATTER_LED_HANDLE handle, uint16_t brightness);

#ifdef __cplusplus
}
#endif

#endif /*MATTER_LED_H*/

/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/