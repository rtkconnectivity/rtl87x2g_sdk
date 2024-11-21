/**
*****************************************************************************************
*     Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      dfu_task.h
   * @brief     Routines to create dfu task and handle events & messages
   * @author    Grace
   * @date      2023-12-06
   * @version   v1.1
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2023 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */
#ifndef _DFU_TASK_H_
#define _DFU_TASK_H_


/** @defgroup DFU_TASK DFU Task
  * @brief Peripheral DFU Task
  * @{
  */
extern void *dfu_evt_queue_handle;
/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup DFU_TASK_Exported_Functions DFU Task Exported Functions
  * @brief
  * @{
  */
/**
 * @brief        dfu task to handle events & messages
 * @param[in]    p_param    Parameters sending to the task
 * @return       void
 */
void dfu_main_task(void *p_param);

/**
 * @brief  Initialize DFU task
 * @return void
 */
void dfu_task_init(void);

/** End of DFU_TASK_Exported_Functions
  * @}
  */

/** End of DFU_TASK
  * @}
  */

#endif

