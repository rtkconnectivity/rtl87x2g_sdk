#include <stdio.h>
#include <trace.h>
#include <os_msg.h>
#include <os_task.h>
#include <gap.h>
#include <gap_adv.h>
#include <gap_bond_le.h>
#include <profile_server.h>
#include <gap_msg.h>
#include <gap_conn_le.h>
#include <app_msg.h>
#include "matter_ble.h"
#include "matter_ble_service.h"

#if (SUPPORT_BLE_OTA == 1)
#include "ota_service.h"
#include "dfu_service.h"
#endif

#ifndef EXTERNAL_BLE

#define APP_TASK_PRIORITY             4         //!< Task priorities
#define APP_TASK_STACK_SIZE           1024 * 4   //!<  Task stack size
#define MAX_NUMBER_OF_GAP_MESSAGE     0x20      //!<  GAP message queue size
#define MAX_NUMBER_OF_IO_MESSAGE      0x20      //!<  IO message queue size
#define MAX_NUMBER_OF_EVENT_MESSAGE   (MAX_NUMBER_OF_GAP_MESSAGE + MAX_NUMBER_OF_IO_MESSAGE)    //!< Event message queue size

static void *bt_matter_app_task_handle;   //!< APP Task handle
static void *bt_matter_evt_queue_handle;  //!< Event queue handle
static void *bt_matter_io_queue_handle;   //!< IO queue handle

/** @brief  Default minimum advertising interval when device is discoverable (units of 625us, 160=100ms) */
#define DEFAULT_ADVERTISING_INTERVAL_MIN            320
/** @brief  Default maximum advertising interval */
#define DEFAULT_ADVERTISING_INTERVAL_MAX            400

void matter_ble_profile_init(void)
{
#if (SUPPORT_BLE_OTA == 1)
    server_init(3);
    ota_add_service(NULL);
    dfu_add_service(matter_ble_profile_callback);
#else
    server_init(1);
#endif
    matter_ble_add_service((void *)matter_ble_profile_callback);
    server_register_app_cb(matter_ble_profile_callback);
}

void matter_ble_gap_init(void)
{
    /* Device name and device appearance */
    //uint8_t  device_name[GAP_DEVICE_NAME_LEN] = "Ameba_xxyyzz";
    uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;
    uint8_t  slave_init_mtu_req = true;

    /* Advertising parameters */
    uint8_t  adv_evt_type = GAP_ADTYPE_ADV_IND;
    uint8_t  adv_direct_type = GAP_REMOTE_ADDR_LE_PUBLIC;
    uint8_t  adv_direct_addr[GAP_BD_ADDR_LEN] = {0};
    uint8_t  adv_chann_map = GAP_ADVCHAN_ALL;
    uint8_t  adv_filter_policy = GAP_ADV_FILTER_ANY;
    uint16_t adv_int_min = DEFAULT_ADVERTISING_INTERVAL_MIN;
    uint16_t adv_int_max = DEFAULT_ADVERTISING_INTERVAL_MIN;

    /* GAP Bond Manager parameters */
    uint8_t  auth_pair_mode = GAP_PAIRING_MODE_PAIRABLE;
    uint16_t auth_flags = GAP_AUTHEN_BIT_BONDING_FLAG;
    uint8_t  auth_io_cap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
    uint8_t  auth_use_fix_passkey = false;
    uint32_t auth_fix_passkey = 0;
    uint8_t  auth_sec_req_enable = false;
    uint16_t auth_sec_req_flags = GAP_AUTHEN_BIT_BONDING_FLAG;

    /* Set device name and device appearance */
    //le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, device_name);
    le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(appearance), &appearance);
    le_set_gap_param(GAP_PARAM_SLAVE_INIT_GATT_MTU_REQ, sizeof(slave_init_mtu_req),
                     &slave_init_mtu_req);

    /* Set advertising parameters */
    le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(adv_evt_type), &adv_evt_type);
    le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, sizeof(adv_direct_type), &adv_direct_type);
    le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR, sizeof(adv_direct_addr), adv_direct_addr);
    le_adv_set_param(GAP_PARAM_ADV_CHANNEL_MAP, sizeof(adv_chann_map), &adv_chann_map);
    le_adv_set_param(GAP_PARAM_ADV_FILTER_POLICY, sizeof(adv_filter_policy), &adv_filter_policy);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int_min), &adv_int_min);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int_max), &adv_int_max);

    /* Setup the GAP Bond Manager */
    gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
    gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(auth_flags), &auth_flags);
    gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(auth_io_cap), &auth_io_cap);
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY, sizeof(auth_fix_passkey), &auth_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, sizeof(auth_use_fix_passkey),
                      &auth_use_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_ENABLE, sizeof(auth_sec_req_enable), &auth_sec_req_enable);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_REQUIREMENT, sizeof(auth_sec_req_flags),
                      &auth_sec_req_flags);

    /* register gap message callback */
    //le_register_app_cb(bt_matter_adapter_app_gap_callback);

    uint8_t bd_addr[GAP_BD_ADDR_LEN];
    uint8_t local_bd_type = GAP_LOCAL_ADDR_LE_RANDOM;

    if (le_gen_rand_addr(GAP_RAND_ADDR_STATIC, bd_addr) == GAP_CAUSE_SUCCESS)
    {
        APP_PRINT_INFO1("bd_addr %s", TRACE_BDADDR(bd_addr));
    }

    le_cfg_local_identity_address(bd_addr, GAP_IDENT_ADDR_RAND);
    le_set_gap_param(GAP_PARAM_RANDOM_ADDR, 6, bd_addr);
    //only for peripheral,broadcaster
    le_adv_set_param(GAP_PARAM_ADV_LOCAL_ADDR_TYPE, sizeof(local_bd_type), &local_bd_type);
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
    matter_ble_queue_init(bt_matter_evt_queue_handle, bt_matter_io_queue_handle);

    while (1)
    {
        if (os_msg_recv(bt_matter_evt_queue_handle, &event, 0xFFFFFFFF) == true)
        {
            if (event == EVENT_IO_TO_APP)
            {
                T_IO_MSG io_msg;
                if (os_msg_recv(bt_matter_io_queue_handle, &io_msg, 0) == true)
                {
                    matter_ble_handle_io_msg(io_msg);
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

int matter_ble_init(uint8_t link_num)
{
    T_GAP_DEV_STATE new_state;

    le_get_gap_param(GAP_PARAM_DEV_STATE, &new_state);

    if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
    {
        APP_PRINT_INFO0("BT Stack already on");
    }
    else
    {
        le_gap_init(link_num);
        matter_ble_profile_init();
    }

    matter_ble_gap_init();
    bt_matter_adapter_app_task_init();

    //Wait BT init complete*
    do
    {
        //os_delay(100);
        le_get_gap_param(GAP_PARAM_DEV_STATE, &new_state);
    }
    while (new_state.gap_init_state != GAP_INIT_STATE_STACK_READY);

    return 0;
}

#else //#ifndef EXTERNAL_BLE

int __attribute__((weak)) matter_ble_init(uint8_t link_num)
{
    return 0;
}

#endif //#ifndef EXTERNAL_BLE