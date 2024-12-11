/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      peripheral_app.c
   * @brief     This file handles BLE peripheral application routines.
   * @author    jane
   * @date      2017-06-06
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
#include <string.h>
#include <trace.h>
#include <gap.h>
#include <gap_le.h>
#include <gap_msg.h>
#include <gap_adv.h>
#include <gap_bond_le.h>
#include <profile_server.h>
#include <app_msg.h>
#include <gap_conn_le.h>
#include "bt_matter_adapter_service.h"
#include "bt_matter_adapter_peripheral_app.h"

T_GAP_DEV_STATE bt_matter_adapter_gap_dev_state = {0, 0, 0, 0, 0};
P_CHIP_BLEMGR_CBACK chip_blemgr_cback = NULL;
void *chip_blemgr_context = NULL;

void bt_matter_adapter_cback_register(P_CHIP_BLEMGR_CBACK cback, void *context)
{
    chip_blemgr_cback = cback;
    chip_blemgr_context = context;
}

/*============================================================================*
 *                              Functions
 *============================================================================*/
void bt_matter_adapter_app_handle_gap_msg(T_IO_MSG *p_gap_msg)
{
    if (chip_blemgr_cback && chip_blemgr_context)
    {
        chip_blemgr_cback(chip_blemgr_context, (void *)p_gap_msg, 0, CB_GAP_MSG_CALLBACK);
    }
}

/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
void bt_matter_adapter_app_handle_io_msg(T_IO_MSG  io_msg)
{
    uint16_t msg_type = io_msg.type;

    switch (msg_type)
    {
    case IO_MSG_TYPE_BT_STATUS:
        {
            bt_matter_adapter_app_handle_gap_msg(&io_msg);
        }
        break;

    case IO_MSG_TYPE_QDECODE:
        {
            switch (io_msg.subtype)
            {
            case MATTER_BLE_ADV_STOP:
                le_adv_stop();
                break;

            case MATTER_BLE_ADV_START:
                le_adv_start();
                break;

            case MATTER_BLE_DISCONNECT:
                {
                    uint8_t conn_id = io_msg.u.param & 0xFF;

                    le_disconnect(conn_id);
                }
                break;

            default:
                break;
            }
        }
        break;

    default:
        break;
    }
}

/*============================================================================*
 *                              Functions
 *============================================================================*/
void bt_matter_adapter_app_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
{
    APP_PRINT_INFO3("bt_matter_adapter_app_handle_dev_state_evt: init state %d, adv state %d, cause 0x%x",
                    new_state.gap_init_state, new_state.gap_adv_state, cause);

    if (bt_matter_adapter_gap_dev_state.gap_init_state != new_state.gap_init_state)
    {
        if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
        {
            APP_PRINT_INFO0("GAP stack ready");
        }
    }

    bt_matter_adapter_gap_dev_state = new_state;
}

/**
  * @brief Callback for gap le to notify app
  * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
  * @retval result @ref T_APP_RESULT
  */
T_APP_RESULT bt_matter_adapter_app_gap_callback(uint8_t cb_type, void *p_cb_data)
{
    if (chip_blemgr_cback && chip_blemgr_context)
    {
        chip_blemgr_cback(chip_blemgr_context, p_cb_data, cb_type, CB_GAP_CALLBACK);
    }

    return APP_RESULT_SUCCESS;
}

/**
    * @brief    All the BT Profile service callback events are handled in this function
    * @note     Then the event handling function shall be called according to the
    *           service_id
    * @param    service_id  Profile service ID
    * @param    p_data      Pointer to callback data
    * @return   T_APP_RESULT, which indicates the function call is successful or not
    */
T_APP_RESULT bt_matter_adapter_app_profile_callback(T_SERVER_ID service_id, void *p_data)
{
    if (chip_blemgr_cback && chip_blemgr_context)
    {
        chip_blemgr_cback(chip_blemgr_context, p_data, service_id, CB_PROFILE_CALLBACK);
    }

    return APP_RESULT_SUCCESS;
}
