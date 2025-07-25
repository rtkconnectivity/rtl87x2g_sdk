/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_ext_scan.h
* @brief     Header file for GAP ext scan
* @details
* @author    jane
* @date      2016-02-18
* @version   v1.0
* *********************************************************************************************************
*/

/* Define to prevent recursive inclusion **/
#ifndef GAP_EXT_SCAN_H
#define GAP_EXT_SCAN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "gap_le.h"

/** @addtogroup BT_Host Bluetooth Host
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_EXTENDED_SCAN GAP LE Extended Scan Module
  * @brief GAP LE Extended Scan Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_EXTENDED_SCAN_Exported_Macros GAP LE Extended Scan Exported Macros
  * @brief
  * @{
  */

/** @defgroup EXT_SCAN_DEFs Extended Scan Definitions
  * @brief Extended scan related definitions.
  * @{
  */
#define GAP_EXT_SCAN_MAX_PHYS_NUM               2     /**< Maximum number of scan PHYs. */
/** End of EXT_SCAN_DEFs
  * @}
  */

/** @defgroup EXT_SCAN_PHY Extended Scan PHY
  * @brief Indicate the PHY(s) on which the advertising packets should be received
           on the primary advertising channel. LE 1M PHY and LE Coded PHY are primary
           advertising channel.
  * @{
  */
#define GAP_EXT_SCAN_PHYS_1M_BIT                0x01   /**< Scan advertisements on the LE 1M PHY. */
#define GAP_EXT_SCAN_PHYS_CODED_BIT             0x04   /**< Scan advertisements on the LE Coded PHY. */
#define GAP_EXT_SCAN_PHYS_ALL                   0x05   /**< Scan advertisements on the LE 1M PHY and LE Coded PHY. */
/** End of EXT_SCAN_PHY
  * @}
  */

/** @defgroup EXT_ADV_REPORT Extended Advertising Report Flag
  * @brief Indicate properties of advertising event in extended advertising report.
           When a scan response is received, bits 0-2 and 4 of event type shall
           indicate the properties of the original advertising event.
  * @{
  */
#define GAP_EXT_ADV_REPORT_BIT_CONNECTABLE_ADV  0x01   /**< Connectable advertising. */
#define GAP_EXT_ADV_REPORT_BIT_SCANNABLE_ADV    0x02   /**< Scannable advertising. */
#define GAP_EXT_ADV_REPORT_BIT_DIRECTED_ADV     0x04   /**< Directed advertising. */
#define GAP_EXT_ADV_REPORT_BIT_SCAN_RESPONSE    0x08   /**< Scan response. */
#define GAP_EXT_ADV_REPORT_BIT_USE_LEGACY_ADV   0x10   /**< Legacy advertising PDUs used. */
/** End of EXT_ADV_REPORT
  * @}
  */

/** End of GAP_LE_EXTENDED_SCAN_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_EXTENDED_SCAN_Exported_Types GAP LE Extended Scan Exported Types
  * @brief
  * @{
  */
/** @brief Scan PHY type used in @ref le_ext_scan_set_phy_param. */
typedef enum
{
    LE_SCAN_PHY_LE_1M,     /**< Set @ref T_GAP_LE_EXT_SCAN_PARAM for LE 1M PHY. */
    LE_SCAN_PHY_LE_CODED,  /**< Set @ref T_GAP_LE_EXT_SCAN_PARAM for LE Coded PHY. */
} T_LE_EXT_SCAN_PHY_TYPE;

/** @brief Extended scan parameter. */
typedef struct
{
    T_GAP_SCAN_MODE scan_type;      /**< Scan type. Value is @ref T_GAP_SCAN_MODE.
                                         Default value is @ref GAP_SCAN_MODE_ACTIVE. */
    uint16_t scan_interval;  /**< Time interval from when the Controller started its last scan until
                                  it begins the subsequent scan on the primary advertising physical channel.
                                  - Range: 0x0004 to 0xFFFF.
                                  - Time = N * 0.625 ms.
                                  - Time Range: 2.5 ms to 40.959375 s. */
    uint16_t scan_window;    /**< Duration of the scan on the primary advertising physical channel.
                                  - Range: 0x0004 to 0xFFFF.
                                  - Time = N * 0.625 ms.
                                  - Time Range: 2.5 ms to 40.959375 s. */
} T_GAP_LE_EXT_SCAN_PARAM;

