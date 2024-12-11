#include <string.h>
#include <trace.h>
#include <gap.h>
#include <profile_server.h>
#include "matter_ble_service.h"

#define BTCONFIG_READ_V1                                1
#define BTCONFIG_WRITE_V1                               1
#define BT_MATTER_ADAPTER_READ_V1_MAX_LEN               300
#define MATTER_BLE_SERVICE_CHAR_V1_READ_WRITE_INDEX      0x02
#define MATTER_BLE_SERVICE_CHAR_INDICATE_CCCD_INDEX      0x05
#define MATTER_BLE_SERVICE_C3_INDEX                      0x07

#define UUID_RX     0x11, 0x9D, 0x9F, 0x42, 0x9C, 0x4F, 0x9F, 0x95, 0x59, 0x45, 0x3D, 0x26, 0xF5, 0x2E, 0xEE, 0x18
#define UUID_TX     0x12, 0x9D, 0x9F, 0x42, 0x9C, 0x4F, 0x9F, 0x95, 0x59, 0x45, 0x3D, 0x26, 0xF5, 0x2E, 0xEE, 0x18
#define UUID_C3     0x04, 0x8F, 0x21, 0x83, 0x8A, 0x74, 0x7D, 0xB8, 0xF2, 0x45, 0x72, 0x87, 0x38, 0x02, 0x63, 0x64

T_SERVER_ID matter_ble_service_id;
/**<  Value of bt config characteristic. */
static uint8_t bt_matter_adapter_char_read_value[BT_MATTER_ADAPTER_READ_V1_MAX_LEN];
static uint16_t bt_matter_adapter_char_read_len = 1;

/**<  Function pointer used to send event to application from matter ble profile. Initiated in matter_ble_add_service. */
static P_FUN_SERVER_GENERAL_CB pfn_matter_ble_cb = NULL;

/**< @brief  profile/service definition.  */
T_ATTRIB_APPL matter_ble_service_tbl[] =
{
    /* <<Primary Service>>, .. */
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE), /* flags */
        { /* type_value */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(0xFFF6), /* service UUID */
            HI_WORD(0xFFF6)
        },
        UUID_16BIT_SIZE, /* bValueLen */
        NULL, /* p_value_context */
        GATT_PERM_READ /* permissions */
    },
    /* <<Characteristic>> Data RX */
    {
        ATTRIB_FLAG_VALUE_INCL, /* flags */
        { /* type_value */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_WRITE_NO_RSP) /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1, /* bValueLen */
        NULL,
        GATT_PERM_READ /* permissions */
    },
    {
        ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT, /* flags */
        { /* type_value */
            UUID_RX
        },
        0, /* bValueLen */
        NULL,
        GATT_PERM_WRITE /* permissions */
    },
    /* <<Characteristic>> Data TX */
    {
        ATTRIB_FLAG_VALUE_INCL, /* flags */
        { /* type_value */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_READ | GATT_CHAR_PROP_INDICATE) /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1, /* bValueLen */
        NULL,
        GATT_PERM_READ /* permissions */
    },
    {
        ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT, /* flags */
        { /* type_value */
            UUID_TX
        },
        0, /* bValueLen */
        NULL,
        GATT_PERM_READ //GATT_PERM_NONE // permissions
    },
    /* client characteristic configuration */
    {
        ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL, /* flags */
        { /* type_value */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value: */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2, /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE) /* permissions */
    },

    /* <<Characteristic>> C3 Data TX */
    {
        ATTRIB_FLAG_VALUE_INCL, /* flags */
        { /* type_value */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_READ) /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1, /* bValueLen */
        NULL,
        GATT_PERM_READ /* permissions */
    },
    {
        ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT, /* flags */
        { /* type_value */
            UUID_C3
        },
        0, /* bValueLen */
        NULL,
        GATT_PERM_READ //GATT_PERM_NONE // permissions
    },
};

