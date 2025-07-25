/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     gap_credit_based_conn.h
* @brief    Header file for LE Credit-Based Connection message handling.
* @details  None.
* @author   Tifnan
* @date     2016-03-16
* @version  v0.1
*********************************************************************************************************
*/

#ifndef _LE_CREDIT_BASED_CONN_H_
#define _LE_CREDIT_BASED_CONN_H_

#include <gap_le.h>
#include <gap.h>

/** @addtogroup BT_Host Bluetooth Host
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_CREDIT_BASED_CONN GAP LE Credit Based Connection Module
  * @brief GAP LE Credit Based Connection Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_CREDIT_BASED_CONN_Exported_Macros GAP LE Credit Based Connection Exported Macros
  * @{
  */

#define GAP_COC_MSG_LE_CHANN_STATE          0x01    /**< Information msg type for LE L2CAP Connection Oriented Channel state.
                                                         The structure of callback data is @ref T_LE_COC_CHANN_STATE. */
#define GAP_COC_MSG_LE_REG_PSM              0x02    /**< Response msg type for @ref le_coc_reg_psm.
                                                         The structure of callback data is @ref T_LE_COC_CREDIT_BASED_PSM_REG_RSP. */
#define GAP_COC_MSG_LE_SET_PSM_SECURITY     0x03    /**< Response msg type for @ref le_coc_set_psm_security.
                                                         The structure of callback data is @ref T_LE_COC_CREDIT_BASED_SECURITY_REG_RSP. */
#define GAP_COC_MSG_LE_SEND_DATA            0x04    /**< Response msg type for @ref le_coc_send_data.
                                                         The structure of callback data is @ref T_LE_COC_SEND_DATA. */
#define GAP_COC_MSG_LE_RECEIVE_DATA         0x05    /**< Information msg type to notify APP when receiving data from
                                                         LE L2CAP Connection Oriented Channel.
                                                         The structure of callback data is @ref T_LE_COC_RECEIVE_DATA. */