/** @brief Extended scan parameter type. */
typedef enum
{
    GAP_PARAM_EXT_SCAN_LOCAL_ADDR_TYPE   = 0x340,  /**< The type of address being used in the scan request packets. Read/Write. Size is 1 octet.
                                                        Value is @ref T_GAP_LOCAL_ADDR_TYPE. Default value is @ref GAP_LOCAL_ADDR_LE_PUBLIC. */
    GAP_PARAM_EXT_SCAN_FILTER_POLICY     = 0x341,  /**< Scanning filter policy. Read/Write. Size is 1 octet.
                                                        Value is @ref T_GAP_SCAN_FILTER_POLICY. Default value is @ref GAP_SCAN_FILTER_ANY. */
    GAP_PARAM_EXT_SCAN_PHYS              = 0x342,  /**< A bit field indicates scanning PHYs. Read/Write. Size is 1 octet.
                                                        Value is @ref EXT_SCAN_PHY. Default value is @ref GAP_EXT_SCAN_PHYS_1M_BIT. */
    GAP_PARAM_EXT_SCAN_FILTER_DUPLICATES = 0x343,  /**< Scan Filter Duplicates. Read/Write. Size is 1 octet. Value is @ref T_GAP_SCAN_FILTER_DUPLICATE.
                                                        Default value is @ref GAP_SCAN_FILTER_DUPLICATE_ENABLE. */
    GAP_PARAM_EXT_SCAN_DURATION          = 0x344,  /**< Read/Write. Size is 2 octets.
                                                        Value range:
                                                        - 0: Scan continuously until explicitly disable.
                                                        - 0x0001 to 0xFFFF: Scan duration. Time = N * 10 ms. Time Range: 10 ms to 655.35 s.

                                                        Please refer to @ref GAP_PARAM_EXT_SCAN_PERIOD for details on usage.
                                                        Default value is zero. */
    GAP_PARAM_EXT_SCAN_PERIOD            = 0x345,  /**< Read/Write. Size is 2 octets.
                                                        Value range:
                                                        - 0: Scan continuously.
                                                        - 0x0001 to 0xFFFF: Time interval from when Controller started its last Duration
                                                          @ref GAP_PARAM_EXT_SCAN_DURATION until it begins the subsequent Duration.
                                                          Time = N * 1.28 sec. Time Range: 1.28 s to 83,884.8 s.

                                                        Usage:
                                                        - If Duration parameter is zero, Period parameter shall be ignored. Continue scanning
                                                          until scanning is disabled by calling @ref le_ext_scan_stop.
                                                        - If Duration and Period parameters are non-zero, Duration shall be less than Period
                                                          (comparing the times, not how they are represented).
                                                          Scan for the duration within a scan period. After the scan period has expired, a new
                                                          scan period shall begin and scanning shall begin again for the duration specified.
                                                          Continue scanning until scanning is disabled by calling @ref le_ext_scan_stop.
                                                        - If Duration parameter is non-zero and Period parameter is zero, continue scanning
                                                          until duration has expired.

                                                        Default value is zero. */
} T_LE_EXT_SCAN_PARAM_TYPE;

/** End of GAP_LE_EXTENDED_SCAN_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_EXTENDED_SCAN_Exported_Functions GAP LE Extended Scan Exported Functions
  * @brief
  * @{
  */
