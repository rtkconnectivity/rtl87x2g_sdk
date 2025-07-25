/**
 *****************************************************************************************
 *     Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
 *****************************************************************************************
 * @file    fmc_platform.h
 * @brief   Flexible memory controller (SPIC & PSRAMC) platform dependent implementation header file
 * @author  Yao-Yu
 * @date    2020-10-20
 * @version v0.1
 * ***************************************************************************************
 */

#ifndef _FMC_PLATFORM_H
#define _FMC_PLATFORM_H

#include "stdint.h"

/** @defgroup FMC  Flexible Memory Controller
  * @brief
  * @{
  */

/****************************************************************************************
 * Flexible Memory Controller Address Map
 ****************************************************************************************/
/**
  * \cond     private
  * \defgroup FMC_Private_Macros
  * \{
  */
#define SPIC0_ADDR            0x04000000
#define SPIC0_SIZE            (64*1024*1024)

#define FMC_MAIN0_ADDR                                      (SPIC0_ADDR)
#define FMC_MAIN0_SIZE                                      (SPIC0_SIZE)

/****************************************************************************************
 * SPIC Description
 ****************************************************************************************/
/** @defgroup SPIC_Exported_Description  SPIC Exported Description
  * @ingroup  FMC_Private_Macros
  * @brief
  * @{
  */
#define SPIC_NUM                                            (3)
#define SPIC0_RX_FIFO_NUM                                   (64)
#define SPIC0_TX_FIFO_NUM                                   (32)
#define SPIC1_RX_FIFO_NUM                                   (64)
#define SPIC1_TX_FIFO_NUM                                   (64)
#define SPIC2_RX_FIFO_NUM                                   (64)
#define SPIC2_TX_FIFO_NUM                                   (64)

/** End of SPIC_Exported_Description
  * @}
  */

/**
*  End of FMC_Private_Macros
* \}
* \endcond
*/




/** @defgroup FMC_Exported_Macros  FMC_Exported_Macros
  * @brief
  * @{
  */

/****************************************************************************************
 * Flexible Memory Controller Address Map Checking
 ****************************************************************************************/
/** @brief Flexible Memory Controller address map checking. */
#define FMC_IS_SPIC0_ADDR(addr)                             ((addr >= FMC_MAIN0_ADDR) && (addr < FMC_MAIN0_ADDR + FMC_MAIN0_SIZE))


/****************************************************************************************
 * Flexible Memory Controller Calibration Definition
 ****************************************************************************************/
/** @brief Flexible Memory Controller calibration definition. */
#define FMC_CAL_PATTERN                                     (0x5A5A12A5)

/** End of FMC_Exported_Macros
  * @}
  */

/****************************************************************************************
 * FMC Enum Type
 ****************************************************************************************/
/** @defgroup FMC_Exported_Types  FMC Exported Types
  * @brief
  * @{
  */

/** @brief PSRAM interface type. */
typedef enum
{
    /* For SPIC1 PSRAM */
    PSRAM_OPI_TYPE,
} PSRAM_INTERFACE_TYPE;

/** @brief Nor flash erase mode. */
typedef enum
{
    FLASH_NOR_ERASE_SECTOR  = 1, /**< sector erase */
    FLASH_NOR_ERASE_BLOCK   = 2, /**< block erase */
    FLASH_NOR_ERASE_CHIP    = 4, /**< chip erase */
} FLASH_NOR_ERASE_MODE;

