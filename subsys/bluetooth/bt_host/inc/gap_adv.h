/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_adv.h
* @brief     Header file for GAP ADV
* @details   This file defines advertising related API
* @author    ranhui
* @date      2016-02-18
* @version   v1.0
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_ADV_H
#define GAP_ADV_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <gap_le.h>


/** @addtogroup GAP_BROADCASTER_Role GAP Broadcaster Role
  * @brief GAP Broadcaster Role
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @addtogroup GAP_Broadcaster_Exported_Macros GAP Broadcaster Exported Macros
  * @brief
  * @{
  */
#define GAP_MAX_ADV_LEN                   31    //!< Bluetooth LE Maximum Advertising or Scan Response Data Length.

/** End of GAP_Broadcaster_Exported_Macros
* @}
*/

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_Broadcaster_Exported_Types GAP Broadcaster Exported Types
  * @brief
  * @{
  */

/** @brief LE advertising parameter types */
typedef enum
{
    GAP_PARAM_ADV_LOCAL_ADDR_TYPE  = 0x260,/**< The type of local address being used in the advertising packets. Read/Write. Size is 1 octet.
                                                Value is @ref T_GAP_LOCAL_ADDR_TYPE. Default value is @ref GAP_LOCAL_ADDR_LE_PUBLIC. */
    GAP_PARAM_ADV_DATA             = 0x261,/**< Advertisement Data. Read/Write. Maximum size is 31 octets @ref GAP_MAX_ADV_LEN.
                                                Default value is '02:01:05', which means that it is a Limited Discoverable Advertisement. */
    GAP_PARAM_SCAN_RSP_DATA        = 0x262,/**< Scan Response Data. Read/Write. Maximum size is 31 octets @ref GAP_MAX_ADV_LEN.
                                                Default value is all zero. */
    GAP_PARAM_ADV_EVENT_TYPE       = 0x263,/**< Advertisement Type. Read/Write. Size is 1 octet. Value is @ref T_GAP_ADTYPE.
                                                Default value is @ref GAP_ADTYPE_ADV_IND. */
    GAP_PARAM_ADV_DIRECT_ADDR_TYPE = 0x264,/**< Direct Advertisement Address Type. Read/Write. Size is 1 octet.
                                                Value is @ref T_GAP_REMOTE_ADDR_TYPE. Default value is @ref GAP_REMOTE_ADDR_LE_PUBLIC. */
    GAP_PARAM_ADV_DIRECT_ADDR      = 0x265,/**< Direct Advertisement Address. Read/Write. Size is 6 octets @ref GAP_BD_ADDR_LEN.
                                                Default value is all zero. */
    GAP_PARAM_ADV_CHANNEL_MAP      = 0x266,/**< The advertising channel indices that shall be used when transmitting advertising packets.
                                                Read/Write. Size is 1 octet. Value is @ref ADV_CHANNEL_MAP. Default value is @ref GAP_ADVCHAN_ALL. */
    GAP_PARAM_ADV_FILTER_POLICY    = 0x267,/**< Filter Policy. Ignored when directed advertising is used. Read/Write. Size is 1 octet.
                                                Value is @ref T_GAP_ADV_FILTER_POLICY. Default value is @ref GAP_ADV_FILTER_ANY. */
    GAP_PARAM_ADV_INTERVAL_MIN     = 0x268,/**< Minimum advertising interval for undirected and low duty cycle directed advertising.
                                                Read/Write. Size is 2 octets. Value range: 0x0020 - 0x4000 (20ms - 10240ms 0.625ms/step). */
    GAP_PARAM_ADV_INTERVAL_MAX     = 0x269,/**< Maximum advertising interval for undirected and low duty cycle directed advertising.
                                                Read/Write. Size is 2 octets. Value range: 0x0020 - 0x4000 (20ms - 10240ms 0.625ms/step). */
} T_LE_ADV_PARAM_TYPE;

/** End of GAP_Broadcaster_Exported_Types
* @}
*/

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_Broadcaster_Exported_Functions GAP Broadcaster Exported Functions
  * @brief
  * @{
  */

/**
 * @brief       Set a GAP advertising parameter.
 *
 * This function can be called with an advertising parameter type @ref T_LE_ADV_PARAM_TYPE and it will set the advertising parameter.
 * The 'p_value' field must point to an appropriate data type that meets the requirements for the corresponding parameter type.
 * (For example: if required data length for parameter type is 2 octets, p_value should be cast to a pointer of uint16_t.)
 *
 * @param[in]   param   @ref T_LE_ADV_PARAM_TYPE.
 * @param[in]   len     Length of data to write.
 * @param[in]   p_value Pointer to data to write.
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_le_gap_init(void)
    {
        //advertising parameters
        uint8_t  adv_evt_type = GAP_ADTYPE_ADV_IND;
        uint8_t  adv_direct_type = GAP_REMOTE_ADDR_LE_PUBLIC;
        uint8_t  adv_direct_addr[GAP_BD_ADDR_LEN] = {0};
        uint8_t  adv_chann_map = GAP_ADVCHAN_ALL;
        uint8_t  adv_filter_policy = GAP_ADV_FILTER_ANY;
        uint16_t adv_int_min = DEFAULT_ADVERTISING_INTERVAL_MIN;
        uint16_t adv_int_max = DEFAULT_ADVERTISING_INTERVAL_MIN;

        //Set advertising parameters
        le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(adv_evt_type), &adv_evt_type);
        le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, sizeof(adv_direct_type), &adv_direct_type);
        le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR, sizeof(adv_direct_addr), adv_direct_addr);
        le_adv_set_param(GAP_PARAM_ADV_CHANNEL_MAP, sizeof(adv_chann_map), &adv_chann_map);
        le_adv_set_param(GAP_PARAM_ADV_FILTER_POLICY, sizeof(adv_filter_policy), &adv_filter_policy);
        le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int_min), &adv_int_min);
        le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int_max), &adv_int_max);
        le_adv_set_param(GAP_PARAM_ADV_DATA, sizeof(adv_data), adv_data);
        le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, sizeof(scan_rsp_data), scan_rsp_data);
    }
 * \endcode
 */
T_GAP_CAUSE le_adv_set_param(T_LE_ADV_PARAM_TYPE param, uint8_t len, void *p_value);

/**
 * @brief   Get a GAP advertising parameter.
 *
 * This function can be called with an advertising parameter type @ref T_LE_ADV_PARAM_TYPE and it will get the advertising parameter.
 * The 'p_value' field must point to an appropriate data type that meets the requirements for the corresponding parameter type.
 * (For example: if required data length for parameter type is 2 octets, p_value should be cast to a pointer of uint16_t.)
 *
 * @param[in]   param @ref T_LE_ADV_PARAM_TYPE.
 * @param[in,out]  p_value Pointer to location to get the value.
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t local_bd_type = GAP_LOCAL_ADDR_LE_PUBLIC;
        le_adv_get_param(GAP_PARAM_ADV_LOCAL_ADDR_TYPE, &local_bd_type);
    }
 * \endcode
 */
T_GAP_CAUSE le_adv_get_param(T_LE_ADV_PARAM_TYPE param, void *p_value);

/**
 * @brief  Start advertising with parameters set by @ref le_adv_set_param.
 *
 * If sending request operation is successful, the starting result will be returned in one of the following ways:
 * - In the default situation, or when @ref le_gap_msg_info_way (true) has been called, APP will be notified
 *   by message @ref GAP_MSG_LE_DEV_STATE_CHANGE with new_state about gap_adv_state @ref GAP_ADV_STATE.
 * - When @ref le_gap_msg_info_way (false) has been called, APP will be notified with the callback registered
 *   by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_GAP_STATE_MSG.
 *
 * Applications can only call this API after the Bluetooth Host is ready.
 *
 * Explanation: If the Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
 * with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
 *
 * @return The result of sending request.
 * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
 * @retval Others Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t  adv_evt_type = GAP_ADTYPE_ADV_IND;
        uint8_t  adv_direct_type = GAP_REMOTE_ADDR_LE_PUBLIC;
        uint8_t  adv_direct_addr[GAP_BD_ADDR_LEN] = {0};
        uint8_t  adv_chann_map = GAP_ADVCHAN_ALL;
        uint8_t  adv_filter_policy = GAP_ADV_FILTER_ANY;
        uint16_t adv_int_min = DEFAULT_ADVERTISING_INTERVAL_MIN;
        uint16_t adv_int_max = DEFAULT_ADVERTISING_INTERVAL_MIN;

        le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(adv_evt_type), &adv_evt_type);
        le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, sizeof(adv_direct_type), &adv_direct_type);
        le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR, sizeof(adv_direct_addr), adv_direct_addr);
        le_adv_set_param(GAP_PARAM_ADV_CHANNEL_MAP, sizeof(adv_chann_map), &adv_chann_map);
        le_adv_set_param(GAP_PARAM_ADV_FILTER_POLICY, sizeof(adv_filter_policy), &adv_filter_policy);
        le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int_min), &adv_int_min);
        le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int_max), &adv_int_max);
        le_adv_set_param(GAP_PARAM_ADV_DATA, sizeof(adv_data), (void *)adv_data);
        le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, sizeof(scan_rsp_data), (void *)scan_rsp_data);

        le_adv_start();
    }

    void app_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
    {
        APP_PRINT_INFO3("app_handle_dev_state_evt: init state %d, adv state %d, cause 0x%x",
                        new_state.gap_init_state, new_state.gap_adv_state, cause);
        if (gap_dev_state.gap_init_state != new_state.gap_init_state)
        {
            if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
            {
                APP_PRINT_INFO0("GAP ready");

                le_adv_start();
            }
        }

        if (gap_dev_state.gap_adv_state != new_state.gap_adv_state)
        {
            if (new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
            {
                if (new_state.gap_adv_sub_state == GAP_ADV_TO_IDLE_CAUSE_CONN)
                {
                    APP_PRINT_INFO0("GAP ADV stopped: because connection created");
                }
                else
                {
                    APP_PRINT_INFO0("GAP ADV stopped");
                }
            }
            else if (new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                APP_PRINT_INFO0("GAP ADV start");
            }
        }

        gap_dev_state = new_state;
    }
 * \endcode
 */
T_GAP_CAUSE le_adv_start(void);

/**
 * @brief  Stop advertising. When the device is in advertising state, the application can call this API to stop advertising.
 *
 * If sending request operation is successful, the stopping result will be returned in one of the following ways:
 * - In the default situation, or when @ref le_gap_msg_info_way (true) has been called, APP will be notified
 *   by message @ref GAP_MSG_LE_DEV_STATE_CHANGE with new_state about gap_adv_state @ref GAP_ADV_STATE.
 * - When @ref le_gap_msg_info_way (false) has been called, APP will be notified with the callback registered
 *   by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_GAP_STATE_MSG.
 *
 * Applications can only call this API after the Bluetooth Host is ready.
 *
 * Explanation: If the Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
 * with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
 *
 * @return The result of sending request.
 * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
 * @retval Others Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
      le_adv_stop();
    }

    void app_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
    {
        APP_PRINT_INFO3("app_handle_dev_state_evt: init state %d, adv state %d, cause 0x%x",
                        new_state.gap_init_state, new_state.gap_adv_state, cause);
        if (gap_dev_state.gap_init_state != new_state.gap_init_state)
        {
            if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
            {
                APP_PRINT_INFO0("GAP ready");

                le_adv_start();
            }
        }

        if (gap_dev_state.gap_adv_state != new_state.gap_adv_state)
        {
            if (new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
            {
                if (new_state.gap_adv_sub_state == GAP_ADV_TO_IDLE_CAUSE_CONN)
                {
                    APP_PRINT_INFO0("GAP ADV stopped: because connection created");
                }
                else
                {
                    APP_PRINT_INFO0("GAP ADV stopped");
                }
            }
            else if (new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                APP_PRINT_INFO0("GAP ADV start");
            }
        }

        gap_dev_state = new_state;
    }
 * \endcode
 */
T_GAP_CAUSE le_adv_stop(void);

/**
 * @brief  Update advertising parameters.
 *         Applications can update one or more advertising parameters, @ref le_adv_set_param should be called first to specify
 *         which parameter shall be updated. If the GAP ADV state is not @ref GAP_ADV_STATE_IDLE, the application can only update
 *         advertising data and scan response data.
 *
 * If sending request operation is successful, the updating result will be returned by callback
 * registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_ADV_UPDATE_PARAM.
 *
 * Applications can only call this API after the Bluetooth Host is ready.
 *
 * Explanation: If the Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
 * with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
 *
 * @return The result of sending request.
 * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
 * @retval Others Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        ...
        le_adv_set_param(GAP_PARAM_ADV_DATA, sizeof(adv_data), adv_data);
        le_adv_update_param();
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_LE_CB_DATA));
        APP_PRINT_TRACE1("app_gap_callback: cb_type %d", cb_type);
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_ADV_UPDATE_PARAM:
        APP_PRINT_INFO1("GAP_MSG_LE_ADV_UPDATE_PARAM: cause 0x%x",
                      cb_data.p_le_adv_update_param_rsp->cause);
        break;
        }
        ...
    }
 * \endcode
 */
T_GAP_CAUSE le_adv_update_param(void);

/**
 * @brief  Read advertising transmit power.
 *
 * If sending request operation is successful, the reading result will be returned by callback
 * registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_ADV_READ_TX_POWER.
 *
 * Applications can only call this API after Bluetooth Host is ready.
 *
 * Explanation: If Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
 * with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
 *
 * @return The result of sending request.
 * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
 * @retval Others Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        le_adv_read_tx_power();
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_LE_CB_DATA));
        APP_PRINT_TRACE1("app_gap_callback: cb_type %d", cb_type);
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_ADV_READ_TX_POWER:
          APP_PRINT_INFO2("GAP_MSG_LE_ADV_READ_TX_POWER: cause 0x%x, tx_power_level 0x%x",
                          cb_data.p_le_adv_read_tx_power_rsp->cause,
                          cb_data.p_le_adv_read_tx_power_rsp->tx_power_level);
          break;
     }
      ...
    }
 * \endcode
 */
T_GAP_CAUSE le_adv_read_tx_power(void);

/** @} */ /* End of group GAP_Broadcaster_Exported_Functions */
/** @} */ /* End of group GAP_BROADCASTER_Role */


#ifdef __cplusplus
}
#endif

#endif /* GAP_ADV_H */
