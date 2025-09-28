#ifndef _MATTER_BLE_SERVICE_H_
#define _MATTER_BLE_SERVICE_H_

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

#define MATTER_BLE_SERVICE_INDICATE_V3_ENABLE     1
#define MATTER_BLE_SERVICE_INDICATE_V3_DISABLE    2

typedef struct
{
    uint8_t indication_index;
} T_MATTER_BLE_UPSTREAM_MSG_DATA;

typedef struct
{
    uint8_t                        conn_id;
    T_SERVICE_CALLBACK_TYPE        msg_type;
    T_MATTER_BLE_UPSTREAM_MSG_DATA msg_data;
} T_MATTER_BLE_CBACK_DATA;

typedef struct
{
    uint8_t opcode; //!< ref:  @ref SIMP_Service_Write_Info
    T_WRITE_TYPE write_type;
    uint16_t len;
    uint8_t *p_value;
} TBTCONFIG_WRITE_MSG;

typedef union
{
    uint8_t read_value_index; //!< ref: @ref SIMP_Service_Read_Info
    TBTCONFIG_WRITE_MSG write;
    uint16_t read_offset;
} TBTCONFIG_MSG_DATA;

typedef struct
{
    uint8_t                 conn_id;
    T_SERVICE_CALLBACK_TYPE msg_type;
    TBTCONFIG_MSG_DATA msg_data;
} TBTCONFIG_CALLBACK_DATA;

T_SERVER_ID matter_ble_add_service(void *p_func);

bool matter_ble_indication_send(uint8_t conn_id, uint8_t *p_data, uint16_t data_len);

#ifdef __cplusplus
}
#endif

#endif /* _MATTER_BLE_SERVICE_H_ */