T_APP_RESULT matter_ble_service_attr_read_cb(uint8_t conn_id,
                                             T_SERVER_ID service_id,
                                             uint16_t attr_index,
                                             uint16_t offset,
                                             uint16_t *p_length,
                                             uint8_t **pp_value)
{
    T_APP_RESULT cause  = APP_RESULT_SUCCESS;

    switch (attr_index)
    {
    case MATTER_BLE_SERVICE_CHAR_V1_READ_WRITE_INDEX:
        {
            TBTCONFIG_CALLBACK_DATA callback_data;
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = BTCONFIG_READ_V1;
            callback_data.msg_data.read_offset = offset;
            callback_data.conn_id = conn_id;
            if (pfn_matter_ble_cb)
            {
                pfn_matter_ble_cb(service_id, (void *)&callback_data);
            }
            *pp_value = bt_matter_adapter_char_read_value;
            *p_length = bt_matter_adapter_char_read_len;
        }
        break;

    case MATTER_BLE_SERVICE_C3_INDEX:
        {
            TBTCONFIG_CALLBACK_DATA callback_data;
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = BTCONFIG_READ_V1;
            callback_data.msg_data.read_offset = offset;
            callback_data.conn_id = conn_id;
            if (pfn_matter_ble_cb)
            {
                pfn_matter_ble_cb(service_id, (void *)&callback_data);
            }
            *pp_value = callback_data.msg_data.write.p_value;
            *p_length = callback_data.msg_data.write.len;
        }
        break;

    default:
        cause = APP_RESULT_ATTR_NOT_FOUND;
        break;
    }

    return cause;
}

T_APP_RESULT matter_ble_service_attr_write_cb(uint8_t conn_id,
                                              T_SERVER_ID service_id,
                                              uint16_t attr_index,
                                              T_WRITE_TYPE write_type,
                                              uint16_t length,
                                              uint8_t *p_value,
                                              P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    TBTCONFIG_CALLBACK_DATA callback_data;
    T_APP_RESULT  cause = APP_RESULT_SUCCESS;

    APP_PRINT_INFO1("matter_ble_service_attr_write_cb write_type = 0x%x", write_type);
    *p_write_ind_post_proc = NULL;

    if (MATTER_BLE_SERVICE_CHAR_V1_READ_WRITE_INDEX == attr_index)
    {
        /* Make sure written value size is valid. */
        if (p_value == NULL)
        {
            cause  = APP_RESULT_INVALID_VALUE_SIZE;
        }
        else
        {
            /* Notify Application. */
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
            callback_data.conn_id  = conn_id;
            callback_data.msg_data.write.opcode = BTCONFIG_WRITE_V1;
            callback_data.msg_data.write.write_type = write_type;
            callback_data.msg_data.write.len = length;
            callback_data.msg_data.write.p_value = p_value;

            if (pfn_matter_ble_cb)
            {
                pfn_matter_ble_cb(service_id, (void *)&callback_data);
            }
        }
    }
    else
    {
        APP_PRINT_ERROR2("matter_ble_service_attr_write_cb Error: attrib_index 0x%x, length %d",
                         attr_index, length);
        cause = APP_RESULT_ATTR_NOT_FOUND;
    }

    return cause;
}

void matter_ble_service_cccd_update_cb(uint8_t conn_id,
                                       T_SERVER_ID service_id,
                                       uint16_t index,
                                       uint16_t cccbits)
{
    T_MATTER_BLE_CBACK_DATA cback_data;
    bool is_handled = false;

    cback_data.conn_id = conn_id;
    cback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;

    switch (index)
    {
    case MATTER_BLE_SERVICE_CHAR_INDICATE_CCCD_INDEX:
        {
            if (cccbits & GATT_CLIENT_CHAR_CONFIG_INDICATE)
            {
                cback_data.msg_data.indication_index = MATTER_BLE_SERVICE_INDICATE_V3_ENABLE;
            }
            else
            {
                cback_data.msg_data.indication_index = MATTER_BLE_SERVICE_INDICATE_V3_DISABLE;
            }
            is_handled =  true;
        }
        break;

    default:
        break;
    }
    /* Notify Application. */
    if (pfn_matter_ble_cb && (is_handled == true))
    {
        pfn_matter_ble_cb(service_id, (void *)&cback_data);
    }
}

const T_FUN_GATT_SERVICE_CBS matter_ble_service_cbs =
{
    matter_ble_service_attr_read_cb,
    matter_ble_service_attr_write_cb,
    matter_ble_service_cccd_update_cb
};

T_SERVER_ID matter_ble_add_service(void *p_func)
{
    if (false == server_add_service(&matter_ble_service_id,
                                    (uint8_t *)matter_ble_service_tbl,
                                    sizeof(matter_ble_service_tbl),
                                    matter_ble_service_cbs))
    {
        APP_PRINT_ERROR0("matter_ble_add_service: fail");
        matter_ble_service_id = 0xff;
        return matter_ble_service_id;
    }

    pfn_matter_ble_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return matter_ble_service_id;
}

bool matter_ble_indication_send(uint8_t conn_id, uint8_t *p_data, uint16_t data_len)
{
    return server_send_data(conn_id,
                            matter_ble_service_id,
                            MATTER_BLE_SERVICE_CHAR_INDICATE_CCCD_INDEX - 1,
                            p_data,
                            data_len,
                            GATT_PDU_TYPE_INDICATION);
}
