/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     profile_server_ext.h
  * @brief    Header file for server structure.
  * @details  This file can be used when the parameter use_ext of the server_cfg_use_ext_api is true.
  * @author
  * @date     2021-04-07
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef PROFILE_SERVER_EXT_H
#define PROFILE_SERVER_EXT_H

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "bt_types.h"
#include "gatt.h"
#include "gap_le.h"
#include "profile_server_def.h"

/** @addtogroup GATT_SERVER_API GATT Server API
  * @brief GATT Server API
  * @{
  */

/** @defgroup GATT_SERVER_EXT_API GATT Server Extension API
  * @brief The GATT Server extension APIs can be used when the parameter use_ext of the @ref server_cfg_use_ext_api is true.
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GATT_SERVER_EXT_Exported_Macros GATT Server Extension Exported Macros
  * @brief
  * @{
  */
/** @defgroup GATT_SERVER_EXT_CB_DATA Server Extension Callback Data
  * @brief Data for profile to inform application.
  * @{
  */
/** @brief The callback data of PROFILE_EVT_SEND_DATA_COMPLETE. */
typedef struct
{
    uint16_t        credits;
    uint16_t        conn_handle;
    uint16_t        cid;
    T_SERVER_ID     service_id;
    uint16_t        attrib_idx;
    uint16_t        cause;
} T_EXT_SEND_DATA_RESULT;

/** @brief Service callback data. */
typedef union
{
    T_SERVER_RESULT     service_reg_result;
    T_EXT_SEND_DATA_RESULT  send_data_result;
    T_SERVER_REG_AFTER_INIT_RESULT    server_reg_after_init_result;
    T_SERVER_CLEAR_SERVICE_AFTER_INIT_RESULT  clear_service_after_init_result;
    T_SERVER_SERVICE_CHANGE_STATE             service_change_state;
} T_SERVER_EXT_CB_DATA;

typedef struct
{
    T_SERVER_CB_TYPE   eventId;
    T_SERVER_EXT_CB_DATA   event_data;
} T_SERVER_EXT_APP_CB_DATA;
/** @} End of GATT_SERVER_EXT_CB_DATA */

/** @defgroup P_FUN_EXT_WRITE_IND_POST_PROC Write Post Function Point Definition
  * @brief Callback function to execute some post procedure after handling a write request from the client.
  * @{
  */
typedef void (* P_FUN_EXT_WRITE_IND_POST_PROC)(uint16_t conn_handle, uint16_t cid,
                                               T_SERVER_ID service_id,
                                               uint16_t attrib_index, uint16_t length,
                                               uint8_t *p_value);
/** @} End of P_FUN_EXT_WRITE_IND_POST_PROC */

/** @defgroup P_FUN_SERVER_EXT_SPECIFIC_CB Specific Service Callback Function Point Definition
  * @{ Function pointer used in each specific service module, to send events to the specific service module.
  */
typedef T_APP_RESULT(*P_FUN_GATT_EXT_READ_ATTR_CB)(uint16_t conn_handle, uint16_t cid,
                                                   T_SERVER_ID service_id,
                                                   uint16_t attrib_index,
                                                   uint16_t offset, uint16_t *p_length, uint8_t **pp_value);
typedef T_APP_RESULT(*P_FUN_GATT_EXT_WRITE_ATTR_CB)(uint16_t conn_handle, uint16_t cid,
                                                    T_SERVER_ID service_id,
                                                    uint16_t attrib_index, T_WRITE_TYPE write_type,
                                                    uint16_t length, uint8_t *p_value, P_FUN_EXT_WRITE_IND_POST_PROC *p_write_post_proc);
typedef void (*P_FUN_GATT_EXT_CCCD_UPDATE_CB)(uint16_t conn_handle, uint16_t cid,
                                              T_SERVER_ID service_id,
                                              uint16_t attrib_index, uint16_t ccc_bits);
/** End of P_FUN_SERVER_EXT_SPECIFIC_CB
  * @}
  */

/** @defgroup P_FUN_SERVER_EXT_GENERAL_CB General Server Callback Function Point Definition
  * @brief Function pointer type used to generate callback, to send events to the application.
  * @{
  */
typedef T_APP_RESULT(*P_FUN_EXT_SERVER_GENERAL_CB)(T_SERVER_ID service_id, void *p_para);
/** @} End of P_FUN_SERVER_EXT_GENERAL_CB */


