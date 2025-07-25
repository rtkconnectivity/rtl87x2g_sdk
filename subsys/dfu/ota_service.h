/**
*****************************************************************************************
*     Copyright (c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      ota_service.h
   * @brief     Header file for using OTA service
   * @author    Grace
   * @date      2023-12-06
   * @version   v1.1
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2023 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                      Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _OTA_SERVICE_H_
#define _OTA_SERVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#if F_APP_GATT_SERVER_EXT_API_SUPPORT
#include <profile_server_ext.h>
#else
#include <profile_server.h>
#endif

/** @defgroup  OTA_SERVICE OTA Service
  * @brief LE Service to implement OTA feature
  * @{
  */

/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup OTA_SERVICE_Exported_Macros OTA Service Exported Macros
  * @brief
  * @{
  */

/**
 * \defgroup    OTA_GATT_UUID_Characteristic   OTA GATT UUID Characteristic
 * \{
 * \ingroup     OTA_SERVICE_Exported_Macros
 */
#define GATT_UUID_CHAR_OTA                          0xFFD1
#define GATT_UUID_CHAR_MAC                          0xFFD2
#define GATT_UUID_CHAR_PATCH_VERSION                0xFFD3
#define GATT_UUID_CHAR_APP_VERSION                  0xFFD4
#define GATT_UUID_CHAR_DEVICE_INFO                  0xFFF1
#define GATT_UUID_CHAR_IMAGE_VERSION_FIRST          0xFFE0
#define GATT_UUID_CHAR_IMAGE_VERSION_SECOND         0xFFE1
#define GATT_UUID_CHAR_PROTOCOL_INFO                0xFFF3
#define GATT_UUID_CHAR_SECTION_SIZE_FIRST           0xFFF4
#define GATT_UUID_CHAR_SECTION_SIZE_SECOND          0xFFF5
/** End of OTA_GATT_UUID_Characteristic
  * \}
  */

/**
 * \defgroup    OTA_BLE_Service_Characteristic   OTA BLE Service Characteristic
 * \{
 * \ingroup     OTA_SERVICE_Exported_Macros
 */
/** @brief  Index definitions for accessing each BLE characteristic. */
#define BLE_SERVICE_CHAR_OTA_INDEX                  0x02
#define BLE_SERVICE_CHAR_MAC_ADDRESS_INDEX          0x04
#define BLE_SERVICE_CHAR_PATCH_INDEX                0x06
#define BLE_SERVICE_CHAR_APP_VERSION_INDEX          0x08
#define BLE_SERVICE_CHAR_DEVICE_INFO_INDEX          0x0a
#define BLE_SERVICE_CHAR_IMAGE_VERSION_FIRST_INDEX  0x0c
#define BLE_SERVICE_CHAR_IMAGE_VERSION_SECOND_INDEX 0x0e
#define BLE_SERVICE_CHAR_PROTOCOL_INFO_INDEX        0x10
#define BLE_SERVICE_CHAR_SECTION_SIZE_FIRST_INDEX   0x12
#define BLE_SERVICE_CHAR_SECTION_SIZE_SECOND_INDEX  0x14
/** End of OTA_BLE_Service_Characteristic
  * \}
  */

/** @brief  OTA write characteristic value. */
#define OTA_WRITE_CHAR_VAL  0x01

/** @brief  OTA value for entering a state. */
#define OTA_VALUE_ENTER     0x01

/** End of OTA_SERVICE_Exported_Macros
  * @}
  */

/*============================================================================*
 *                              Types
 *============================================================================*/
/** @defgroup OTA_SERVICE_Exported_Types OTA Service Exported Types
  * @brief
  * @{
  */

/** @brief  OTA write message. */
typedef struct
{
    uint8_t opcode;
    uint8_t value;
} T_OTA_WRITE_MSG;

/** @brief  OTA upstream message data. */
typedef union
{
    uint8_t notification_indification_index;
    uint8_t read_value_index;
    T_OTA_WRITE_MSG write;
} T_OTA_UPSTREAM_MSG_DATA;


/** @brief  OTA service callback data to inform application. */
typedef struct
{
    T_SERVICE_CALLBACK_TYPE     msg_type;
#if F_APP_GATT_SERVER_EXT_API_SUPPORT
    uint16_t                    conn_handle;
    uint16_t                    cid;
#endif
    uint8_t                     conn_id;
    T_OTA_UPSTREAM_MSG_DATA     msg_data;
} T_OTA_CALLBACK_DATA;

/** End of OTA_SERVICE_Exported_Types
  * @}
  */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup OTA_SERVICE_Exported_Functions OTA Service Exported Functions
  * @brief
  * @{
  */

/**
 * @brief    Add the ota service to the application.
 * @param[in]     p_func   Pointer to the application callback function invoked by the profile.
 * @return   A value of type \ref T_SERVER_ID, representing the service ID automatically generated by the profile layer.
 */
T_SERVER_ID ota_add_service(void *p_func);

/**
 * @brief    Send a notification to the connected peer device.
 * @param[in]     conn_id   Connection ID used to identify the specific connection.
 * @param[in]     p_data    Pointer to the data to be sent as a notification.
 * @param[in]     data_len  Length of the data to be sent.
 */
void ota_service_send_notification(uint8_t conn_id, uint8_t *p_data, uint16_t data_len);

/** End of OTA_SERVICE_Exported_Functions
  * @}
  */

/** End of OTA_SERVICE
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif


