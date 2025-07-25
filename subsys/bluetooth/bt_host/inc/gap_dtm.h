/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_dtm.h
  * @brief
  * @details
  * @author  jeff_zheng
  * @date    2017-08-02
  * @version v1.0
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
  ******************************************************************************
  */
/* Define to prevent recursive inclusion **/
#ifndef GAP_DTM_H
#define GAP_DTM_H

#include "gap_le.h"
#include <gap_le_types.h>

/** @addtogroup GAP_LE_DTM GAP LE Direct Test Mode
  * @brief GAP LE Direct Test Mode Module
  * @{
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_DTM_Exported_Types GAP LE DTM Exported Types
  * @brief
  * @{
  */
typedef enum
{
    GAP_DTM_MODULATION_INDEX_STANDARD   = 0x00,    /**< Assume transmitter will have a standard modulation index. */
    GAP_DTM_MODULATION_INDEX_STABLE     = 0x01,    /**< Assume transmitter will have a stable modulation index. */
} T_GAP_DTM_MODULATION_INDEX_TYPE;

typedef enum
{
    GAP_DTM_PACKET_PAYLOAD_PRBS9 = 0x00,    /**< PRBS9 sequence '11111111100000111101' (in transmission order). */
    GAP_DTM_PACKET_PAYLOAD_1100 = 0x01,     /**< Repeated '11110000' (in transmission order) sequence. */
    GAP_DTM_PACKET_PAYLOAD_10 = 0x02,       /**< Repeated '10101010' (in transmission order) sequence. */
    GAP_DTM_PACKET_PAYLOAD_PRBS15 = 0x03,   /**< PRBS15 sequence. */
    GAP_DTM_PACKET_PAYLOAD_ALL1 = 0x04,     /**< Repeated '11111111' (in transmission order) sequence. */
    GAP_DTM_PACKET_PAYLOAD_ALL0 = 0x05,     /**< Repeated '00000000' (in transmission order) sequence. */
    GAP_DTM_PACKET_PAYLOAD_0011 = 0x06,     /**< Repeated '00001111' (in transmission order) sequence. */
    GAP_DTM_PACKET_PAYLOAD_01 = 0x07        /**< Repeated '01010101' (in transmission order) sequence. */
} T_GAP_DTM_PACKET_PAYLOAD_TYPE;

typedef enum
{
    GAP_DTM_PHYS_1M       = 0x01,    /**< LE PHY 1M used. */
    GAP_DTM_PHYS_2M       = 0x02,    /**< LE PHY 2M used. */
    GAP_DTM_PHYS_CODED_S8 = 0x03,    /**< LE Coded PHY with S=8 data coding. */
    GAP_DTM_PHYS_CODED_S2 = 0x04,    /**< LE Coded PHY with S=2 data coding. */
} T_GAP_DTM_PHYS_TYPE;

/** End of GAP_LE_DTM_Exported_Types
 * @}
 */

/*============================================================================*
 *                         Functions
 *============================================================================*/

/** @defgroup GAP_LE_DTM_Exported_Functions GAP LE Direct Test Mode Exported Functions
  * @brief GAP LE Direct Test Mode Exported Functions
  * @{
  */

