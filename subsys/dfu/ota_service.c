/**
*****************************************************************************************
*     Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      ota_service.c
   * @brief     Source file for using OTA service
   * @author    Grace
   * @date      2023-12-6
   * @version   v1.1
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2023 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/

#include <string.h>
#include "bt_types.h"
#include "ble_dfu_transport.h"
#include "dfu_common.h"
#include "gatt.h"
#include "ota_service.h"
#include "trace.h"

/** @defgroup  OTA_SERVICE OTA Service
  * @brief LE Service to implement OTA feature
  * @{
  */

/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup OTA_SERVICE_Exported_Macros OTA service Exported Macros
  * @brief
  * @{
  */

/* Indicate whether support ota image transfer during normal working mode */
#define OTA_NORMAL_MODE 1

/** End of OTA_SERVICE_Exported_Macros
  * @}
  */

/*============================================================================*
 *                              Constants
 *============================================================================*/
/** @defgroup OTA_SERVICE_Exported_Constants OTA service Exported Constants
  * @{
  */

/** @brief  OTA service UUID */
static const uint8_t GATT_UUID_OTA_SERVICE[16] = { 0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0xFF, 0xD0, 0x00, 0x00};

/** @brief  OTA profile/service definition
*   @note   Here is an example of OTA service table including Write
*/
static const T_ATTRIB_APPL ota_service_table[] =
{
    /*--------------------------OTA Service ---------------------------*/
    /* <<Primary Service>>, .. 0 */
    {
        (ATTRIB_FLAG_VOID | ATTRIB_FLAG_LE),        /* flags */
        {
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),     /* type_value */
        },
        UUID_128BIT_SIZE,                           /* bValueLen */
        (void *)GATT_UUID_OTA_SERVICE,              /* p_value_context */
        GATT_PERM_READ                              /* permissions */
    },

    /* <<Characteristic1>>, .. 1 */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* flags */
        {   /* type_value */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE_NO_RSP,            /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,     /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* permissions */
    },
    /*  OTA characteristic value 2 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* flags */
        {   /* type_value */
            LO_WORD(GATT_UUID_CHAR_OTA),
            HI_WORD(GATT_UUID_CHAR_OTA),
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ | GATT_PERM_WRITE            /* permissions */
    },

    /* <<Characteristic2>>, .. 3, MAC Address */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* flags */
        {   /* type_value */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,     /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* permissions */
    },
    /*  OTA characteristic value 4 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* flags */
        {   /* type_value */
            LO_WORD(GATT_UUID_CHAR_MAC),
            HI_WORD(GATT_UUID_CHAR_MAC),
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ                              /* permissions */
    },

    /* <<Characteristic3>>, .. 5, Patch version */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* flags */
        {   /* type_value */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,     /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* permissions */
    },
    /*  OTA characteristic value 6 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* flags */
        {   /* type_value */
            LO_WORD(GATT_UUID_CHAR_PATCH_VERSION),
            HI_WORD(GATT_UUID_CHAR_PATCH_VERSION),
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ                              /* permissions */
    },

    /* <<Characteristic4>>, .. 7 App version */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* flags */
        {   /* type_value */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,     /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* permissions */
    },
    /*  OTA characteristic value 8 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* flags */
        {   /* type_value */
            LO_WORD(GATT_UUID_CHAR_APP_VERSION),
            HI_WORD(GATT_UUID_CHAR_APP_VERSION),
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ                              /* permissions */
    },

    /* <<Characteristic5>>, .. 9, Extended Device information */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* flags */
        {   /* type_value */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* permissions */
    },

    /*  OTA characteristic value 10 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* flags */
        {   /* type_value */
            LO_WORD(GATT_UUID_CHAR_DEVICE_INFO),
            HI_WORD(GATT_UUID_CHAR_DEVICE_INFO),
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ                              /* permissions */
    },

    /* <<Characteristic6>>, .. 11*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /*  OTA characteristic value 12*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_IMAGE_VERSION_FIRST),
            HI_WORD(GATT_UUID_CHAR_IMAGE_VERSION_FIRST),
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },

    /* <<Characteristic7>>, .. 13*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /*  OTA characteristic value 14*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_IMAGE_VERSION_SECOND),
            HI_WORD(GATT_UUID_CHAR_IMAGE_VERSION_SECOND),
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },

    /* <<Characteristic8>>, .. 15*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /*  OTA characteristic value 16*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_PROTOCOL_INFO),
            HI_WORD(GATT_UUID_CHAR_PROTOCOL_INFO),
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },

    /* <<Characteristic9>>, .. 17*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /*  OTA characteristic value 18*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_SECTION_SIZE_FIRST),
            HI_WORD(GATT_UUID_CHAR_SECTION_SIZE_FIRST),
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },

    /* <<Characteristic10>>, .. 19*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /*  OTA characteristic value 20*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_SECTION_SIZE_SECOND),
            HI_WORD(GATT_UUID_CHAR_SECTION_SIZE_SECOND),
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },
};

/** End of OTA_SERVICE_Exported_Constants
    * @}
    */

/*============================================================================*
 *                              Variables
 *============================================================================*/
/** @defgroup OTA_SERVICE_Exported_Variables OTA service Exported Variables
    * @brief
    * @{
    */

/** @brief  Service ID only used in this file */
static T_SERVER_ID ota_srv_id_local;

/** @brief  Function pointer used to send event to application from OTA service
*   @note   It is initiated in ota_add_service()
*/
#if F_APP_GATT_SERVER_EXT_API_SUPPORT
static P_FUN_EXT_SERVER_GENERAL_CB p_ota_extended_cb = NULL;
#else
static P_FUN_SERVER_GENERAL_CB p_ota_extended_cb = NULL;
#endif

/** @brief  Array used to temporarily store BD Addr */
static uint8_t mac_addr[12];

static uint8_t img_ver[IMG_INFO_LEN];
static uint8_t section_size[SECTION_SIZE_LEN];
static DEVICE_INFO device_info;

static uint16_t protocol_info = BLE_PROTOCOL_INFO;

/** End of OTA_SERVICE_Exported_Variables
  * @}
  */
/** End of OTA_SERVICE
  * @}
  */

/*============================================================================*
 *                              Private Functions
 *============================================================================*/
/**
    * @brief    Write characteristic data from service
    * @param    conn_id     ID to identify the connection
    * @param    service_id   Service ID to be written
    * @param    attr_index  Attribute index of characteristic
    * @param    write_type  Write type of data to be written
    * @param    length      Length of value to be written
    * @param    p_value     Value to be written
    * @param    p_write_ind_post_proc   Write indicate post procedure
    * @return   T_APP_RESULT
    * @retval   Profile procedure result
    */
void ota_service_write_post_callback(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                     uint16_t length, uint8_t *p_value)
{
    APP_PRINT_INFO4("ota_service_write_post_callback: conn_id %d, service_id %d, attrib_index 0x%x, length %d",
                    conn_id, service_id, attrib_index, length);
}

static T_APP_RESULT ota_service_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id,
                                              uint16_t attr_index, T_WRITE_TYPE write_type, uint16_t length,
                                              uint8_t *p_value, P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_OTA_CALLBACK_DATA callback_data;
    T_APP_RESULT  cause = APP_RESULT_SUCCESS;

    APP_PRINT_INFO2("ota_service_attr_write_cb: attr_index 0x%02x, length %d", attr_index, length);

//    app_reg_le_link_disc_cb(conn_id, ble_ota_service_le_disconnect_cb);

    if (BLE_SERVICE_CHAR_OTA_INDEX == attr_index)
    {
        /* Make sure written value size is valid. */
        if ((length != sizeof(uint8_t)) || (p_value == NULL))
        {
            cause  = APP_RESULT_INVALID_VALUE_SIZE;
        }
        else
        {
            /* Notify Application. */
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
            callback_data.msg_data.write.opcode = OTA_WRITE_CHAR_VAL;
            callback_data.conn_id = conn_id;
            callback_data.msg_data.write.value = p_value[0];

            if (p_ota_extended_cb)
            {
                p_ota_extended_cb(service_id, (void *)&callback_data);
            }
        }
    }
    else
    {
        APP_PRINT_ERROR0("ota_service_attr_write_cb: unknown attr_index");
        cause = APP_RESULT_ATTR_NOT_FOUND;
    }
    return cause;

}

