/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_vendor.h
  * @brief
  * @details
  * @author  ranhui_xia
  * @date    2017-08-02
  * @version v1.0
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
  ******************************************************************************
  */
#ifndef GAP_VNR_H
#define GAP_VNR_H

#include <gap_le.h>

/** @addtogroup GAP_LE_VENDOR  GAP LE Vendor
  * @brief GAP LE vendor command API provides extended function for controller.
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup Gap_LE_Vendor_Exported_Macros GAP LE Vendor Exported Macros
  * @{
  */

/** @defgroup GAP_LE_MSG_Types GAP LE Msg Types
  * @{
  */
#define GAP_MSG_LE_VENDOR_ADV_3_DATA_ENABLE         0xA0 //!<Response msg type for le_vendor_adv_3_data_enable
#define GAP_MSG_LE_VENDOR_ADV_3_DATA_SET            0xA1 //!<Response msg type for le_vendor_adv_3_data_set
#define GAP_MSG_GAP_SW_RESET                        0xA5 //!<Response msg type for gap_sw_reset_req
#define GAP_MSG_LE_AE_CODING_SCHEME                 0xA7 //!<Response msg type for le_ae_coding_scheme
#define GAP_MSG_LE_VENDOR_SET_PRIORITY              0xA8 //!<Response msg type for le_vendor_set_priority

/**
  * @}
  */

/** End of GAP_LE_Vendor_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_Vendor_Exported_Types GAP LE Vendor Exported Types
  * @{
  */

typedef enum
{
    GAP_SW_RESET_WHEN_ADV = 1,    //!< Software reset when advertising
} T_GAP_SW_RESET_MODE;

/** @brief Coding scheme of LE Coded PHY when device uses LE Advertising Extensions.*/
typedef enum
{
    GAP_AE_CODING_SCHEME_S8 = 2,  //!< Coding scheme S8
    GAP_AE_CODING_SCHEME_S2 = 3,  //!< Coding scheme S2
} T_GAP_AE_CODING_SCHEME;

/** @brief Mode of setting priority. */
typedef enum
{
    GAP_VENDOR_UPDATE_PRIORITY, //!< Set priority without operation of resetting priority
    GAP_VENDOR_SET_PRIORITY,    //!< Set priority after operation of resetting priority
    GAP_VENDOR_RESET_PRIORITY,  //!< Reset priority
} T_GAP_VENDOR_SET_PRIORITY_MODE;

/** @brief Definition of priority level. */
typedef enum
{
    GAP_VENDOR_PRIORITY_LEVEL_0,  //!< Level 0
    GAP_VENDOR_PRIORITY_LEVEL_1,  //!< Level 1
    GAP_VENDOR_PRIORITY_LEVEL_2,  //!< Level 2
    GAP_VENDOR_PRIORITY_LEVEL_3,  //!< Level 3
    GAP_VENDOR_PRIORITY_LEVEL_4,  //!< Level 4
    GAP_VENDOR_PRIORITY_LEVEL_5,  //!< Level 5
    GAP_VENDOR_PRIORITY_LEVEL_6,  //!< Level 6
    GAP_VENDOR_PRIORITY_LEVEL_7,  //!< Level 7
    GAP_VENDOR_PRIORITY_LEVEL_8,  //!< Level 8
    GAP_VENDOR_PRIORITY_LEVEL_9,  //!< Level 9
    GAP_VENDOR_PRIORITY_LEVEL_10, //!< Highest priority level 10
    GAP_VENDOR_RESERVED_PRIORITY, //!< Reserved
} T_GAP_VENDOR_PRIORITY_LEVEL;

/** @brief Mode of setting link priority. */
typedef enum
{
    GAP_VENDOR_NOT_SET_LINK_PRIORITY,       //!< Not set priority of link
    GAP_VENDOR_SET_SPECIFIC_LINK_PRIORITY,  //!< Set priority of specific links
    GAP_VENDOR_SET_ALL_LINK_PRIORITY,       //!< Set priority of all links
} T_GAP_VENDOR_SET_LINK_PRIORITY_MODE;

/** @brief Definition of common priority. */
typedef struct
{
    bool set_priority_flag;
    T_GAP_VENDOR_PRIORITY_LEVEL priority_level; /**< Priority is valid if set_priority_flag is true. */
} T_GAP_VENDOR_COMMON_PRIORITY;

/** @brief Definition of connection priority. */
typedef struct
{
    uint8_t conn_id;
    T_GAP_VENDOR_PRIORITY_LEVEL conn_priority_level;/**< Priority of specific connection. */
} T_GAP_VENDOR_CONN_PRIORITY;

