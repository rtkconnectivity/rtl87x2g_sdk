/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_storage_le.h
* @brief     Key storage function.
* @details
* @author    Jane
* @date      2016-02-18
* @version   v1.0
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef     FLASH_KEY_MGR_LE_H
#define     FLASH_KEY_MGR_LE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <gap.h>
#include <gap_le_types.h>

/** @defgroup GAP_LE_STORAGE GAP LE Storage
  * @brief GAP LE Storage
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_STORAGE_Exported_Macros GAP LE Storage Exported Macros
  * @{
  */

/** @defgroup GAP_LE_STORAGE_BITS LE Key Storage Bits
* @{
 */
#define LE_KEY_STORE_REMOTE_BD_BIT   0x01   /**< Save remote device address to flash.
                                                 The data structure is @ref T_LE_REMOTE_BD. */
#define LE_KEY_STORE_LOCAL_LTK_BIT   0x02   /**< Save local device Long Term Key to flash.
                                                 The data structure is @ref T_LE_LTK. */
#define LE_KEY_STORE_REMOTE_LTK_BIT  0x04   /**< Save remote device Long Term Key to flash.
                                                 The data structure is @ref T_LE_LTK. */
#define LE_KEY_STORE_REMOTE_IRK_BIT  0x08   /**< Save remote device Identity Resolving Key to flash. */
#define LE_KEY_STORE_LOCAL_CSRK_BIT  0x10   /**< Save local device Connection Signature Resolving Key to flash. */
#define LE_KEY_STORE_REMOTE_CSRK_BIT 0x20   /**< Save remote device Connection Signature Resolving Key to flash. */
#define LE_KEY_STORE_CCCD_DATA_BIT   0x40   /**< Save local device CCCD data to flash.
                                                 The data structure is @ref T_CCCD_DATA. */
#define LE_KEY_STORE_LOCAL_IRK_BIT   0x80   /**< Save local device Identity Resolving Key to flash.
                                                 The data structure is @ref T_LOCAL_IRK. */
#define LE_KEY_STORE_LOCAL_BD_BIT    0x8000 /**< Save local device address to flash. */
/**
  * @}
  */

/** End of GAP_LE_STORAGE_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_STORAGE_Exported_Types GAP LE Storage Exported Types
  * @{
  */

/** @brief Local Device Name */
typedef struct
{
    uint8_t  local_name[40];
} T_LOCAL_NAME;

/** @brief Local Device Appearance */
typedef struct
{
    uint16_t local_appearance;
    uint8_t  padding[2];
} T_LOCAL_APPEARANCE;

/** @brief Local IRK */
typedef struct
{
    uint8_t local_irk[16];
} T_LOCAL_IRK;

/** @brief Remote Bluetooth device address info */
typedef struct
{
    uint8_t  addr[6];
    uint8_t  remote_bd_type;
    uint8_t  bond_flags;
} T_LE_REMOTE_BD;

/** @brief LE privacy information */
typedef struct
{
    bool is_discov;
    bool central_addr_resolv;
    bool resolv_addr_only;
} T_LE_PRIVACY_INFO;

/** @brief LE CCCD info */
typedef struct
{
    uint16_t        handle;
    uint16_t        ccc_bits;
} T_CCCD_ELEM;

typedef struct
{
    uint8_t data_length;
    uint8_t padding[3];
    uint8_t data[1];
} T_CCCD_DATA;

/** @brief LE Key Type */
typedef enum
{
    LE_KEY_UNAUTHEN              = 0x04, /**< SSP generated link key without MITM protection. */
    LE_KEY_AUTHEN                = 0x05, /**< SSP generated link key with MITM protection. */
    LE_KEY_UNAUTHEN_P256         = 0x07, /**< Security Connections generated link key without MITM protection. */
    LE_KEY_AUTHEN_P256           = 0x08, /**< Security Connections link key with MITM protection. */
} T_LE_KEY_TYPE;

