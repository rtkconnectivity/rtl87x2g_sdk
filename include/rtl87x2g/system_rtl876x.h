/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    System_rtl876x_ns.h
  * @brief   CMSIS Device System Header file
  * @details CMSIS Device System Header File for RTL876x Device Series
  * @author
  * @date    3. March 2016
  * @version v1.0
  * *************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2016 Realtek Semiconductor Corporation</center></h2>
   * ************************************************************************************
   */

/*============================================================================*
 *                      Define to prevent recursive inclusion
 *============================================================================*/
#ifndef SYSTEM_RTL876X_NS_H
#define SYSTEM_RTL876X_NS_H


/*============================================================================*
 *                      Headers
 *============================================================================*/
#include <stdint.h>
#include <stdbool.h>

/** @defgroup SYSTEM_RTL876X_API  System RTL876x
  * @brief CMSIS API sets for RTL876x Device Series
  * @{
  */

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                      Types
 *============================================================================*/
/** @defgroup SYSTEM_RTL876X_Exported_types System RTL876x Exported types
  * @{
  */
/* @brief Application main entry point function prototype. */
typedef bool (*APP_MAIN_FUNC)();

/* @brief Generic user callback function prototype */
typedef void (*USER_CALL_BACK)();

/** End of SYSTEM_RTL876X_Exported_types
  * @}
  */

/*============================================================================*
 *                      Variables
 *============================================================================*/
/** @defgroup SYSTEM_RTL876X_Exported_Variables System RTL876x Exported Variables
  * @{
  */
/* @brief System core clock frequency in Hz. */
extern uint32_t SystemCoreClock;

/* @brief Pre-main application initialization function pointer. */
extern APP_MAIN_FUNC app_pre_main;

/* @brief Main application entry function pointer. */
extern APP_MAIN_FUNC app_main;

/* @brief Pre-main callback function pointer. */
extern USER_CALL_BACK app_pre_main_cb;

/* @brief OS patch initialization callback. */
extern USER_CALL_BACK os_patch_init;

/** End of SYSTEM_RTL876X_Exported_Variables
  * @}
  */

/** End of SYSTEM_RTL876X_API
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_RTL876X_H */

