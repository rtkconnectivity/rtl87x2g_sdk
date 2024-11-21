/**
****************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
****************************************************************************************************
* @file     test_mode.h
* @brief    test mode definition.
* @author   Serval Li
* @date     2017-08-29
* @version  v1.0
****************************************************************************************************
*/

#ifndef _TEST_MODE_H_
#define _TEST_MODE_H_

#include <stdbool.h>
#include "wdt.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup TEST_MODE_API  Test Mode
  * @brief test mode APIs
  * @{
  */

/*============================================================================*
 *                      Functions
 *============================================================================*/
/** @defgroup TEST_MODE_Exported_Functions Test Mode Sets Exported Functions
  * @brief
  * @{
  */
/**
  * @brief  check whether mcu should be single tone test mode or not
  * @retval true means mcu is in single tone test mode now
  * @retval false means mcu is not in single tone test mode now
  */
extern bool (*is_single_tone_test_mode)(void);

/**
  * @brief  switch to single tone test mode
  * @param  none
  * @return none
  */
extern void (*switch_into_single_tone_test_mode)(void);

/**
  * @brief  reset single tone test mode
  * @param  none
  * @return none
  */
extern void (*reset_single_tone_test_mode)(void);

/**
  * @brief  set hci mode flag
  * @param  true means enable, flase means disable
  * @retval none
  */
extern void (*set_hci_mode_flag)(bool enable);

/**
  * @brief  check whether mcu should be hci mode or not
  * @retval true means mcu is in hci mode now
  * @retval false means mcu is not in hci mode now
  */
bool check_hci_mode_flag(void);

/**
  * @brief  switch to hci mode
  * @param  none
  * @return none
  */
extern void (*switch_into_hci_mode)(void);


/** End of TEST_MODE_Exported_Functions
  * @}
  */

/** End of TEST_MODE_API
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* _TEST_MODE_H_ */