/** @brief GATT extension service callbacks */
typedef struct
{
    P_FUN_GATT_EXT_READ_ATTR_CB read_attr_cb;     /**< Read callback function pointer.
                                                   Return value: @ref T_APP_RESULT. */
    P_FUN_GATT_EXT_WRITE_ATTR_CB write_attr_cb;   /**< Write callback function pointer.
                                                   Return value: @ref T_APP_RESULT. */
    P_FUN_GATT_EXT_CCCD_UPDATE_CB cccd_update_cb; /**< Update CCCD callback function pointer. */
} T_FUN_GATT_EXT_SERVICE_CBS;

/** End of GATT_SERVER_EXT_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GATT_SERVER_EXT_Exported_Functions GATT Server Extension Exported Functions
  * @brief
  * @{
  */
/**
 * @brief Register specific service without start handle.
 *
 * Add specific service information to gatt_svc_table struct, will be registered to GATT later.
 *
 * @param[in,out] p_out_service_id  Pointer to service ID of specific service @ref T_SERVER_ID.
 * @param[in] p_database            Pointer to Database of specific service.
 * @param[in] length                Length of Database of specific service.
 * @param[in] p_srv_ext_cbs         Pointer to service callback functions of specific service.
 * @return Operation result.
 * @retval true  Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    T_SERVER_ID bas_ext_add_service(void *p_func)
    {
        T_SERVER_ID service_id;
        if (false == server_ext_add_service(&service_id,
                                       (uint8_t *)bas_attr_tbl,
                                       bas_attr_tbl_size,
                                       bas_cbs))
        {
            APP_PRINT_ERROR1("bas_add_service: service_id %d", service_id);
            service_id = 0xFF;
        }
        pfn_bas_cb = (P_FUN_EXT_SERVER_GENERAL_CB)p_func;
        return service_id;
    }
 * \endcode
 */
bool server_ext_add_service(T_SERVER_ID *p_out_service_id, uint8_t *p_database, uint16_t length,
                            const T_FUN_GATT_EXT_SERVICE_CBS *p_srv_ext_cbs);

/**
 * @brief Register specific service with start handle.
 *
 * Add specific service information to gatt_svc_table struct, will be registered to GATT later.
 *
 * @param[in,out] p_out_service_id  Pointer to service ID of specific service @ref T_SERVER_ID.
 * @param[in] p_database            Pointer to Database of specific service.
 * @param[in] length                Length of Database of specific service.
 * @param[in] p_srv_ext_cbs         Pointer to service callback functions of specific service.
 * @param[in] start_handle          Start handle of this service.
 * @return Operation result.
 * @retval true  Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    T_SERVER_ID bas_ext_add_service(void *p_func)
    {
        T_SERVER_ID service_id;
        if (false == server_ext_add_service_by_start_handle(&service_id,
                                       (uint8_t *)bas_attr_tbl,
                                       bas_attr_tbl_size,
                                       bas_cbs, 0x00f0))
        {
            APP_PRINT_ERROR1("bas_add_service: service_id %d", service_id);
            service_id = 0xFF;
        }
        pfn_bas_cb = (P_FUN_EXT_SERVER_GENERAL_CB)p_func;
        return service_id;
    }
 * \endcode
 */
bool server_ext_add_service_by_start_handle(uint8_t *p_out_service_id, uint8_t *p_database,
                                            uint16_t length,
                                            T_FUN_GATT_EXT_SERVICE_CBS *p_srv_ext_cbs, uint16_t start_handle);
/**
 * @brief Register callback function to send events to the application.
 *
 * @param[in] p_fun_cb          Callback function.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_le_profile_init(void)
    {
        server_init(service_num);
        simp_srv_id = simp_ble_service_ext_add_service(app_ext_profile_callback);
        server_ext_register_app_cb(app_ext_profile_callback);
    }
 * \endcode
 */
void server_ext_register_app_cb(P_FUN_EXT_SERVER_GENERAL_CB p_fun_cb);

/**
  * @brief  Confirm from the application when receiving a read request from the client.
  * @param[in]  conn_handle   Connection handle of the ACL link.
  * @param[in]  cid           Local Channel Identifier. The value shall be gotten from @ref P_FUN_GATT_EXT_READ_ATTR_CB callback.
  * @param[in]  service_id    Service ID @ref T_SERVER_ID.
  * @param[in]  attrib_index  Attribute index of the attribute to read confirmation from the application.
  * @param[in]  p_data        Pointer to the read value.
  * @param[in]  data_len      The length of the data.
  * @param[in]  total_len     Total length.
  * @param[in]  cause         Cause for read confirmation. Value is @ref T_APP_RESULT.
  * @return Operation result.
  * @retval true  Operation success.
  * @retval false Operation failure.
  */
bool server_ext_attr_read_confirm(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id,
                                  uint16_t attrib_index, uint8_t *p_data, uint16_t data_len,
                                  uint16_t total_len, T_APP_RESULT cause);