/**
* @brief   Start a test where the DUT receives test reference packets at a fixed interval.
            The tester generates the test reference packets.
*
* If sending request operation is successful, the starting result will be returned by callback
* registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_DTM_RECEIVER_TEST.
*
* @param[in]   rx_chann   Channel to receive packets.
*                         - N = (F-2402) / 2.
*                         - Range: 0x00 to 0x27.
*                         - Frequency Range: 2402 MHz to 2480 MHz.
*
* @return The result of sending request.
* @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
* @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void dtm_test_req(uint16_t command)
    {
        rx_chann = (command & 0x3f00) >> 8;
        le_dtm_receiver_test_req(rx_chann);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        uint16_t status  = 0;
        uint16_t event = 0;
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_DTM_RECEIVER_TEST:
           status = p_data->p_le_receive_test_rsp->cause;
           if (status == 0)
           {
               APP_PRINT_INFO2("dtm_event_trace: ev 0x%x, st 0x%x", (event & 0x8000) >> 15, event & 0x1);
           }
           else
           {
               event |= 1;
               APP_PRINT_INFO2("dtm_evt_trace: EV 0x%x, st 0x%x", (event & 0x8000) >> 15, event & 0x1);
           }
           dtm_uart_send_bytes(event);
           break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_dtm_receiver_test(uint8_t rx_chann);

/**
* @brief   Start a test where the DUT generates test reference packets at a fixed interval.
            The Controller shall transmit at maximum power.
*
* If sending request operation is successful, the starting result will be returned by callback
* registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_DTM_TRANSMITTER_TEST.
*
* @param[in]   tx_chann   Channel to transmit packets.
*                         - N = (F-2402) / 2.
*                         - Range: 0x00 to 0x27.
*                         - Frequency Range: 2402 MHz to 2480 MHz.
* @param[in]   data_len   Length in octets of payload data in each packet.
*                         - Range: 0x00 to 0xFF.
* @param[in]   pkt_pl    Contents of the payload of the test reference packets.
*                        Value is @ref T_GAP_DTM_PACKET_PAYLOAD_TYPE.
*
* @return The result of sending request.
* @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
* @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void dtm_test_req(uint16_t command)
    {
        tx_chann = (command & 0x3f00) >> 8;
        data_len = up_2_bits << 6 | (command & 0xfc) >> 2;
        pkt_pl = command & 0x03;
        le_dtm_transmitter_test(tx_chann, data_len, pkt_pl);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        uint16_t status  = 0;
        uint16_t event = 0;
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_DTM_TRANSMITTER_TEST:
        status = p_data->le_cause.cause;
           if (status == 0)
           {
               APP_PRINT_INFO2("dtm_evt_trace: ev 0x%x, st 0x%x", (event & 0x8000) >> 15, event & 0x1);
           }
           else
           {
               event |= 1;
               APP_PRINT_INFO2("dtm_evt_trace: st 0x%x, st 0x%x", (event & 0x8000) >> 15, event & 0x1);
           }
           dtm_uart_send_bytes(event);
           break;
        }
        ...
    }
  * \endcode
  */

T_GAP_CAUSE le_dtm_transmitter_test(uint8_t tx_chann, uint8_t data_len, uint8_t pkt_pl);

/**
* @brief   Stop any test which is in progress.
*
* If sending request operation is successful, the stopping result will be returned by callback
* registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_DTM_TEST_END.
*
* @return The result of sending request.
* @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
* @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void dtm_test_req(uint16_t command)
    {
        ...
        le_dtm_test_end();
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        uint16_t status  = 0;
        uint16_t event = 0;
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_DTM_TEST_END:
             status = p_data->p_le_dtm_test_end_rsp->cause;
             if (status == 0)
             {
                 event |= 1 << 15;
                 event |= p_data->p_le_dtm_test_end_rsp->num_pkts;
                 APP_PRINT_INFO2("dtm_evt_trace: ev 0x%x, packet count 0x%x", (event & 0x8000) >> 15,
                                 event & 0x7fff);
             }
             else
             {
                 event |= 1;
                 APP_PRINT_INFO2("dtm_evt_trace: ev 0x%x, st 0x%x", (event & 0x8000) >> 15, event & 0x1);
             }
             dtm_uart_send_bytes(event);
             break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_dtm_test_end(void);

/**
* @brief   Start a test where the DUT receives test reference packets at a fixed interval.
*
* If sending request operation is successful, the starting result will be returned by callback
* registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_DTM_ENHANCED_RECEIVER_TEST.
*
* @param[in]   rx_chann   Channel to receive packets.
*                         - N = (F-2402) / 2.
*                         - Range: 0x00 to 0x27.
*                         - Frequency Range: 2402 MHz to 2480 MHz.
* @param[in]   phy   Physical to receive packets. Value is @ref T_GAP_PHYS_TYPE.
* @param[in]   mod_idx   Modulation index to receive packets.
*                        Value is @ref T_GAP_DTM_MODULATION_INDEX_TYPE.
*
* @return The result of sending request.
* @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
* @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void dtm_test_req(uint16_t command)
    {
        static uint8_t phy = 1;
        static uint8_t mod_idx = 0;
        rx_chann = (command & 0x3f00) >> 8;

        if (le_dtm_enhanced_receiver_test(rx_chann, phy, mod_idx) != GAP_CAUSE_SUCCESS)
        {
            APP_PRINT_ERROR0("dtm_test_req: le_dtm_enhanced_receiver_test fail");
        }
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        uint16_t status  = 0;
        uint16_t event = 0;
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_DTM_ENHANCED_RECEIVER_TEST:
            status = p_data->le_cause.cause;
            if (status == 0)
            {
                APP_PRINT_INFO2("app_gap_callback: event 0x%x, status 0x%x", (event & 0x8000) >> 15, event & 0x1);
            }
            else
            {
                event |= 1;
                APP_PRINT_INFO2("app_gap_callback: event 0x%x, status 0x%x", (event & 0x8000) >> 15, event & 0x1);
            }
            dtm_uart_send_bytes(event);
            break;
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_dtm_enhanced_receiver_test(uint8_t rx_chann, uint8_t phy, uint8_t mod_idx);

/**
* @brief   Start a test where the DUT generates test reference packets at a fixed interval.
*
* If sending request operation is successful, the starting result will be returned by callback
* registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_DTM_ENHANCED_TRANSMITTER_TEST.
*
* @param[in]   tx_chann   Channel to transmit packets.
*                         - N = (F-2402) / 2.
*                         - Range: 0x00 to 0x27.
*                         - Frequency Range: 2402 MHz to 2480 MHz.
* @param[in]   data_len   Length in octets of payload data in each packet.
*                         - Range: 0x00 to 0xFF.
* @param[in]   pkt_pl    Contents of the payload of the test reference packets.
*                        Value is @ref T_GAP_DTM_PACKET_PAYLOAD_TYPE.
* @param[in]   phy   Physical to transmit packets. Value is @ref T_GAP_DTM_PHYS_TYPE.
*
* @return The result of sending request.
* @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
* @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void dtm_test_req(uint16_t command)
    {
        static uint8_t phy = 1;
        tx_chann = (command & 0x3f00) >> 8;
        data_len = up_2_bits << 6 | (command & 0xfc) >> 2;
        pkt_pl = command & 0x03;

        if (le_dtm_enhanced_transmitter_test(tx_chann, data_len, pkt_pl, phy) != GAP_CAUSE_SUCCESS)
        {
           APP_PRINT_ERROR0("dtm_test_req: le_dtm_enhanced_transmitter_test fail");
        }
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        uint16_t status  = 0;
        uint16_t event = 0;
        switch (cb_type)
        {
         ...
        case GAP_MSG_LE_DTM_ENHANCED_TRANSMITTER_TEST:
            status = p_data->le_cause.cause;
            if (status == 0)
            {
                APP_PRINT_INFO2("app_gap_callback: event 0x%x, status 0x%x", (event & 0x8000) >> 15, event & 0x1);
            }
            else
            {
                event |= 1;
                APP_PRINT_INFO2("app_gap_callback: event 0x%x, status 0x%x", (event & 0x8000) >> 15, event & 0x1);
            }
            dtm_uart_send_bytes(event);
            break;

         ...
    }
  * \endcode
  */

