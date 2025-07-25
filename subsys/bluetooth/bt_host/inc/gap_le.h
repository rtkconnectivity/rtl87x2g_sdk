/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_le.h
  * @brief   This file contains all the constants and function prototypes for the GAP protocol.
  * @details
  * @author  jane
  * @date    2016-02-18
  * @version v1.0
  * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_LE_H
#define GAP_LE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <gap.h>
#include <gap_callback_le.h>


/** @addtogroup GAP_LE_COMMON GAP LE Common Module
  * @brief GAP LE common module
  * @{
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_Exported_Types GAP LE Exported Types
  * @brief
  * @{
  */
/** @brief  GAP LE Parameter Types.*/
typedef enum
{
    GAP_PARAM_DEV_STATE                = 0x220, /**< GAP device state. Read only. Size is 1 octet. Value is @ref T_GAP_DEV_STATE. */
    GAP_PARAM_APPEARANCE               = 0x221, /**< Local Device's Appearance. Read/Write. Size is 2 octets.
                                                     Value is @ref GAP_LE_APPEARANCE_VALUES. Default value is @ref GAP_GATT_APPEARANCE_UNKNOWN. */
    GAP_PARAM_DEVICE_NAME              = 0x222, /**< Local Device's Name. Write only. Maximum name string length is @ref GAP_DEVICE_NAME_LEN.
                                                     Default value is 'GAP'. */
    GAP_PARAM_SLAVE_INIT_GATT_MTU_REQ  = 0x223, /**< Whether to initiate the GATT exchange MTU procedure once a connection is established when
                                                     link role is Peripheral.
                                                     Write only. Size is 1 octet. Default value is 0 (disabled). */
    GAP_PARAM_RANDOM_ADDR              = 0x224, /**< Random address. Write only. Size is 6 octets @ref GAP_BD_ADDR_LEN. */
    GAP_PARAM_LATEST_CONN_BD_ADDR      = 0x226, /**< Latest remote connected Bluetooth device address. Read only. Size is 6 octets @ref GAP_BD_ADDR_LEN. */
    GAP_PARAM_LATEST_CONN_BD_ADDR_TYPE = 0x227, /**< Latest remote connected Bluetooth device address type. Read only. Size is 1 octet.
                                                     Value is @ref T_GAP_REMOTE_ADDR_TYPE. */
    GAP_PARAM_HANDLE_CREATE_CONN_IND   = 0x228, /**< Whether to enable APP to handle create connection indication message when link role is Peripheral.
                                                     Write only. Size is 1 octet. Default value is 0 (disabled). */
    GAP_PARAM_DEFAULT_PHYS_PREFER      = 0x229, /**< Preferred values for the transmitter PHY and receiver PHY to be used for all connections over the LE transport.
                                                     Write only. Size is 1 octet. Value is @ref GAP_PHYS_PREFER. Default value is @ref GAP_PHYS_PREFER_ALL. */
    GAP_PARAM_DEFAULT_TX_PHYS_PREFER   = 0x22A, /**< The transmitter PHYs that the Host prefers the Controller to use. Write only. Size is 1 octet.
                                                     Value is @ref GAP_PHYS_PREFER. Default value is @ref GAP_PHYS_PREFER_ALL. */
    GAP_PARAM_DEFAULT_RX_PHYS_PREFER   = 0x22B, /**< The receiver PHYs that the Host prefers the Controller to use. Write only. Size is 1 octet.
                                                     Value is @ref GAP_PHYS_PREFER. Default value is @ref GAP_PHYS_PREFER_ALL. */
    GAP_PARAM_USE_EXTENDED_ADV         = 0x22C, /**< Whether to use LE Advertising Extensions. Write only. Size is 1 octet.
                                                     Default value is 0 (disabled). */
    GAP_PARAM_DEFAULT_DATA_LEN_MAX_TX_OCTETS = 0x22E, /**< The Host's suggested value for the Controller's maximum transmitted number of payload octets in LL Data PDUs.
                                                           Write only. Size is 2 octets. Value range: 0x001B to 0x00FB. */
    GAP_PARAM_DEFAULT_DATA_LEN_MAX_TX_TIME   = 0x22F, /**< The Host's suggested value for the Controller's maximum packet transmission time in microseconds for packets
                                                           containing LL Data PDUs. Write only. Size is 2 octets. Value range: 0x0148 to 0x4290. Units: microsecond. */

    GAP_PARAM_LOCAL_FEATURES           = 0x230, /**< Local supported features. Read only. Size is 8 octets @ref GAP_LE_SUPPORTED_FEATURES_LEN. */
    GAP_PARAM_DS_POOL_ID               = 0x231, /**< Downstream Pool ID. Read only. Size is 2 octets. */
    GAP_PARAM_DS_DATA_OFFSET           = 0x232, /**< Downstream pool buffer data offset. Read only. Size is 2 octets. */
    GAP_PARAM_LE_REMAIN_CREDITS        = 0x233, /**< Remaining credits available for TX. Read only. Size is 1 octet. */
    GAP_PARAM_MAX_WL_SIZE              = 0x234, /**< Maximum Filter Accept List size. Read only. Size is 1 octet. */
    GAP_PARAM_MIN_TX_POWER             = 0x235, /**< The minimum transmit powers supported by the Controller. Read only. Size is 1 octet.
                                                     Range: -127 to +20. Units: dBm. */
    GAP_PARAM_MAX_TX_POWER             = 0x236, /**< The maximum transmit powers supported by the Controller. Read only. Size is 1 octet.
                                                     Range: -127 to +20. Units: dBm. */
    GAP_PARAM_CIS_HOST_SUPPORT         = 0x237, /**< Whether to use CIS of LE Isochronous Channels. Write only. Size is 1 octet.
                                                     Default value is 0 (disabled). */
    GAP_PARAM_MASTER_INIT_GATT_MTU_REQ = 0x238, /**< Whether to initiate the GATT exchange MTU procedure once a connection is established when
                                                     link role is Central.
                                                     Write only. Size is 1 octet. Default value is 1 (enabled). */
} T_GAP_LE_PARAM_TYPE;

/** End of GAP_LE_Exported_Types
  * @}
  */


/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * @defgroup GAP_LE_COMMON_Exported_Functions GAP LE Common Exported Functions
 *
 * @{
 */

/**
  * @brief Callback for GAP LE to notify APP.
  * @param[in] cb_type Callback msg type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data Pointer to callback data @ref T_LE_CB_DATA.
  * @return    Result.
  * @retval    result @ref T_APP_RESULT.
  */
typedef T_APP_RESULT(*P_FUN_LE_APP_CB)(uint8_t cb_type, void *p_cb_data);

/**
 * @brief  Initialize parameters of GAP.
 * @param[in] link_num Initialize link number.
 * @return Operation result.
 * @retval true    Operation success.
 * @retval false   Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    int main(void)
    {
        board_init();
        driver_init();
        le_gap_init(1);
        app_le_gap_init();
        app_le_profile_init();
        pwr_mgr_init();
        task_init();
        os_sched_start();

        return 0;
    }
 * \endcode
 */
bool        le_gap_init(uint8_t link_num);

/**
 * @brief  Set GAP message inform way.
 *
 * Default value is true.
 * - When use_msg is true, GAP will send the GAP message to io_queue registered by gap_start_bt_stack.
 *   Message type is @ref GAP_MSG_TYPE.
 * - When use_msg is false, GAP will send the GAP message using callback function registered by le_register_app_cb.
 *   Message type is @ref GAP_MSG_LE_GAP_STATE_MSG.
 *
 * @param[in] use_msg Whether to use message.
 *
 * <b>Example usage</b>
 * \code{.c}
    int test(void)
    {
        le_gap_msg_info_way(false);
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
        case GAP_MSG_LE_GAP_STATE_MSG:
            APP_PRINT_INFO0("GAP_MSG_LE_GAP_STATE_MSG");
            app_handle_gap_msg((T_IO_MSG *)cb_data->p_gap_state_msg);
            break;
        ...
        }
    }

 * \endcode
 */
void        le_gap_msg_info_way(bool use_msg);

/**
  * @brief  Get maximum supported LE link count.
  * @retval max_link_num Maximum supported LE link count.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        link_num = le_get_max_link_num();
        le_gap_init(link_num);
    }
 * \endcode
 */
uint8_t     le_get_max_link_num(void);

/**
  * @brief  Register APP callback from GAP LE.
  * @param[in]  app_callback Callback function defined by APP.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_le_profile_init(void)
    {
        le_register_app_cb(app_gap_callback);
        client_init(client_num);
        simple_ble_client_id = simp_ble_add_client(app_client_callback);
        client_register_general_client_cb(app_client_callback);
    }
 * \endcode
 */
void        le_register_app_cb(P_FUN_LE_APP_CB app_callback);

/**
  * @brief  Set a GAP LE parameter.
  *
  * This function can be called with a GAP LE Parameter type @ref T_GAP_LE_PARAM_TYPE and it will set the GAP LE parameter.
  * The 'p_value' field must point to an appropriate data type that meets the requirements for the corresponding parameter type.
  * (For example: if required data length for parameter type is 2 octets, p_value should be cast to a pointer of uint16_t.)
  *
  * @param[in]  param GAP LE parameter type @ref T_GAP_LE_PARAM_TYPE.
  * @param[in]  len Length of data to write.
  * @param[in]  p_value Pointer to data to write.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS    Operation success.
  * @retval Others   Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void app_le_gap_init(void)
    {
        ...
        //device name and device appearance
        uint8_t  device_name[GAP_DEVICE_NAME_LEN] = "BLE_PERIPHERAL";
        uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;

        //Set device name and device appearance
        le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, device_name);
        le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(appearance), &appearance);
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_set_gap_param(T_GAP_LE_PARAM_TYPE param, uint8_t len, void *p_value);

/**
  * @brief  Get a GAP LE parameter.
  *
  * This function can be called with a GAP LE Parameter type @ref T_GAP_LE_PARAM_TYPE and it will get the GAP LE parameter.
  * The 'p_value' field must point to an appropriate data type that meets the requirements for the corresponding parameter type.
  * (For example: if required data length for parameter type is 2 octets, p_value should be cast to a pointer of uint16_t.)
  *
  * @param[in]  param GAP LE parameter type @ref T_GAP_LE_PARAM_TYPE.
  * @param[in,out] p_value Pointer to location to get the parameter value.
  *
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS    Operation success.
  * @retval Others   Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        uint8_t gap_link_credits;

        T_GAP_CAUSE cause = le_get_gap_param(GAP_PARAM_LE_REMAIN_CREDITS, &gap_link_credits);
    }
  * \endcode
  */
T_GAP_CAUSE le_get_gap_param(T_GAP_LE_PARAM_TYPE param, void *p_value);

/**
  * @brief  Modify local Filter Accept List.
  *
  * If sending request operation is successful, the modifying result will be returned by callback
  * registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_MODIFY_WHITE_LIST.
  *
  * Applications can only call this API after the Bluetooth Host is ready.
  *
  * Explanation: If the Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
  * with new_state about gap_init_state, which is configured as @ref GAP_INIT_STATE_STACK_READY.
  *
  * @param[in] operation @ref T_GAP_WHITE_LIST_OP.
  * @param[in] bd_addr Pointer to Bluetooth Device Address.
  * @param[in] bd_type Bluetooth Device Address type. Value is @ref T_GAP_REMOTE_ADDR_TYPE.
  * @return The result of sending request.
  * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
  * @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        T_GAP_WHITE_LIST_OP operation = GAP_WHITE_LIST_OP_ADD;
        uint8_t bd_addr[BD_ADDR_SIZE] = {0};
        T_GAP_REMOTE_ADDR_TYPE bd_type = GAP_REMOTE_ADDR_LE_PUBLIC;
        le_modify_white_list(operation, bd_addr, bd_type);
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
        case GAP_MSG_LE_MODIFY_WHITE_LIST:
           APP_PRINT_INFO2("GAP_MSG_LE_MODIFY_WHITE_LIST: operation  0x%x, cause 0x%x",
                           cb_data.p_le_modify_white_list_rsp->operation,
                           cb_data.p_le_modify_white_list_rsp->cause);
           break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_modify_white_list(T_GAP_WHITE_LIST_OP operation, uint8_t *bd_addr,
                                 T_GAP_REMOTE_ADDR_TYPE bd_type);

/**
  * @brief    Generate a local random address.
  * @param[in] rand_addr_type Bluetooth Device Random Address type. Value is @ref T_GAP_RAND_ADDR_TYPE.
  * @param[in,out] random_bd Pointer to Bluetooth Device Address.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS    Operation success.
  * @retval Others   Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        T_GAP_RAND_ADDR_TYPE rand_addr_type = GAP_RAND_ADDR_RESOLVABLE;
        uint8_t random_bd[BD_ADDR_SIZE] = {0};
        le_gen_rand_addr(rand_addr_type, random_bd);
    }
  *
  * \endcode
  */
T_GAP_CAUSE le_gen_rand_addr(T_GAP_RAND_ADDR_TYPE rand_addr_type, uint8_t *random_bd);

/**
  * @brief    Set local random address.
  *
  * If sending request operation is successful, the setting result will be returned by callback
  * registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_SET_RAND_ADDR.
  *
  * Applications can only call this API after the Bluetooth Host is ready.
  *
  * Explanation: If the Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
  * with new_state about gap_init_state, which is configured as @ref GAP_INIT_STATE_STACK_READY.
  *
  * @param[in] random_bd Pointer to local random address.
  * @return The result of sending request.
  * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
  * @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        uint8_t random_bd[BD_ADDR_SIZE] = {0};
        le_set_rand_addr(random_bd);
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
        case GAP_MSG_LE_SET_RAND_ADDR:
            APP_PRINT_INFO1("GAP_MSG_LE_SET_RAND_ADDR: cause 0x%x",
                       cb_data.p_le_set_rand_addr_rsp->cause);
        break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_set_rand_addr(uint8_t *random_bd);

/**
  * @brief    Configure local identity address.
  *
  * This function can be called before @ref gap_start_bt_stack is invoked.
  *
  * @param[in] addr Pointer to local identity address.
  * @param[in] type Local identity address type. Value is @ref T_GAP_IDENT_ADDR_TYPE.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS    Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        T_GAP_IDENT_ADDR_TYPE type = GAP_IDENT_ADDR_PUBLIC;
        uint8_t addr[BD_ADDR_SIZE] = {0};
        le_cfg_local_identity_address(addr, type);
    }
  * \endcode
  */
T_GAP_CAUSE le_cfg_local_identity_address(uint8_t *addr, T_GAP_IDENT_ADDR_TYPE type);

/**
  * @brief   Set Host channel classification.
  *
  * If the request to send the operation is successful, the setting result will be returned by the callback function
  *          registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_SET_HOST_CHANN_CLASSIF.
  *
  * Applications can only call this API after the Bluetooth Host is ready.
  *
  * Explanation: If the Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
  * with new_state about gap_init_state, which is configured as @ref GAP_INIT_STATE_STACK_READY.
  *
  * @param[in]   p_channel_map Pointer to channel map.
  *                            Size: 5 octets (37 bits meaningful).
  *                            This parameter contains 37 1-bit fields.
  *                            The nth such field (in the range 0 to 36) contains the value for the
  *                            Link Layer channel index n.
  *                            Channel n is bad = 0.
  *                            Channel n is unknown = 1.
  *                            The most significant bits are reserved for future use.
  *                            At least one channel shall be marked as unknown.
  *
  * @return The result of sending request.
  * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
  * @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_set_host_chann_classif(p_channel_map);
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
        case GAP_MSG_LE_SET_HOST_CHANN_CLASSIF:
           APP_PRINT_INFO1("GAP_MSG_LE_SET_HOST_CHANN_CLASSIF: cause 0x%x",
                           cb_data.p_le_set_host_chann_classif_rsp->cause);
           break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_set_host_chann_classif(uint8_t *p_channel_map);

/**
  * @brief   Specify suggested values for the maximum transmission number of payload
             octets and maximum packet transmission time for new connections.
  *
  * If sending request operation is successful, the writing result will be returned by callback
  * registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_WRITE_DEFAULT_DATA_LEN.
  *
  * Applications can only call this API after the Bluetooth Host is ready.
  *
  * Explanation: If the Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
  * with new_state about gap_init_state, which is configured as @ref GAP_INIT_STATE_STACK_READY.
  *
  * @param[in]   tx_octets Maximum transmission number of payload octets.
  *                        - Range: 0x001B-0x00FB.
  * @param[in]   tx_time   Maximum packet transmission time in microseconds.
  *                        - Range: 0x0148-0x4290.
  *
 * @return The result of sending request.
 * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
 * @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        uint16_t tx_octets = 0x00FB;
        uint16_t tx_time = 0x0848;
        le_write_default_data_len(tx_octets, tx_time);
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
        case GAP_MSG_LE_WRITE_DEFAULT_DATA_LEN:
           APP_PRINT_INFO1("GAP_MSG_LE_WRITE_DEFAULT_DATA_LEN: cause 0x%x",
                           cb_data->le_cause.cause);
           break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_write_default_data_len(uint16_t tx_octets, uint16_t tx_time);

/**
  * @brief   Check local supported features.
  *
  * @param[in]   array_index Supported features mask array index.
  * @param[in]   feature_mask   Supported features mask.
  *
  * @return Operation result.
  * @retval true  Operation success.
  * @retval false Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
         bool supported = le_check_supported_features(LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX3, LE_SUPPORT_FEATURES_PAST_RECIPIENT_MASK_BIT);
    }
  * \endcode
  */
bool le_check_supported_features(uint8_t array_index, uint8_t feature_mask);
/** @} */ /* End of group GAP_LE_COMMON_Exported_Functions */
/** @} */ /* End of group GAP_LE_COMMON */


/*-------------------------------------------------------------------
-------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* GAP_LE_H */




