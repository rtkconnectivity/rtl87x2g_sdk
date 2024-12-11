#ifndef _MATTER_BLE_H_
#define _MATTER_BLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <profile_server.h>

typedef enum
{
    MATTER_BLE_MSG_ADV_STOP = 0,
    MATTER_BLE_MSG_ADV_START = 1,
    MATTER_BLE_MSG_DISCONNECT = 2,
    MATTER_BLE_MSG_HAND_OVER = 3,
    MATTER_BLE_MSG_MAX,
} T_MATTER_BLE_MSG_TYPE;

typedef enum
{
    CB_PROFILE_CALLBACK = 0x1,
    CB_GAP_MSG_CALLBACK = 0x2,
} T_CHIP_BLEMGR_CALLBACK_TYPE;

typedef int (*P_MATTER_BLE_CBACK)(void                       *cb_data,
                                  int                         type,
                                  T_CHIP_BLEMGR_CALLBACK_TYPE callback_type);

void matter_ble_cback_register(P_MATTER_BLE_CBACK cback);

int matter_ble_init(uint8_t link_num);

void matter_ble_queue_init(void *evt_queue, void *io_queue);

void matter_ble_adv_start();

void matter_ble_adv_stop();

void matter_ble_adv_config(uint16_t adv_int_min,
                           uint16_t adv_int_max,
                           uint8_t *adv_data,
                           uint8_t  adv_data_len);

void matter_ble_scan_rsp_data_set(uint8_t *scan_rsp_data, uint8_t scan_rsp_data_len);

void matter_ble_device_name_set(void *device_name, uint8_t device_name_len);

uint16_t matter_ble_mtu_get(uint16_t conn_id);

uint8_t matter_ble_disconnect(uint8_t conn_id);

/**
 * @brief    All the BT Profile service callback events are handled in this function
 * @note     Then the event handling function shall be called according to the
 *           service_id.
 * @param[in] service_id  Profile service ID
 * @param[in] p_data      Pointer to callback data
 * @return   Indicates the function call is successful or not
 * @retval   result @ref T_APP_RESULT
 */
T_APP_RESULT matter_ble_profile_callback(T_SERVER_ID service_id, void *p_data);

void matter_ble_handle_io_msg(T_IO_MSG io_msg);

void matter_ble_handover();

#ifdef __cplusplus
}
#endif
#endif