/**
 * @brief       Set a GAP extended scan parameter.
 *
 * This function can be called with an extended scan parameter type @ref T_LE_EXT_SCAN_PARAM_TYPE and it will set the extended scan parameter.
 * The 'p_value' field must point to an appropriate data type that meets the requirements for the corresponding parameter type.
 * (For example: if required data length for parameter type is 2 octets, p_value should be cast to a pointer of uint16_t.)
 *
 * Combination of Duration parameter (@ref GAP_PARAM_EXT_SCAN_DURATION) and Period
 * parameter (@ref GAP_PARAM_EXT_SCAN_PERIOD) determines scan mode, please refer to below code about scan_mode.
 *
 * @param[in]   param    Extended scan parameter type @ref T_LE_EXT_SCAN_PARAM_TYPE.
 * @param[in]   len      Length of data to write.
 * @param[in]   p_value  Pointer to data to write.
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS    Operation success.
 * @retval Others   Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
  void test(void)
  {
    T_GAP_CAUSE cause;
    T_GAP_LOCAL_ADDR_TYPE  own_address_type = GAP_LOCAL_ADDR_LE_PUBLIC;
    T_GAP_SCAN_FILTER_POLICY  ext_scan_filter_policy = GAP_SCAN_FILTER_ANY;
    T_GAP_SCAN_FILTER_DUPLICATE  ext_scan_filter_duplicate = GAP_SCAN_FILTER_DUPLICATE_ENABLE;
    uint16_t ext_scan_duration;
    uint16_t ext_scan_period;
    uint8_t  scan_phys = GAP_EXT_SCAN_PHYS_1M_BIT | GAP_EXT_SCAN_PHYS_CODED_BIT;
    T_EXT_SCAN_MODE  scan_mode;

    link_mgr_clear_device_list();
    if (scan_mode == SCAN_UNTIL_DISABLED)
    {
        // If Duration parameter is zero, continue scanning until scanning is disabled.
        ext_scan_duration = 0;
        ext_scan_period = 0;
    }
    else if (scan_mode == PERIOD_SCAN_UNTIL_DISABLED)
    {
        // If Duration and Period parameters are non-zero, scan for the duration within a scan period,
        // and scan periods continue until scanning is disabled. Duration shall be less than Period.
        ext_scan_duration = 500;
        ext_scan_period = 8;
        ext_scan_filter_duplicate = GAP_SCAN_FILTER_DUPLICATE_ENABLED_RESET_FOR_EACH_PERIOD;
    }
    else if (scan_mode == SCAN_UNTIL_DURATION_EXPIRED)
    {
        // If Duration parameter is non-zero and Period parameter is zero, continue scanning until duration has expired.
        ext_scan_duration = 500;
        ext_scan_period = 0;
    }

    le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_LOCAL_ADDR_TYPE, sizeof(own_address_type), &own_address_type);
    le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_PHYS, sizeof(scan_phys), &scan_phys);
    le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_DURATION, sizeof(ext_scan_duration), &ext_scan_duration);
    le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_PERIOD, sizeof(ext_scan_period),&ext_scan_period);
    le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_FILTER_POLICY, sizeof(ext_scan_filter_policy), &ext_scan_filter_policy);
    le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_FILTER_DUPLICATES, sizeof(ext_scan_filter_duplicate), &ext_scan_filter_duplicate);

    cause = le_ext_scan_start();
  }
  * \endcode
  */
T_GAP_CAUSE le_ext_scan_set_param(T_LE_EXT_SCAN_PARAM_TYPE param, uint8_t len, void *p_value);

/**
 * @brief       Get an extended scan parameter.
 *
 * This function can be called with an extended scan parameter type @ref T_LE_EXT_SCAN_PARAM_TYPE and it will get the extended scan parameter.
 * The 'p_value' field must point to an appropriate data type that meets the requirements for the corresponding parameter type.
 * (For example: if required data length for parameter type is 2 octets, p_value should be cast to a pointer of uint16_t.)
 *
 * @param[in]      param   Extended scan parameter type @ref T_LE_EXT_SCAN_PARAM_TYPE.
 * @param[in,out]  p_value Pointer to location to get the parameter value.
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS    Operation success.
 * @retval Others               Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        uint16_t scan_duration;
        le_ext_scan_get_param(GAP_PARAM_EXT_SCAN_DURATION, &scan_duration);
    }
  * \endcode
  */
T_GAP_CAUSE le_ext_scan_get_param(T_LE_EXT_SCAN_PARAM_TYPE param, void *p_value);

/**
 * @brief       Set an extended scan PHY parameter including scan type, scan interval and scan window
                for scan advertisements on LE 1M PHY or/and LE Coded PHY.
 *
 * @param[in]   type    Scan PHY type @ref T_LE_EXT_SCAN_PHY_TYPE.
 * @param[in]   p_param Pointer to data to write @ref T_GAP_LE_EXT_SCAN_PARAM.
  *
  * <b>Example usage</b>
  * \code{.c}
  static T_USER_CMD_PARSE_RESULT cmd_escan(T_USER_CMD_PARSED_VALUE *p_parse_value)
  {
    uint8_t  scan_phys = GAP_EXT_SCAN_PHYS_1M_BIT | GAP_EXT_SCAN_PHYS_CODED_BIT;
    T_GAP_LE_EXT_SCAN_PARAM extended_scan_param[GAP_EXT_SCAN_MAX_PHYS_NUM];
    extended_scan_param[0].scan_type = GAP_SCAN_MODE_ACTIVE;
    extended_scan_param[0].scan_interval = 400;
    extended_scan_param[0].scan_window = 200;
    extended_scan_param[1].scan_type = GAP_SCAN_MODE_ACTIVE;
    extended_scan_param[1].scan_interval = 440;
    extended_scan_param[1].scan_window = 220;

    le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_PHYS, sizeof(scan_phys), &scan_phys);

    le_ext_scan_set_phy_param(LE_SCAN_PHY_LE_1M, &extended_scan_param[0]);
    le_ext_scan_set_phy_param(LE_SCAN_PHY_LE_CODED, &extended_scan_param[1]);

    return (RESULT_SUCESS);
  }
  * \endcode
  */