/** @brief  Parameters of setting priority.*/
typedef struct
{
    T_GAP_VENDOR_SET_PRIORITY_MODE set_priority_mode;/**< Mode of setting priority.
                                                            (@ref T_GAP_VENDOR_SET_PRIORITY_MODE). */
    T_GAP_VENDOR_COMMON_PRIORITY adv_priority;/**< Priority of advertising.
                                                     (@ref T_GAP_VENDOR_COMMON_PRIORITY). */
    T_GAP_VENDOR_COMMON_PRIORITY scan_priority;/**< Priority of scan.
                                                      (@ref T_GAP_VENDOR_COMMON_PRIORITY). */
    T_GAP_VENDOR_COMMON_PRIORITY initiate_priority;/**< Priority of initiating.
                                                          (@ref T_GAP_VENDOR_COMMON_PRIORITY). */
    T_GAP_VENDOR_SET_LINK_PRIORITY_MODE link_priority_mode;/**< Mode of setting link priority.
                                                                  (@ref T_GAP_VENDOR_SET_LINK_PRIORITY_MODE). */
    T_GAP_VENDOR_PRIORITY_LEVEL link_priority_level;/**< Priority of all links is valid
                                                           if link_priority_mode is GAP_VENDOR_SET_ALL_LINK_PRIORITY. */
    uint8_t num_conn_ids;/**< Number of specific links is valid if link_priority_mode is GAP_VENDOR_SET_SPECIFIC_LINK_PRIORITY. */
    T_GAP_VENDOR_CONN_PRIORITY p_conn_id_list[1];/**< List of connection priority is valid
                                                        if link_priority_mode is GAP_VENDOR_SET_SPECIFIC_LINK_PRIORITY.
                                                        (@ref T_GAP_VENDOR_CONN_PRIORITY). */
} T_GAP_VENDOR_PRIORITY_PARAM;

typedef struct
{
    uint16_t cause;
} T_GAP_VENDOR_CAUSE;

typedef union
{
    T_GAP_VENDOR_CAUSE gap_vendor_cause;
} T_GAP_VENDOR_CB_DATA;

/** End of GAP_LE_Vendor_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/

/** @defgroup GAP_LE_VENDOR_Exported_Functions GAP LE Vendor Exported Functions
  * @brief GAP LE vendor command Exported Functions
  * @{
  */

/**
 * @brief  Register callback to vendor command, when messages in @ref GAP_VENDOR_MSG_TYPE happens, it will callback to APP.
 * @param[in] vendor_callback Callback function provided by the APP to handle @ref GAP_VENDOR_MSG_TYPE messages sent from the GAP.
 *              @arg NULL -> Not send @ref GAP_VENDOR_MSG_TYPE messages to APP.
 *              @arg Other -> Use application defined callback function.
 * @return void.
 *
 * <b>Example usage</b>
 * \code{.c}
   void app_le_gap_init(void)
    {
        ......
        gap_vendor_register_cb(app_gap_vendor_callback);
    }
    void app_gap_vendor_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_GAP_VENDOR_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_GAP_VENDOR_CB_DATA));
        APP_PRINT_INFO1("app_gap_vendor_callback: cb_type is %d", cb_type);
        switch (cb_type)
        {
        case GAP_MSG_GAP_VENDOR_SET_ANT_CTRL:
            APP_PRINT_INFO1("GAP_MSG_GAP_VENDOR_SET_ANT_CTRL: cause 0x%x",
                            cb_data.gap_vendor_cause.cause);
            break;
        default:
            break;
        }
        return;
    }
   \endcode
 */
void gap_vendor_register_cb(P_FUN_GAP_APP_CB vendor_callback);