typedef struct
{
    uint8_t local_bd_type;
    uint16_t flags;
    T_LE_REMOTE_BD remote_bd;
    uint8_t local_ltk[32];
    uint8_t remote_ltk[32];
    uint8_t remote_irk[24];
    uint8_t client_supported_feature[4];
    uint8_t reserved[4];
    uint8_t cccd[204];
} T_LE_DEV_INFO;

/** @brief LE key entry */
typedef struct
{
    bool is_used;
    uint8_t idx;
    uint16_t flags;
    uint8_t local_bd_type;
    uint8_t app_data;
    uint8_t reserved[2];
    T_LE_REMOTE_BD remote_bd;
    T_LE_REMOTE_BD resolved_remote_bd;
    uint8_t local_bd_addr[6];
} T_LE_KEY_ENTRY;

typedef struct
{
    uint8_t link_key_length;
    uint8_t padding[3];
    uint8_t key[28];
} T_LE_LTK;

/** End of GAP_LE_STORAGE_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * @defgroup GAP_LE_STORAGE_EXPORT_Functions GAP LE Storage Exported Functions
 *
 * @{
 */

/**
 * @brief  Save local device name to flash.
 * @param[in] p_data Pointer to local device name @ref T_LOCAL_NAME.
 * @return Operation result.
 * @retval 0 Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    T_APP_RESULT gap_service_callback(T_SERVER_ID service_id, void *p_para)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        T_GAPS_CALLBACK_DATA *p_gap_data = (T_GAPS_CALLBACK_DATA *)p_para;
        APP_PRINT_INFO2("gap_service_callback conn_id %d msg_type %d", p_gap_data->conn_id,
                       p_gap_data->msg_type);
        if (p_gap_data->msg_type == SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE)
        {
            switch (p_gap_data->msg_data.opcode)
            {
            case GAPS_WRITE_DEVICE_NAME:
               {
                    T_LOCAL_NAME device_name;
                    memcpy(device_name.local_name, p_gap_data->msg_data.p_value, p_gap_data->msg_data.len);
                    device_name.local_name[p_gap_data->msg_data.len] = 0;
                    flash_save_local_name(&device_name);
               }
               break;

            case GAPS_WRITE_APPEARANCE:
               {
                    uint16_t appearance_val;
                    T_LOCAL_APPEARANCE appearance;

                    LE_ARRAY_TO_UINT16(appearance_val, p_gap_data->msg_data.p_value);
                    appearance.local_appearance = appearance_val;
                    flash_save_local_appearance(&appearance);
               }
               break;

            default:
               break;
            }
        }
        else if (p_gap_data->msg_type == SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION)
        {
            if (p_gap_data->msg_data.opcode == GATT_SERVICE_CHANGE_CCCD_ENABLE)
            {
                APP_PRINT_INFO0("GATT_SERVICE_CHANGE_CCCD_ENABLE");
            }
        }
        return result;
    }
 * \endcode
 */
uint32_t flash_save_local_name(T_LOCAL_NAME *p_data);

/**
 * @brief  Load local device name from flash.
 * @param[in,out] p_data Pointer to location to get local device name @ref T_LOCAL_NAME.
 * @return Operation result.
 * @retval 0 Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t device_name[GAP_DEVICE_NAME_LEN] = "BB3_GapTest";
        uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;
        uint8_t appearance_prop = GAPS_PROPERTY_WRITE_ENABLE;
        uint8_t device_name_prop = GAPS_PROPERTY_WRITE_ENABLE;
        T_LOCAL_APPEARANCE appearance_local;
        T_LOCAL_NAME local_device_name;
        if (flash_load_local_appearance(&appearance_local) == 0)
        {
            if (appearance_local.local_appearance != 0xffff)
            {
                appearance = appearance_local.local_appearance;
            }
        }

        if (flash_load_local_name(&local_device_name) == 0)
        {
            if (local_device_name.local_name[0] != 0xFF)
            {
                memcpy(device_name, local_device_name.local_name, GAP_DEVICE_NAME_LEN);
            }
        }
    }
 * \endcode
 */
