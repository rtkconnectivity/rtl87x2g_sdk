/**
*****************************************************************************************
*     Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_ecfc.h
  * @brief   This file contains all the constants and functions prototypes for L2CAP Enhanced Credit Based Flow Control Mode.
  * @details
  * @author  jane
  * @date    2021-04-06
  * @version v1.0
  * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef __GAP_ECFC_H__
#define __GAP_ECFC_H__

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <stdint.h>
#include "gap.h"

#ifdef  __cplusplus
extern "C" {
#endif

/** @defgroup GAP_ECFC_MODULE GAP ECFC Module
  * @brief GAP Enhanced Credit Based Flow Control Mode module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_ECFC_Exported_Macros GAP ECFC Exported Macros
  * @{
  */

#define GAP_ECFC_CREATE_CHANN_MAX_NUM        5  //!< The maximum number of channels created at one time.
#define GAP_ECFC_MIN_MTU                     64 //!< Minimum length of MTU.
#define GAP_ECFC_MIN_MPS                     64 //!< Minimum length of MPS.


/** @defgroup GAP_ECFC_SEC_SETTING GAP ECFC Security Setting
 * @{
 */
#define GAP_ECFC_SEC_BIT_AUTHEN              0x02 //!< Authentication is required.
#define GAP_ECFC_SEC_BIT_MITM                0x04 //!< MITM authentication is required.
#define GAP_ECFC_SEC_BIT_AUTHOR              0x08 //!< User level authorization is required.
#define GAP_ECFC_SEC_BIT_ENCRYPT             0x10 //!< Encryption on link is required.
#define GAP_ECFC_SEC_BIT_SC                  0x20 //!< Secure connection on link is required.
/**
  * @}
  */

/** @defgroup GAP_ECFC_DATA_PATH GAP ECFC Data Path
 * @{
 */
#define GAP_ECFC_DATA_PATH_APP               0x00 //!< The application needs to handle the data. The ATT layer will not handle the data.
#define GAP_ECFC_DATA_PATH_GATT              0x01 //!< The ATT layer handles the data.
/**
  * @}
  */

/** End of GAP_ECFC_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_ECFC_Exported_Types GAP ECFC Exported Types
  * @{
  */

/** @brief L2CAP ECFC channel connection state*/
typedef enum
{
    GAP_ECFC_STATE_DISCONNECTED  = 0, //!< Disconnected.
    GAP_ECFC_STATE_CONNECTED     = 2, //!< Connected.
    GAP_ECFC_STATE_DISCONNECTING = 3  //!< Disconnecting.
} T_GAP_ECFC_STATE;

/** @brief GAP ECFC Parameter List */
typedef enum
{
    GAP_ECFC_PARAM_INIT_CREDITS      = 0x430, //!< Local initial credits. Write only. Size is 2 octets. Default value is 4.
    GAP_ECFC_PARAM_LOCAL_MPS         = 0x431, //!< Local MPS. Write only. Size is 2 octets. Default value is 254.
} T_GAP_ECFC_PARAM_TYPE;

/**
 * @brief  Response of registering protocol over L2CAP. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_PROTO_REG_RSP.
 */
typedef struct
{
    bool      is_le;    //!< Whether using LE transport.
    uint16_t  proto_id; //!< Protocol ID assigned by GAP ECFC Module.
    uint16_t  psm;      //!< PSM of the protocol.
    uint16_t  cause;    //!< Result of registering protocol to L2CAP layer.
} T_GAP_ECFC_PROTO_REG_RSP;

/**
 * @brief  Indication of remote L2CAP ECFC connection request. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_CONN_IND.
 */
typedef struct
{
    uint16_t    proto_id;    //!< Protocol ID assigned by GAP ECFC Module.
    uint16_t    conn_handle; //!< Connection handle.
    uint16_t    remote_mtu;  //!< Remote MTU value.
    uint8_t     cid_num;     //!< The number of the L2CAP ECFC channels request to establish.
    uint16_t
    cid[GAP_ECFC_CREATE_CHANN_MAX_NUM]; //!< Local Channel Identifier array assigned by Bluetooth Host.
    uint8_t     bd_addr[6];  //!< Bluetooth address of remote device.
    uint8_t
    bd_type;     //!< Bluetooth address type of remote device. Value is @ref T_GAP_REMOTE_ADDR_TYPE.
    uint16_t    identity_id; //!< Identity ID for @ref gap_ecfc_send_conn_cfm.
} T_GAP_ECFC_CONN_IND;