T_GAP_CAUSE le_dtm_enhanced_transmitter_test(uint8_t tx_chann, uint8_t data_len, uint8_t pkt_pl,
                                             uint8_t phy);

/**
* @brief   Start a test where the DUT receives test reference packets at a fixed interval.
*
* If sending request operation is successful, the starting result will be returned by callback
* registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_DTM_RECEIVER_TEST_V3.
*
* @param[in]   rx_channel   Channel to receive packets.
*                           - N = (F-2402) / 2.
*                           - Range: 0x00 to 0x27.
*                           - Frequency Range: 2402 MHz to 2480 MHz.
* @param[in]   phy   Physical to receive packets. Value is @ref T_GAP_PHYS_TYPE.
* @param[in]   modulation_index   Modulation index to receive packets. Value is @ref T_GAP_DTM_MODULATION_INDEX_TYPE.
* @param[in]   expected_cte_length   The expected length of the Constant Tone Extensions in received
                                     test reference packets.
                                     - Range: 0x00 (No Constant Tone Extension expected (default)) or 0x02 to 0x14.
                                     - Units: 8 us.
* @param[in]   expected_cte_type   The expected type of the Constant Tone Extensions in received
                                   test reference packets. Value is @ref T_GAP_CTE_TYPE.
* @param[in]   slot_durations   Modulation index to receive packets and shall be ignored when expected_cte_type
                                is not set to @ref GAP_CTE_TYPE_AOA. Value is @ref T_GAP_SLOT_DUATIONS_TYPE.
* @param[in]   switching_pattern_length   The number of Antenna IDs in the pattern and shall be ignored when
                                          expected_cte_type is not set to @ref GAP_CTE_TYPE_AOA.
                                          - Range: 0x02 to max_switching_pattern_length supported by Controller
                                                 max_switching_pattern_length shall be less than or equal to 0x4B.
* @param[in]   p_antenna_ids   Pointer to antenna ID in the pattern and shall be ignored when expected_cte_type
                               is not set to @ref GAP_CTE_TYPE_AOA.
*
* @return The result of sending request.
* @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
* @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void dtm_test_req(uint16_t command)
    {
        uint8_t rx_channel = 0;
        T_GAP_PHYS_TYPE phy = GAP_PHYS_1M;
        T_GAP_DTM_MODULATION_INDEX_TYPE modulation_index = GAP_DTM_MODULATION_INDEX_STANDARD;
        uint8_t expected_cte_length = 2;
        T_GAP_CTE_TYPE expected_cte_type = GAP_CTE_TYPE_AOA;
        T_GAP_SLOT_DUATIONS_TYPE slot_durations = GAP_SLOT_DURATIONS_SWITCH_SAMPLE_1US;
        uint8_t switching_pattern_length = 2;
        uint8_t p_antenna_ids[2] = {0, 1};

        le_dtm_receiver_test_v3(rx_channel, phy, modulation_index, expected_cte_length,
                                expected_cte_type, slot_durations,
                                switching_pattern_length, p_antenna_ids);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        uint16_t status  = 0;
        uint16_t event = 0;
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_DTM_RECEIVER_TEST_V3:
            APP_PRINT_INFO1("GAP_MSG_LE_DTM_RECEIVER_TEST_V3: cause 0x%x",
                            p_data->le_cause.cause);
            break;
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_dtm_receiver_test_v3(uint8_t rx_channel, T_GAP_PHYS_TYPE phy,
                                    T_GAP_DTM_MODULATION_INDEX_TYPE modulation_index,
                                    uint8_t expected_cte_length, T_GAP_CTE_TYPE expected_cte_type,
                                    T_GAP_SLOT_DUATIONS_TYPE slot_durations,
                                    uint8_t switching_pattern_length, uint8_t *p_antenna_ids);

/**
* @brief   Start a test where the DUT receives test reference packets at a fixed interval.
*
* If sending request operation is successful, the starting result will be returned by callback
* registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_DTM_TRANSMITTER_TEST_V3.
*
* @param[in]   tx_channel   Channel to transmit packets.
                            - N = (F-2402) / 2.
                            - Range: 0x00 to 0x27.
                            - Frequency Range: 2402 MHz to 2480 MHz.
* @param[in]   test_data_length   Length in octets of payload data in each packet.
                                  - Range: 0x00 to 0xFF.
* @param[in]   packet_payload   Contents of the payload of the test reference packets.
*                               Value is @ref T_GAP_DTM_PACKET_PAYLOAD_TYPE.
* @param[in]   phy   Physical to transmit packets. Value is @ref T_GAP_DTM_PHYS_TYPE.
* @param[in]   cte_length   The length of the Constant Tone Extension in the test reference packets.
                            - Range: 0x00 (Do not transmit a Constant Tone Extension) or 0x02 to 0x14.
                            - Units: 8 us.
* @param[in]   cte_type   The type of the Constant Tone Extension in the test reference packets.
                          @ref T_GAP_CTE_TYPE.
* @param[in]   switching_pattern_length   The number of Antenna IDs in the pattern and shall be ignored when
                                          cte_type is set to @ref GAP_CTE_TYPE_AOA.
                                          - Range: 0x02 to max_switching_pattern_length supported by Controller
                                                 max_switching_pattern_length shall be less than or equal to 0x4B.
* @param[in]   p_antenna_ids   Pointer to antenna ID in the pattern and shall be ignored when expected_cte_type
*                              is set to @ref GAP_CTE_TYPE_AOA.
*
* @return The result of sending request.
* @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
* @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void dtm_test_req(uint16_t command)
    {
        uint8_t tx_channel = 0;
        uint8_t test_data_length = 2;
        T_GAP_DTM_PACKET_PAYLOAD_TYPE packet_payload = GAP_DTM_PACKET_PAYLOAD_PRBS9;
        T_GAP_DTM_PHYS_TYPE phy = GAP_DTM_PHYS_1M;
        uint8_t cte_length = 2;
        T_GAP_CTE_TYPE cte_type = GAP_CTE_TYPE_AOD_2US_SLOT;
        uint8_t switching_pattern_length = 2;
        uint8_t p_antenna_ids[2] = {0, 1};

        le_dtm_transmitter_test_v3(tx_channel, test_data_length,
                                   packet_payload, phy, cte_length, cte_type,
                                   switching_pattern_length, p_antenna_ids);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        uint16_t status  = 0;
        uint16_t event = 0;
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_DTM_TRANSMITTER_TEST_V3:
            APP_PRINT_INFO1("GAP_MSG_LE_DTM_TRANSMITTER_TEST_V3: cause 0x%x",
                            p_data->le_cause.cause);
            break;
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_dtm_transmitter_test_v3(uint8_t tx_channel, uint8_t test_data_length,
                                       T_GAP_DTM_PACKET_PAYLOAD_TYPE packet_payload, T_GAP_DTM_PHYS_TYPE phy,
                                       uint8_t cte_length, T_GAP_CTE_TYPE cte_type,
                                       uint8_t switching_pattern_length, uint8_t *p_antenna_ids);

/**
* @brief   Start a test where the DUT transmits test reference packets at a fixed interval.
*
* If sending request operation is successful, the starting result will be returned by callback
* registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_DTM_TRANSMITTER_TEST_V4.
*
* @param[in]   tx_channel   Channel to transmit packets.
                            - N = (F-2402) / 2.
                            - Range: 0x00 to 0x27.
                            - Frequency Range: 2402 MHz to 2480 MHz.
* @param[in]   test_data_length   Length in octets of payload data in each packet.
                                  - Range: 0x00 to 0xFF.
* @param[in]   packet_payload   Contents of the payload of the test reference packets.
*                               Value is @ref T_GAP_DTM_PACKET_PAYLOAD_TYPE.
* @param[in]   phy   Physical to transmit packets. Value is @ref T_GAP_DTM_PHYS_TYPE.
* @param[in]   cte_length   The length of the Constant Tone Extension in the test reference packets.
                            - 0x00: Do not transmit a Constant Tone Extension.
                            - 0x02 to 0x14: Length of the Constant Tone Extension in 8 us units.
* @param[in]   cte_type   The type of the Constant Tone Extension in the test reference packets.
                          @ref T_GAP_CTE_TYPE.
* @param[in]   switching_pattern_length   The number of Antenna IDs in the pattern and shall be ignored when
                                          cte_type is set to @ref GAP_CTE_TYPE_AOA.
                                          - Range: 0x02 to max_switching_pattern_length supported by Controller
                                                   max_switching_pattern_length shall be less than or equal to 0x4B.
* @param[in]   p_antenna_ids   Pointer to antenna ID in the pattern and shall be ignored when expected_cte_type
*                              is set to @ref GAP_CTE_TYPE_AOA.
* @param[in]   tx_power_level   Set transmitter to the specified or the nearest transmit power level.
                                - Range: -127 to +20.
                                - Units: dBm.
                                - 0x7E: Set transmitter to minimum transmit power level.
                                - 0x7F: Set transmitter to maximum transmit power level.
*
* @return The result of sending request.
* @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
* @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void dtm_test_req(uint16_t command)
    {
        uint8_t tx_channel = 0;
        uint8_t test_data_length = 2;
        T_GAP_DTM_PACKET_PAYLOAD_TYPE packet_payload = GAP_DTM_PACKET_PAYLOAD_PRBS9;
        T_GAP_DTM_PHYS_TYPE phy = GAP_DTM_PHYS_1M;
        uint8_t cte_length = 2;
        T_GAP_CTE_TYPE cte_type = GAP_CTE_TYPE_AOD_2US_SLOT;
        uint8_t switching_pattern_length = 2;
        uint8_t p_antenna_ids[2] = {0, 1};
        int8_t tx_power_level = 0x10;

        le_dtm_transmitter_test_v4(tx_channel, test_data_length, packet_payload, phy,
                                   cte_length, cte_type, switching_pattern_length,
                                   *p_antenna_ids, tx_power_level);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        uint16_t status  = 0;
        uint16_t event = 0;
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_DTM_TRANSMITTER_TEST_V4:
            APP_PRINT_INFO1("GAP_MSG_LE_DTM_TRANSMITTER_TEST_V4: cause 0x%x",
                            p_data->le_cause.cause);
            break;
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_dtm_transmitter_test_v4(uint8_t tx_channel, uint8_t test_data_length,
                                       T_GAP_DTM_PACKET_PAYLOAD_TYPE packet_payload, T_GAP_DTM_PHYS_TYPE phy,
                                       uint8_t cte_length, T_GAP_CTE_TYPE cte_type,
                                       uint8_t switching_pattern_length, uint8_t *p_antenna_ids, int8_t tx_power_level);

/** End of GAP_LE_DTM_Exported_Functions
  * @}
  */

/** End of GAP_LE_DTM
  * @}
  */

#endif /* GAP_DTM_H */
