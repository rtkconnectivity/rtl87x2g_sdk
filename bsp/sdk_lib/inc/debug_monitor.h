/**
*****************************************************************************************
*     Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    debug_monitor.h
  * @brief   Debug Monitor
  * @author  Henry_Huang
  * @date    2020-09-18
  * @version v1.0
  * *************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2020 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef __DEBUG_MONITOR_H
#define __DEBUG_MONITOR_H

/*============================================================================*
 *                               Header Files
*============================================================================*/

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @defgroup DBG_MONITOR Debug Monitor
  * @brief
  * @{
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup DBG_MONITOR_Exported_Types Debug Monitor Exported Types
  * @brief
  * @{
  */

/* DWT Config*/
typedef enum
{
    BYTE  = 0,
    HALFWORD,
    WORD,
    WATCH_SIZE_MAX
} DWT_WATCH_SIZE;

typedef enum
{
    TRIGGER_ONLY = 0,
    DEBUG_EVENT,
    ACTION_MAX
} DWT_ACTION_TYPE;


typedef enum
{
    FUNCTION_DISABLED = 0,
    FUNCTION_INSTR_ADDR = 2,
    FUNCTION_INSTR_ADDR_LIM,
    FUNCTION_DADDR_RW,
    FUNCTION_DADDR_W,
    FUNCTION_DADDR_R,
    FUNCTION_DADDR_LIM,
    FUNCTION_DVAL_RW,
    FUNCTION_DVAL_W,
    FUNCTION_DVAL_R,
    FUNCTION_DVAL_LINK,
    FUNCTION_MAX
} DWT_FUNCTION_TYPE;

/** End of group DBG_MONITOR_Exported_Types
  * @}
  */

/*============================================================================*
 *                              Functions
*============================================================================*/
/** @defgroup DBG_MONITOR_Exported_Functions Debug Monitor Exported Functions
  * @{
  */

/**
 * @brief  enable debug monitor
 * @param  none
 * @return none
*/
void enable_debug_monitor(void);

/**
 * @brief  watch point 0 setting
 * @param  watch_address the ram address to be detected
 * @param  watch_size    the ram size to be detected
 * @param  action_type
 * @param  read_write_func: watch function type
 * @return none
*/
void watch_point_0_setting(uint32_t watch_address,
                           DWT_WATCH_SIZE watch_size,
                           DWT_ACTION_TYPE action_type,
                           DWT_FUNCTION_TYPE read_write_func);

/**
 * @brief  watch point 1 setting
 * @param  watch_address the ram address to be detected
 * @param  watch_size    the ram size to be detected
 * @param  action_type
 * @param  read_write_func: watch function type
 * @return none
*/
void watch_point_1_setting(uint32_t watch_address,
                           DWT_WATCH_SIZE watch_size,
                           DWT_ACTION_TYPE action_type,
                           DWT_FUNCTION_TYPE read_write_func);

/** End of group DBG_MONITOR_Exported_Functions
  * @}
  */

/** End of DBG_MONITOR
  * @}
  */

#endif  /* __DEBUG_MONITOR_H */