/**
 * @brief  Response of starting L2CAP ECFC connection request. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_CONN_RSP.
 */
typedef struct
{
    uint16_t    proto_id;   //!< Protocol ID assigned by GAP ECFC Module.
    uint8_t     cid_num;    //!< The number of the L2CAP ECFC channels request to establish.
    uint16_t    cause;      //!< Result of executing L2CAP ECFC connection request.
    uint8_t     bd_addr[6]; //!< Bluetooth address of remote device.
    uint8_t
    bd_type;    //!< Bluetooth address type of remote device. Value is @ref T_GAP_REMOTE_ADDR_TYPE.
} T_GAP_ECFC_CONN_RSP;

/**
 * @brief  Information of L2CAP ECFC channel establish result. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_CONN_CMPL.
 */
typedef struct
{
    uint16_t    proto_id;       //!< Protocol ID assigned by GAP ECFC Module.
    uint16_t    cause;          //!< Result of establishing a L2CAP ECFC channels with remote device.
    uint16_t    conn_handle;    //!< Connection handle.
    uint8_t     cid_num;        //!< The number of the L2CAP ECFC channels successfully established.
    uint16_t
    cid[GAP_ECFC_CREATE_CHANN_MAX_NUM]; //!< Local Channel Identifier array assigned by Bluetooth Host.
    uint16_t    remote_mtu;     //!< Remote MTU value.
    uint16_t    local_mtu;      //!< Local MTU value.
    uint16_t    local_mps;      //!< Local MPS value.
    uint16_t    ds_data_offset; //!< Offset used to fill user data when sending L2CAP data to remote.
    uint8_t     bd_addr[6];     //!< Bluetooth address of remote device.
    uint8_t
    bd_type;        //!< Bluetooth address type of remote device. Value is @ref T_GAP_REMOTE_ADDR_TYPE.
} T_GAP_ECFC_CONN_CMPL_INFO;

/**
 * @brief  Indication of L2CAP data received from remote side. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_DATA_IND.
 */
typedef struct
{
    uint16_t    proto_id;    //!< Protocol ID assigned by GAP ECFC Module.
    uint16_t    conn_handle; //!< Connection handle.
    uint16_t    cid;         //!< Local Channel Identifier assigned by Bluetooth Host.
    uint16_t    length;      //!< Length of L2CAP data.
    uint16_t    gap;         //!< Offset from data parameter to the real L2CAP data.
    uint8_t     data[1];     //!< The rest of message which contains real L2CAP data at offset of gap.
} T_GAP_ECFC_DATA_IND;

/**
 * @brief  Response of local L2CAP data transmission. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_DATA_RSP.
 */
typedef struct
{
    uint16_t    proto_id;    //!< Protocol ID assigned by GAP ECFC Module.
    uint16_t    conn_handle; //!< Connection handle.
    uint16_t    cid;         //!< Local Channel Identifier assigned by Bluetooth Host.
    uint16_t    cause;       //!< Result of sending data.
} T_GAP_ECFC_DATA_RSP;

/**
 * @brief  Indication of receiving L2CAP disconnection request from remote device. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_DISCONN_IND.
 */
typedef struct
{
    uint16_t    proto_id;    //!< Protocol ID assigned by GAP ECFC Module.
    uint16_t    conn_handle; //!< Connection handle.
    uint16_t    cid;         //!< Local Channel Identifier assigned by Bluetooth Host.
    uint16_t    cause;       //!< L2CAP channel disconnect reason received.
} T_GAP_ECFC_DISCONN_IND;

/**
 * @brief  Response of sending L2CAP disconnection request to remote device. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_DISCONN_RSP.
 */
typedef struct
{
    uint16_t    proto_id;    //!< Protocol ID assigned by GAP ECFC Module.
    uint16_t    conn_handle; //!< Connection handle.
    uint16_t    cid;         //!< Local Channel Identifier assigned by Bluetooth Host.
    uint16_t    cause;       //!< Result of disconnecting L2CAP channel with remote device.
} T_GAP_ECFC_DISCONN_RSP;

/**
 * @brief  Response of registering protocol security into Bluetooth Host. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_SEC_REG_RSP.
 */
typedef struct
{
    bool        is_le;        //!< Whether using LE transport.
    uint16_t    psm;          //!< PSM of the protocol.
    uint8_t     active;       //!< Active/Deactive the security entry.
    uint16_t    uuid;         //!< UUID of the service.
    uint16_t    cause;        //!< Result of registering protocol security.
} T_GAP_ECFC_SEC_REG_RSP;

