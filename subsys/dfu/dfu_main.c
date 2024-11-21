/**
**********************************************************************************************************
*               Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     dfu_mian.c
* @brief    normal ota main
* @details
* @author   Grace
* @date     2023-12-06
* @version  v1.1
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2023 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "dfu_common.h"
#include "dfu_service.h"
#include "dfu_main.h"
#include "dfu_task.h"
#include "dfu_app.h"
#include "fmc_api.h"
#include "fmc_platform.h"
#include "gap.h"
#include "gap_adv.h"
#include "gap_bond_le.h"
#include "profile_server.h"
#include "rtl_wdt.h"
#include "os_sync.h"
#include "os_timer.h"
#include "trace.h"

#if (SUPPORT_NORMAL_OTA == 1)
/*============================================================================*
 *                              Macros
 *============================================================================*/
#define TIMER_ID_DFU_TOTAL              1
#define TIMER_ID_DFU_WAIT4_CONN         2


#define BD_ADDR_SIZE                    6

/* What is the advertising interval when device is discoverable (units of 625us, 160=100ms)*/
#define DEFAULT_ADVERTISING_INTERVAL_MIN            160 /* 100ms */
#define DEFAULT_ADVERTISING_INTERVAL_MAX            176 /* 110ms */

/*============================================================================*
 *                               Types
 *============================================================================*/


/*============================================================================*
 *                               Variables
 *============================================================================*/
void *normal_ota_total_timer_handle;
void *normal_ota_wait4_conn_timer_handle;

bool is_normal_ota_mode = false;
T_SERVER_ID rtk_dfu_service_id;

/*============================================================================*
 *                              Local Functions
 *============================================================================*/
/*
 * @fn          Initial gap parameters
 * @brief      Initialize peripheral and gap bond manager related parameters
 *
 * @return     void
 */
void dfu_le_gap_init(void)
{
    uint16_t appearance = GAP_GATT_APPEARANCE_KEYBOARD;
    uint8_t  slave_init_mtu_req = true;

    //advertising parameters
    uint8_t  adv_evt_type = GAP_ADTYPE_ADV_IND;
    uint8_t  adv_direct_type = GAP_REMOTE_ADDR_LE_PUBLIC;
    uint8_t  adv_direct_addr[GAP_BD_ADDR_LEN] = {0};
    uint8_t  adv_chann_map = GAP_ADVCHAN_ALL;
    uint8_t  adv_filter_policy = GAP_ADV_FILTER_ANY;
    uint16_t adv_int_min = DEFAULT_ADVERTISING_INTERVAL_MIN;
    uint16_t adv_int_max = DEFAULT_ADVERTISING_INTERVAL_MIN;
    uint8_t local_bd_type = GAP_LOCAL_ADDR_LE_RANDOM;

    //scan response data
    uint8_t scan_rsp_data[] =
    {
        /*Complete local name*/
        7,                                    /* default target name length */
        GAP_ADTYPE_LOCAL_NAME_COMPLETE,       /* type="Complete local name" */
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
    };

    scan_rsp_data[2] = 'B';
    scan_rsp_data[3] = 'e';
    scan_rsp_data[4] = 'e';
    scan_rsp_data[5] = 'T';
    scan_rsp_data[6] = 'g';
    scan_rsp_data[7] = 't';
    scan_rsp_data[8] = '\0';
    scan_rsp_data[9] = '\0';
    if (scan_rsp_data[9] != '\0')
    {
        scan_rsp_data[0] = 0x09; /* target name length */
    }
    else
    {
        scan_rsp_data[0] = strlen((char *)(scan_rsp_data + 2)) + 1;
    }

    //GAP Bond Manager parameters
    uint8_t  gap_param_cccd_storage = false;
    uint8_t  auth_pair_mode = GAP_PAIRING_MODE_PAIRABLE;
    uint16_t auth_flags = GAP_AUTHEN_BIT_BONDING_FLAG | GAP_AUTHEN_BIT_MITM_FLAG;
    uint8_t  auth_io_cap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
#ifndef SUPPORT_ALONE_UPPERSTACK_IMG
    uint8_t  auth_oob = false;
#endif
    uint8_t  auth_use_fix_passkey = false;
    uint32_t auth_fix_passkey = 0;
    uint8_t  auth_sec_req_enalbe = false;
    uint16_t auth_sec_req_flags = GAP_AUTHEN_BIT_NONE;

    //Set device appearance
    le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(appearance), &appearance);
    le_set_gap_param(GAP_PARAM_SLAVE_INIT_GATT_MTU_REQ, sizeof(slave_init_mtu_req),
                     &slave_init_mtu_req);

    //Set advertising parameters
    le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(adv_evt_type), &adv_evt_type);
    le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, sizeof(adv_direct_type), &adv_direct_type);
    le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR, sizeof(adv_direct_addr), adv_direct_addr);
    le_adv_set_param(GAP_PARAM_ADV_CHANNEL_MAP, sizeof(adv_chann_map), &adv_chann_map);
    le_adv_set_param(GAP_PARAM_ADV_FILTER_POLICY, sizeof(adv_filter_policy), &adv_filter_policy);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int_min), &adv_int_min);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int_max), &adv_int_max);
    le_adv_set_param(GAP_PARAM_ADV_LOCAL_ADDR_TYPE, sizeof(local_bd_type), &local_bd_type);
    le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, scan_rsp_data[0] + 1, scan_rsp_data);

    // Setup the GAP Bond Manager
    le_bond_set_param(GAP_PARAM_BOND_CCCD_STORAGE, sizeof(gap_param_cccd_storage),
                      &gap_param_cccd_storage);
    gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
    gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(auth_flags), &auth_flags);
    gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(auth_io_cap), &auth_io_cap);
