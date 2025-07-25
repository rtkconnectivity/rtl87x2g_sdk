/**
*****************************************************************************************
*     Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_chann.h
  * @brief   This file contains all the constants and function prototypes for L2CAP channel.
  * @details This L2CAP channel can transmit ATT PDU.
  * @author  jane
  * @date    2021-04-06
  * @version v1.0
  * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_CHANN_H
#define GAP_CHANN_H

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup GAP_CHANNEL_MODULE GAP L2CAP Channel Module
  * @brief GAP L2CAP Channel module
  * @{
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_CHANNEL_Exported_Types GAP L2CAP Channel Exported Types
  * @{
  */

/** @brief ATT bearer type*/
typedef enum
{
    GAP_CHANN_TYPE_LE_ATT,     //!< ATT bearer on LE transport.
    GAP_CHANN_TYPE_LE_ECFC,    //!< EATT bearer on LE transport.
    GAP_CHANN_TYPE_BREDR_ATT,  //!< ATT bearer on BR/EDR transport.
    GAP_CHANN_TYPE_BREDR_ECFC, //!< EATT bearer on BR/EDR transport.
} T_GAP_CHANN_TYPE;

/** @brief L2CAP channel connection state*/
typedef enum
{
    GAP_CHANNEL_STATE_DISCONNECTED  = 0, //!< Disconnected.
    GAP_CHANNEL_STATE_CONNECTING    = 1, //!< Connecting.
    GAP_CHANNEL_STATE_CONNECTED     = 2, //!< Connected.
    GAP_CHANNEL_STATE_DISCONNECTING = 3  //!< Disconnecting.
} T_GAP_CHANNEL_STATE;

/** @brief  L2CAP channel information.*/
typedef struct
{
    uint8_t           bd_addr[6];
    uint8_t           bd_type;
    T_GAP_CHANN_TYPE  chann_type;
    T_GAP_CHANNEL_STATE state;
    uint16_t          mtu_size;
} T_GAP_CHANN_INFO;

/** End of GAP_CHANNEL_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * @defgroup GAP_CHANNEL_EXPORT_Functions GAP L2CAP Channel Exported Functions
 *
 * @{
 */

/**
 * @brief  Get L2CAP channel information.
 * @param[in]  conn_handle Connection handle of the ACL link.
 * @param[in]  cid Channel Identifier.
 * @param[in,out] p_info Pointer to L2CAP channel information @ref T_GAP_CHANN_INFO.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 */
bool gap_chann_get_info(uint16_t conn_handle, uint16_t cid, T_GAP_CHANN_INFO *p_info);

/**
 * @brief  Get channel address information.
 * @param[in]  conn_handle Connection handle of the ACL link.
 * @param[in,out] bd_addr Pointer to remote Bluetooth device address.
 * @param[in,out] p_bd_type Pointer to remote Bluetooth device address type.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 */
bool gap_chann_get_addr(uint16_t conn_handle, uint8_t *bd_addr, uint8_t *p_bd_type);

/**
 * @brief  Get connection handle information.
 * @param[in]  bd_addr Pointer to remote Bluetooth device address.
 * @param[in]  bd_type Remote Bluetooth device address type.
 * @param[in,out] p_conn_handle Pointer to Connection handle.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 */
bool gap_chann_get_handle(uint8_t *bd_addr, uint8_t bd_type, uint16_t *p_conn_handle);

/**
 * @brief  Get active L2CAP channel number.
 * @param[in]  conn_handle Connection handle of the ACL link.
 * @return Active channel number.
 * @retval 0 No channel created.
 * @retval Others Connected channel number.
 */
uint8_t gap_chann_get_num(uint16_t conn_handle);

/**
 * @brief  Get Channel Identifier.
 *
 * @param[in]       conn_handle      Connection handle of the ACL link.
 * @param[in]       cid_record_num   Number of CID could be recorded by @ref p_cid.
 * @param[in, out]  p_cid            Pointer to Channel Identifier.
 * @param[in, out]  p_cid_num        Pointer to number of Channel Identifier.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 */
bool gap_chann_get_cid(uint16_t conn_handle, uint8_t cid_record_num, uint16_t *p_cid,
                       uint8_t *p_cid_num);
/** @} */ /* End of group GAP_CHANNEL_EXPORT_Functions */

/** @} */ /* End of group GAP_CHANNEL_MODULE */

#ifdef __cplusplus
}
#endif

#endif /* GAP_CHANN_H */
