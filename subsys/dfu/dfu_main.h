/**
**********************************************************************************************************
*               Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     dfu_mian.h
* @brief    normal ota APIs
* @details
* @author   Grace
* @date     2023-12-06
* @version  v1.1
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2023 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */


#ifndef _DFU_MAIN_H_
#define  _DFU_MAIN_H_

#include <stdint.h>
#include <stdbool.h>
#include "profile_server.h"

/** @defgroup  DFU_MAIN DFU Main
  * @brief
  * @{
  */

/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup DFU_MAIN_Exported_Macros DFU Main Exported Macros
  * @brief
  * @{
  */
/* when start dfu will modify the value depend on dfu image total length.
So this value is only need set based on the image size of 100KB */
#define NORMAL_OTA_TIMEOUT_TOTAL               240  //uint: second
//This value won't be modofy
#define NORMAL_OTA_TIMEOUT_WAIT4_CONN          60   //uint: second

/** End of DFU_MAIN_Exported_Macros
  * @}
  */

extern bool is_normal_ota_mode;
extern void *normal_ota_total_timer_handle;
extern void *normal_ota_wait4_conn_timer_handle;

extern T_SERVER_ID rtk_dfu_service_id;

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup DFU_MAIN_Exported_Functions DFU Main Exported Functions
  * @brief
  * @{
  */
/**
 * @brief  dfu sw timer create
 * @param  None
 * @return None
 */
void dfu_timer_init(void);

/**
 * @brief  dfu set random address
 * @param  None
 * @return None
 */
void dfu_set_rand_addr(void);

/**
 * @brief  dfu main function
 * @param  None
 * @return None
 */
void dfu_main(void);

/** End of DFU_MAIN_Exported_Functions
  * @}
  */

/** End of DFU_MAIN
  * @}
  */

#endif
