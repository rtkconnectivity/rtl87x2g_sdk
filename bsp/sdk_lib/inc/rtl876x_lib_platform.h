/**
************************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
************************************************************************************************************
* @file     rtl876x_lib_platform.h
* @brief
* @details
* @author
* @date
* @version
*************************************************************************************************************
*/

#ifndef _RTL876X_LIB_PLATFORM_H_
#define _RTL876X_LIB_PLATFORM_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "mem_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup  RTL876X_LIB_PLATFORM   RTL876x LIB Platform
  * @brief
  * @{
  */
/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup RTL876X_LIB_PLATFORM_Exported_Types RTL876x LIB Platform Exported Types
  * @brief
  * @{
  */
typedef void (*WDG_APP_CB)(void);

/** End of RTL876X_LIB_PLATFORM_Exported_Types
  * @}
  */

/*============================================================================*
 *                       Functions
 *============================================================================*/
/** @defgroup RTL876X_LIB_PLATFORM_Exported_Functions RTL876x LIB Platform Exported Functions
  * @brief
  * @{
  */
/**
    * @brief    sdk lib funtion pointer init
    * @param    void
    * @return   void
    */
void patch_func_ptr_init(void);

/**
    * @brief    get usb efuse infomation
    * @param    pointer to data
    * @return   void
    */
void usb_info_get(uint8_t *data);

/**
    * @brief    whether ota support dual bank merge
    * @param    void
    * @return   ture:support ; false: not support
    */
bool is_ota_support_dual_bank_merge(void);

/**
    * @brief    print sdk lib version
    * @param    void
    * @return   void
    */
void print_sdk_lib_version(void);

/**
    * @brief    register watchdog app callback
    * @param    callback function
    * @return   void
    */
void wdg_register_app_cb(WDG_APP_CB func);

/**
 * @brief  get ic type
 * @param  none
 * @return uint8_t, ic type:
 * @code
 * #define RTL8762GTP              0x03
 * #define RTL8762GKH              0x07
 * #define RTL8762GKU              0x08
 * #define RTL8762GTH              0x09
 * #define RTL8762GRH              0x0A
 * #define RTL8762GRU              0x0B
 * #define RTL8762GTU              0x0C
 * #define RTL8777GKF-VA4          0x10
 * #define RTL8772GWP              0x11
 * #define RTL8772GWP-VI           0x11
 * #define RTL8772GWP-VM           0x12
 * #define RTL8772GWP-VM           0x13
 * #define RTL8762GC               0x14
 * #define RTL8762GWF              0x15
 * #define RTL8771GUV              0x16
 * #define RTL8777GKF-VA5          0x17
* @endcode
 */
uint8_t get_ic_type(void);

/**
 * @brief  get 14 bytes EUID
 * @param  none
 * @return uint8_t *, the pointer to a copy of EUID.
  */
uint8_t *get_ic_euid(void);

/**
   * @brief    init I-cache/D-cache hit rate and miss rate
   * @param    void
   * @return   void
   */
void init_pmu_counter(void);

/**
   * @brief    print I-cache/D-cache hit rate and miss rate
   * @param    void
   * @return   void
   */
void pmu_print_counter(void);

/**
 * @brief  disable swd pin
 * @param  none
 * @return none
*/
void swd_pin_disable(void);

/**
 * @brief  bypass pmu check for wfi
 * @param  none
 * @return none
*/
void bypass_pmu_check_for_wfi(bool enable);

/**
 * @brief  enable xtal power in dlps
 * @param  none
 * @return none
*/
void set_xtal_power_in_dlps(bool enable);

void disable_xip_access(bool set);
/** End of RTL876X_LIB_PLATFORM_Exported_Functions
  * @}
  */

/** End of RTL876X_LIB_PLATFORM
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_LIB_PLATFORM_H_ */