/**
    * @brief    Read characteristic data from service
    * @param    conn_id     ID to identify the connection
    * @param    service_id  ServiceID of characteristic data
    * @param    attr_index  Attribute index of getting characteristic data
    * @param    offset      Used for Blob Read
    * @param    p_length    Length of getting characteristic data
    * @param    pp_value    Data got from service
    * @return   T_APP_RESULT
    * @retval   Profile procedure result
    */
static T_APP_RESULT ota_service_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id,
                                             uint16_t attr_index,
                                             uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT  cause = APP_RESULT_SUCCESS;

    APP_PRINT_INFO1("ota_service_attr_read_cb: attr_index 0x%02x", attr_index);

    switch (attr_index)
    {
    default:
        APP_PRINT_ERROR0("ota_service_attr_read_cb: unknown attr_index");
        cause  = APP_RESULT_ATTR_NOT_FOUND;
        break;
    case BLE_SERVICE_CHAR_MAC_ADDRESS_INDEX:
        {
            uint8_t addr[6];
            gap_get_param(GAP_PARAM_BD_ADDR, mac_addr);

            for (int i = 0; i < 6; i++)
            {
                addr[i] = mac_addr[5 - i];
            }
            memcpy(mac_addr, addr, 6);
            memcpy((uint8_t *)&mac_addr[6], addr, 6);

            *pp_value  = (uint8_t *)mac_addr;
            *p_length = sizeof(mac_addr);
        }
        break;
    case BLE_SERVICE_CHAR_DEVICE_INFO_INDEX:
        {
            ble_dfu_get_device_info(conn_id, &device_info);
            *pp_value  = (uint8_t *)&device_info;
            *p_length = sizeof(device_info);
        }
        break;
    case BLE_SERVICE_CHAR_IMAGE_VERSION_FIRST_INDEX:
        {
            img_ver[0] = ACTIVE_BANK;
            ble_dfu_get_img_version(&img_ver[1], img_ver[0]);
            *pp_value  = img_ver;
            *p_length = img_ver[1] * 6 + 2;
            DFU_PRINT_INFO1("ota_service_attr_read_cb: version length=%d", *p_length);
        }
        break;
    case BLE_SERVICE_CHAR_IMAGE_VERSION_SECOND_INDEX:
        {
            *pp_value  = NULL;
            *p_length = 0;
        }
        break;
    case BLE_SERVICE_CHAR_PROTOCOL_INFO_INDEX:
        {
            *pp_value  = (uint8_t *)&protocol_info;
            *p_length = sizeof(protocol_info);
        }
        break;
    case BLE_SERVICE_CHAR_SECTION_SIZE_FIRST_INDEX:
        {
            ble_dfu_get_section_size(section_size);
            *pp_value  = section_size;
            *p_length = section_size[0] * 6 + 1;
        }
        break;
    case BLE_SERVICE_CHAR_SECTION_SIZE_SECOND_INDEX:
        {
            *pp_value  = NULL;
            *p_length = 0;
        }
        break;
        /* TODO: add other version later */
    }

    APP_PRINT_INFO1("ota_service_attr_read_cb: %b", TRACE_BINARY(*p_length, *pp_value));

    return (cause);
}


