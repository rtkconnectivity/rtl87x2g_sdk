/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      bt_matter_adapter_app_main.c
   * @brief     Source file for BLE peripheral project, mainly used for initialize modules
   * @author
   * @date
   * @version
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <trace.h>
#include <os_sched.h>
#include <gap.h>
#include <gap_adv.h>
#include <gap_bond_le.h>
#include <profile_server.h>
#include <gap_msg.h>
#include <gap_conn_le.h>
#include "bt_matter_adapter_service.h"
#include "bt_matter_adapter_app_task.h"
#include "bt_matter_adapter_peripheral_app.h"
#include "bt_matter_adapter_app_main.h"
#include "board.h"
#include "io_dlps.h"
#include "app_section.h"

/*============================================================================*
 *                              Constants
 *============================================================================*/
/** @brief  Default minimum advertising interval when device is discoverable (units of 625us, 160=100ms) */
#define DEFAULT_ADVERTISING_INTERVAL_MIN            320
/** @brief  Default maximum advertising interval */
#define DEFAULT_ADVERTISING_INTERVAL_MAX            400

/*============================================================================*
 *                              Functions
 *============================================================================*/
/**
 * @brief  Config bt stack related feature
 *
 * NOTE: This function shall be called before @ref bte_init is invoked.
 * @return void
 */
void bt_matter_adapter_stack_config_init(uint8_t link_num)
{
//    gap_config_max_le_paired_device(link_num);
}

/**
 * @brief  Add GATT services and register callbacks
 * @return void
 */
void bt_matter_adapter_app_le_profile_init(void)
{
    server_init(1);
    bt_matter_adapter_service_add_service((void *)bt_matter_adapter_app_profile_callback);
    server_register_app_cb(bt_matter_adapter_app_profile_callback);
}

/**
 * @brief this function will be called before enter DLPS
 *
 *  set PAD and wakeup pin config for enterring DLPS
 *
 * @param none
 * @return none
 * @retval void
*/
RAM_FUNCTION
void app_enter_dlps_config(void)
{
    DBG_DIRECT("DLPS ENTER");

//    System_WakeUpDebounceTime(P2_7, PAD_DEB_FREQ_500Hz, 255);
//    System_WakeUpPinEnable(P2_7, PAD_WAKEUP_POL_HIGH, PAD_WAKEUP_DEB_ENABLE);

}

/**
 * @brief this function will be called after exit DLPS
 *
 *  set PAD and wakeup pin config for enterring DLPS
 *
 * @param none
 * @return none
 * @retval void
*/
RAM_FUNCTION
void app_exit_dlps_config(void)
{
    DBG_DIRECT("DLPS EXIT");
//    DBG_DIRECT("VENDOR 0X58 0x%x", HAL_READ32(SOC_VENDOR2_REG_BASE, 0x0058));
}

/*************************************** CHIP API *************************************************/

/**
  * @brief  Initialize peripheral and gap bond manager related parameters
  * @return void
  */
void bt_matter_adapter_app_le_gap_init_chip(void)
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
#if F_BT_LE_SMP_OOB_SUPPORT
    uint8_t  auth_oob = false;
#endif
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
#if F_BT_LE_SMP_OOB_SUPPORT
    gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(auth_oob), &auth_oob);
#endif
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY, sizeof(auth_fix_passkey), &auth_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, sizeof(auth_use_fix_passkey),
                      &auth_use_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_ENABLE, sizeof(auth_sec_req_enable), &auth_sec_req_enable);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_REQUIREMENT, sizeof(auth_sec_req_flags),
                      &auth_sec_req_flags);

    /* register gap message callback */
    le_register_app_cb(bt_matter_adapter_app_gap_callback);

#if (F_BT_LE_USE_STATIC_RANDOM_ADDR==1)
    T_APP_STATIC_RANDOM_ADDR random_addr;
    bool gen_addr = true;
    uint8_t local_bd_type = GAP_LOCAL_ADDR_LE_RANDOM;
#if 0
    if (ble_peripheral_app_load_static_random_address(&random_addr) == 0)
    {
        if (random_addr.is_exist == true)
        {
            gen_addr = false;
        }
    }
#endif
    if (gen_addr)
    {
        if (le_gen_rand_addr(GAP_RAND_ADDR_STATIC, random_addr.bd_addr) == GAP_CAUSE_SUCCESS)
        {
            random_addr.is_exist = true;
            // Don't save, we use a newly generated random address every boot
            //ble_peripheral_app_save_static_random_address(&random_addr);
            DBG_DIRECT("bd addr: %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                       random_addr.bd_addr[5],
                       random_addr.bd_addr[4],
                       random_addr.bd_addr[3],
                       random_addr.bd_addr[2],
                       random_addr.bd_addr[1],
                       random_addr.bd_addr[0]
                      );
        }
    }
    le_cfg_local_identity_address(random_addr.bd_addr, GAP_IDENT_ADDR_RAND);
    int ret1 = le_set_gap_param(GAP_PARAM_RANDOM_ADDR, 6, random_addr.bd_addr);
    //only for peripheral,broadcaster
    int ret2 = le_adv_set_param(GAP_PARAM_ADV_LOCAL_ADDR_TYPE, sizeof(local_bd_type), &local_bd_type);
    //only for central,observer
    //le_scan_set_param(GAP_PARAM_SCAN_LOCAL_ADDR_TYPE, sizeof(local_bd_type), &local_bd_type);
#endif
}

int bt_matter_adapter_init(uint8_t link_num)
{
    T_GAP_DEV_STATE new_state;

    le_get_gap_param(GAP_PARAM_DEV_STATE, &new_state);

    if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
    {
        APP_PRINT_INFO0("BT Stack already on");
    }
    else
    {
        bt_matter_adapter_stack_config_init(link_num);
        le_gap_init(link_num);
        bt_matter_adapter_app_le_profile_init();
    }

    bt_matter_adapter_app_le_gap_init_chip();
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

void bt_matter_adapter_device_name_set(void *device_name, uint8_t device_name_len)
{
    le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, device_name);
}

uint16_t bt_matter_adapter_mtu_get(uint16_t conn_id)
{
    uint16_t mtu_size = 0;

    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    return mtu_size;
}