uint32_t flash_load_local_name(T_LOCAL_NAME *p_data);

/**
 * @brief  Save local device appearance to flash.
 * @param[in] p_data Pointer to local appearance @ref T_LOCAL_APPEARANCE.
 * @return Operation result.
 * @retval 0 Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
   T_APP_RESULT gap_service_callback(T_SERVER_ID service_id, void *p_para)
   {
       T_APP_RESULT  result = APP_RESULT_SUCCESS;
       T_GAPS_CALLBACK_DATA *p_gap_data = (T_GAPS_CALLBACK_DATA *)p_para;
       APP_PRINT_INFO2("gap_service_callback conn_id %d msg_type %d", p_gap_data->conn_id,
                       p_gap_data->msg_type);
       if (p_gap_data->msg_type == SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE)
       {
           switch (p_gap_data->msg_data.opcode)
           {
           case GAPS_WRITE_DEVICE_NAME:
               {
                   T_LOCAL_NAME device_name;
                   memcpy(device_name.local_name, p_gap_data->msg_data.p_value, p_gap_data->msg_data.len);
                   device_name.local_name[p_gap_data->msg_data.len] = 0;
                   flash_save_local_name(&device_name);
               }
               break;

           case GAPS_WRITE_APPEARANCE:
               {
                   uint16_t appearance_val;
                   T_LOCAL_APPEARANCE appearance;

                   LE_ARRAY_TO_UINT16(appearance_val, p_gap_data->msg_data.p_value);
                   appearance.local_appearance = appearance_val;
                   flash_save_local_appearance(&appearance);
               }
               break;

           default:
               break;
           }
       }
       else if (p_gap_data->msg_type == SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION)
       {
           if (p_gap_data->msg_data.opcode == GATT_SERVICE_CHANGE_CCCD_ENABLE)
           {
               APP_PRINT_INFO0("GATT_SERVICE_CHANGE_CCCD_ENABLE");
           }
       }
       return result;
   }
 * \endcode
 */
uint32_t flash_save_local_appearance(T_LOCAL_APPEARANCE *p_data);

/**
 * @brief  Load local device appearance from flash.
 * @param[in,out] p_data Pointer to location to get local appearance @ref T_LOCAL_APPEARANCE.
 * @return Operation result.
 * @retval 0 Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t device_name[GAP_DEVICE_NAME_LEN] = "BB3_GapTest";
        uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;
        uint8_t appearance_prop = GAPS_PROPERTY_WRITE_ENABLE;
        uint8_t device_name_prop = GAPS_PROPERTY_WRITE_ENABLE;
        T_LOCAL_APPEARANCE appearance_local;
        T_LOCAL_NAME local_device_name;
        if (flash_load_local_appearance(&appearance_local) == 0)
        {
            if (appearance_local.local_appearance != 0xffff)
            {
                appearance = appearance_local.local_appearance;
            }
        }

        if (flash_load_local_name(&local_device_name) == 0)
        {
            if (local_device_name.local_name[0] != 0xFF)
            {
                memcpy(device_name, local_device_name.local_name, GAP_DEVICE_NAME_LEN);
            }
        }
    }
 * \endcode
 */
uint32_t flash_load_local_appearance(T_LOCAL_APPEARANCE *p_data);

/**
 * @brief  Save local IRK to flash.
 * @param[in] p_data Pointer to local IRK @ref T_LOCAL_IRK.
 * @return Operation result.
 * @retval 0 Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
   void test(void)
   {
       T_LOCAL_IRK irk = {0x01,0x02,0x03,};
       flash_save_local_irk(&irk);
   }
 * \endcode
 */
uint32_t flash_save_local_irk(T_LOCAL_IRK *p_data);

