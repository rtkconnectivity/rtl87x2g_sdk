/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      bt_matter_adapter_app_task.c
   * @brief     Routines to create App task and handle events & messages
   * @author    jane
   * @date      2017-06-02
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <stdio.h>
#include <trace.h>
#include <os_msg.h>
#include <os_task.h>
#include <gap.h>
#include <gap_adv.h>
#include <gap_le.h>
#include <app_msg.h>
#include <bt_matter_adapter_app_task.h>
#include <bt_matter_adapter_peripheral_app.h>

/** @defgroup  BT_MATTER_ADAPTER_PERIPH_APP_TASK BT Config Peripheral App Task
    * @brief This file handles the implementation of application task related functions.
    *
    * Create App task and handle events & messages
    * @{
    */
/*============================================================================*
 *                              Macros
 *============================================================================*/
#define APP_TASK_PRIORITY             4         //!< Task priorities
#define APP_TASK_STACK_SIZE           1024 * 4   //!<  Task stack size
#define MAX_NUMBER_OF_GAP_MESSAGE     0x20      //!<  GAP message queue size
#define MAX_NUMBER_OF_IO_MESSAGE      0x20      //!<  IO message queue size
#define MAX_NUMBER_OF_EVENT_MESSAGE   (MAX_NUMBER_OF_GAP_MESSAGE + MAX_NUMBER_OF_IO_MESSAGE)    //!< Event message queue size

/*============================================================================*
 *                              Variables
 *============================================================================*/
static void *bt_matter_app_task_handle;   //!< APP Task handle
static void *bt_matter_evt_queue_handle;  //!< Event queue handle
static void *bt_matter_io_queue_handle;   //!< IO queue handle
extern T_GAP_DEV_STATE bt_matter_adapter_gap_dev_state;

/*============================================================================*
 *                              Functions
 *============================================================================*/

void bt_matter_adapter_send_msg(uint16_t sub_type, uint32_t param)
{
    uint8_t event = EVENT_IO_TO_APP;
    T_IO_MSG io_msg;

    io_msg.type = IO_MSG_TYPE_QDECODE;
    io_msg.subtype = sub_type;
    io_msg.u.param = param;

    if (os_msg_send(bt_matter_io_queue_handle, &io_msg, 0) == false)
    {
        APP_PRINT_ERROR1("bt_matter_adapter_send_msg fail: subtype 0x%x", io_msg.subtype);
    }
    if (os_msg_send(bt_matter_evt_queue_handle, &event, 0) == false)
    {
        APP_PRINT_ERROR1("bt_matter_adapter_send_msg fail: subtype 0x%x", io_msg.subtype);
    }
}

void bt_matter_adapter_app_main_task(void *p_param)
{
    (void) p_param;
    uint8_t event;

#if defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1)
    osif_create_secure_context(configMINIMAL_SECURE_STACK_SIZE + 256);
#endif

    os_msg_queue_create(&bt_matter_io_queue_handle, "ioQ", MAX_NUMBER_OF_IO_MESSAGE, sizeof(T_IO_MSG));
    os_msg_queue_create(&bt_matter_evt_queue_handle, "evtQ", MAX_NUMBER_OF_EVENT_MESSAGE,
                        sizeof(uint8_t));

    gap_start_bt_stack(bt_matter_evt_queue_handle, bt_matter_io_queue_handle,
                       MAX_NUMBER_OF_GAP_MESSAGE);

    while (1)
    {
        if (os_msg_recv(bt_matter_evt_queue_handle, &event, 0xFFFFFFFF) == true)
        {
            if (event == EVENT_IO_TO_APP)
            {
                T_IO_MSG io_msg;
                if (os_msg_recv(bt_matter_io_queue_handle, &io_msg, 0) == true)
                {
                    bt_matter_adapter_app_handle_io_msg(io_msg);
                }
            }
            else
            {
                gap_handle_msg(event);
            }
        }
    }
}

void bt_matter_adapter_app_task_init()
{
    if (bt_matter_app_task_handle == NULL)
    {
        os_task_create(&bt_matter_app_task_handle, "bt_matter_adapter_app", bt_matter_adapter_app_main_task,
                       0, APP_TASK_STACK_SIZE, APP_TASK_PRIORITY);
    }
    else
    {
        APP_PRINT_ERROR0("bt_matter_adapter_app_task_init fail");
    }
}

void bt_matter_adapter_app_task_deinit()
{
    //gap_stop_bt_stack
    if (bt_matter_app_task_handle)
    {
        os_task_delete(bt_matter_app_task_handle);
    }
    if (bt_matter_io_queue_handle)
    {
        os_msg_queue_delete(bt_matter_io_queue_handle);
    }
    if (bt_matter_evt_queue_handle)
    {
        os_msg_queue_delete(bt_matter_evt_queue_handle);
    }

    bt_matter_io_queue_handle = NULL;
    bt_matter_evt_queue_handle = NULL;
    bt_matter_app_task_handle = NULL;

    bt_matter_adapter_gap_dev_state.gap_init_state = 0;
    bt_matter_adapter_gap_dev_state.gap_adv_sub_state = 0;
    bt_matter_adapter_gap_dev_state.gap_adv_state = 0;
    bt_matter_adapter_gap_dev_state.gap_scan_state = 0;
    bt_matter_adapter_gap_dev_state.gap_conn_state = 0;
}

void bt_matter_adapter_adv_start()
{
    bt_matter_adapter_send_msg(MATTER_BLE_ADV_START, 0);
}

void bt_matter_adapter_adv_stop()
{
    bt_matter_adapter_send_msg(MATTER_BLE_ADV_STOP, 0);
}

void bt_matter_adapter_adv_config(uint16_t adv_int_min,
                                  uint16_t adv_int_max,
                                  uint8_t  *adv_data,
                                  uint8_t  adv_data_len)
{
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int_min), &adv_int_min);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int_max), &adv_int_max);
    le_adv_set_param(GAP_PARAM_ADV_DATA, adv_data_len, (void *)adv_data);
}

uint8_t bt_matter_adapter_le_disconnect(uint8_t conn_id)
{
    bt_matter_adapter_send_msg(MATTER_BLE_DISCONNECT, conn_id);
    return 0;
}

/** @} */ /* End of group PERIPH_APP_TASK */

