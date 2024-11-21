/**
*****************************************************************************************
*     Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      dfu_task.c
   * @brief     Routines to create App task and handle events & messages
   * @author    Grace
   * @date      2023-12-06
   * @version   v1.1
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2023 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/


#include "app_msg.h"
#include "dfu_task.h"
#include "dfu_app.h"
#include "dfu_main.h"
#include "gap.h"
#include "gap_le.h"
#include "os_msg.h"
#include "os_task.h"
#include "trace.h"

#if (SUPPORT_NORMAL_OTA == 1)
/*============================================================================*
 *                              Macros
 *============================================================================*/
#define DFU_TASK_PRIORITY               1
#define DFU_TASK_STACK_SIZE             512 * 8  //todo: sync with bee2, may modify

#define MAX_NUMBER_OF_GAP_MESSAGE     0x20  //!<  GAP message queue size
#define MAX_NUMBER_OF_IO_MESSAGE      0x20  //!<  IO message queue size
#define MAX_NUMBER_OF_GAP_TIMER       0x10    //!< indicate gap timer queue size
#define MAX_NUMBER_OF_EVENT_MESSAGE   (MAX_NUMBER_OF_GAP_MESSAGE + MAX_NUMBER_OF_IO_MESSAGE + MAX_NUMBER_OF_GAP_TIMER) //!< Event message queue size

/*============================================================================*
 *                              Variables
 *============================================================================*/
void *dfu_task_handle;
void *dfu_evt_queue_handle;
void *dfu_io_queue_handle;

/*============================================================================*
 *                              Functions
 *============================================================================*/
void dfu_main_task(void *p_param);

bool dfu_send_msg_to_dfutask(T_IO_MSG *p_msg)
{
    uint8_t event = EVENT_IO_TO_APP;

    if (os_msg_send(dfu_io_queue_handle, p_msg, 0) == false)
    {
        DFU_PRINT_ERROR0("dfu_send_msg_to_dfutask fail");
        return false;
    }
    if (os_msg_send(dfu_evt_queue_handle, &event, 0) == false)
    {
        DFU_PRINT_ERROR0("dfu_send_msg_to_dfutask fail");
        return false;
    }
    return true;
}




void dfu_task_init()
{
    os_task_create(&dfu_task_handle, "dfu", dfu_main_task, 0, DFU_TASK_STACK_SIZE,
                   DFU_TASK_PRIORITY);
}

/**
 * @brief        dfu task to handle events & messages
 * @param[in]    p_param    Parameters sending to the task
 * @return       void
 */
void dfu_main_task(void *p_param)
{
    uint8_t event;

    os_alloc_secure_ctx(1024);
    os_msg_queue_create(&dfu_io_queue_handle, "ioQ", MAX_NUMBER_OF_IO_MESSAGE, sizeof(T_IO_MSG));
    os_msg_queue_create(&dfu_evt_queue_handle, "evtQ", MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(uint8_t));

    gap_start_bt_stack(dfu_evt_queue_handle, dfu_io_queue_handle, MAX_NUMBER_OF_GAP_MESSAGE);

    dfu_timer_init();

    while (true)
    {
        if (os_msg_recv(dfu_evt_queue_handle, &event, 0xFFFFFFFF) == true)
        {
            if (event == EVENT_IO_TO_APP)
            {
                T_IO_MSG io_msg;
                if (os_msg_recv(dfu_io_queue_handle, &io_msg, 0) == true)
                {
                    dfu_handle_io_msg(io_msg);
                }
            }
            else
            {
                gap_handle_msg(event);
            }
        }
    }
}
#endif //end SUPPORT_NORMAL_OTA