/**
  * @brief  Enable 3 advertising channel advertising data.
  *         Set 3 ADV data please refer to @ref le_vendor_adv_3_data_enable.
  *
  * @param[in] enable
  * @arg true:  Enable each ADV channel with different data.
  * @arg false: Disable.
  *
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS: Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    void gap_vendor_test(bool enable)
    {
        ...
        cause = le_vendor_adv_3_data_enable(enable);
        return cause;
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
        case GAP_MSG_LE_VENDOR_ADV_3_DATA_ENABLE:
            APP_PRINT_INFO1("GAP_MSG_LE_VENDOR_ADV_3_DATA_ENABLE: cause 0x%x",
                            p_data->le_cause.cause);
            break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_vendor_adv_3_data_enable(bool enable);

/**
  * @brief  Set different advertising date or scan response data in 3 advertising channels.
  *         This command is used to set 38 / 39 channel data, set 37 channel data please use normal HCI command.
  *         It is necessary to enable 3 ADV data with @ref le_vendor_adv_3_data_enable.
  *
  * @param[in] type   LE  vendor advertising data type @ref T_GAP_ADV_VENDOR_DATA_TYPE.
  * @param[in] len    The number of significant octets in the advertising data.
  * @param[in] p_data Pointer to data to write.
  *
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS: Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  * @retval GAP_CAUSE_INVALID_PARAM: Invalid parameter.
  *
  * <b>Example usage</b>
  * \code{.c}
    T_GAP_CAUSE gap_vendor_test(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        T_GAP_CAUSE cause;
        T_GAP_ADV_VENDOR_DATA_TYPE type = (T_GAP_ADV_VENDOR_DATA_TYPE)p_parse_value->dw_param[0];
        uint8_t len = p_parse_value->dw_param[1];
        uint8_t value = p_parse_value->dw_param[2];
        uint8_t adv_data[31];
        if (len > GAP_MAX_ADV_LEN)
        {
            return (RESULT_ERR);
        }
        memset(adv_data, value, len);

        cause = le_vendor_adv_3_data_set((T_GAP_ADV_VENDOR_DATA_TYPE)type, len, adv_data);
        return (T_GAP_CAUSE)cause;
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
        case GAP_MSG_LE_VENDOR_ADV_3_DATA_SET:
            APP_PRINT_INFO2("GAP_MSG_LE_VENDOR_ADV_3_DATA_SET: type %d, cause 0x%x",
                            p_data->p_le_vendor_adv_3_data_set_rsp->type,
                            p_data->p_le_vendor_adv_3_data_set_rsp->cause);
            break;
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_vendor_adv_3_data_set(T_GAP_ADV_VENDOR_DATA_TYPE type,
                                     uint8_t len, uint8_t *p_data);

/**
  * @brief  LE Drop Acl Data.
  *
  * Drop pending LE acl packet that assigned by user except the acl packet which is currently waiting for ack.
  * The packet-dropping rule is cleared if link is disconnected or HCI_RESET.
  *
  * @param[in] conn_id  Connection ID for this link.
  * @param[in] mask     Assign the mask to compare with data.
  * @param[in] pattern  Drop data that match pattern.
  * @param[in] offset   The offset in bytes started from Data.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS: Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_vdropdata(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        T_GAP_CAUSE cause;
        uint8_t conn_id = p_parse_value->dw_param[0];

        cause = le_vendor_drop_acl_data(conn_id, 0xffff, 0x0015, 5);
        return (T_USER_CMD_PARSE_RESULT)cause;
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
        case GAP_MSG_LE_VENDOR_DROP_ACL_DATA:
            APP_PRINT_INFO1("GAP_MSG_LE_VENDOR_DROP_ACL_DATA: cause 0x%x",
                            p_data->le_cause.cause);
            break;
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_vendor_drop_acl_data(uint8_t conn_id, uint16_t mask, uint16_t pattern,
                                    uint8_t offset);

/**
  * @brief  Modify Bluetooth Host LE Fw Policy.
  *
  * @param[in] mask     Assign the mask to compare with data.
  * @param[in] value    Value to set with the mask.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS: Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  */
T_GAP_CAUSE le_vendor_modify_bt_le_fw_policy(uint32_t mask, uint32_t value);

/**
  * @brief  Reset Bluetooth Host Controller.
  *
  * @param[in] reset_mode  GAP software reset mode @ref T_GAP_SW_RESET_MODE.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS: Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_reset(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        T_GAP_CAUSE cause;
        T_GAP_SW_RESET_MODE reset_mode = (T_GAP_SW_RESET_MODE)p_parse_value->dw_param[0];
        cause = gap_sw_reset_req(reset_mode);
        return (T_USER_CMD_PARSE_RESULT)cause;
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
        case GAP_MSG_GAP_SW_RESET:
            APP_PRINT_INFO1("GAP_MSG_GAP_SW_RESET: cause 0x%x",
                            p_data->le_cause.cause);
            break;
        ...
    }
  * \endcode
  */
T_GAP_CAUSE gap_sw_reset_req(T_GAP_SW_RESET_MODE reset_mode);