/**
 * @brief  Load local IRK from flash.
 * @param[in,out] p_data Pointer to location to get local IRK @ref T_LOCAL_IRK.
 * @return Operation result.
 * @retval 0 Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_LOCAL_IRK irk;
        flash_load_local_irk(&irk);
    }
 * \endcode
 */
uint32_t flash_load_local_irk(T_LOCAL_IRK *p_data);


/**
 * @brief      Find key entry by bluetooth device address and address type.
 *
 * @param[in] bd_addr Pointer to remote bluetooth device address.
 * @param[in] bd_type Remote bluetooth device address type @ref T_GAP_REMOTE_ADDR_TYPE.
 * @return Pointer to the found key entry @ref T_LE_KEY_ENTRY.
 * @retval NULL No entry found.
 * @retval Others Pointer to the found key entry.
 */
T_LE_KEY_ENTRY *le_find_key_entry(uint8_t *bd_addr, T_GAP_REMOTE_ADDR_TYPE bd_type);

/**
 * @brief      Find key entry by bluetooth device address and address type.
 *
 * @param[in] remote_bd_addr Pointer to remote bluetooth device address.
 * @param[in] remote_bd_type Remote bluetooth device address type @ref T_GAP_REMOTE_ADDR_TYPE.
 * @param[in] local_bd_addr  Pointer to local bluetooth device address.
 * @param[in] local_bd_type  Local bluetooth device address type.
 * @return Pointer to the found key entry @ref T_LE_KEY_ENTRY.
 * @retval NULL No entry found.
 * @retval Others Pointer to the found key entry.
 */
T_LE_KEY_ENTRY *le_find_key_entry_v2(uint8_t *remote_bd_addr, T_GAP_REMOTE_ADDR_TYPE remote_bd_type,
                                     uint8_t *local_bd_addr, uint8_t local_bd_type);

/**
 * @brief      Find key entry by index.
 *
 * @param[in] idx Key entry index.
 * @return Pointer to the found key entry @ref T_LE_KEY_ENTRY.
 * @retval NULL No entry found.
 * @retval Others Pointer to the found key entry.
 */
T_LE_KEY_ENTRY *le_find_key_entry_by_idx(uint8_t idx);

/**
 * @brief      Get the bonded device count.
 *
 * @note This function can be called after @ref le_gap_init is invoked.
 *
 * @return Bonded device count.
 */
uint8_t le_get_bond_dev_num(void);

/**
 * @brief      Get the low priority bond device key entry.
 *
 * @note This function can be called after @ref le_gap_init is invoked.
 *
 * @return Pointer to the found key entry @ref T_LE_KEY_ENTRY.
 * @retval NULL No entry found.
 * @retval Others Pointer to the found key entry.
 */
T_LE_KEY_ENTRY *le_get_low_priority_bond(void);

/**
 * @brief      Get the high priority bond device key entry.
 *
 * @note This function can be called after @ref le_gap_init is invoked.
 *
 * @return Pointer to the found key entry @ref T_LE_KEY_ENTRY.
 * @retval NULL No entry found.
 * @retval Others Pointer to the found key entry.
 */
T_LE_KEY_ENTRY *le_get_high_priority_bond(void);

/**
 * @brief   Make the specified bonded device with high priority.
 *
 * @note This function cannot be used if local device uses different local
 *               addresses to bond with the same remote device. In this situation,
 *               @ref le_set_high_priority_bond_v2 shall be called.
 *
 * @param[in] bd_addr Pointer to remote bluetooth device address.
 * @param[in] bd_type Remote bluetooth device address type @ref T_GAP_REMOTE_ADDR_TYPE.
 * @return Operation result.
 * @retval true     Operation success.
 * @retval false    Operation failure.
 */
bool le_set_high_priority_bond(uint8_t *bd_addr, T_GAP_REMOTE_ADDR_TYPE bd_type);

/**
 * @brief   Make the specified bonded device with the high priority.
 *
 * @param[in]       p_entry       Pointer the key entry of the bonded device @ref T_LE_KEY_ENTRY.
 * @return Operation result.
 * @retval true     Operation success.
 * @retval false    Operation failure.
 */
bool le_set_high_priority_bond_v2(T_LE_KEY_ENTRY *p_entry);

/**
 * @brief   Resolve the specified bonded device.
 *
 * @param[in]       unresolved_addr       Pointer to unresolved remote bluetooth device address.
 * @param[in,out]   resolved_addr         Pointer to location to get resolved remote bluetooth device address.
 * @param[in,out]   resolved_addr_type    Pointer to to location to get resolved remote bluetooth device
 *                                        address type @ref T_GAP_IDENT_ADDR_TYPE.
 * @return Operation result.
 * @retval true     Operation success.
 * @retval false    Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_handle_authen_state_evt(uint8_t conn_id, uint8_t new_state, uint16_t status)
    {
        APP_PRINT_INFO1("app_handle_authen_state_evt:conn_id %d", conn_id);
        switch (new_state)
        {
        ...
        case GAP_AUTHEN_STATE_COMPLETE:
           {
               APP_PRINT_INFO1("GAP_MSG_LE_AUTHEN_STATE_CHANGE:(GAP_AUTHEN_STATE_COMPLETE) status 0x%x",
                               status);
               if (status == 0)
               {
                   APP_PRINT_INFO0("GAP_MSG_LE_AUTHEN_STATE_CHANGE pair success");
                   {
                       uint8_t addr[6];
                       T_GAP_REMOTE_ADDR_TYPE bd_type;
                       uint8_t resolved_addr[6];
                       T_GAP_IDENT_ADDR_TYPE resolved_bd_type;
                       le_get_conn_addr(conn_id, addr, &bd_type);
                       if (bd_type == GAP_REMOTE_ADDR_LE_RANDOM)
                       {
                           if (le_resolve_random_address(addr, resolved_addr, &resolved_bd_type))
                           {
                               APP_PRINT_INFO2("GAP_AUTHEN_STATE_COMPLETE: resolved_addr %s, resolved_addr_type %d",
                                               TRACE_BDADDR(resolved_addr), resolved_bd_type);
                           }
                           else
                           {
                               APP_PRINT_INFO0("GAP_AUTHEN_STATE_COMPLETE: resolved addr failed");
                           }
                       }
                   }
               }
               else
               {
                   APP_PRINT_INFO0("GAP_MSG_LE_AUTHEN_STATE_CHANGE pair failed");
               }
           }
           break;
           ...
        }
    }
 * \endcode
 */
bool le_resolve_random_address(uint8_t *unresolved_addr, uint8_t *resolved_addr,
                               T_GAP_IDENT_ADDR_TYPE *resolved_addr_type);

/**
 * @brief   Generate new bonded device information by APP.
 *
 * @param[in] remote_bd_addr    Pointer to remote bluetooth device address.
 * @param[in] remote_bd_type    Remote bluetooth device address type @ref T_GAP_REMOTE_ADDR_TYPE.
 * @param[in] local_bd_addr     Pointer to local bluetooth device address.
 * @param[in] local_bd_type     Local bluetooth device address type @ref T_GAP_LOCAL_ADDR_TYPE.
 * @param[in] ltk_length        LTK length.
 * @param[in] p_ltk             Pointer to LTK to write.
 * @param[in] key_type          Key type @ref T_LE_KEY_TYPE.
 * @param[in] p_cccd            Pointer to CCCD data to write @ref T_CCCD_DATA.
 * @return Operation result.
 * @retval true     Operation success.
 * @retval false    Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_REMOTE_ADDR_TYPE remote_addr_type = GAP_REMOTE_ADDR_LE_PUBLIC;
        uint8_t remote_bd_addr[6] = {0x11,0x22,0x33,0x44,0x55,0x66};
        uint8_t local_bd_addr[6] = {0x00,0x22,0x33,0x44,0x55,0x66};
        uint8_t ltk_length = 16;
        uint8_t ltk[16] = {0x0, 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
        T_LE_KEY_TYPE key_type = LE_KEY_UNAUTHEN;
        uint8_t ccc_bits_count = 16;
        T_CCCD_DATA *p_cccd_data = os_mem_alloc(RAM_TYPE_DATA_ON, 4 + ccc_bits_count * 4);

        p_cccd_data->data_length = 4;
        p_cccd_data->data[0] = 0x04;
        p_cccd_data->data[1] = 0x00;
        p_cccd_data->data[2] = 0x02;
        p_cccd_data->data[3] = 0x00;

        if(le_gen_bond_dev_v2(remote_bd_addr, remote_addr_type, local_bd_addr, GAP_LOCAL_ADDR_LE_PUBLIC,
                                ltk_length, ltk, key_type, p_cccd_data))
        {
            APP_PRINT_INFO0("Generate bond device success");
        }

        os_mem_free(p_cccd_data);
    }
    * \endcode
    */
