/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_le_rf.h
* @brief    Header file for GAP RF
* @details
* @author
* @date      2020-06-18
* @version   v0.8
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_LE_RF_H
#define GAP_LE_RF_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
//#include "bt_flags.h"
#include "gap_le.h"

/** @addtogroup BT_Host Bluetooth Host
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_RF GAP LE RF Module
  * @brief GAP LE RF Module
  * @{
  */


/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_RF_Exported_Macros GAP LE Rf Exported Macros
  * @{
  */

/** @defgroup GAP_LE_RF_MSG_Types GAP LE RF Msg Types
  * @{
  */
#define GAP_MSG_LE_RF_INFO                  0xD1
/** End of GAP_LE_RF_MSG_Types
  * @}
  */

/** @defgroup GAP_LE_RF_MSG_Opcodes GAP LE RF Msg Opcodes
 * @{
 */
#define GAP_LE_RF_READ_RF_PATH_COMPENSATION                0x0000 //!<Response msg opcode for le_rf_read_rf_path_compensation
#define GAP_LE_RF_WRITE_RF_PATH_COMPENSATION               0x0001 //!<Response msg opcode for le_rf_write_rf_path_compensation

#define GAP_LE_RF_ENHANCED_READ_TRANSMIT_POWER_LEVEL       0x0010 //!<Response msg opcode for le_rf_enhanced_read_transmit_power_level
#define GAP_LE_RF_READ_REMOTE_TRANSMIT_POWER_LEVEL         0x0011 //!<Response msg opcode for le_rf_read_remote_transmit_power_level
#define GAP_LE_RF_SET_TRANSMIT_POWER_REPORTING_ENABLE      0x0012 //!<Response msg opcode for le_rf_set_transmit_power_reporting_enable
#define GAP_LE_RF_TRANSMIT_POWER_REPORTING_INFO            0x0013 //!<Notification msg opcode for LE transmit power reporting

#define GAP_LE_RF_SET_PATH_LOSS_REPORTING_PARAMS           0x0020 //!<Response msg opcode for le_rf_set_path_loss_reporting_params
#define GAP_LE_RF_SET_PATH_LOSS_REPORTING_ENABLE           0x0021 //!<Response msg opcode for le_rf_set_path_loss_reporting_enable
#define GAP_LE_RF_PATH_LOSS_THRESHOLD_INFO                 0x0022 //!<Notification msg opcode for LE path loss threshold
/**
  * @}
  */

/** @defgroup GAP_LE_RF_TRANSMIT_POWER_LEVEL_FLAG_BITS LE Transmit Power Level Flag Bits
* @{
 */
#define GAP_LE_RF_TRANSMIT_POWER_LEVEL_FLAG_MIN_BIT       0x01     /**< Transmit power level is at minimum level. */
#define GAP_LE_RF_TRANSMIT_POWER_LEVEL_FLAG_MAX_BIT       0x02     /**< Transmit power level is at maximum level. */
/**
  * @}
  */

/** End of GAP_LE_RF_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_RF_Exported_Types GAP LE RF Exported Types
  * @brief
  * @{
  */
typedef enum
{
    GAP_LE_RF_POWER_CONTROL_PHYS_1M          = 0x01, /**<  LE 1M PHY. */
    GAP_LE_RF_POWER_CONTROL_PHYS_2M          = 0x02, /**<  LE 2M PHY. */
    GAP_LE_RF_POWER_CONTROL_PHYS_CODED_S8    = 0x03, /**<  LE Coded PHY with S=8 data coding. */
    GAP_LE_RF_POWER_CONTROL_PHYS_CODED_S2    = 0x04, /**<  LE Coded PHY with S=2 data coding. */
} T_GAP_LE_RF_POWER_CONTROL_PHYS_TYPE;

/** @defgroup GAP_LE_RF_CB_Msg_Exported_Types GAP RF Callback Msg Exported Types
  * @{
  */
typedef struct
{
    uint16_t            cause;
} T_LE_RF_CAUSE;

/** @brief  Response for read rf path compensation.*/
typedef struct
{
    uint16_t        cause;
    int16_t         rf_tx_path_comp_value;
    /**<  Range: -128.0 dB (0xFB00) to 128.0 dB (0x0500) Units: 0.1 dB. */
    int16_t         rf_rx_path_comp_value;
    /**<  Range: -128.0 dB (0xFB00) to 128.0 dB (0x0500) Units: 0.1 dB. */
} T_LE_RF_READ_RF_PATH_COMPENSATION_RSP;

/** @brief  Response for enhanced read transmit power level.*/
typedef struct
{
    uint8_t                               conn_id;
    uint16_t                              cause;
    T_GAP_LE_RF_POWER_CONTROL_PHYS_TYPE   phy;
    int8_t                                cur_tx_power_level;
    /**<  Current transmit power level Range: -127 to 20 Units: dBm. \n
          0x7F Current transmit power level is unavailable. */
    int8_t                                max_tx_power_level;
    /**<  Maximum transmit power level Range: -127 to 20 Units: dBm. */
} T_LE_RF_ENHANCED_READ_TRANSMIT_POWER_LEVEL_RSP;

typedef struct
{
    uint16_t        cause;
    uint8_t         conn_id;
} T_LE_RF_READ_REMOTE_TRANSMIT_POWER_LEVEL_RSP;

/** @brief  Response for set transmit power reporting enable.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
} T_LE_RF_SET_TRANSMIT_POWER_REPORTING_ENABLE_RSP;

/** @brief  Information for transmit power reporting.*/
typedef struct
{
    uint8_t                               conn_id;
    uint16_t                              cause;
    uint8_t                               reason;
    /**<  @arg 0x00: Local transmit power changed.
          @arg 0x01: Remote transmit power changed.
          @arg 0x02: LE read remote transmit power level command completed. */
    T_GAP_LE_RF_POWER_CONTROL_PHYS_TYPE   phy;
    int8_t                                tx_power_level;
    /**<  Transmit power level Range: -127 to 20 Units: dBm. \n
          0x7E Remote device is not managing power levels on this PHY. \n
          0x7F Transmit power level is not available. */
    uint8_t                               tx_power_level_flag;
    /**<  @ref  GAP_LE_RF_TRANSMIT_POWER_LEVEL_FLAG_BITS.*/
    int8_t                                delta;/**<  Change in transmit power level Units: dB,
                                                      positive indicates increased power, negative indicates
                                                      decreased power, zero indicates unchanged.*/
} T_LE_RF_TRANSMIT_POWER_REPORTING_INFO;

/** @brief  Response for set path loss reporting params rsp.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
} T_LE_RF_SET_PATH_LOSS_REPORTING_PARAMS_RSP;

/** @brief  Response for set path loss reporting enable rsp.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
} T_LE_RF_SET_PATH_LOSS_REPORTING_ENABLE_RSP;

/** @brief  Information for path loss threshold.*/
typedef struct
{
    uint8_t        conn_id;
    uint8_t        current_path_loss; /**<  Current path loss Units: dB,
                                            always zero or positive.
                                            @arg  0xFF Unavailable. */
    uint8_t        zone_entered;/**<  @arg 0x00: Entered low zone.
                                      @arg 0x01: Entered middle zone.
                                      @arg 0x02: Entered high zone. */
} T_LE_RF_PATH_LOSS_THRESHOLD_INFO;

/** @brief  GAP RF Callback Data*/
typedef union
{
    T_LE_RF_CAUSE                                     le_rf_cause;

    T_LE_RF_READ_RF_PATH_COMPENSATION_RSP            *p_le_rf_read_rf_path_compensation_rsp;

    T_LE_RF_ENHANCED_READ_TRANSMIT_POWER_LEVEL_RSP   *p_le_rf_enhanced_read_transmit_power_level_rsp;
    T_LE_RF_READ_REMOTE_TRANSMIT_POWER_LEVEL_RSP     *p_le_rf_read_remote_transmit_power_level_rsp;
    T_LE_RF_SET_TRANSMIT_POWER_REPORTING_ENABLE_RSP  *p_le_rf_set_transmit_power_reporting_enable_rsp;
    T_LE_RF_TRANSMIT_POWER_REPORTING_INFO            *p_le_rf_transmit_power_reporting_info;

    T_LE_RF_SET_PATH_LOSS_REPORTING_PARAMS_RSP       *p_le_rf_set_path_loss_reporting_params_rsp;
    T_LE_RF_SET_PATH_LOSS_REPORTING_ENABLE_RSP       *p_le_rf_set_path_loss_reporting_enable_rsp;
    T_LE_RF_PATH_LOSS_THRESHOLD_INFO                 *p_le_rf_path_loss_threshold_info;
} T_GAP_LE_RF_CB_DATA;

typedef struct
{
    uint16_t                      opcode;
    T_GAP_LE_RF_CB_DATA           data;
} T_GAP_LE_RF_CB;
/**
  * @}
  */

/** End of GAP_LE_RF_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_RF_Exported_Functions GAP LE RF Exported Functions
  * @brief
  * @{
  */
/**
* @brief   Read the RF path compensation values parameter used in the Tx power level and RSSI calculation.
*          Read result will be returned by @ref app_gap_callback with cb_type @ref GAP_MSG_LE_RF_INFO with
*          opcode @ref GAP_LE_RF_READ_RF_PATH_COMPENSATION.
*
* @retval GAP_CAUSE_SUCCESS: Send request success.
* @retval GAP_CAUSE_SEND_REQ_FAILED: Send request fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
      T_GAP_CAUSE cause;
      cause = le_rf_read_rf_path_compensation();
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;
        T_GAP_LE_RF_CB *p_le_rf_cb  = NULL;

        switch (cb_type)
        {
        ...
          case GAP_MSG_LE_RF_INFO:
              APP_PRINT_INFO0("GAP_MSG_LE_RF_INFO");
              p_le_rf_cb  = (T_GAP_LE_RF_CB *)p_data->p_le_cb_data;

              switch (p_le_rf_cb->opcode)
              {
                ...
                case GAP_LE_RF_READ_RF_PATH_COMPENSATION:
                    APP_PRINT_INFO3("GAP_LE_RF_READ_RF_PATH_COMPENSATION:cause 0x%x, rf tx path comp value %d, rf rx path comp value %d",
                                    p_le_rf_cb->data.p_le_rf_read_rf_path_compensation_rsp->cause,
                                    p_le_rf_cb->data.p_le_rf_read_rf_path_compensation_rsp->rf_tx_path_comp_value,
                                    p_le_rf_cb->data.p_le_rf_read_rf_path_compensation_rsp->rf_rx_path_comp_value);
                    break;
                ...
                }
        ...
        }
  * \endcode
  */
T_GAP_CAUSE le_rf_read_rf_path_compensation(void);

/**
* @brief   Indicate the RF path gain or loss between the RF transceiver and the antenna contributed
*          by intermediate components.
*
* @param[in] rf_tx_path_comp_value RF Tx Path Compensation.
                                   @arg Range: -128.0 dB (0xFB00) to 128.0 dB (0x0500).
                                   @arg Units: 0.1 dB.
* @param[in] rf_rx_path_comp_value RF Rx Path Compensation.
                                   @arg Range: -128.0 dB (0xFB00) to 128.0 dB (0x0500).
                                   @arg Units: 0.1 dB.
* @retval GAP_CAUSE_SUCCESS: Send request success.
* @retval GAP_CAUSE_SEND_REQ_FAILED: Send request fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
      T_GAP_CAUSE cause;

      cause = le_rf_write_rf_path_compensation(rf_tx_path_comp_value,
                                              rf_rx_path_comp_value);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;
        T_GAP_LE_RF_CB *p_le_rf_cb  = NULL;

        switch (cb_type)
        {
        ...
          case GAP_MSG_LE_RF_INFO:
              APP_PRINT_INFO0("GAP_MSG_LE_RF_INFO");
              p_le_rf_cb  = (T_GAP_LE_RF_CB *)p_data->p_le_cb_data;

              switch (p_le_rf_cb->opcode)
              {
                ...
                  case GAP_LE_RF_WRITE_RF_PATH_COMPENSATION:
                      APP_PRINT_INFO1("GAP_LE_RF_WRITE_RF_PATH_COMPENSATION:cause 0x%x",
                                      p_le_rf_cb->data.le_rf_cause.cause);
                      break;
                ...
                }
        ...
        }
  * \endcode
  */
T_GAP_CAUSE le_rf_write_rf_path_compensation(int16_t rf_tx_path_comp_value,
                                             int16_t rf_rx_path_comp_value);

/**
* @brief   Read the current and maximum transmit power levels of the local Controller on the ACL connection and the PHY.
*          Read result will be returned by @ref app_gap_callback with cb_type @ref GAP_MSG_LE_RF_INFO with
*          opcode @ref GAP_LE_RF_ENHANCED_READ_TRANSMIT_POWER_LEVEL.
*
* @param[in] conn_id Connection ID.
* @param[in] phy The PHY involved: @ref T_GAP_LE_RF_POWER_CONTROL_PHYS_TYPE.
* @retval GAP_CAUSE_SUCCESS: Send request success.
* @retval GAP_CAUSE_SEND_REQ_FAILED: Send request fail.
* @retval GAP_CAUSE_NON_CONN: Set request sent fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
      T_GAP_CAUSE cause;
      uint8_t conn_id;

      cause = le_rf_enhanced_read_transmit_power_level(conn_id, phy);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;
        T_GAP_LE_RF_CB *p_le_rf_cb  = NULL;

        switch (cb_type)
        {
        ...
          case GAP_MSG_LE_RF_INFO:
              APP_PRINT_INFO0("GAP_MSG_LE_RF_INFO");
              p_le_rf_cb  = (T_GAP_LE_RF_CB *)p_data->p_le_cb_data;

              switch (p_le_rf_cb->opcode)
              {
                ...
                case GAP_LE_RF_ENHANCED_READ_TRANSMIT_POWER_LEVEL:
                    APP_PRINT_INFO5("GAP_LE_RF_ENHANCED_READ_TRANSMIT_POWER_LEVEL: conn id %d, cause 0x%x, phy %d, cur trans power level %d, max trans power level %d",
                                    p_le_rf_cb->data.p_le_rf_enhanced_read_transmit_power_level_rsp->conn_id,
                                    p_le_rf_cb->data.p_le_rf_enhanced_read_transmit_power_level_rsp->cause,
                                    p_le_rf_cb->data.p_le_rf_enhanced_read_transmit_power_level_rsp->phy,
                                    p_le_rf_cb->data.p_le_rf_enhanced_read_transmit_power_level_rsp->cur_tx_power_level,
                                    p_le_rf_cb->data.p_le_rf_enhanced_read_transmit_power_level_rsp->max_tx_power_level);
                    break;
                ...
                }
        ...
        }
  * \endcode
  */
T_GAP_CAUSE le_rf_enhanced_read_transmit_power_level(uint8_t conn_id,
                                                     T_GAP_LE_RF_POWER_CONTROL_PHYS_TYPE phy);

/**
* @brief   Read the transmit power level used by the remote Controller on the ACL connection and the PHY.
*          Read result will be returned by @ref app_gap_callback with cb_type @ref GAP_MSG_LE_RF_INFO with
*          opcode @ref GAP_LE_RF_TRANSMIT_POWER_REPORTING_INFO.
*
* @param[in] conn_id Connection ID.
* @param[in] phy The PHY involved: @ref T_GAP_LE_RF_POWER_CONTROL_PHYS_TYPE.
* @retval GAP_CAUSE_SUCCESS: Send request success.
* @retval GAP_CAUSE_SEND_REQ_FAILED: Send request fail.
* @retval GAP_CAUSE_NON_CONN: Set request sent fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
      T_GAP_CAUSE cause;
      uint8_t conn_id;

      cause = le_rf_read_remote_transmit_power_level(conn_id, phy);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;
        T_GAP_LE_RF_CB *p_le_rf_cb  = NULL;

        switch (cb_type)
        {
        ...
          case GAP_MSG_LE_RF_INFO:
              APP_PRINT_INFO0("GAP_MSG_LE_RF_INFO");
              p_le_rf_cb  = (T_GAP_LE_RF_CB *)p_data->p_le_cb_data;

              switch (p_le_rf_cb->opcode)
              {
                ...
                case GAP_LE_RF_READ_REMOTE_TRANSMIT_POWER_LEVEL:
                    APP_PRINT_INFO2("GAP_LE_RF_SET_TRANSMIT_POWER_REPORTING_ENABLE: conn id %d, cause 0x%x",
                                    p_le_rf_cb->data.p_le_rf_read_remote_transmit_power_level_rsp->conn_id,
                                    p_le_rf_cb->data.p_le_rf_read_remote_transmit_power_level_rsp->cause);
                    break;

                case GAP_LE_RF_TRANSMIT_POWER_REPORTING_INFO:
                    APP_PRINT_INFO7("GAP_LE_RF_TRANSMIT_POWER_REPORTING_INFO: conn id %d, cause 0x%x, reason 0x%x, phy %d, transmit power level %d, transmit power level flag %d, delta %d",
                                    p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info->conn_id,
                                    p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info->cause,
                                    p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info->reason,
                                    p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info->phy,
                                    p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info->tx_power_level,
                                    p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info->tx_power_level_flag,
                                    p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info->delta);
                    break;
                ...
                }
        ...
        }
  * \endcode
  */
T_GAP_CAUSE le_rf_read_remote_transmit_power_level(uint8_t conn_id,
                                                   T_GAP_LE_RF_POWER_CONTROL_PHYS_TYPE phy);

/**
* @brief   Enable or disable the reporting to the local Host of transmit power level changes
*          in the local and remote Controllers for the ACL connection.
*
* @param[in] conn_id Connection ID.
* @param[in] local_enable Enable or disable local transmit power reports.
* @param[in] remote_enable Enable or disable remote transmit power reports.
* @retval GAP_CAUSE_SUCCESS: Send request success.
* @retval GAP_CAUSE_SEND_REQ_FAILED: Send request fail.
* @retval GAP_CAUSE_NON_CONN: Set request sent fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
      T_GAP_CAUSE cause;
      uint8_t conn_id;

      cause = le_rf_set_transmit_power_reporting_enable(conn_id, local_enable, remote_enable);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;
        T_GAP_LE_RF_CB *p_le_rf_cb  = NULL;

        switch (cb_type)
        {
        ...
          case GAP_MSG_LE_RF_INFO:
              APP_PRINT_INFO0("GAP_MSG_LE_RF_INFO");
              p_le_rf_cb  = (T_GAP_LE_RF_CB *)p_data->p_le_cb_data;

              switch (p_le_rf_cb->opcode)
              {
                ...
                case GAP_LE_RF_SET_TRANSMIT_POWER_REPORTING_ENABLE:
                     APP_PRINT_INFO2("GAP_LE_RF_SET_TRANSMIT_POWER_REPORTING_ENABLE:conn id %d, cause 0x%x",
                                     p_le_rf_cb->data.p_le_rf_set_transmit_power_reporting_enable_rsp->conn_id,
                                     p_le_rf_cb->data.p_le_rf_set_transmit_power_reporting_enable_rsp->cause);
                    break;

                case GAP_LE_RF_TRANSMIT_POWER_REPORTING_INFO:
                    APP_PRINT_INFO7("GAP_LE_RF_TRANSMIT_POWER_REPORTING_INFO:conn id %d, cause 0x%x, reason 0x%x, phy %d, transmit power level %d, transmit power level flag %d, delta %d",
                                    p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info->conn_id,
                                    p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info->cause,
                                    p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info->reason,
                                    p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info->phy,
                                    p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info->tx_power_level,
                                    p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info->tx_power_level_flag,
                                    p_le_rf_cb->data.p_le_rf_transmit_power_reporting_info->delta);
                    break;
                ...
                }
        ...
        }
  * \endcode
  */
T_GAP_CAUSE le_rf_set_transmit_power_reporting_enable(uint8_t conn_id, uint8_t local_enable,
                                                      uint8_t remote_enable);

/**
* @brief   Set the path loss threshold reporting parameters for the ACL connection.
*
* @param[in] conn_id Connection ID.
* @param[in] high_threshold High threshold for the path loss.
                            @arg Units: dB.
                            @arg 0xFF: High Threshold unused.
* @param[in] high_hysteresis Hysteresis value for the high threshold.
                             @arg Units: dB.
* @param[in] low_threshold Low threshold for the path loss.
                           @arg Units: dB.
* @param[in] low_hysteresis Hysteresis value for the low threshold.
                            @arg Units: dB.
* @param[in] min_time_spent Minimum time in number of connection events to be observed once the
                            path crosses the threshold before an event is generated.
* @retval GAP_CAUSE_SUCCESS: Send request success.
* @retval GAP_CAUSE_SEND_REQ_FAILED: Send request fail.
* @retval GAP_CAUSE_NON_CONN: Set request sent fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
      T_GAP_CAUSE cause;
      uint8_t conn_id;

      cause = le_rf_set_path_loss_reporting_params(conn_id, high_threshold, high_hysteresis,
                                                  low_threshold, low_hysteresis, min_time_spent);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;
        T_GAP_LE_RF_CB *p_le_rf_cb  = NULL;

        switch (cb_type)
        {
        ...
          case GAP_MSG_LE_RF_INFO:
              APP_PRINT_INFO0("GAP_MSG_LE_RF_INFO");
              p_le_rf_cb  = (T_GAP_LE_RF_CB *)p_data->p_le_cb_data;

              switch (p_le_rf_cb->opcode)
              {
                ...
                case GAP_LE_RF_SET_PATH_LOSS_REPORTING_PARAMS:
                    APP_PRINT_INFO2("GAP_LE_RF_SET_PATH_LOSS_REPORTING_PARAMS:conn id %d, cause 0x%x",
                                    p_le_rf_cb->data.p_le_rf_set_path_loss_reporting_params_rsp->conn_id,
                                    p_le_rf_cb->data.p_le_rf_set_path_loss_reporting_params_rsp->cause);
                    break;
                ...
                }
        ...
        }
  * \endcode
  */
T_GAP_CAUSE le_rf_set_path_loss_reporting_params(uint8_t conn_id, uint8_t high_threshold,
                                                 uint8_t high_hysteresis, uint8_t low_threshold,
                                                 uint8_t low_hysteresis, uint16_t min_time_spent);

/**
* @brief   Enable or disable path loss reporting.
*
* @param[in] conn_id Connection ID.
* @param[in] enable Enable or disable path loss reporting.
* @retval GAP_CAUSE_SUCCESS: Send request success.
* @retval GAP_CAUSE_SEND_REQ_FAILED: Send request fail.
* @retval GAP_CAUSE_NON_CONN: Set request sent fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
      T_GAP_CAUSE cause;
      uint8_t conn_id;

      cause = le_rf_set_path_loss_reporting_enable(conn_id, enable);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;
        T_GAP_LE_RF_CB *p_le_rf_cb  = NULL;

        switch (cb_type)
        {
        ...
          case GAP_MSG_LE_RF_INFO:
              APP_PRINT_INFO0("GAP_MSG_LE_RF_INFO");
              p_le_rf_cb  = (T_GAP_LE_RF_CB *)p_data->p_le_cb_data;

              switch (p_le_rf_cb->opcode)
              {
                ...
                case GAP_LE_RF_SET_PATH_LOSS_REPORTING_ENABLE:
                    APP_PRINT_INFO2("GAP_LE_RF_SET_PATH_LOSS_REPORTING_ENABLE:conn id %d, cause 0x%x",
                                    p_le_rf_cb->data.p_le_rf_set_path_loss_reporting_enable_rsp->conn_id,
                                    p_le_rf_cb->data.p_le_rf_set_path_loss_reporting_enable_rsp->cause);
                    break;

                case GAP_LE_RF_PATH_LOSS_THRESHOLD_INFO:
                    APP_PRINT_INFO3("GAP_LE_RF_PATH_LOSS_THRESHOLD_INFO:conn id %d, current path loss %d, zone entered %d",
                                    p_le_rf_cb->data.p_le_rf_path_loss_threshold_info->conn_id,
                                    p_le_rf_cb->data.p_le_rf_path_loss_threshold_info->current_path_loss,
                                    p_le_rf_cb->data.p_le_rf_path_loss_threshold_info->zone_entered);
                    break;
                ...
                }
        ...
        }
  * \endcode
  */
T_GAP_CAUSE le_rf_set_path_loss_reporting_enable(uint8_t conn_id, uint8_t enable);

/** End of GAP_LE_RF_Exported_Functions
  * @}
  */

/** End of GAP_LE_RF
  * @}
  */

/** End of GAP_LE
  * @}
  */

/** End of BT_Host
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* GAP_LE_RF_H */