/**
  * @brief  Configure coding scheme of LE Coded PHY when device uses LE Advertising Extensions.
  *
  * @note Advertiser should delete advertising set before changing coding scheme.
  *
  * @param[in] coding_scheme  Coding scheme of LE Coded PHY when device uses LE Advertising Extensions, @ref T_GAP_AE_CODING_SCHEME.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS Send request success.
  * @retval other             Send request failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        le_ae_coding_scheme(GAP_AE_CODING_SCHEME_S8);
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
        case GAP_MSG_LE_AE_CODING_SCHEME:
            APP_PRINT_INFO1("GAP_MSG_LE_AE_CODING_SCHEME: cause 0x%x",
                            cb_data->le_cause.cause);
        break;
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_ae_coding_scheme(T_GAP_AE_CODING_SCHEME coding_scheme);

/**
  * @brief  Set the advertising tx power for the device, or reset advertising tx power to default value.
  *
  * Applications can only call this API after Bluetooth Host is ready. \n
  *                Explanation: If Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
  *                             with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
  *
  * @note Different types of IC support different tx power levels. For the actual supported levels,
  *               please refer to the IC manual and related documentation. \n
  *               The resolution of tx power is 0.5 dBm, not 1 dBm. Therefore, when setting the tx_power_double
  *               parameter, it should be set to twice the actual tx power.
  *
  * @param[in] type            ADV tx power type.
  * @param[in] tx_power_double Twice the actual tx power.
  *
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
     void test(void)
     {
         T_GAP_CAUSE cause;
         uint8_t type = GAP_ADV_TX_POW_SET_1M;
         uint8_t tx_power_double = 0x80;
         cause = le_adv_set_tx_power((T_GAP_ADV_TX_POW_TYPE)type, tx_power_double);
     }

     T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
     {
         T_APP_RESULT result = APP_RESULT_SUCCESS;
         T_LE_CB_DATA cb_data;
         memcpy(&cb_data, p_cb_data, sizeof(T_LE_CB_DATA));
         APP_PRINT_TRACE1("app_gap_callback: msgType %d", cb_type);
         switch (cb_type)
         {
         ...
         case GAP_MSG_LE_ADV_SET_TX_POWER:
              APP_PRINT_INFO1("GAP_MSG_LE_ADV_SET_TX_POWER: cause 0x%x",
                          p_data->le_cause.cause);
              break;
         ...
         }
     }
  * \endcode
  */
T_GAP_CAUSE le_adv_set_tx_power(T_GAP_ADV_TX_POW_TYPE type, uint8_t tx_power_double);

/**
  * @brief  Trigger One Shot Advertising.
  *
  * @note Trigger advertising after guard_slot * 625 + guard_usec + wait_usec(us). \n
  *               This function cannot be used if advertising type is connectable high duty cycle directed advertising.
  *
  * Applications can only call this API after Bluetooth Host is ready. \n
  *                Explanation: If Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
  *                             with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
  *
  * @param[in] guard_slot The delay time to trigger advertising in units of 625us, range: 0~224.
  * @param[in] guard_usec The delay time to trigger advertising in units of us, range: 0~65535.
  * @param[in] wait_usec  The delay time to trigger advertising in units of us, range: 0~65535.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS: Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_oneshot(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t guard_slot = p_parse_value->dw_param[0];
        uint16_t guard_usec = p_parse_value->dw_param[1];
        uint16_t wait_usec = p_parse_value->dw_param[2];
        T_GAP_CAUSE cause;

        cause = le_vendor_one_shot_adv(guard_slot, guard_usec, wait_usec);
        return (T_USER_CMD_PARSE_RESULT)cause;
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
        case GAP_MSG_LE_VENDOR_ONE_SHOT_ADV:
            APP_PRINT_INFO1("GAP_MSG_LE_VENDOR_ONE_SHOT_ADV: cause 0x%x",
                            p_data->le_cause.cause);
            break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_vendor_one_shot_adv(uint8_t guard_slot, uint16_t guard_usec, uint16_t wait_usec);

/**
  * @brief   Set Minimum Remote SCA.
  *
  * Applications can only call this API after Bluetooth Host is ready. \n
  *                 Explanation: If Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
  *                              with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
  *
  * Set Min_sca to compare with M_sca sent by master (carried in CONNECT_REQ pdu).
  * The finally used master sca is set to MIN(Min_sca, M_sca).
  * The default Min_sca is 7.
  * The setting is used by all links in slave role, can set when boot up or link exist, and will reset to default value when HCI_RESET.
  *
  * @param[in] index Minimum Remote SCA Index.
  *
  * @retval GAP_CAUSE_SUCCESS: Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause;
        T_GAP_SCA_FIELD_ENCODING index = GAP_SCA_76_TO_100_PPM;
        cause = le_vendor_set_rem_min_sca(index);
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
        case GAP_MSG_LE_VENDOR_SET_MIN_REM_SCA:
            APP_PRINT_INFO1("GAP_MSG_LE_VENDOR_SET_MIN_REM_SCA: cause 0x%x",
                            p_data->le_cause.cause);
            break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_vendor_set_rem_min_sca(T_GAP_SCA_FIELD_ENCODING min_sca);

/**
  * @brief  Set the link tx power for the device.
  *
  * Set LE link tx power. Or reset link tx power to default value.
  *         Tx power is set to default every time a link is connect. Use this command after the link is connected.
  *
  * @note Different types of IC support different tx power levels. For the actual supported levels,
  *               please refer to the IC manual and related documentation. \n
  *               The resolution of tx power is 0.5 dBm, not 1 dBm. Therefore, when setting the tx_power_double
  *               parameter, it should be set to twice the actual tx power.
  *
  * @param[in] conn_id          Connection ID for this link.
  * @param[in] reset            Whether to reset to default value.
  * @param[in] tx_power_double  Twice the actual tx power.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
     void test(void)
     {
        T_GAP_CAUSE cause;
        uint8_t conn_id = 0;
        bool reset = false;
        uint8_t tx_power_double = 0;

        cause = le_set_conn_tx_power(conn_id, reset, tx_power_double);
     }

     T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
     {
         T_APP_RESULT result = APP_RESULT_SUCCESS;
         T_LE_CB_DATA cb_data;
         memcpy(&cb_data, p_cb_data, sizeof(T_LE_CB_DATA));
         APP_PRINT_TRACE1("app_gap_callback: msgType %d", cb_type);
         switch (cb_type)
         {
         ...
            case GAP_MSG_LE_SET_CONN_TX_PWR:
                APP_PRINT_INFO1("GAP_MSG_LE_SET_CONN_TX_PWR: cause 0x%x",
                                p_data->le_cause.cause);
                break;
         ...
         }
     }
  * \endcode
  */
T_GAP_CAUSE le_set_conn_tx_power(uint8_t conn_id, bool reset, uint8_t tx_power_double);

/**
 * @brief  Send vendor command.
 * @param[in] op       Opcode of command.
 * @param[in] len      Length of parameters.
 * @param[in] p_param  Pointer to parameters to write.
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS Send request successfully.
 * @retval Others Failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    //send vendor command
    void test(void)
    {
        T_GAP_CAUSE cause = GAP_CAUSE_SUCCESS;
        uint16_t opcode;
        uint8_t param[2];
        param[0] = 0;
        param[1] = 0;
        cause = gap_vendor_cmd_req(opcode, 2, param);
    }

    //register callback, and handle response
    void app_le_gap_init(void)
    {
        ......
        gap_register_app_cb(app_gap_common_callback);
    }

    void app_gap_common_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_GAP_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_GAP_CB_DATA));
        APP_PRINT_INFO1("app_gap_common_callback: cb_type %d", cb_type);
        switch (cb_type)
        {
        case GAP_MSG_VENDOR_CMD_RSP:
            APP_PRINT_INFO3("GAP_MSG_VENDOR_CMD_RSP: command 0x%x, cause 0x%x, param_len %d",
                        cb_data.p_gap_vendor_cmd_rsp->command,
                        cb_data.p_gap_vendor_cmd_rsp->cause,
                        cb_data.p_gap_vendor_cmd_rsp->param_len);
            break;
       ......
    }
 * \endcode
 */
T_GAP_CAUSE gap_vendor_cmd_req(uint16_t op, uint8_t len, uint8_t *p_param);

/**
 *
 * @brief  Set vendor feature of LE Host.
 *
 * @param[in]  bit_number
 * @arg    2  : Whether to skip reading remote features when the connection is established.
                Disabled by default, the local device reads remote features when the connection is established.
 * @param[in]  bit_value
 * @arg    0  : Disable.
 * @arg    1  : Enable.
 *
 * @return         The result of set parameter request.
 * @retval true    Set parameter request is successful.
 * @retval false   Set parameter request is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool retval = gap_vendor_le_set_host_feature(bit_number, bit_value);
    }
 * \endcode
 */
bool gap_vendor_le_set_host_feature(uint16_t bit_number, uint8_t bit_value);

/** End of GAP_LE_VENDOR_Exported_Functions
  * @}
  */

/** End of GAP_LE_VENDOR
  * @}
  */
#endif /* GAP_VNR_H */