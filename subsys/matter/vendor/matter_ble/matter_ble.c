#include <stdio.h>
#include <trace.h>
#include <os_msg.h>
#include <gap.h>
#include <gap_adv.h>
#include <gap_le.h>
#include <gap_conn_le.h>
#include <app_msg.h>
#include "matter_ble.h"

static void *matter_ble_evt_queue_handle;  //!< Event queue handle
static void *matter_ble_io_queue_handle;   //!< IO queue handle
P_MATTER_BLE_CBACK matter_ble_cback = NULL;

void matter_ble_queue_init(void *evt_queue, void *io_queue)
{
    matter_ble_evt_queue_handle = evt_queue;
    matter_ble_io_queue_handle = io_queue;
}

void matter_ble_send_msg(uint16_t sub_type, uint32_t param)
{
    uint8_t event = EVENT_IO_TO_APP;
    T_IO_MSG io_msg;

    io_msg.type = IO_MSG_TYPE_QDECODE;
    io_msg.subtype = sub_type;
    io_msg.u.param = param;

    if (os_msg_send(matter_ble_io_queue_handle, &io_msg, 0) == false)
    {
        APP_PRINT_ERROR1("matter_ble_send_msg fail: subtype 0x%x", io_msg.subtype);
    }
    if (os_msg_send(matter_ble_evt_queue_handle, &event, 0) == false)
    {
        APP_PRINT_ERROR1("matter_ble_send_msg fail: subtype 0x%x", io_msg.subtype);
    }
}

void matter_ble_handle_io_msg(T_IO_MSG io_msg)
{
    uint16_t msg_type = io_msg.type;

    switch (msg_type)
    {
    case IO_MSG_TYPE_BT_STATUS:
        if (matter_ble_cback)
        {
            matter_ble_cback((void *)&io_msg, 0, CB_GAP_MSG_CALLBACK);
        }
        break;

    case IO_MSG_TYPE_QDECODE:
        {
            switch (io_msg.subtype)
            {
            case MATTER_BLE_MSG_ADV_STOP:
                le_adv_stop();
                break;

            case MATTER_BLE_MSG_ADV_START:
                le_adv_start();
                break;

            case MATTER_BLE_MSG_DISCONNECT:
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

void matter_ble_cback_register(P_MATTER_BLE_CBACK cback)
{
    matter_ble_cback = cback;
}

T_APP_RESULT matter_ble_profile_callback(T_SERVER_ID service_id, void *p_data)
{
    if (matter_ble_cback)
    {
        matter_ble_cback(p_data, service_id, CB_PROFILE_CALLBACK);
    }

    return APP_RESULT_SUCCESS;
}

void matter_ble_adv_start()
{
    matter_ble_send_msg(MATTER_BLE_MSG_ADV_START, 0);
}

void matter_ble_adv_stop()
{
    matter_ble_send_msg(MATTER_BLE_MSG_ADV_STOP, 0);
}

void matter_ble_adv_config(uint16_t adv_int_min,
                           uint16_t adv_int_max,
                           uint8_t  *adv_data,
                           uint8_t  adv_data_len)
{
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int_min), &adv_int_min);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int_max), &adv_int_max);
    le_adv_set_param(GAP_PARAM_ADV_DATA, adv_data_len, (void *)adv_data);
}

void matter_ble_scan_rsp_data_set(uint8_t *scan_rsp_data, uint8_t scan_rsp_data_len)
{
    le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, scan_rsp_data_len, (void *)scan_rsp_data);
}

void matter_ble_device_name_set(void *device_name, uint8_t device_name_len)
{
    le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, device_name);
}

uint16_t matter_ble_mtu_get(uint16_t conn_id)
{
    uint16_t mtu_size = 0;

    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    return mtu_size;
}

uint8_t matter_ble_disconnect(uint8_t conn_id)
{
    matter_ble_send_msg(MATTER_BLE_MSG_DISCONNECT, conn_id);
    return 0;
}

void matter_ble_handover()
{
#ifdef EXTERNAL_BLE
    matter_ble_send_msg(MATTER_BLE_MSG_HAND_OVER, 0);
#endif
}