/**
 * @brief  Response of starting L2CAP ECFC reconfigure request. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_RECONFIGURE_IND.
 */
typedef struct
{
    uint16_t    proto_id;    //!< Protocol ID assigned by GAP ECFC Module.
    uint16_t    cause;       //!< Result of reconfiguration.
    uint16_t    conn_handle; //!< Connection handle.
    uint8_t     cid_num;     //!< The number of the L2CAP ECFC channels successfully reconfigured.
    uint16_t
    cid[GAP_ECFC_CREATE_CHANN_MAX_NUM]; //!< Local Channel Identifier array assigned by Bluetooth Host.
    uint16_t    local_mtu;   //!< Local MTU value.
    uint16_t    local_mps;   //!< Local MPS value.
} T_GAP_ECFC_RECONFIGURE_RSP;

/**
 * @brief  Indication of remote L2CAP ECFC reconfigure request. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_RECONFIGURE_RSP.
 */
typedef struct
{
    uint16_t    proto_id;    //!< Protocol ID assigned by GAP ECFC Module.
    uint16_t    conn_handle; //!< Connection handle.
    uint8_t     cid_num;     //!< The number of the L2CAP ECFC channels need to reconfigure.
    uint16_t
    cid[GAP_ECFC_CREATE_CHANN_MAX_NUM]; //!< Local Channel Identifier array assigned by Bluetooth Host.
    uint16_t    remote_mtu;  //!< Remote MTU.
    uint16_t    remote_mps;  //!< REmote MPS.
} T_GAP_ECFC_RECONFIGURE_IND;

/** @brief L2CAP ECFC connection confirm cause used in @ref gap_ecfc_send_conn_cfm API. */
typedef enum
{
    GAP_ECFC_CONN_ACCEPT               = L2C_ECFC_ALL_CONN_SUCCESS,                                  //!< All connections accept.
    GAP_ECFC_CONN_NO_RESOURCES         = (L2C_ERR | L2C_ECFC_CONN_RSP_SOME_CONN_NO_RESOURCES),       //!< Some connections refused:insufficient resources available.
    GAP_ECFC_CONN_UNACCEPTABLE_PARAMS  = (L2C_ERR | L2C_ECFC_CONN_RSP_ALL_CONN_UNACCEPTABLE_PARAMS), //!< All connections refused:unacceptable parameters.
    GAP_ECFC_CONN_INVALID_PARAMS       = (L2C_ERR | L2C_ECFC_CONN_RSP_ALL_CONN_INVALID_PARAMS),      //!< All connections refused:invalid parameters.
    GAP_ECFC_CONN_PENDING_NO_INFO      = (L2C_ERR | L2C_ECFC_CONN_RSP_ALL_CONN_PENDING_NO_INFO),     //!< All connections pending:no further information available.
    GAP_ECFC_CONN_PENDING_AUTHOR       = (L2C_ERR | L2C_ECFC_CONN_RSP_ALL_CONN_PENDING_AUTHOR),      //!< All connections pending:authorization pending.
} T_GAP_ECFC_CONN_CFM_CAUSE;

/** @brief L2CAP reconfigure confirm cause used in @ref GAP_ECFC_RECONFIGURE_IND callback.*/
typedef enum
{
    GAP_ECFC_RCFG_ACCEPT               = L2C_ECFC_ALL_CONN_SUCCESS,                         //!< All connections accept.
    GAP_ECFC_RCFG_UNACCEPTABLE_PARAMS  = (L2C_ERR | L2C_ECFC_RCFG_RSP_UNACCEPTABLE_PARAMS), //!< Reconfiguration failed - other unacceptable parameters.
} T_GAP_ECFC_RCFG_CFM_CAUSE;