/** End of GAP_LE_CREDIT_BASED_CONN_Exported_Macros
* @}
*/

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_CREDIT_BASED_CONN_Exported_Types GAP LE Credit Based Connection Exported Types
  * @{
  */
typedef enum
{
    GAP_CHANN_STATE_DISCONNECTED, //!< Disconnected.
    GAP_CHANN_STATE_CONNECTING,   //!< Connecting.
    GAP_CHANN_STATE_CONNECTED,    //!< Connected.
    GAP_CHANN_STATE_DISCONNECTING //!< Disconnecting.
} T_GAP_CHANN_STATE;

typedef enum
{
    COC_PARAM_CREDITS_THRESHOLD                = 0x400,   /**< Credits Threshold. Write only. Size is 1 octet. Default value 2. */
    COC_PARAM_LOCAL_MTU                        = 0x401,   /**< Local MTU Size. Write only. Size is 2 octets. Default value is 200. */
} T_LE_COC_PARAM_TYPE;

typedef enum
{
    COC_CHANN_PARAM_CUR_CREDITS                = 0x410,   //!< Current Credits.
    COC_CHANN_PARAM_MAX_CREDITS                = 0x411,   //!< Maximum Credits.
    COC_CHANN_PARAM_MTU                        = 0x412,   //!< MTU Size.
} T_LE_COC_CHANN_PARAM_TYPE;

/** @brief Definition of LE security requirement*/
typedef enum
{
    LE_COC_SECURITY_NONE,              /**< Security None. */
    LE_COC_SECURITY_UNAUTHEN_ENCRYT,   /**< Security unauthenticated encryption. */
    LE_COC_SECURITY_AUTHEN_ENCRYT,     /**< Security authenticated encryption. */
    LE_COC_SECURITY_UNAUTHEN_DATA_SIGN,/**< Security unauthenticated data signed. */
    LE_COC_SECURITY_AUTHEN_DATA_SIGN,  /**< Security authenticated data signed. */
    LE_COC_SECURITY_AUTHOR             /**< Security authorized. */
} T_LE_COC_SECURITY_MODE;

typedef struct
{
    uint8_t         conn_id;
    uint16_t        cid;
    uint16_t        value_len;
    uint8_t         *p_data;
} T_LE_COC_RECEIVE_DATA;

typedef struct
{
    uint8_t         conn_id;
    uint16_t        cid;
    uint16_t        cause;
    uint8_t         credit;
} T_LE_COC_SEND_DATA;

typedef struct
{
    uint8_t         conn_id;
    uint16_t        cid;
    T_GAP_CHANN_STATE conn_state;
    uint16_t        cause;
} T_LE_COC_CHANN_STATE;

/** @brief Response of LE credit based security register request.*/
typedef struct
{
    uint16_t        cause;
} T_LE_COC_CREDIT_BASED_SECURITY_REG_RSP;

/** @brief Response of LE credit based psm register request.*/
typedef struct
{
    uint16_t        le_psm;
    uint16_t        cause;
} T_LE_COC_CREDIT_BASED_PSM_REG_RSP;

typedef union
{
    T_LE_COC_CHANN_STATE                            *p_le_chann_state;
    T_LE_COC_RECEIVE_DATA                           *p_le_receive_data;
    T_LE_COC_SEND_DATA                              *p_le_send_data;
    T_LE_COC_CREDIT_BASED_SECURITY_REG_RSP          *p_le_set_psm_security_rsp;
    T_LE_COC_CREDIT_BASED_PSM_REG_RSP               *p_le_reg_psm_rsp;
} T_LE_COC_DATA;

/** End of GAP_LE_CREDIT_BASED_CONN_Exported_Types
* @}
*/

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_CREDIT_BASED_CONN_Exported_Functions GAP LE Credit Based Connection Exported Functions
  * @brief
  * @{
  */
/**
  * @brief Callback for COC to notify APP.
  *
  * @param[in] coc_type    Callback msg type.
  * @param[in] p_coc_data  Pointer to callback data @ref T_LE_COC_DATA.
  * @return    Result.
  * @retval    result @ref T_APP_RESULT.
  */
typedef T_APP_RESULT(*P_FUN_LE_COC_APP_CB)(uint8_t coc_type, void *p_coc_data);

/**
 * @brief       Initialize the number of LE L2CAP Connection Oriented Channels.
 *
 * @param[in]   chann_num  Channel number.
 * @return Operation result.
 * @retval true    Operation success.
 * @retval false   Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool ret = le_coc_init(chann_num);
        le_coc_register_app_cb(app_credit_based_conn_callback);
    }
 * \endcode
 */
bool        le_coc_init(uint8_t chann_num);

/**
 * @brief       Register the callback function of LE L2CAP Connection Oriented Channels.
 *
 * @param[in]   app_callback  Callback function.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool ret = le_coc_init(chann_num);
        le_coc_register_app_cb(app_credit_based_conn_callback);
    }

    T_APP_RESULT app_credit_based_conn_callback(uint8_t coc_type, void *p_coc_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_COC_DATA cb_data;
        memcpy(&cb_data, p_coc_data, sizeof(T_LE_COC_DATA));
        APP_PRINT_TRACE1("app_credit_based_conn_callback: coc_type %d", coc_type);
        switch (coc_type)
        {
        case GAP_COC_MSG_LE_CHANN_STATE:
            APP_PRINT_INFO4("GAP_COC_MSG_LE_CHANN_STATE: conn_id %d, cid 0x%x, conn_state %d, cause 0x%x",
                            cb_data.p_le_chann_state->conn_id,
                            cb_data.p_le_chann_state->cid,
                            cb_data.p_le_chann_state->conn_state,
                            cb_data.p_le_chann_state->cause);
            if (cb_data.p_le_chann_state->conn_state == GAP_CHANN_STATE_CONNECTED)
            {
                uint16_t mtu;
                le_coc_get_chann_param(COC_CHANN_PARAM_MTU, &mtu, cb_data.p_le_chann_state->cid);
                APP_PRINT_INFO1("GAP_CHANN_STATE_CONNECTED: mtu %d", mtu);
            }
            break;

        case GAP_COC_MSG_LE_RECEIVE_DATA:
            APP_PRINT_INFO3("GAP_COC_MSG_LE_RECEIVE_DATA: conn_id %d, cid 0x%x, value_len %d",
                            cb_data.p_le_receive_data->conn_id,
                            cb_data.p_le_receive_data->cid,
                            cb_data.p_le_receive_data->value_len);
            break;

        ......

        default:
            break;
        }
        return result;
    }
 * \endcode
 */
void        le_coc_register_app_cb(P_FUN_LE_COC_APP_CB app_callback);

/**
 * @brief       Set parameters of LE L2CAP Connection Oriented Channel.
 *
 * This function can be called with a COC parameter type @ref T_LE_COC_PARAM_TYPE and it will set the COC parameter.
 * The 'p_value' field must point to an appropriate data type that meets the requirements for the corresponding parameter type.
 * (For example: if required data length for parameter type is 2 octets, p_value should be cast to a pointer of uint16_t.)
 *
 * @param[in]   param    COC parameter type @ref T_LE_COC_PARAM_TYPE.
 * @param[in]   len      Length of data to write.
 * @param[in]   p_value  Pointer to data to write.
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS    Operation success.
 * @retval Others               Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_coc_set_param(param, len, p_value);
    }
 * \endcode
 */
T_GAP_CAUSE le_coc_set_param(T_LE_COC_PARAM_TYPE param, uint8_t len, void *p_value);

/**
 * @brief       Get channel parameters of LE L2CAP Connection Oriented Channel.
 *
 * This function can be called with a channel parameter type @ref T_LE_COC_CHANN_PARAM_TYPE and it will get the channel parameter.
 * The 'p_value' field must point to an appropriate data type that meets the requirements for the corresponding parameter type.
 * (For example: if required data length for parameter type is 2 octets, p_value should be cast to a pointer of uint16_t.)
 *
 * @param[in]       param    Channel parameter type @ref T_LE_COC_CHANN_PARAM_TYPE.
 * @param[in, out]  p_value  Pointer to location to get the value.
 * @param[in]       cid      Channel Identifier.
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS    Operation success.
 * @retval Others   Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_coc_get_chann_param(param, p_value, cid);
    }
 * \endcode
 */
T_GAP_CAUSE le_coc_get_chann_param(T_LE_COC_CHANN_PARAM_TYPE param, void *p_value, uint16_t cid);

/**
 * @brief       Create LE L2CAP Connection Oriented Channel.
 *
 * If sending request operation is successful, the creating result will be returned by callback registered
 * by @ref le_coc_register_app_cb with msg type @ref GAP_COC_MSG_LE_CHANN_STATE.
 *
 * @param[in]   conn_id   Connection ID.
 * @param[in]   le_psm    LE PSM.
 * @return The result of sending request.
 * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
 * @retval Others Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_coc_create(conn_id, le_psm);
    }

    T_APP_RESULT app_credit_based_conn_callback(uint8_t coc_type, void *p_coc_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_COC_DATA cb_data;
        memcpy(&cb_data, p_coc_data, sizeof(T_LE_COC_DATA));
        APP_PRINT_TRACE1("app_credit_based_conn_callback: coc_type %d", coc_type);
        switch (coc_type)
        {
        case GAP_COC_MSG_LE_CHANN_STATE:
            APP_PRINT_INFO4("GAP_COC_MSG_LE_CHANN_STATE: conn_id %d, cid 0x%x, conn_state %d, cause 0x%x",
                            cb_data.p_le_chann_state->conn_id,
                            cb_data.p_le_chann_state->cid,
                            cb_data.p_le_chann_state->conn_state,
                            cb_data.p_le_chann_state->cause);
            if (cb_data.p_le_chann_state->conn_state == GAP_CHANN_STATE_CONNECTED)
            {
                uint16_t mtu;
                le_coc_get_chann_param(COC_CHANN_PARAM_MTU, &mtu, cb_data.p_le_chann_state->cid);
                APP_PRINT_INFO1("GAP_CHANN_STATE_CONNECTED: mtu %d", mtu);
            }
            break;
        ......

        default:
            break;
        }
        return result;
    }
 * \endcode
 */
T_GAP_CAUSE le_coc_create(uint8_t conn_id, uint16_t le_psm);

/**
 * @brief       Disconnect LE L2CAP Connection Oriented Channel.
 *
 * If sending request operation is successful, the disconnecting result will be returned by callback registered
 * by @ref le_coc_register_app_cb with msg type @ref GAP_COC_MSG_LE_CHANN_STATE.
 *
 * @param[in]   cid   Channel Identifier.
 * @return The result of sending request.
 * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
 * @retval Others Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_coc_disc(cid);
    }

    T_APP_RESULT app_credit_based_conn_callback(uint8_t coc_type, void *p_coc_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_COC_DATA cb_data;
        memcpy(&cb_data, p_coc_data, sizeof(T_LE_COC_DATA));
        APP_PRINT_TRACE1("app_credit_based_conn_callback: coc_type %d", coc_type);
        switch (coc_type)
        {
        case GAP_COC_MSG_LE_CHANN_STATE:
            APP_PRINT_INFO4("GAP_COC_MSG_LE_CHANN_STATE: conn_id %d, cid 0x%x, conn_state %d, cause 0x%x",
                            cb_data.p_le_chann_state->conn_id,
                            cb_data.p_le_chann_state->cid,
                            cb_data.p_le_chann_state->conn_state,
                            cb_data.p_le_chann_state->cause);
            break;
        ......

        default:
            break;
        }
        return result;
    }
 * \endcode
 */
T_GAP_CAUSE le_coc_disc(uint16_t cid);

/**
 * @brief       Send data by L2CAP Connection Oriented Channel.
 *
 * If sending request operation is successful, the sending result will be returned by callback
 * registered by @ref le_coc_register_app_cb with msg type @ref GAP_COC_MSG_LE_SEND_DATA.
 *
 * @param[in]   cid       Channel Identifier.
 * @param[in]   p_data    The pointer for data will be sent.
 * @param[in]   data_len  Data length.
 * @return The result of sending request.
 * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
 * @retval Others Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_coc_send_data(cid, p_data, data_len);
    }

    T_APP_RESULT app_credit_based_conn_callback(uint8_t coc_type, void *p_coc_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_COC_DATA cb_data;
        memcpy(&cb_data, p_coc_data, sizeof(T_LE_COC_DATA));
        APP_PRINT_TRACE1("app_credit_based_conn_callback: coc_type %d", coc_type);
        switch (coc_type)
        {
        case GAP_COC_MSG_LE_SEND_DATA:
            APP_PRINT_INFO4("GAP_COC_MSG_LE_SEND_DATA: conn_id %d, cid 0x%x, cause 0x%x, credit %d",
                            cb_data.p_le_send_data->conn_id,
                            cb_data.p_le_send_data->cid,
                            cb_data.p_le_send_data->cause,
                            cb_data.p_le_send_data->credit);
            break;
        ......

        default:
            break;
        }
        return result;
    }
 * \endcode
 */
T_GAP_CAUSE le_coc_send_data(uint16_t cid, uint8_t *p_data, uint16_t data_len);

/**
 * @brief       Register or deregister LE PSM.
 *
 * If sending request operation is successful, the registering or deregistering result will be returned by callback
 * registered by @ref le_coc_register_app_cb with msg type @ref GAP_COC_MSG_LE_REG_PSM.
 *
 * @param[in]   le_psm   LE PSM.
 * @param[in]   action   The action for LE PSM.
 *                       - 1:     Register PSM.
 *                       - 0:     Deregister PSM.
 * @return The result of sending request.
 * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
 * @retval Others Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_coc_reg_psm(le_psm, action);
    }

    T_APP_RESULT app_credit_based_conn_callback(uint8_t coc_type, void *p_coc_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_COC_DATA cb_data;
        memcpy(&cb_data, p_coc_data, sizeof(T_LE_COC_DATA));
        APP_PRINT_TRACE1("app_credit_based_conn_callback: coc_type %d", coc_type);
        switch (coc_type)
        {
        case GAP_COC_MSG_LE_REG_PSM:
            APP_PRINT_INFO2("GAP_COC_MSG_LE_REG_PSM: le_psm 0x%x, cause 0x%x",
                            cb_data.p_le_reg_psm_rsp->le_psm,
                            cb_data.p_le_reg_psm_rsp->cause);
            break;
        ......

        default:
            break;
        }
        return result;
    }
 * \endcode
 */
T_GAP_CAUSE le_coc_reg_psm(uint16_t le_psm, uint8_t action);

/**
 * @brief       Set PSM security for LE L2CAP Connection Oriented Channels.
 *
 * If sending request operation is successful, the setting result will be returned by callback
 * registered by @ref le_coc_register_app_cb with msg type @ref GAP_COC_MSG_LE_SET_PSM_SECURITY.
 *
 * @param[in]   le_psm    LE PSM.
 * @param[in]   active    Allocate or release secure entry.
 *                        -    true     Allocate secure entry.
 *                        -    false    Release secure entry.
 * @param[in]   mode      COC security mode @ref T_LE_COC_SECURITY_MODE.
 * @param[in]   key_size  Secure key size.
 * @return The result of sending request.
 * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
 * @retval Others Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_coc_set_psm_security(le_psm, active, mode, key_size);
    }

    T_APP_RESULT app_credit_based_conn_callback(uint8_t coc_type, void *p_coc_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_COC_DATA cb_data;
        memcpy(&cb_data, p_coc_data, sizeof(T_LE_COC_DATA));
        APP_PRINT_TRACE1("app_credit_based_conn_callback: coc_type %d", coc_type);
        switch (coc_type)
        {
        case GAP_COC_MSG_LE_SET_PSM_SECURITY:
            APP_PRINT_INFO1("GAP_COC_MSG_LE_SET_PSM_SECURITY: cause 0x%x",
                            cb_data.p_le_set_psm_security_rsp->cause);
            break;
        ......

        default:
            break;
        }
        return result;
    }
 * \endcode
 */
T_GAP_CAUSE le_coc_set_psm_security(uint16_t le_psm, bool active, T_LE_COC_SECURITY_MODE mode,
                                    uint8_t key_size);
/** End of GAP_LE_CREDIT_BASED_CONN_Exported_Functions
  * @}
  */

/** End of GAP_LE_CREDIT_BASED_CONN
  * @}
  */

/** End of GAP_LE
  * @}
  */

/** End of BT_Host
  * @}
  */



#endif /* _LE_CREDIT_BASED_CONN_H_ */
