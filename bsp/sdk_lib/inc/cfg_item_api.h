/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     cfg_item_api.h
* @brief    This file provides api for cfg item operation.
* @details
* @author
* @date     2021-12-9
* @version  v1.0
*****************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef __CFG_ITEM_API_H_
#define __CFG_ITEM_API_H_


/*============================================================================*
 *                               Header Files
*============================================================================*/
#include <stdint.h>
#include <stdbool.h>

/** @defgroup HAL_CFG_ITEM_API Config Item Api
    * @brief config item api
    * @{
    */
#ifdef __cplusplus
extern "C" {
#endif
/*============================================================================*
 *                              Types
*============================================================================*/
/** @defgroup HAL_CFG_ITEM_API_Exported_Types Config Item Exported Types
  * @brief
  * @{
  */

typedef enum
{
    CFG_SERACH_ENTRY_NOT_EXIST = -1,
    CFG_SERACH_ENTRY_SUCCESS = 0,
    CFG_SERACH_ENTRY_SIZE_MISMATCH = 1,
    CFG_SERACH_MODULE_SIZE_MISMATCH = 2,
    CFG_SERACH_MODULE_SIG_MISMATCH = 3,
    CFG_SEARCH_ENTRY_SIZE_INVALID = 4,
} CFG_SERACH_RESULT_E;

typedef enum
{
    CFG_UPDATE_SUCCESS = 0,
    CFG_UPDATE_UNLOCK_BP_FAIL = 1,
    CFG_UPDATE_ERASE_FAIL = 2,
    CFG_UPDATE_WRITE_FAIL = 3,
    CFG_UPDATE_LOCK_BP_FAIL = 4,
} CFG_UPDATE_RESULT_E;

typedef enum
{
    CFG_READ   = 1,
    CFG_UPDATE = 2,
} CFG_OP_CODE_E;

typedef enum
{
    CFG_READ_NONE      = 0, //!< dong't need to read
    CFG_READ_SUCCESS   = 1,
    CFG_READ_NOT_EXIST = 2,
    CFG_READ_FAIL      = 3,
} CFG_READ_RESULT_E;

typedef struct
{
    /*Adaptivity (LBT)*/
    /* LBTEnableFlag:
     * update(1: set the LBTEnable, 0: don't set LBTEnable),
     * read request(1: read the LBTEnable, 0: don't read LBTEnable),
     * read response(CFG_READ_RESULT_E).
    */
    uint8_t LBTEnableFlag;
    uint8_t LBTEnable;     //!< 0:Disable 1:Enable

    /* LBTModeFlag:
     * update(1: set the LBTMode, 0: don't set LBTMode),
     * read request(1: read the LBTMode, 0: don't read LBTMode),
     * read response(CFG_READ_RESULT_E).
    */
    uint8_t LBTModeFlag;
    uint8_t LBTMode;      //!< 0:No Tx, 1: Reduce Power Tx

    /* LBTThresholdFlag:
     * update(1: set the LBTThreshold, 0: don't set LBTThreshold)
     * read request(1: read the LBTThreshold, 0: don't read LBTThreshold)
     * read response(CFG_READ_RESULT_E)
    */
    uint8_t LBTThresholdFlag;
    union
    {
        uint8_t LBTThresholdIndex; //!< set LBTThreshold with index value
        int8_t  LBTThresholdDBI; //!< set LBTThreshold with dbi value
    } LBTThreshold;

    /* LBTAntennaGainFlag:
     * update(1: set the LBTAntennaGain, 0: don't set LBTAntennaGain)
     * read request(1: read the LBTAntennaGain, 0: don't read LBTAntennaGain)
     * read response(CFG_READ_RESULT_E)
    */
    uint8_t LBTAntennaGainFlag;
    union
    {
        uint8_t LBTAntennaGainIndex; //!< set LBTAntennaGain with index value
        int8_t LBTAntennaGainDBM; //!< set LBTAntennaGain with dbm value
    } LBTAntennaGain;
} LBT_CFG_t;

typedef struct
{
    /* txgain_br_1M_flag:
     * update(1: set the txgain_br_1M, 0: don't set txgain_br_1M)
     * read request(1: set the txgain_br_1M, 0: don't set txgain_br_1M)
     * read response(CFG_READ_RESULT_E)
     */
    uint8_t txgain_br_1M_flag; //!< 1: set the txgain_br_1M, 0: don't set txgain_br_1M
    union
    {
        uint8_t txgain_br_1M_index; //!< set txgain_br_1M with index value
        int8_t  txgain_br_1M_dbm; //!< set txgain_br_1M with dbm value
    } txgain_br_1M;

    /* txgain_edr_2M_flag:
     * update(1: set the txgain_edr_2M, 0: don't set txgain_edr_2M)
     * read request(1: set the txgain_edr_2M, 0: don't set txgain_edr_2M)
     * read response(CFG_READ_RESULT_E)
     */
    uint8_t txgain_edr_2M_flag;
    union
    {
        uint8_t txgain_edr_2M_index; //!< set txgain_edr_2M with index value
        int8_t txgain_edr_2M_dbm; //!< set txgain_edr_2M with dbm value
    } txgain_edr_2M;

    /* txgain_edr_3M_flag:
     * update(1: set the txgain_edr_3M, 0: don't set txgain_edr_3M)
     * read request(1: set the txgain_edr_3M, 0: don't set txgain_edr_3M)
     * read response(CFG_READ_RESULT_E)
     */
    uint8_t txgain_edr_3M_flag;
    union
    {
        uint8_t txgain_edr_3M_index; //!< set txgain_edr_3M with index value
        int8_t txgain_edr_3M_dbm; //!< set txgain_edr_3M with dbm value
    } txgain_edr_3M;

    /* txgain_br_LE1M_flag:
     * update(1: set the txgain_br_LE1M, 0: don't set txgain_br_LE1M)
     * read request(1: set the txgain_br_LE1M, 0: don't set txgain_br_LE1M)
     * read response(CFG_READ_RESULT_E)
    */
    uint8_t txgain_br_LE1M_flag;
    union
    {
        uint8_t txgain_edr_LE1M_index; //!< set txgain_br_LE1M with index value
        int8_t txgain_edr_LE1M_dbm; //!< set txgain_br_LE1M with dbm value
    } txgain_edr_LE1M;

    /* txgain_br_LE2M_flag:
     * update(1: set the txgain_br_LE2M, 0: don't set txgain_br_LE2M)
     * read request(1: set the txgain_br_LE2M, 0: don't set txgain_br_LE2M)
     * read response(CFG_READ_RESULT_E)
    */
    uint8_t txgain_br_LE2M_flag;
    union
    {
        uint8_t txgain_edr_LE2M_index; //!< set txgain_br_LE2M with index value
        int8_t txgain_edr_LE2M_dbm; //!< set txgain_br_LE2M with dbm value
    } txgain_edr_LE2M;
} TX_POWER_CFG_t;

/** End of HAL_CFG_ITEM_API_Exported_Types
 * @}
 */
/*============================================================================*
 *                              Functions
*============================================================================*/
/** @addtogroup HAL_Cfg_Item_Ext_Exported_Functions Config Item  Exported Functions
  * @{
  */

/**
 * @brief get the config payload length
 * \param p_cfg_payload   NULL: get the cfg module total length on flash OCCD_ADDRESS.
 *                        ram address: pointer to the occd cfg payload data backup on ram
 *
 * @return total cfg module length
 */
uint32_t cfg_get_size(void *p_cfg_payload);
/**
 * @brief read back the config data on the flash to the ram buffer
 * \param address   specify the OCCD read address,default set as OCCD_ADDRESS
 * \param backup_len  specify the backup cfg data length
 *
 * @return the cfg buffer backup pointer on heap
 */
void *cfg_backup(uint32_t address, uint32_t backup_len);

/**
 * @brief write the config data on the ram to the flash
 * \param p_new_cfg_buf   pointer to the occd cfg payload data backup on ram
 * \param backup_len      specify the cfg buffer length
 *
 * @return the cfg buffer written result
 */
bool cfg_write_to_flash(void *p_new_cfg_buf, uint32_t backup_len);

/**
 * @brief update the cfg entry item in the config data buffer
 * \param p_new_cfg_buf   pointer to the occd cfg payload data backup on ram
 * \param module_id       specify the cfg item module id
 * \param p_cfg_entry      pointer to the cfg entry item to update
 * @return the result of updating the cfg item in p_new_cfg_buf
 */
bool cfg_update_item_in_store(uint8_t *p_new_cfg_buf, uint16_t module_id,
                              void *p_cfg_entry);

/**
 * @brief update the cfg entry item on the occd flash
 * \param module_id       specify the cfg item module id
 * \param p_cfg_entry      pointer to the cfg entry item to update
 * @return the result of updating the cfg item on the flash of OCCD
 */
bool cfg_add_item(uint16_t module_id, void *p_cfg_entry);

/**
  * @brief Write MAC address to config, this is mainly used on production line.
  * @param[in] p_mac_addr  The buffer hold MAC address (6 bytes).
  * @return Write MAC to config fail or success.
  * @retval true    Write MAC to config success.
  * @retval false   Write MAC to config fails or not write existed MAC.
 */
bool cfg_update_mac(uint8_t *p_mac_addr);

/**
  * @brief Write 40M XTAL calibration data to config sc_xi_40m and sc_xo_40m,
  *        this is mainly used on production line.
  * @param[in] xtal               The value of 40M XTAL calibration data
  * @return Write calibration data to config fail or success.
  * @retval true              Success.
  * @retval false             Fail.
  */
bool cfg_update_xtal(uint8_t xtal);

/**
  * @brief read the cfg entry item on the occd flash
  * \param module_id       specify the cfg item module id
  * \param p_cfg_entry      pointer to the cfg entry item to update
  * @return CFG_SERACH_RESULT_E
  */

CFG_SERACH_RESULT_E cfg_read_item(uint16_t module_id, void *p_cfg_entry);
/**
  * @brief Read MAC address.
  * @param[in] p_mac_addr  The buffer to read back the MAC address (6 bytes).
  * @return read MAC result, fail or success.
  * @retval true    read MAC success.
  * @retval false   read MAC fail or not existed.
 */
bool cfg_read_mac(uint8_t *p_mac_addr);

/**
  * @brief Read 40M XTAL XI(sc_xi_40m) and XO(sc_xo_40m) data.
  * @param[out] p_xtal_xi    Get the value of 40M XTAL XI calibration data.
  * @param[out] p_xtal_xo    Get the value of 40M XTAL XO calibration data.
  * @return The result of reading 40M XTAL calibration data.
  * @retval true              Success.
  * @retval false             Fail.
  */
bool cfg_read_xtal(uint8_t *p_xtal_xi, uint8_t *p_xtal_xo);


/** End of HAL_Cfg_Item_Ext_Exported_Functions
  * @}
  */
#ifdef __cplusplus
}
#endif
/** @} */ /* End of group HAL_CFG_ITEM_API */
#endif