void le_ext_scan_set_phy_param(T_LE_EXT_SCAN_PHY_TYPE type, T_GAP_LE_EXT_SCAN_PARAM *p_param);

/**
 * @brief   Start extended scan.
 *
 * If sending request operation is successful, the starting result will be returned in one of the following ways:
 * - In the default situation, or when @ref le_ext_scan_gap_msg_info_way (true) has been called, APP will be notified
 *   by message @ref GAP_MSG_LE_DEV_STATE_CHANGE with new_state about gap_scan_state @ref GAP_SCAN_STATE.
 * - When @ref le_ext_scan_gap_msg_info_way (false) has been called, APP will be notified with the callback registered
 *   by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_EXT_SCAN_STATE_CHANGE_INFO.
 *
 * If the device is in the scanning state @ref GAP_SCAN_STATE_SCANNING, the advertising data or scan response data will be returned by
 * the callback registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_EXT_ADV_REPORT_INFO.
 *
 * @return The result of sending request.
 * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
 * @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        le_ext_scan_start();
    }

    void app_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
    {
        APP_PRINT_INFO5("app_handle_dev_state_evt: init state %d scan state %d adv state %d conn state %d cause 0x%x",
                       new_state.gap_init_state,
                       new_state.gap_scan_state, new_state.gap_adv_state, new_state.gap_conn_state, cause);

        if (gap_dev_state.gap_scan_state != new_state.gap_scan_state)
        {
           if (new_state.gap_scan_state == GAP_SCAN_STATE_IDLE)
           {
               APP_PRINT_INFO0("GAP scan stop");
               data_uart_print("GAP scan stop\r\n");
           }
           else if (new_state.gap_scan_state == GAP_SCAN_STATE_SCANNING)
           {
               APP_PRINT_INFO0("GAP scan start");
               data_uart_print("GAP scan start\r\n");
           }
        }

        gap_dev_state = new_state;
    }

    // Received advertising or scan response data will be handled in app_gap_callback
    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;
        APP_PRINT_TRACE1("app_gap_callback: cb_type %d", cb_type);

        switch (cb_type)
        {
        case GAP_MSG_LE_EXT_ADV_REPORT_INFO:
        APP_PRINT_INFO6("GAP_MSG_LE_EXT_ADV_REPORT_INFO: connectable %d, scannable %d, direct %d, scan response %d, legacy %d, data status 0x%x",
                        p_data->p_le_ext_adv_report_info->event_type & GAP_EXT_ADV_REPORT_BIT_CONNECTABLE_ADV,
                        p_data->p_le_ext_adv_report_info->event_type & GAP_EXT_ADV_REPORT_BIT_SCANNABLE_ADV,
                        p_data->p_le_ext_adv_report_info->event_type & GAP_EXT_ADV_REPORT_BIT_DIRECTED_ADV,
                        p_data->p_le_ext_adv_report_info->event_type & GAP_EXT_ADV_REPORT_BIT_SCAN_RESPONSE,
                        p_data->p_le_ext_adv_report_info->event_type & GAP_EXT_ADV_REPORT_BIT_USE_LEGACY_ADV,
                        p_data->p_le_ext_adv_report_info->data_status);
        APP_PRINT_INFO5("GAP_MSG_LE_EXT_ADV_REPORT_INFO: event_type 0x%x, bd_addr %s, addr_type %d, RSSI %d, data_len %d",
                        p_data->p_le_ext_adv_report_info->event_type,
                        TRACE_BDADDR(p_data->p_le_ext_adv_report_info->bd_addr),
                        p_data->p_le_ext_adv_report_info->addr_type,
                        p_data->p_le_ext_adv_report_info->rssi,
                        p_data->p_le_ext_adv_report_info->data_len);
        APP_PRINT_INFO5("GAP_MSG_LE_EXT_ADV_REPORT_INFO: primary_phy %d, secondary_phy %d, adv_sid %d, tx_power %d, peri_adv_interval %d",
                        p_data->p_le_ext_adv_report_info->primary_phy,
                        p_data->p_le_ext_adv_report_info->secondary_phy,
                        p_data->p_le_ext_adv_report_info->adv_sid,
                        p_data->p_le_ext_adv_report_info->tx_power,
                        p_data->p_le_ext_adv_report_info->peri_adv_interval);
        APP_PRINT_INFO2("GAP_MSG_LE_EXT_ADV_REPORT_INFO: direct_addr_type 0x%x, direct_addr %s",
                        p_data->p_le_ext_adv_report_info->direct_addr_type,
                        TRACE_BDADDR(p_data->p_le_ext_adv_report_info->direct_addr));
        if (!(p_data->p_le_ext_adv_report_info->event_type & GAP_EXT_ADV_REPORT_BIT_USE_LEGACY_ADV))
        {
            // Recombine adv data
            app_handle_ext_adv_report(p_data->p_le_ext_adv_report_info->event_type,
                                      p_data->p_le_ext_adv_report_info->data_status, p_data->p_le_ext_adv_report_info->bd_addr,
                                      p_data->p_le_ext_adv_report_info->data_len, p_data->p_le_ext_adv_report_info->p_data);
        }

        break;
        }
    }
  * \endcode
  */