/** @brief  */
typedef enum
{
    GAP_ECFC_PROTO_REG_RSP,              /**< Response msg type for @ref gap_ecfc_reg_proto.
                                              The structure of callback data is @ref T_GAP_ECFC_PROTO_REG_RSP. */
    GAP_ECFC_SEC_REG_RSP,                /**< Response msg type for @ref gap_ecfc_send_sec_reg_req.
                                              The structure of callback data is @ref T_GAP_ECFC_SEC_REG_RSP. */
    GAP_ECFC_CONN_IND,                   /**< Indication msg type for receiving L2CAP ECFC connection request from remote device.
                                              APP shall call @ref gap_ecfc_send_conn_cfm when receiving this msg.
                                              The structure of callback data is @ref T_GAP_ECFC_CONN_IND. */
    GAP_ECFC_CONN_RSP,                   /**< Response msg type for @ref gap_ecfc_send_conn_req.
                                              The structure of callback data is @ref T_GAP_ECFC_CONN_RSP. */
    GAP_ECFC_CONN_CMPL,                  /**< Information msg type for L2CAP ECFC connection establish result.
                                              The structure of callback data is @ref T_GAP_ECFC_CONN_CMPL_INFO. */
    GAP_ECFC_DATA_IND,                   /**< Indication msg type for receiving L2CAP ECFC data from remote device.
                                              The structure of callback data is @ref T_GAP_ECFC_DATA_IND. */
    GAP_ECFC_DATA_RSP,                   /**< Response msg type for @ref gap_ecfc_send_data_req.
                                              The structure of callback data is @ref T_GAP_ECFC_DATA_RSP. */
    GAP_ECFC_DISCONN_IND,                /**< Indication msg type for receiving L2CAP disconnection request from remote device.
                                              The structure of callback data is @ref T_GAP_ECFC_DISCONN_IND. */
    GAP_ECFC_DISCONN_RSP,                /**< Response msg type for @ref gap_ecfc_send_disconn_req.
                                              The structure of callback data is @ref T_GAP_ECFC_DISCONN_RSP. */
    GAP_ECFC_RECONFIGURE_IND,            /**< Indication msg type for receiving ECFC reconfigure request from remote device.
                                              The structure of callback data is @ref T_GAP_ECFC_RECONFIGURE_IND. */
    GAP_ECFC_RECONFIGURE_RSP             /**< Response msg type for @ref gap_ecfc_send_reconfigure_req.
                                              The structure of callback data is @ref T_GAP_ECFC_RECONFIGURE_RSP. */
} T_GAP_ECFC_MSG;

/** End of GAP_ECFC_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * @defgroup GAP_ECFC_EXPORT_Functions GAP ECFC Exported Functions
 *
 * @{
 */
/**
 * @brief Prototype of callback function to handle L2CAP ECFC related messages.
 * @param[in] p_buf     Pointer to the buffer of message data. The content of data is dependent
 *                      on the message type.
 * @param[in] msg       Callback msg type @ref T_GAP_ECFC_MSG.
 * @return    Result used for msg @ref GAP_ECFC_RECONFIGURE_IND.
 * @retval    result @ref T_GAP_ECFC_RCFG_CFM_CAUSE.
 */
typedef uint16_t (* P_GAP_ECFC_CB)(void *p_buf, T_GAP_ECFC_MSG msg);

/**
 * @brief  Initialize parameters of GAP ECFC Module.
 * @param[in] proto_num Initialize protocol number.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        gap_ecfc_init(proto_num);
    }
 * \endcode
 */
bool gap_ecfc_init(uint8_t proto_num);

/**
 * @brief  Set a GAP ECFC parameter.
 *
 * This function can be called with a GAP ECFC parameter type @ref T_GAP_ECFC_PARAM_TYPE and it will set the GAP ECFC parameter.
 * The 'p_value' field must point to an appropriate data type that meets the requirements for the corresponding parameter type.
 * (For example: if required data length for parameter type is 2 octets, p_value should be cast to a pointer of uint16_t.)
 *
 * @param[in] param     GAP ECFC parameter type @ref T_GAP_ECFC_PARAM_TYPE
 * @param[in] len       Length of data to write.
 * @param[in] p_value   Pointer to data to write.
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    bool test(void)
    {
        uint8_t type;
        if (type == 0)
        {
            uint16_t initial_credits;
            if (gap_ecfc_set_param(GAP_ECFC_PARAM_INIT_CREDITS, sizeof(uint16_t), &initial_credits))
            {
                return true;
            }
        }
        else if (type == 1)
        {
            uint16_t local_mps;
            if (gap_ecfc_set_param(GAP_ECFC_PARAM_LOCAL_MPS, sizeof(uint16_t), &local_mps))
            {
                return true;
            }
        }
        return false;
    }
 * \endcode
 */
bool gap_ecfc_set_param(T_GAP_ECFC_PARAM_TYPE param, uint8_t len, void *p_value);