bool le_gen_bond_dev_v2(uint8_t *remote_bd_addr, T_GAP_REMOTE_ADDR_TYPE remote_bd_type,
                        uint8_t *local_bd_addr, T_GAP_LOCAL_ADDR_TYPE local_bd_type, uint8_t ltk_length,
                        uint8_t *p_ltk, T_LE_KEY_TYPE key_type, T_CCCD_DATA *p_cccd);

/**
 * @brief   Save privacy information.
 *
 * @param[in]  p_entry        Pointer to the key entry of bonded device @ref T_LE_KEY_ENTRY.
 * @param[in]  p_privacy_info Pointer to privacy information to write @ref T_LE_PRIVACY_INFO.
 * @return Operation result.
 * @retval true     Operation success.
 * @retval false    Operation failure.
 */
bool le_set_privacy_info(T_LE_KEY_ENTRY *p_entry, T_LE_PRIVACY_INFO *p_privacy_info);

/**
 * @brief   Get privacy information.
 *
 * @param[in]  p_entry        Pointer to the key entry of bonded device @ref T_LE_KEY_ENTRY.
 * @param[in,out] p_privacy_info Pointer to location to get privacy information @ref T_LE_PRIVACY_INFO.
 * @return Operation result.
 * @retval true     Operation success.
 * @retval false    Operation failure.
 */
bool le_get_privacy_info(T_LE_KEY_ENTRY *p_entry, T_LE_PRIVACY_INFO *p_privacy_info);

/**
 * @brief   Check if the bonded device is a privacy device.
 *
 * @param[in] p_entry         Pointer to the key entry of the bonded device @ref T_LE_KEY_ENTRY.
 * @return Operation result.
 * @retval true     This bonded device is a privacy device.
 * @retval false    This bonded device is not a privacy device.
 */
bool le_check_privacy_bond(T_LE_KEY_ENTRY *p_entry);

/**
 * @brief   Get the length of device bond information.
 *
 * @return Length of device bond information.
 */
uint16_t le_get_dev_bond_info_len(void);

/**
 * @brief   Get device bond information.
 *
 * @param[in] p_entry        Pointer to the key entry of the bonded device.
 * @param[in,out] p_data     Pointer to location to get bond information.
 * @return Operation result.
 * @retval true     Operation success.
 * @retval false    Operation failure.
 */
bool le_get_dev_bond_info(T_LE_KEY_ENTRY *p_entry, uint8_t *p_data);

/**
 * @brief   Set device bond information.
 *
 * @param[in] length         Length of device bond information.
 * @param[in] p_data         Pointer to bond information to write.
 * @param[in,out] exist      Pointer to location to get value that
 *                           indicates whether bond information of the device exists.
 * @return Pointer to the key entry @ref T_LE_KEY_ENTRY.
 * @retval NULL No entry, operation failure.
 * @retval Others Pointer to the key entry.
 *         - If parameter exist is false, operation success.
 *         - If parameter exist is true, bond information of the device exists.
 */
