/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     profile_client_def.h
  * @brief    Header file for profile client structure.
  * @details  Common data struct definition.
  * @author   ethan_su
  * @date     2016-02-18
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef PROFILE_CLIENT_DEF_H
#define PROFILE_CLIENT_DEF_H

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/*============================================================================*
 *                        Header Files
 *============================================================================*/

/** @defgroup GATT_CLIENT_API GATT Client API
  * @brief GATT client API
  * @{
  */

/** @defgroup GATT_CLIENT_COMMON_API GATT Client Common API
  * @brief GATT client common API
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GATT_Client_Common_Exported_Macros GATT Client Exported Macros
  * @{
  */
#define CLIENT_PROFILE_GENERAL_ID   0xFF        //!< General Client ID used by application, when directly calls the APIs of profile client layer. Distinguish with other specific client module.

/** End of GATT_Client_Common_Exported_Macros
* @}
*/
/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GATT_Client_Common_Exported_Types GATT Client Common Exported Types
  * @{
  */

typedef uint8_t T_CLIENT_ID; //!< Client ID.

/** @brief  Discovery state during discovery procedure.*/
typedef enum
{
    DISC_STATE_IDLE,                    //!< Discovery state idle.
    DISC_STATE_SRV,                     //!< Discovery services.
    DISC_STATE_SRV_DONE,                //!< Discovery services done.
    DISC_STATE_RELATION,                //!< Discovery relation.
    DISC_STATE_RELATION_DONE,           //!< Discovery relation done.
    DISC_STATE_CHAR,                    //!< Discovery characteristics.
    DISC_STATE_CHAR_DONE,               //!< Discovery characteristics done.
    DISC_STATE_CHAR_UUID16_DONE,        //!< Discovery 16 bit UUID characteristics done.
    DISC_STATE_CHAR_UUID128_DONE,       //!< Discovery 128 bit UUID characteristics done.
    DISC_STATE_CHAR_DESCRIPTOR,         //!< Discovery characteristic descriptors.
    DISC_STATE_CHAR_DESCRIPTOR_DONE,    //!< Discovery characteristic descriptors done.
    DISC_STATE_FAILED                   //!< Discovery failed.
} T_DISCOVERY_STATE;

/** @brief  Discovery result type*/
typedef enum
{
    DISC_RESULT_ALL_SRV_UUID16,         //!< Discovery all 16 bit UUID services.
    DISC_RESULT_ALL_SRV_UUID128,        //!< Discovery all 128 bit UUID services.
    DISC_RESULT_SRV_DATA,               //!< Discovery service data.
    DISC_RESULT_CHAR_UUID16,            //!< Discovery 16 bit UUID characteristics.
    DISC_RESULT_CHAR_UUID128,           //!< Discovery 128 bit UUID characteristics.
    DISC_RESULT_CHAR_DESC_UUID16,       //!< Discovery 16 bit UUID characteristic descriptors.
    DISC_RESULT_CHAR_DESC_UUID128,      //!< Discovery 128 bit UUID characteristic descriptors.
    DISC_RESULT_RELATION_UUID16,        //!< Discovery 16 bit UUID relations.
    DISC_RESULT_RELATION_UUID128,       //!< Discovery 128 bit UUID relations.
    DISC_RESULT_BY_UUID16_CHAR,         //!< Discovery characteristics by 16 bit UUID.
    DISC_RESULT_BY_UUID128_CHAR,        //!< Discovery characteristics by 128 bit UUID.
} T_DISCOVERY_RESULT_TYPE;

/** @brief  GATT write type*/
typedef enum
{
    GATT_WRITE_TYPE_REQ = 0x01,           /**<  ATT Write Request.  */
    GATT_WRITE_TYPE_CMD = 0x02,           /**<  ATT Write Command.  */
    GATT_WRITE_TYPE_SIGNED_CMD = 0x04     /**<  ATT Signed Write Command.  */
} T_GATT_WRITE_TYPE;

typedef struct
{
    uint16_t    att_handle;         /**< The handle for the service declaration. */
    uint16_t    end_group_handle;   /**< The handle of the last attribute
    within the service definition. */
    uint16_t    uuid16;             /**< 16 bit UUID. */
} T_GATT_SERVICE_ELEM16;

typedef struct
{
    uint16_t    att_handle;         /**< The handle for the service declaration. */
    uint16_t    end_group_handle;
    /**< The handle of the last attribute within the service definition. */
    uint8_t     uuid128[16];        /**< 128 bit UUID. */
} T_GATT_SERVICE_ELEM128;


typedef struct
{
    uint16_t    att_handle;         /**< The handle for the service declaration. */
    uint16_t    end_group_handle;
    /**< The handle of the last attribute within the service definition. */
} T_GATT_SERVICE_BY_UUID_ELEM;

/** @brief  Characteristic declaration for 16 bit UUID.*/
typedef struct
{
    uint16_t    decl_handle;   /**<  Attribute handle. */
    uint16_t    properties;    /**<  Characteristic Properties, high octet is reserved. */
    uint16_t    value_handle;  /**<  Characteristic Value Handle. */
    uint16_t    uuid16;        /**<  16-bit Bluetooth UUID for Characteristic Value. */
} T_GATT_CHARACT_ELEM16;

/** @brief  Characteristic declaration for 128 bit UUID.*/
typedef struct
{
    uint16_t    decl_handle;    /**<  Attribute handle. */
    uint16_t    properties;     /**<  Characteristic Properties, high octet is reserved. */
    uint16_t    value_handle;   /**<  Characteristic Value Handle. */
    uint8_t     uuid128[16];    /**<  128-bit UUID for Characteristic Value. */
} T_GATT_CHARACT_ELEM128;

/** @brief  Characteristic descriptor for 16 bit UUID.*/
typedef struct
{
    uint16_t    handle;         /**< Attribute handle. */
    uint16_t    uuid16;         /**< 16 bit UUID. */
} T_GATT_CHARACT_DESC_ELEM16;

/** @brief  Characteristic descriptor for 128 bit UUID.*/
typedef struct
{
    uint16_t    handle;         /**< Attribute handle. */
    uint8_t     uuid128[16];    /**< 128 bit UUID. */
} T_GATT_CHARACT_DESC_ELEM128;

/** @brief  Relationship discovery for 16 bit UUID.*/
typedef struct
{
    uint16_t    decl_handle;
    uint16_t    att_handle;
    uint16_t    end_group_handle;
    uint16_t    uuid16;
} T_GATT_RELATION_ELEM16;

/** @brief  Relationship discovery for 128 bit UUID.*/
typedef struct
{
    uint16_t    decl_handle;
    uint16_t    att_handle;
    uint16_t    end_group_handle;
    uint8_t     uuid128[16];
} T_GATT_RELATION_ELEM128;

/** @brief  Discovery result data*/
typedef union
{
    T_GATT_SERVICE_ELEM16        *p_srv_uuid16_disc_data;
    T_GATT_SERVICE_ELEM128       *p_srv_uuid128_disc_data;
    T_GATT_SERVICE_BY_UUID_ELEM  *p_srv_disc_data;
    T_GATT_CHARACT_ELEM16        *p_char_uuid16_disc_data;
    T_GATT_CHARACT_ELEM128       *p_char_uuid128_disc_data;
    T_GATT_CHARACT_DESC_ELEM16   *p_char_desc_uuid16_disc_data;
    T_GATT_CHARACT_DESC_ELEM128  *p_char_desc_uuid128_disc_data;
    T_GATT_RELATION_ELEM16       *p_relation_uuid16_disc_data;
    T_GATT_RELATION_ELEM128      *p_relation_uuid128_disc_data;
} T_DISCOVERY_RESULT_DATA;

/** @defgroup General_cb_data General Client Callback Data
  * @{
  */
/** @brief  The callback data of CLIENT_APP_CB_TYPE_DISC_STATE.
  *
  * Discovery procedure related data to inform application.
  */
typedef struct
{
    T_DISCOVERY_STATE disc_state;
} T_DISC_STATE_CB_DATA;

/** @brief  The callback data of CLIENT_APP_CB_TYPE_DISC_RESULT.
  *
  * Discovery result data will be sent to Bluetooth Host through the callback.
  */
typedef struct
{
    T_DISCOVERY_RESULT_TYPE result_type;
    T_DISCOVERY_RESULT_DATA result_data;
} T_DISC_RESULT_CB_DATA;

typedef struct
{
    uint16_t handle;
    uint16_t read_len;
    uint16_t total_len;
    uint8_t *p_data;
} T_READ_MULTI_DATA;

typedef struct
{
    T_CLIENT_ID client_id;
    uint16_t cause;
    uint16_t num_handle;
    T_READ_MULTI_DATA *p_read_tbl;
} T_READ_MULTI_RESULT_CB_DATA;

/** @brief  The callback type of T_CLIENT_APP_CB_DATA.
  *
  * Message data type, when data sent to APP directly.
  */
typedef enum
{
    CLIENT_APP_CB_TYPE_DISC_STATE,
    CLIENT_APP_CB_TYPE_DISC_RESULT,
    CLIENT_APP_CB_TYPE_READ_MULTI_RESULT
} T_CLIENT_CB_TYPE;

/** @brief  The callback data of T_CLIENT_APP_CB_DATA.
  *
  * Client received data from server, when no specific client registered, will be sent to APP directly.
  */
typedef union
{
    T_DISC_STATE_CB_DATA   disc_state_data;
    T_DISC_RESULT_CB_DATA  disc_result_data;
    T_READ_MULTI_RESULT_CB_DATA  read_multi_result_data;
} T_CLIENT_CB_DATA;

/** @brief  The general client callback data struct.
  *
  * Callback data sent to application directly from client, include type and content.
  */
typedef struct
{
    T_CLIENT_CB_TYPE     cb_type;
    T_CLIENT_CB_DATA     cb_content;
} T_CLIENT_APP_CB_DATA;
/** End of General_cb_data
  * @}
  */
/** End of GATT_Client_Common_Exported_Types
* @}
*/
/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GATT_Client_Common_Exported_Functions GATT Client Common Exported Functions
  * @{
  */
/**
 * @brief Initialize parameters of GATT client.
 *
 * @param[in] client_num  Set the number of clients that need to register.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_le_profile_init(void)
    {
        client_init(client_num);
    }
  * \endcode
  */
void client_init(uint8_t client_num);

/**
 * @brief Configure the client interface.
 *
 * @param[in] use_ext Whether to use the extension API. Default value is false.
 *                    - true: Use the API in @ref GATT_CLIENT_EXT_API.
 *                    - false: Use the API in @ref GATT_CLIENT_LEGACY_API.
 * @return Operation result.
 * @retval true    Operation success.
 * @retval false   Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool ret = client_cfg_use_ext_api(true);
    }
 * \endcode
 */
bool client_cfg_use_ext_api(bool use_ext);

/** End of GATT_Client_Common_Exported_Functions
* @}
*/
/** End of GATT_CLIENT_COMMON_API
* @}
*/
/** End of GATT_CLIENT_API
* @}
*/


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif /* PROFILE_CLIENT_DEF_H */