/**
 * @brief  Register a callback function to handle L2CAP ECFC related messages for a specific PSM.
 *
 * If sending request operation is successful, the registering result will be returned by callback
 * registered by @ref gap_ecfc_reg_proto with msg type @ref GAP_ECFC_PROTO_REG_RSP.
 *
 * @param[in] psm       Protocol service multiplexer that the callback function is related.
 * @param[in] callback  Callback function to handle L2CAP ECFC messages. The function
 *                      must have the prototype defined as @ref P_GAP_ECFC_CB.
 * @param[in] is_le     Whether is LE transport.
 * @param[in,out] p_proto_id Pointer to protocol ID assigned by GAP ECFC module which will be used in @ref gap_ecfc_send_conn_req.
 * @param[in] data_path Data path @ref GAP_ECFC_DATA_PATH.
 * @return The result of sending request.
 * @retval true Sending request operation is successful.
 * @retval false Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
   uint8_t ecfc_proto_id;

   uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
   {
       switch (msg)
       {
        case GAP_ECFC_PROTO_REG_RSP:
            {
                T_GAP_ECFC_PROTO_REG_RSP *p_rsp = (T_GAP_ECFC_PROTO_REG_RSP *)p_buf;
                APP_PRINT_INFO3("GAP_ECFC_PROTO_REG_RSP: proto_id %d, psm 0x%x, cause 0x%x",
                                p_rsp->proto_id,
                                p_rsp->psm,
                                p_rsp->cause);
            }
            break;

       default:
           break;
       }
   }

   int test(void)
   {
       gap_ecfc_reg_proto(PSM_EATT, app_ecfc_callback, true, &ecfc_proto_id, GAP_ECFC_DATA_PATH_GATT);

       return 0;
   }
 * \endcode
 */
bool gap_ecfc_reg_proto(uint16_t psm, P_GAP_ECFC_CB callback, bool is_le, uint8_t *p_proto_id,
                        uint8_t data_path);

/**
 * @brief    Send a request to create L2CAP ECFC connections.
 *
 * If the request was successfully sent, @ref GAP_ECFC_CONN_RSP will be received in the callback function registered by
 * @ref gap_ecfc_reg_proto to indicate whether the procedure was started successfully.
 *
 * If the procedure was started, @ref GAP_ECFC_CONN_CMPL will be received later about the
 * result of L2CAP channel establishment.
 *
 * @param[in] psm       PSM of the L2CAP ECFC channel that will be established.
 * @param[in] proto_id  Protocol ID.
 * @param[in] local_mtu Preferred MTU size value of the L2CAP ECFC channel.
 * @param[in] bd_addr   Pointer to Bluetooth address of remote device.
 * @param[in] bd_type   Bluetooth address type of remote device.
 * @param[in] cid_num   The number of the L2CAP ECFC channels request to establish.
 *                      - Range: (1- @ref GAP_ECFC_CREATE_CHANN_MAX_NUM)
 * @return The result of sending request.
 * @retval true Sending request operation is successful.
 * @retval false Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
   uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
   {
       switch (msg)
       {
        case GAP_ECFC_CONN_RSP:
            {
                T_GAP_ECFC_CONN_RSP *p_rsp = (T_GAP_ECFC_CONN_RSP *)p_buf;
                APP_PRINT_INFO5("GAP_ECFC_CONN_RSP: proto_id %d, cid_num %d, cause 0x%x, bd_addr %s, bd_type 0x%x",
                                p_rsp->proto_id,
                                p_rsp->cid_num,
                                p_rsp->cause,
                                TRACE_BDADDR(p_rsp->bd_addr),
                                p_rsp->bd_type);
            }
            break;

        case GAP_ECFC_CONN_CMPL:
            {
                T_GAP_ECFC_CONN_CMPL_INFO *p_info = (T_GAP_ECFC_CONN_CMPL_INFO *)p_buf;
                APP_PRINT_INFO6("GAP_ECFC_CONN_CMPL: proto_id %d, cause 0x%x, conn_handle 0x%x, ds_data_offset %d, bd_addr %s, bd_type 0x%x",
                                p_info->proto_id,
                                p_info->cause,
                                p_info->conn_handle,
                                p_info->ds_data_offset,
                                TRACE_BDADDR(p_info->bd_addr),
                                p_info->bd_type);
                APP_PRINT_INFO3("GAP_ECFC_CONN_CMPL: remote_mtu %d, local_mtu %d, local_mps %d",
                                p_info->remote_mtu,
                                p_info->local_mtu,
                                p_info->local_mps);
                for (uint8_t i = 0; i < p_info->cid_num; i++)
                {
                    APP_PRINT_INFO2("GAP_ECFC_CONN_CMPL: cis[%d] 0x%x", i, p_info->cid[i]);
                }
            }
            break;

       default:
           break;
       }
   }

   int test(void)
   {
       gap_ecfc_reg_proto(PSM_EATT, app_ecfc_callback, true, &ecfc_proto_id, GAP_ECFC_DATA_PATH_GATT);

       return 0;
   }
    static T_USER_CMD_PARSE_RESULT cmd_ecfccon(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t addr[6] = {0};
        uint8_t addr_len;
        for (addr_len = 0; addr_len < GAP_BD_ADDR_LEN; addr_len++)
        {
            addr[addr_len] = p_parse_value->dw_param[GAP_BD_ADDR_LEN - addr_len - 1];
        }
        if (gap_ecfc_send_conn_req(PSM_EATT, ecfc_proto_id,
                                ecfc_local_mtu, addr, GAP_REMOTE_ADDR_LE_PUBLIC, p_parse_value->dw_param[6]))
        {
            return RESULT_SUCESS;
        }
        return RESULT_ERR;
    }
 * \endcode
 */