/**
  * @brief  Confirm from the application when receiving an execute write request from the client.
  * @param[in]  conn_handle Connection handle of the ACL link.
  * @param[in]  cid         Local Channel Identifier. The value shall be gotten from @ref P_FUN_GATT_EXT_WRITE_ATTR_CB callback.
  * @param[in]  cause       Cause for execute write confirmation. Value is @ref T_APP_RESULT.
  * @param[in]  handle      GATT attribute handle.
  * @return Operation result.
  * @retval true  Operation success.
  * @retval false Operation failure.
  */
bool server_ext_exec_write_confirm(uint16_t conn_handle, uint16_t cid, uint16_t cause,
                                   uint16_t handle);

/**
  * @brief  Confirm from the application when receiving a write request from the client.
  * @param[in]  conn_handle  Connection handle of the ACL link.
  * @param[in]  cid          Local Channel Identifier. The value shall be gotten from @ref P_FUN_GATT_EXT_WRITE_ATTR_CB callback.
  * @param[in]  service_id   Service ID @ref T_SERVER_ID.
  * @param[in]  attrib_index Attribute index of the attribute to write confirmation from the application.
  * @param[in]  cause        Cause for write confirmation. Value is @ref T_APP_RESULT.
  * @return Operation result.
  * @retval true  Operation success.
  * @retval false Operation failure.
  */
bool server_ext_attr_write_confirm(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id,
                                   uint16_t attrib_index, T_APP_RESULT cause);

/**
 * @brief Send a notification or indication to peer device.
 *
 * If sending request operation is successful, the sending result will be returned by callback
 * registered by @ref server_ext_register_app_cb with service_id set to @ref SERVICE_PROFILE_GENERAL_ID,
 * and eventId of p_data (@ref T_SERVER_EXT_APP_CB_DATA) set to @ref PROFILE_EVT_SEND_DATA_COMPLETE.
 *
 * @param[in] conn_handle     Connection handle of the ACL link.
 * @param[in] cid             Local Channel Identifier assigned by Bluetooth Host. Shall use the channel
 *                            in the @ref GAP_CHANNEL_STATE_CONNECTED state.
 * @param[in] service_id      Service ID @ref T_SERVER_ID.
 * @param[in] attrib_index    Attribute index of the characteristic.
 * @param[in] p_data          Pointer to data to be sent.
 * @param[in] data_len        Length of value to be sent, range: 0~(mtu_size - 3).
                              uint16_t mtu_size is acquired by @ref gap_chann_get_info (conn_handle, cid, p_info).
 * @param[in] type            GATT PDU type @ref T_GATT_PDU_TYPE.
 * @return The result of sending request.
 * @retval true Sending request operation is successful.
 * @retval false Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
  bool simp_ext_service_send_v3_notify(uint16_t conn_handle,  uint16_t cid, T_SERVER_ID service_id,
                                      void *p_value,
                                      uint16_t length)
  {
      APP_PRINT_INFO0("simp_ext_service_send_v3_notify");
      // send notification to client
      return server_ext_send_data(conn_handle, cid, service_id, SIMPLE_EXT_SERVICE_CHAR_V3_NOTIFY_INDEX,
                                  p_value,
                                  length,
                                  GATT_PDU_TYPE_ANY);
  }
 * \endcode
 */
bool server_ext_send_data(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id,
                          uint16_t attrib_index,
                          uint8_t *p_data, uint16_t data_len, T_GATT_PDU_TYPE type);

/**
 * @brief Get the header pointer of the write command data buffer.
 *
 * This function is used to get the buffer pointer of the write command data.
 * This function only can be called in write_attr_cb.
 *
 * @param[in]  conn_handle    Connection handle of the ACL link.
 * @param[in]  cid            Local Channel Identifier.
 *                            The value shall be gotten from @ref P_FUN_GATT_EXT_WRITE_ATTR_CB callback.
 * @param[in,out] pp_buffer   Pointer to the address of the buffer.
 * @param[in,out] p_offset    Pointer to the offset of the data.
 * @return Operation result.
 * @retval true  Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    uint8_t *p_data_buf;
    uint16_t data_offset;
    T_APP_RESULT simp_ext_service_attr_write_cb(uint16_t conn_handle,  uint16_t cid,
                                                T_SERVER_ID service_id,
                                                uint16_t attrib_index, T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                                P_FUN_EXT_WRITE_IND_POST_PROC *p_write_post_proc)
    {
        ......
        server_ext_get_write_cmd_data_buffer(conn_handle, cid, &p_data_buf, &data_offset);
        return APP_RESULT_NOT_RELEASE;
    }
    void release(void)
    {
        if(p_data_buf != NULL)
        {
            gap_buffer_free(p_data_buf);
            p_data_buf = NULL;
        }
    }
 * \endcode
 */