/** @brief Nor flash return type. */
typedef enum
{
    FLASH_NOR_RET_UNKNOWN,
    FLASH_NOR_RET_NOT_EXIST_BASIC_CMD,
    FLASH_NOR_RET_NOT_EXIST_ADV_CMD,
    FLASH_NOR_RET_NOT_EXIST_QUERY_INFO,
    FLASH_NOR_RET_CMD_NOT_SUPPORT,
    FLASH_NOR_RET_DEV_NOT_SUPPORT,
    FLASH_NOR_RET_VENDOR_NOT_SUPPORT,
    FLASH_NOR_RET_LOCK_FAILED,
    FLASH_NOR_RET_UNLOCK_FAILED,
    FLASH_NOR_RET_BIT_MODE_SET_FAILED,
    FLASH_NOR_RET_BIT_MODE_NOT_SUPPORT,
    FLASH_NOR_RET_ILLEGAL_OPERATION,
    FLASH_NOR_RET_PARAM_INVALID,
    FLASH_NOR_RET_WAIT_BUSY_FAILED,
    FLASH_NOR_RET_IDX_OUT_OF_RANGE,
    FLASH_NOR_RET_ADDR_OUT_OF_RANGE,
    FLASH_NOR_RET_ADDR_LARGER_THAN_FLASH_SIZE,
    FLASH_NOR_RET_CAL_IN_PHYSICAL_CYC_NOT_FOUND,
    FLASH_NOR_RET_CAL_RD_DUMMY_LENGTH_NOT_FOUND,
    FLASH_NOR_RET_CAL_FAILED,
    FLASH_NOR_RET_MALLOC_FAILED,
    FLASH_NOR_RET_HOOK_FUNC,
    FLASH_NOR_RET_SUSPEND_UNNECCESSARY,
    FLASH_NOR_RET_SET_4_BYTE_ADDRESS_MODE_FAILED,
    FLASH_NOR_RET_SUCCESS,
    FLASH_NOR_RET_SET_RD_DUMMY_LENGTH_FAIL
} FLASH_NOR_RET_TYPE;

/** @brief Nor flash channel. */
typedef enum
{
    FLASH_NOR_IDX_SPIC0,            /* Nor flash controlled by SPIC0 */
    FLASH_NOR_IDX_SPIC1,
    FLASH_NOR_IDX_SPIC2,
    FLASH_NOR_IDX_SPIC3,
    FLASH_NOR_IDX_MAX
} FLASH_NOR_IDX_TYPE;

/** @brief PSRAM channel. */
typedef enum
{
    PSRAM_IDX_SPIC0,
    PSRAM_IDX_SPIC1,               /* PSRAM controlled by SPIC1 */
    PSRAM_IDX_SPIC2,
    PSRAM_IDX_SPIC3,
    PSRAM_IDX_MAX
} PSRAM_IDX_TYPE;

/** @brief Nor flash bit mode. */
typedef enum
{
    FLASH_NOR_1_BIT_MODE,
    FLASH_NOR_2_BIT_MODE,
    FLASH_NOR_4_BIT_MODE,
    FLASH_NOR_DTR_4_BIT_MODE,
    FLASH_NOR_8_BIT_MODE,
} FLASH_NOR_BIT_MODE;

/**
  * \cond     private
  * \defgroup FMC_Private_type
  * \{
  */

typedef enum
{
    FLASH_NOR_REQ_NONE          = 0x00,
    FLASH_NOR_REQ_READ          = 0x01,
    FLASH_NOR_REQ_WRITE         = 0x02,
    FLASH_NOR_REQ_RW_MASK       = 0x03,

    FLASH_NOR_REQ_ERASE_SECTOR  = 0x04,
    FLASH_NOR_REQ_ERASE_BLOCK   = 0x08,
    FLASH_NOR_REQ_ERASE_CHIP    = 0x10,
    FLASH_NOR_REQ_ERASE_MASK    = 0x1C,

    FLASH_NOR_REQ_DMA_READ      = 0x20,
    FLASH_NOR_REQ_DMA_WRITE     = 0x40,
    FLASH_NOR_REQ_DMA_MASK      = 0x60,
} FLASH_NOR_REQ_TYPE;

typedef enum
{
    FLASH_NOR_EXIST_NONE        = 0,
    FLASH_NOR_EXIST_BASIC_CMD   = 1,
    FLASH_NOR_EXIST_ADV_CMD     = 2,
    FLASH_NOR_EXIST_QUERY_INFO  = 4,
    FLASH_NOR_EXIST_ALL         = 7,
} FLASH_NOR_EXIST_LV;


/****************************************************************************************
 * Nor Flash Callback Definition
 ****************************************************************************************/

typedef void (*FLASH_NOR_ASYNC_CB)(void);

/**
*  End of FMC_Private_type
* \}
* \endcond
*/


/** End of FMC_Exported_Types
  * @}
  */
/** End of FMC
 * @}
 */

#endif