bool gap_ecfc_send_conn_req(uint16_t psm, uint16_t proto_id,
                            uint16_t local_mtu, uint8_t *bd_addr, uint8_t bd_type, uint8_t cid_num);

/**
 * @brief    Send a confirmation for a L2CAP ECFC connection request from remote device.
 *
 * If the confirmation was successfully sent with cause as @ref GAP_ECFC_CONN_ACCEPT,
 * @ref GAP_ECFC_CONN_CMPL will also be received in the callback function registered by
 * @ref gap_ecfc_reg_proto about the result of L2CAP ECFC channels establishment.
 *
 * If there was no resources to establishment all cid_num (@ref T_GAP_ECFC_CONN_IND) channels,
 * the confirmation can reduce cid_num and sent with cause as @ref GAP_ECFC_CONN_NO_RESOURCES.
 *
 * @param[in] conn_handle Connection handle of the ACL link.
 * @param[in] identity_id Identity ID. The value shall be gotten from @ref T_GAP_ECFC_CONN_IND message.
 * @param[in] cause       Confirmation cause for the connection request from remote device.
 *                        Value is @ref T_GAP_ECFC_CONN_CFM_CAUSE.
 * @param[in] p_cid       Pointer to the cid array allow to establish.
 *                        The array shall be the subset of the p_cid get from @ref T_GAP_ECFC_CONN_IND message.
 * @param[in] cid_num     Confirmation the number of the L2CAP ECFC channels allow to establish.
 * @param[in] local_mtu   Preferred MTU size value of the L2CAP ECFC channel.
 * @return Operation result.
 * @retval true    Operation success.
 * @retval false   Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
   uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
   {
       switch (msg)
       {
        case GAP_ECFC_CONN_IND:
            {
                T_GAP_ECFC_CONN_IND *p_ind = (T_GAP_ECFC_CONN_IND *)p_buf;
                T_GAP_ECFC_CONN_CFM_CAUSE cause = GAP_ECFC_CONN_ACCEPT;
                APP_PRINT_INFO8("GAP_ECFC_CONN_IND: proto_id %d, conn_handle 0x%x, remote_mtu %d, cid_num %d, cis %b, bd_addr %s, bd_type 0x%x, identity_id 0x%x",
                                p_ind->proto_id,
                                p_ind->conn_handle,
                                p_ind->remote_mtu,
                                p_ind->cid_num,
                                TRACE_BINARY(GAP_ECFC_CREATE_CHANN_MAX_NUM * 2, p_ind->cid),
                                TRACE_BDADDR(p_ind->bd_addr),
                                p_ind->bd_type,
                                p_ind->identity_id);
                gap_ecfc_send_conn_cfm(p_ind->conn_handle, p_ind->identity_id,
                                    cause, p_ind->cid, p_ind->cid_num, ecfc_local_mtu);
            }
            break;

        case GAP_ECFC_CONN_CMPL:
            {
                T_GAP_ECFC_CONN_CMPL_INFO *p_info = (T_GAP_ECFC_CONN_CMPL_INFO *)p_buf;
                APP_PRINT_INFO6("GAP_ECFC_CONN_CMPL: proto_id %d, cause 0x%x, conn_handle 0x%x, ds_data_offset %d, bd_addr %s, bd_type 0x%x",
                                p_info->proto_id,
                                p_info->cause,
                                p_info->conn_handle,
                                p_info->ds_data_offset,
                                TRACE_BDADDR(p_info->bd_addr),
                                p_info->bd_type);
                APP_PRINT_INFO3("GAP_ECFC_CONN_CMPL: remote_mtu %d, local_mtu %d, local_mps %d",
                                p_info->remote_mtu,
                                p_info->local_mtu,
                                p_info->local_mps);
                for (uint8_t i = 0; i < p_info->cid_num; i++)
                {
                    APP_PRINT_INFO2("GAP_ECFC_CONN_CMPL: cis[%d] 0x%x", i, p_info->cid[i]);
                }
            }
            break;

       default:
           break;
       }
   }

   int test(void)
   {
       gap_ecfc_reg_proto(PSM_EATT, app_ecfc_callback, true, &ecfc_proto_id, GAP_ECFC_DATA_PATH_GATT);

       return 0;
   }
 * \endcode
 */