T_GAP_CAUSE le_ext_scan_start(void);

/**
  * @brief   Stop extended scan.
  *
  * If sending request operation is successful, the stopping result will be returned in one of the following ways:
  * - In the default situation, or when @ref le_ext_scan_gap_msg_info_way (true) has been called, APP will be notified
  *   by message @ref GAP_MSG_LE_DEV_STATE_CHANGE with new_state about gap_scan_state @ref GAP_SCAN_STATE.
  * - When @ref le_ext_scan_gap_msg_info_way (false) has been called, APP will be notified with the callback registered
  *   by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_EXT_SCAN_STATE_CHANGE_INFO.
  *
  * @return The result of sending request.
  * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
  * @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        le_ext_scan_stop();
    }

    void app_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
    {
        APP_PRINT_INFO5("app_handle_dev_state_evt: init state %d scan state %d adv state %d conn state %d cause 0x%x",
                       new_state.gap_init_state,
                       new_state.gap_scan_state, new_state.gap_adv_state, new_state.gap_conn_state, cause);

        if (gap_dev_state.gap_scan_state != new_state.gap_scan_state)
        {
            if (new_state.gap_scan_state == GAP_SCAN_STATE_IDLE)
            {
                APP_PRINT_INFO0("GAP scan stop");
                data_uart_print("GAP scan stop\r\n");
            }
            else if (new_state.gap_scan_state == GAP_SCAN_STATE_SCANNING)
            {
                APP_PRINT_INFO0("GAP scan start");
                data_uart_print("GAP scan start\r\n");
            }
        }

        gap_dev_state = new_state;
    }
  * \endcode
  */
T_GAP_CAUSE le_ext_scan_stop(void);

/**
* @brief  Set extended scanning GAP message inform way.
*
* Default value is true.
*
* If extended scanning state changes, the state infomation will be returned in one of the following ways:
* - In the default situation, or when @ref le_ext_scan_gap_msg_info_way (true) has been called, APP will be notified
*   by message @ref GAP_MSG_LE_DEV_STATE_CHANGE with new_state about gap_scan_state @ref GAP_SCAN_STATE.
* - When @ref le_ext_scan_gap_msg_info_way (false) has been called, APP will be notified with the callback registered
*   by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_EXT_SCAN_STATE_CHANGE_INFO.
*
* @param[in] use_msg Whether to use message.
*
* <b>Example usage</b>
* \code{.c}
    int test(void)
    {
        le_ext_scan_gap_msg_info_way(false);
    }

    void app_handle_ext_scan_state_evt(uint8_t new_state, uint16_t cause)
    {
        ......
        if (new_state == GAP_SCAN_STATE_IDLE)
        {
        }
        else if (new_state == GAP_SCAN_STATE_SCANNING)
        {
        }
        ......
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case GAP_MSG_LE_EXT_SCAN_STATE_CHANGE_INFO:
            {
                app_handle_ext_scan_state_evt(p_data->p_le_ext_scan_state_change_info->state,
                                              p_data->p_le_ext_scan_state_change_info->cause);
            }
            break;
        }
        ......
    }
* \endcode
*/
void le_ext_scan_gap_msg_info_way(bool use_msg);

/** End of GAP_LE_EXTENDED_SCAN_Exported_Functions
  * @}
  */

/** End of GAP_LE_EXTENDED_SCAN
  * @}
  */

/** End of GAP_LE
  * @}
  */

/** End of BT_Host
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* GAP_EXT_SCAN_H */
