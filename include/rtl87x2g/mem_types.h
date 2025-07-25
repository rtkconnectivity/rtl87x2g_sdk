/**
*****************************************************************************************
*     Copyright (c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    Mem_types.h
  * @brief   Define memory types for RAM
  * @date    2017.6.6
  * @version v1.0
  * *************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
 */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _MEM_TYPES_H_
#define _MEM_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup MEM_CONFIG Memory Configure
  * @{
  */

/*============================================================================*
 *                               Types
*============================================================================*/
/** @defgroup MEM_CONFIG_Exported_Types Memory Configure Exported Types
  * @{
  */

typedef enum
{
    RAM_TYPE_DATA_ON,                         /**< DTCM Heap.   */
    RAM_TYPE_BUFFER_ON,                       /**< Buffer Heap. */
    RAM_TYPE_EXT_DATA_SRAM,                   /**< Ext DATA SRAM Heap. */
    RAM_TYPE_NUM                              /**< MAX RAM Type Number. */
} RAM_TYPE;

/** End of MEM_TYPES_Exported_Types
  * @}
  */

/** End of MEM_CONFIG
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /* _MEM_TYPES_H_ */
