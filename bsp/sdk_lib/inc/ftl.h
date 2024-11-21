/**
****************************************************************************************************
*               Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
****************************************************************************************************
* @file      ftl.h
* @brief     Flash Transport Layer(FTL) is used as an abstraction layer for the user to read&write data on flash.
* @note      FTL is designed for lightweight dynamical parameter storage, heavyweight and read-only data should not use it.
* @author    Grace
* @date      2023-11-30
* @version   v1.0
* **************************************************************************************************
*/
#ifndef _FTL_H_
#define _FTL_H_
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef  __cplusplus
extern  "C" {
#endif  // __cplusplus
/** @defgroup  FTL    Flash Transport Layer
    * @brief simple implementation of Flash Translation Layer
    * @{
    */
/**
 *
 * @section FTL_Usage_Chapter How to use this module
 *
 *  - Ext FTL module access. \n
 *   - step1: Init an ext FTL module.
 *   - step2: Save data to the ext FTL module.
 *   - step3: Load data from the ext FTL module.
 *   - Sample code:
 *   @code
        #define EXT_FTL_NAME "TEST_FTL"
        #define EXT_FTL_LOGIC_SIZE     (0x1000)
        #define EXT_FTL_BLOCK_SIZE     (64)
        void ftl_ext_module_demo(void)
        {
            // Init an ext FTL module
            ftl_init_module(EXT_FTL_NAME, EXT_FTL_LOGIC_SIZE, EXT_FTL_BLOCK_SIZE);
            // Save data
            uint8_t data_buf[EXT_FTL_BLOCK_SIZE];
            memset(data_buf, 0x5A, EXT_FTL_BLOCK_SIZE);
            uint16_t test_offset = 0x800;
            uint32_t ret = ftl_save_to_module(EXT_FTL_NAME, data_buf, test_offset, EXT_FTL_BLOCK_SIZE);
            if (ret != ESUCCESS)
            {
                //save data error
                return;
            }
            //Load data
            uint8_t read_buf[EXT_FTL_BLOCK_SIZE];
            ret = ftl_load_from_module(EXT_FTL_NAME, read_buf, test_offset, EXT_FTL_BLOCK_SIZE);
            if (ret != ESUCCESS)
            {
                //load data error
                return;
            }
        }
 *
 *   @endcode
 */
/*============================================================================*
  *                                   Macros
  *============================================================================*/
/** @defgroup FTL_Exported_Macros Flash Transport Layer Exported Macros
    * @brief
    * @{
    */
#define FTL_POOL                1

#define FTL_PARTITION_NAME      "FTL"
/** End of FTL_Exported_Macros
    * @}
    */
/*============================================================================*
  *                                   Types
  *============================================================================*/
/** @defgroup FTL_Exported_Types Flash Transport Layer Exported Types
  * @brief
  * @{
  */
typedef enum
{
    FTL_IOCTL_DEBUG                 = 0,  /**< IO code for ftl debug */
    FTL_IOCTL_CLEAR_ALL             = 2,  /**< IO code for clear ftl section*/
    FTL_IOCTL_DO_GC_IN_APP          = 6,  /**< IO code to do garbage collection in app*/
} T_FTL_IOCTL_CODE;
/** End of FTL_Exported_Types
  * @}
  */
/*============================================================================*
  *                                Functions
  *============================================================================*/
/** @defgroup FTL_Exported_Functions Flash Transport Layer Exported Functions
    * @brief
    * @{
    */
/**
 * @brief    Control function entry for FTL
 *
 * @param    cmd Command code for different operations
 * @param    p1  Command parameter (see @ref FTL_IO_CTL_CODE).
 * @param    p2  Extended command parameters.
 *                - FTL_IOCTL_DEBUG/FTL_IOCTL_CLEAR_ALL: p1: 0, p2: 0
 *                - FTL_IOCTL_DO_GC_IN_APP: p1: Free page count; p2: Free cell address, from 0 to 0x1000
 *
 * @return   results of control
 * @retval   0  Status is successful.
 * @retval   otherwise fail
 */
extern uint32_t (*ftl_ioctl)(uint32_t cmd, uint32_t p1, uint32_t p2);
/**
    * @brief    Init an ext FTL module
    * @param    module_name  specify FTL ext name, the first 4 bytes can't be the same
    * @param    malloc_size  ftl module logical size
    * @param    block_len    minimum access unit. Must be an integral multiple of 4 and cannot exceed 128
    * @return   status
    * @retval   0  status successful
    * @retval   otherwise fail
    * @note     Up to 6 modules can be applied
    */
int32_t ftl_init_module(char *module_name, uint16_t malloc_size, uint8_t block_len);
/**
    * @brief    Save specified value to ftl module
    * @param    module_name  specify ftl module name
    * @param    pdata specify data buffer
    * @param    offset specify FTL offset to store, need to be an integer multiple of block_len in ftl_init_module
    * @param    size   size to store, need to be an integer multiple of block_len in ftl_init_module
    * @return   status
    * @retval   0  status successful
    * @retval   otherwise fail
    */
int32_t ftl_save_to_module(char *module_name, void *pdata, uint16_t offset, uint16_t size);
/**
    * @brief    Load specified ftl module offset parameter to specified buffer
    * @param    module_name  specify ftl module name
    * @param    pdata  specify data buffer
    * @param    offset specify FTL offset to load, need to be an integer multiple of block_len in ftl_init_module
    * @param    size   size to load, need to be an integer multiple of block_len in ftl_init_module
    * @return   status
    * @retval   0  status successful
    * @retval   otherwise fail
    */
int32_t ftl_load_from_module(char *module_name, void *pdata, uint16_t offset, uint16_t size);
/**
    * @brief    Init ftl cache and set ftl cache enable
    * @param    cache_size  ftl cache size
    * @return   status
    * @retval   true: ftl cache init successfully
    * @retval   otherwise fail
    */
bool ftl_cache_init(uint16_t cache_size);
/** @} */ /* End of group FTL_Exported_Functions */
/** @} */ /* End of group HAL_FTL */
#ifdef  __cplusplus
}
#endif // __cplusplus
#endif // _FTL_H_