/** @brief  OTA BLE Service Callbacks */
#if F_APP_GATT_SERVER_EXT_API_SUPPORT
static const T_FUN_GATT_EXT_SERVICE_CBS  ota_service_cbs =
#else
static const T_FUN_GATT_SERVICE_CBS ota_service_cbs =
#endif
{
    ota_service_attr_read_cb,   /**< Read callback function pointer */
    ota_service_attr_write_cb,  /**< Write callback function pointer */
    NULL                        /**< CCCD update callback function pointer */
};

/**
 * @brief    Add the ota service to the application.
 * @param    p_func   Pointer to the application callback function invoked by the profile.
 * @return   A value of type \ref T_SERVER_ID, representing the service ID automatically generated by the profile layer.
 */
T_SERVER_ID ota_add_service(void *p_func)
{
    T_SERVER_ID service_id;
#if F_APP_GATT_SERVER_EXT_API_SUPPORT
    if (false == server_ext_add_service(&service_id,
                                        (uint8_t *)gatt_extended_service_table,
                                        sizeof(gatt_extended_service_table),
                                        &ota_service_cbs))
    {
        APP_PRINT_ERROR1("ota_add_service: service_id %d", service_id);
        service_id = 0xff;
        return service_id;
    }
    p_ota_extended_cb = (P_FUN_EXT_SERVER_GENERAL_CB)p_func;
    ota_srv_id_local = service_id;
    return service_id;
#else
    if (false == server_add_service(&service_id,
                                    (uint8_t *)ota_service_table,
                                    sizeof(ota_service_table),
                                    ota_service_cbs))
    {
        APP_PRINT_ERROR1("ota_add_service fail: service_id %d", service_id);
        service_id = 0xff;
        return service_id;
    }
    p_ota_extended_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    ota_srv_id_local = service_id;
    APP_PRINT_ERROR1("ota_add_service success: service_id %d", ota_srv_id_local);
    return service_id;
#endif
}

