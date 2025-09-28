#include <stdio.h>
#include <trace.h>
#include <string.h>
#include <os_msg.h>
#include <gap.h>
#include <gap_adv.h>
#include <gap_le.h>
#include <gap_conn_le.h>
#include <app_msg.h>
#include "matter_ble.h"

#if (SUPPORT_BLE_OTA == 1)
#include "ble_dfu_transport.h"
#include "dfu_common.h"
#endif

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

#if MATTER_ENABLE_CFU
    if (sub_type == MATTER_BLE_MSG_ENTER_CFU)
    {
        io_msg.type = IO_MSG_TYPE_UART;
    }
    else
#endif
    {
        io_msg.type = IO_MSG_TYPE_QDECODE;
        io_msg.subtype = sub_type;
        io_msg.u.param = param;
    }

    if (os_msg_send(matter_ble_io_queue_handle, &io_msg, 0) == false)
    {
        APP_PRINT_ERROR1("matter_ble_send_msg fail: subtype 0x%x", io_msg.subtype);
    }
    if (os_msg_send(matter_ble_evt_queue_handle, &event, 0) == false)
    {
        APP_PRINT_ERROR1("matter_ble_send_msg fail: subtype 0x%x", io_msg.subtype);
    }
}

#if (SUPPORT_BLE_OTA == 1)
void matter_ble_handle_conn_state_evt(uint8_t conn_id, T_GAP_CONN_STATE new_state,
                                      uint16_t disc_cause)
{
    MATTER_PRINT_INFO3("matter_ble_handle_conn_state_evt: conn_id %d, new_state %d, disc_cause 0x%x",
                       conn_id, new_state, disc_cause);
    switch (new_state)
    {
    case GAP_CONN_STATE_DISCONNECTED:
        {
            if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE))
                && (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE)))
            {
                MATTER_PRINT_ERROR1("matter_ble_handle_conn_state_evt: connection lost cause 0x%x", disc_cause);
            }

            if (dfu_switch_to_ota_mode_pending)
            {
#if (SUPPORT_NORMAL_OTA == 1)
                dfu_switch_to_ota_mode_pending = false;

                dfu_switch_to_ota_mode();
#endif
            }
            else
            {
                if (dfu_active_reset_pending)
                {
                    DBG_DIRECT("OTA APP Active reset....");
                    dfu_active_reset_pending = false;

#if (ENABLE_AUTO_BANK_SWITCH == 1)
                    if (is_ota_support_bank_switch())
                    {
                        uint32_t ota_addr;
                        ota_addr = get_header_addr_by_img_id(IMG_OTA);
                        DBG_DIRECT("FOR QC Test: Bank switch Erase OTA Header=0x%x", ota_addr);
                        fmc_flash_nor_erase(ota_addr, FMC_FLASH_NOR_ERASE_SECTOR);
                    }
#endif

                    //unlock_flash_bp_all();   //GRACE TO CHECK
                    dfu_fw_reboot(RESET_ALL, DFU_ACTIVE_RESET);
                }
            }
        }
        break;

    default:
        break;
    }
}

void matter_ble_handle_gap_msg(T_IO_MSG *p_gap_msg)
{
    T_LE_GAP_MSG gap_msg;
    uint8_t conn_id;
    memcpy(&gap_msg, &p_gap_msg->u.param, sizeof(p_gap_msg->u.param));

    MATTER_PRINT_TRACE1("matter_ble_handle_gap_msg: subtype %d", p_gap_msg->subtype);
    switch (p_gap_msg->subtype)
    {
    case GAP_MSG_LE_CONN_STATE_CHANGE:
        {
            matter_ble_handle_conn_state_evt(gap_msg.msg_data.gap_conn_state_change.conn_id,
                                             (T_GAP_CONN_STATE)gap_msg.msg_data.gap_conn_state_change.new_state,
                                             gap_msg.msg_data.gap_conn_state_change.disc_cause);
        }
        break;

    default:
        break;
    }
}
#endif

extern bool zbmac_pm_check_inactive(void);
extern void zbmac_pm_initiate_wakeup(void);

static volatile bool matter_ble_is_connected = false;
bool matter_ble_check_connected(void)
{
    return matter_ble_is_connected;
}

void matter_ble_handle_io_msg(T_IO_MSG io_msg)
{
    uint16_t msg_type = io_msg.type;
    T_LE_GAP_MSG gap_msg;
    memcpy(&gap_msg, &io_msg.u.param, sizeof(io_msg.u.param));

    switch (msg_type)
    {
    case IO_MSG_TYPE_BT_STATUS:
        {
#if (SUPPORT_BLE_OTA == 1)
            matter_ble_handle_gap_msg(&io_msg);
#endif
            switch (io_msg.subtype)
            {
                case GAP_MSG_LE_CONN_STATE_CHANGE:
                    {
                        uint16_t conn_id    = gap_msg.msg_data.gap_conn_state_change.conn_id;
                        uint16_t new_state  = gap_msg.msg_data.gap_conn_state_change.new_state;
                        uint16_t disc_cause = gap_msg.msg_data.gap_conn_state_change.disc_cause;
                        if (new_state == GAP_CONN_STATE_CONNECTED)
                        {
                            matter_ble_is_connected = true;
                            if (zbmac_pm_check_inactive())
                            {
                                zbmac_pm_initiate_wakeup();
                            }
                        }
                        else if (new_state == GAP_CONN_STATE_DISCONNECTED)
                        {
                            matter_ble_is_connected = false;
                        }
                    }
                    break;

                default:
                    break;
            }

            if (matter_ble_cback)
            {
                matter_ble_cback((void *)&io_msg, 0, CB_GAP_MSG_CALLBACK);
            }
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

#if MATTER_ENABLE_CFU
    case IO_MSG_TYPE_UART:
        {
            extern void enter_to_cfu_mode(const uint8_t *data, uint8_t data_len);

            uint8_t data[] = {0x5d, 0x00, 0x01};
            enter_to_cfu_mode(data, sizeof(data));
        }
        break;
#endif

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