bool gap_ecfc_send_conn_cfm(uint16_t conn_handle, uint16_t identity_id,
                            T_GAP_ECFC_CONN_CFM_CAUSE cause,
                            uint16_t *p_cid, uint8_t cid_num, uint16_t local_mtu);

/**
 * @brief    Send a request to disconnect a L2CAP ECFC connection.
 *
 * If sending request operation is successful, the disconnecting result will be returned by callback
 * registered by @ref gap_ecfc_reg_proto with msg type @ref GAP_ECFC_DISCONN_RSP.
 *
 * @param[in] conn_handle Connection handle of the ACL link.
 * @param[in] cid         Local Channel Identifier of the L2CAP ECFC channel to disconnect.
 * @return The result of sending request.
 * @retval true Sending request operation is successful.
 * @retval false Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
   uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
   {
       switch (msg)
       {
        case GAP_ECFC_DISCONN_RSP:
            {
                T_GAP_ECFC_DISCONN_RSP *p_rsp = (T_GAP_ECFC_DISCONN_RSP *)p_buf;
                APP_PRINT_INFO4("GAP_ECFC_DISCONN_RSP: proto_id %d, conn_handle 0x%x, cid 0x%x, cause 0x%x",
                                p_rsp->proto_id,
                                p_rsp->conn_handle,
                                p_rsp->cid,
                                p_rsp->cause);
            }
            break;

       default:
           break;
       }
   }

   void test(void)
   {
        bool result = gap_ecfc_send_disconn_req(conn_handle, cid);
   }
 * \endcode
 */
bool gap_ecfc_send_disconn_req(uint16_t conn_handle, uint16_t cid);

/**
 * @brief  Send a request to send L2CAP ECFC channel data to remote device.
 *
 * Applications can use this API when data_path is @ref GAP_ECFC_DATA_PATH_APP configured by @ref gap_ecfc_reg_proto.
 *
 * If sending request operation is successful, the sending result will be returned by callback
 * registered by @ref gap_ecfc_reg_proto with msg type @ref GAP_ECFC_DATA_RSP.
 *
 * @param[in] conn_handle Connection handle of the ACL link.
 * @param[in] cid         Local Channel Identifier. The value can be gotten from @ref GAP_ECFC_CONN_CMPL message.
 * @param[in] p_data      Pointer to data to be sent.
 * @param[in] length      Length of value to be sent.
 *                        - Range: 0 ~ local_mtu.
 * @return The result of sending request.
 * @retval true Sending request operation is successful.
 * @retval false Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
   uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
   {
       switch (msg)
       {
        case GAP_ECFC_DATA_RSP:
            {
                T_GAP_ECFC_DATA_RSP *p_rsp = (T_GAP_ECFC_DATA_RSP *)p_buf;
                APP_PRINT_INFO4("GAP_ECFC_DATA_RSP: proto_id %d, conn_handle 0x%d, cid 0x%x, cause 0x%x",
                                p_rsp->proto_id,
                                p_rsp->conn_handle,
                                p_rsp->cid,
                                p_rsp->cause);
            }
            break;

       default:
           break;
       }
   }

   void test(void)
   {
        bool result = gap_ecfc_send_data_req(conn_handle, cid, &data, length);
   }
 * \endcode
 */
bool gap_ecfc_send_data_req(uint16_t conn_handle, uint16_t cid, uint8_t *p_data, uint16_t length);

