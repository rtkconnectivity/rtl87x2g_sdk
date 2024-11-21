/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     vector_table_ext.h
* @brief    Non-secure Vector table extend implementaion header file
* @details
* @author   Grace
* @date     2021-9-02
* @version  v1.0
*********************************************************************************************************
*/

#include <stdbool.h>
#include "vector_table.h"

/** @defgroup  VECTOR_TABLE   Vector table
  * @brief
  * @{
  */

/** @defgroup VECTOR_TABLE_Exported_Functions Vector table Exported Functions
  * @brief
  * @{
  */
/**
 * @brief  Update ISR Handler in RAM Vector Table.
 * @param  v_num: Vector number(index)
 * @param  isr_handler: User defined ISR Handler.
 * @retval true: Success
 *         false: Fail
 */
bool RamVectorTableUpdate_ext(VECTORn_Type v_num, IRQ_Fun isr_handler);

/** End of VECTOR_TABLE_Exported_Functions
  * @}
  */

/** End of VECTOR_TABLE
  * @}
  */
