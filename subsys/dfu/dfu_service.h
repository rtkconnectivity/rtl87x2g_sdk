/**
*****************************************************************************************
*     Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      dfu_service.h
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
#ifndef _DFU_SERVICE_H_
#define _DFU_SERVICE_H_

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
#include "app_msg.h"

/** @defgroup  DFU_SERVICE DFU Service
  * @brief DFU Service to implement DFU feature
  * @{
  */
/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup DFU_SERVICE_Exported_Macros DFU Service Exported Macros
  * @brief
  * @{
  */

/** @brief  UUID for the DFU Service in a 128-bit format. */
#define BLE_GATT_UUID128_DFU_SERVICE       0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0x87, 0x62, 0x00, 0x00

/** @brief  UUID for the DFU Packet characteristic in a 128-bit format. */
#define BLE_GATT_UUID128_DFU_PACKET        0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0x87, 0x63, 0x00, 0x00

/** @brief  UUID for the DFU Control Point characteristic in a 128-bit format. */
#define BLE_GATT_UUID128_DFU_CONTROL_POINT 0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0x87, 0x64, 0x00, 0x00

/** @brief  Index of the DFU Packet characteristic. */
#define INDEX_DFU_CHAR_DFU_PACKET_INDEX           0x2

/** @brief  Index of the DFU Control Point characteristic. */
#define INDEX_DFU_CHAR_DFU_CONTROL_POINT_INDEX    0x4

/** @brief  Index of the Client Characteristic Configuration Descriptor (CCCD) for DFU Control Point. */
#define INDEX_DFU_CHAR_DFU_CP_CCCD_INDEX          0x5

/** @brief  Value to enable notifications for the DFU Control Point. */
#define DFU_CP_NOTIFY_ENABLE    0x1

/** @brief  Value to disable notifications for the DFU Control Point. */
#define DFU_CP_NOTIFY_DISABLE   0x2

/** End of DFU_SERVICE_Exported_Macros
  * @}
  */

/*============================================================================*
 *                              Types
 *============================================================================*/
/** @defgroup DFU_SERVICE_Exported_Types DFU Service Exported Types
  * @brief
  * @{
  */
/**  @brief DFU failure reason. */
typedef enum
{
    DFU_FAIL_UPDATE_FLASH,
    DFU_FAIL_SYSTEM_RESET_CMD,
    DFU_FAIL_EXCEED_MAX_BUFFER_SIZE,
    DFU_FAIL_EXCEED_IMG_TOTAL_LEN,
} T_DFU_FAIL_REASON;

/** @brief  DFU write operation opcode. */
typedef enum
{
    DFU_WRITE_ATTR_ENTER,
    DFU_WRITE_ATTR_EXIT,
    DFU_WRITE_START,
    DFU_WRITE_DOING,
    DFU_WRITE_END,
    DFU_WRITE_FAIL,
} T_DFU_WRITE_OPCODE;

/** @brief  DFU write message. */
typedef struct
{
    uint8_t opcode;
    uint8_t write_attrib_index;
    uint16_t length;
    uint8_t *p_value;
} T_DFU_WRITE_MSG;

/** @brief  DFU upstream message data. */
typedef union
{
    uint8_t notification_indification_index;
    T_DFU_WRITE_MSG write;
} T_DFU_UPSTREAM_MSG_DATA;

/** @brief  DFU service data to inform the application. */
typedef struct
{
    T_SERVICE_CALLBACK_TYPE  msg_type;
    uint8_t                  conn_id;
#if F_APP_GATT_SERVER_EXT_API_SUPPORT
    uint16_t                    conn_handle;
    uint16_t                    cid;
#endif
    T_DFU_UPSTREAM_MSG_DATA  msg_data;
} T_DFU_CALLBACK_DATA;

/** End of DFU_SERVICE_Exported_Types
  * @}
  */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup DFU_SERVICE_Exported_Functions DFU Service Exported Functions
  * @brief
  * @{
  */

/**
 * @brief    Add the dfu service to the application.
 * @param[in]    p_func   Pointer to the application callback function invoked by the profile.
 * @return   A value of type \ref T_SERVER_ID, representing the service ID automatically generated by the profile layer.
 */
T_SERVER_ID dfu_add_service(void *p_func);

/**
 * @brief    Send a notification to the connected peer device.
 * @param[in]     conn_id   Connection ID used to identify the specific connection.
 * @param[in]     p_data    Pointer to the data to be sent as a notification.
 * @param[in]     data_len  Length of the data to be sent.
 */
void dfu_service_send_notification(uint8_t conn_id, uint8_t *p_data, uint16_t data_len);

/** End of DFU_SERVICE_Exported_Functions
  * @}
  */

/** End of DFU_SERVICE
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