T_LE_KEY_ENTRY *le_set_dev_bond_info(uint16_t length, uint8_t *p_data, bool *exist);

/**
 * @brief   Get the maximum number of pairing information that can be stored.
 *
 * @note This function can be called after @ref le_gap_init is invoked.
 *
 * @return Maximum number of LE paired device information that can be stored.
 */
uint8_t le_get_max_le_paired_device_num(void);

/**
 * @brief   Get device information.
 *
 * @param[in] p_entry        Pointer to the key entry of the bonded device @ref T_LE_KEY_ENTRY.
 * @param[in,out] p_info     Pointer to location to get bond information @ref T_LE_DEV_INFO.
 * @return Operation result.
 * @retval true     Operation success.
 * @retval false    Operation failure.
 */
bool le_get_dev_info(T_LE_KEY_ENTRY *p_entry, T_LE_DEV_INFO *p_info);

/**
 * @brief   Get device LTK information.
 *
 * @param[in] p_entry        Pointer to the key entry of the bonded device @ref T_LE_KEY_ENTRY.
 * @param[in] remote         Read the remote LTK or the local LTK.
 * @param[in,out] p_key_len     Pointer to location to get LTK key length.
 * @param[in,out] p_ltk         Pointer to location to get LTK.
 * @return Operation result.
 * @retval true     Operation success.
 * @retval false    Operation failure.
 */
bool le_get_dev_ltk(T_LE_KEY_ENTRY *p_entry, bool remote, uint8_t *p_key_len, uint8_t *p_ltk);

/**
 * @brief   Get remote device IRK information.
 *
 * @param[in] p_entry        Pointer to the key entry of the bonded device @ref T_LE_KEY_ENTRY.
 * @param[in] remote         Read the remote IRK or the local IRK.
 * @param[in,out] p_irk         Pointer to location to get IRK.
 * @return Operation result.
 * @retval true     Operation success.
 * @retval false    Operation failure.
 */
bool le_get_dev_irk(T_LE_KEY_ENTRY *p_entry, bool remote, uint8_t *p_irk);

/**
 * @brief   Get device priority information.
 *
 * @param[in,out] p_bond_num        Pointer to location to get the bonded device number.
 * @param[in,out] p_pri_table        Pointer to location to get priority information.
 * @return Operation result.
 * @retval true     Operation success.
 * @retval false    Operation failure.
 */
bool le_get_bond_priority(uint8_t *p_bond_num, uint8_t *p_pri_table);

/**
 * @brief   Modify local LTK.
 *
 * @param[in] p_entry        Pointer to the key entry of bonded device.
 * @param[in] key_length     Key length, Range: 7 to 16.
 * @param[in] p_ltk          Pointer to local ltk to save.
 * @return Operation result.
 * @retval true     Operation success.
 * @retval false    Operation failure.
 */
bool le_set_local_ltk(T_LE_KEY_ENTRY *p_entry, uint8_t key_length, uint8_t *p_ltk);

/**
 * @brief   Erase CCCD except specific attribute handle.
 *
 * @note This function can only be used for standby state (i.e., no link, not initiating, etc.).
 *
 * @param[in] handle_num     Number of attribute handle of CCCD which will not be erased.
 * @param[in] p_handle       Pointer to attribute handle of CCCD which will not be erased.
 * @return Operation result.
 * @retval true     Operation success.
 * @retval false    Operation failure.
 */
bool le_clear_cccd_data(uint16_t handle_num, uint16_t *p_handle);

/** @} */ /* End of group GAP_LE_STORAGE_EXPORT_Functions */
/** @} */ /* End of group GAP_LE_STORAGE */


#ifdef __cplusplus
}
#endif

#endif /* FLASH_KEY_MGR_LE_H */