bool server_ext_get_write_cmd_data_buffer(uint16_t conn_handle, uint16_t cid, uint8_t **pp_buffer,
                                          uint16_t *p_offset);

/**
 * @brief  Send the multiple variable notifications.
 *
 * If sending request operation is successful, the sending result will be returned by callback
 * registered by @ref server_ext_register_app_cb with service_id set to @ref SERVICE_PROFILE_GENERAL_ID,
 * and eventId of p_data (@ref T_SERVER_EXT_APP_CB_DATA) set to @ref PROFILE_EVT_SEND_DATA_COMPLETE.
 *
 * @param[in] conn_handle   Connection handle of the ACL link.
 * @param[in] cid           Local Channel Identifier assigned by Bluetooth Host. Shall use the channel
 *                          in the @ref GAP_CHANNEL_STATE_CONNECTED state.
 * @param[in] p_data        Pointer to data to be sent.
 * @param[in] data_len      Length of value to be sent, range: 0~(mtu_size - 1).
                            uint16_t mtu_size is acquired by @ref gap_chann_get_info (conn_handle, cid, p_info).
 * @return The result of sending request.
 * @retval true Sending request operation is successful.
 * @retval false Sending request operation is failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    bool simp_ble_service_ext_send_v3_v8_notify(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id, void *p_v3_value,
                                            uint16_t v3_length, void *p_v8_value, uint16_t v8_length)
    {
        bool ret;
        uint8_t *p_value;
        uint8_t *p_data;
        uint16_t length = v3_length + v8_length + 2 * 4;
        uint16_t handle;

        p_value = os_mem_zalloc(RAM_TYPE_DATA_ON, length);
        if (p_value == NULL)
        {
            return false;
        }
        p_data = p_value;
        handle = server_get_start_handle(service_id) + SIMPLE_BLE_SERVICE_CHAR_V3_NOTIFY_INDEX;
        LE_UINT16_TO_STREAM(p_data, handle);
        LE_UINT16_TO_STREAM(p_data, v3_length);
        memcpy(p_data, p_v3_value, v3_length);
        p_data += v3_length;

        handle = server_get_start_handle(service_id) + SIMPLE_BLE_SERVICE_CHAR_V8_NOTIFY_INDICATE_INDEX;
        LE_UINT16_TO_STREAM(p_data, handle);
        LE_UINT16_TO_STREAM(p_data, v8_length);
        memcpy(p_data, p_v8_value, v8_length);
        p_data += v8_length;

        ret = server_ext_send_multi_notify(conn_handle, cid, p_value, length);
        if (p_value)
        {
            os_mem_free(p_value);
        }
        return ret;
    }
 * \endcode
 */
bool server_ext_send_multi_notify(uint16_t conn_handle, uint16_t cid,
                                  uint8_t *p_data, uint16_t data_len);

/**
 * @brief  Get the CCCD information.
 * @param[in]  conn_handle  Connection handle of the ACL link.
 * @param[in]  service_id   Service ID @ref T_SERVER_ID.
 * @param[in]  attrib_index Attribute index of attribute to get CCCD.
 * @param[in,out] p_cccd    Pointer to location to get CCCD information.
 * @return Operation result.
 * @retval true  Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    bool simp_ble_service_ext_send_v3_notify(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id, void *p_value,
                                        uint16_t length)
    {
        uint16_t cccd_bits;
        APP_PRINT_INFO0("simp_ble_service_ext_send_v3_notify");
        if (server_ext_get_cccd_info(conn_handle, service_id, SIMPLE_BLE_SERVICE_CHAR_NOTIFY_CCCD_INDEX,
                                &cccd_bits))
        {
            APP_PRINT_INFO1("simp_ble_service_ext_send_v3_notify: cccd_bits 0x%x", cccd_bits);
        }
        return server_ext_send_data(conn_handle, cid, service_id, SIMPLE_BLE_SERVICE_CHAR_V3_NOTIFY_INDEX, p_value,
                                length,
                                GATT_PDU_TYPE_ANY);
    }
 * \endcode
*/
bool server_ext_get_cccd_info(uint16_t conn_handle, T_SERVER_ID service_id, uint16_t attrib_index,
                              uint16_t *p_cccd);
/** @} End of GATT_SERVER_EXT_Exported_Functions */

/** @} End of GATT_SERVER_EXT_API */

/** @} End of GATT_SERVER_API */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif /* PROFILE_SERVER_EXT_H */