#ifndef SUPPORT_ALONE_UPPERSTACK_IMG
    gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(auth_oob), &auth_oob);
#endif
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY, sizeof(auth_fix_passkey), &auth_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, sizeof(auth_use_fix_passkey),
                      &auth_use_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_ENABLE, sizeof(auth_sec_req_enalbe), &auth_sec_req_enalbe);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_REQUIREMENT, sizeof(auth_sec_req_flags),
                      &auth_sec_req_flags);

    //Register gap callback
    le_register_app_cb(dfu_gap_callback);
}

/******************************************************************
 * @fn          Initial profile
 * @brief      Add simple profile service and register callbacks
 *
 * @return     void
 */
void dfu_le_profile_init(void)
{
    server_init(1);
    rtk_dfu_service_id = dfu_add_service(dfu_profile_callback);
    server_register_app_cb(dfu_profile_callback);
}

void dfu_monitor_timeout_handler(void *p_xtimer)
{
    uint32_t timer_id = 0;

    os_timer_id_get(&p_xtimer, &timer_id);

    DFU_PRINT_ERROR1("dfu_monitor_timeout_handler: TimerID(%u)", timer_id);

    switch (timer_id)
    {
    default:
        break;
    case TIMER_ID_DFU_TOTAL:
    case TIMER_ID_DFU_WAIT4_CONN:
        dfu_fw_reboot(RESET_ALL, DFU_TIMEOUT_RESET);
        break;
    }
}

void dfu_timer_init(void)
{
    os_timer_create(&normal_ota_total_timer_handle, "dfuTotalTimer", TIMER_ID_DFU_TOTAL,
                    NORMAL_OTA_TIMEOUT_TOTAL * 1000, false, dfu_monitor_timeout_handler);
    os_timer_create(&normal_ota_wait4_conn_timer_handle, "dfuWait4ConTimer", TIMER_ID_DFU_WAIT4_CONN,
                    NORMAL_OTA_TIMEOUT_WAIT4_CONN * 1000, false, dfu_monitor_timeout_handler);

    os_timer_start(&normal_ota_total_timer_handle);
    os_timer_start(&normal_ota_wait4_conn_timer_handle);
}

void dfu_init(void)
{
    WDT_Disable();

    is_normal_ota_mode = true;

    uint32_t s;
    s = os_lock();
    if (fmc_flash_nor_set_bp_lv(FMC_MAIN0_ADDR, 0))
    {
        DFU_PRINT_INFO0("[==>dfu_init: Flash unlock BP all success!");
    }
    else
    {
        DBG_DIRECT("dfu init unlock BP fail!");
    }
    os_unlock(s);
}
/*============================================================================*
 *                              Local Functions
 *============================================================================*/
void dfu_set_rand_addr(void)
{
    T_GAP_RAND_ADDR_TYPE rand_addr_type = GAP_RAND_ADDR_NON_RESOLVABLE;
    uint8_t random_bd[BD_ADDR_SIZE] = {0};
    le_gen_rand_addr(rand_addr_type, random_bd);
    DFU_PRINT_INFO1("dfu_set_rand_addr: rand_addr %b", TRACE_BDADDR(random_bd));
    le_set_rand_addr(random_bd);
}

void dfu_main(void)
{
    DBG_DIRECT("Enter DFU mode");
    le_gap_init(1);
    gap_lib_init();
    dfu_le_gap_init();
    dfu_le_profile_init();
    dfu_init();
    dfu_task_init();
}

#endif //end SUPPORT_NORMAL_OTA