/**
 * @brief    Send a request to register or deregister a protocol security entry over L2CAP.
 *           The security entry will be used when establishing a L2CAP ECFC channel.
 *
 * If the request was successfully sent, a message whose type is @ref GAP_ECFC_SEC_REG_RSP
 * and data is @ref T_GAP_ECFC_SEC_REG_RSP will be received in the callback
 * function registered by @ref gap_ecfc_reg_proto.
 *
 * @param[in] is_le       Whether is LE transport.
 * @param[in] active      Register or deregister the security entry.
 * @param[in] psm         PSM value for protocol.
 * @param[in] uuid        UUID of the service.
 * @param[in] requirement A bit field security requirement of the entry.
 *                        Valid values are combinations of @ref GAP_ECFC_SEC_SETTING.
 * @param[in] key_size    Key size requirement of the entry.
 * @return The result of sending request.
 * @retval true Sending request operation is successful.
 * @retval false Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
   uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
   {
       switch (msg)
       {
        case GAP_ECFC_SEC_REG_RSP:
            {
                T_GAP_ECFC_SEC_REG_RSP *p_rsp = (T_GAP_ECFC_SEC_REG_RSP *)p_buf;
                APP_PRINT_INFO5("GAP_ECFC_SEC_REG_RSP: is_le %d, psm 0x%x, active %d, uuid 0x%x, cause 0x%x",
                                p_rsp->is_le,
                                p_rsp->psm,
                                p_rsp->active,
                                p_rsp->uuid,
                                p_rsp->cause);
            }
            break;

       default:
           break;
       }
   }

   int test(void)
   {
       gap_ecfc_reg_proto(PSM_EATT, app_ecfc_callback, true, &ecfc_proto_id, GAP_ECFC_DATA_PATH_GATT);

       return 0;
   }
   bool test(void)
   {
        uint8_t requirements;
        if (gap_ecfc_send_sec_reg_req(true, 1, PSM_EATT, 0, requirements, 16))
        {
            return true;
        }
        else
        {
            return false;
        }
   }
 * \endcode
 */
bool gap_ecfc_send_sec_reg_req(bool is_le, uint8_t active, uint16_t psm,
                               uint16_t uuid, uint8_t requirement, uint8_t key_size);

/**
 * @brief    Send a request to reconfigure L2CAP ECFC channel.
 *
 * If sending request operation is successful, the reconfiguring result will be returned by callback
 * registered by @ref gap_ecfc_reg_proto with msg type @ref GAP_ECFC_RECONFIGURE_RSP.
 *
 * @param[in] conn_handle Connection handle of the ACL link.
 * @param[in] p_cid       Pointer to local Channel Identifier array need to reconfigure.
 * @param[in] cid_num     The cid number of p_cid array.
 *                        - Range: (1 - @ref GAP_ECFC_CREATE_CHANN_MAX_NUM).
 * @param[in] local_mtu   Preferred Local MTU size value of the L2CAP ECFC channel.
 * @param[in] local_mps   Preferred Local MPS value of the L2CAP ECFC channel.
 * @return The result of sending request.
 * @retval true Sending request operation is successful.
 * @retval false Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
   uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
   {
       switch (msg)
       {
        case GAP_ECFC_RECONFIGURE_RSP:
            {
                T_GAP_ECFC_RECONFIGURE_RSP *p_rsp = (T_GAP_ECFC_RECONFIGURE_RSP *)p_buf;
                APP_PRINT_INFO7("GAP_ECFC_RECONFIGURE_RSP: proto_id %d, cause 0x%x, conn_handle 0x%x, local_mtu %d, local_mps %d,cid_num %d, cis %b",
                                p_rsp->proto_id,
                                p_rsp->cause,
                                p_rsp->conn_handle,
                                p_rsp->local_mtu,
                                p_rsp->local_mps,
                                p_rsp->cid_num,
                                TRACE_BINARY(GAP_ECFC_CREATE_CHANN_MAX_NUM * 2, p_rsp->cid));
            }
            break;

       default:
           break;
       }
   }

   void test(void)
   {
        bool result = gap_ecfc_send_reconfigure_req(conn_handle, &cid, cid_num, local_mtu, local_mps);
   }
 * \endcode
 */
bool gap_ecfc_send_reconfigure_req(uint16_t conn_handle, uint16_t *p_cid, uint8_t cid_num,
                                   uint16_t local_mtu, uint16_t local_mps);

/** @} */ /* End of group GAP_ECFC_EXPORT_Functions */
/** @} */ /* End of group GAP_ECFC_MODULE */

#ifdef  __cplusplus
}
#endif
#endif  /*  __GAP_ECFC_H__ */
