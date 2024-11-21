/**
*********************************************************************************************************
*               Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl_codec.h
* \brief    The header file of the peripheral CODEC driver.
* \details  This file provides all CODEC firmware functions.
* \author   echo gao
* \date     2023-10-17
* \version  v1.0
* *******************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef RTL_CODEC_H
#define RTL_CODEC_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "utils/rtl_utils.h"
#if defined (CONFIG_SOC_SERIES_RTL87X2G)
#include "codec/src/rtl87x2g/rtl_codec_def.h"
#endif

/** \defgroup CODEC       CODEC
  * \brief
  * \{
  */

/*============================================================================*
 *                         Constants
 *============================================================================*/
/** \defgroup CODEC_Exported_Constants CODEC Exported Constants
  * \brief
  * \{
  */

/**
 * \defgroup    I2S_Rx_Data_Width I2S Rx Data Width
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    CODEC_I2S_Rx_DataWidth_16Bits,
    CODEC_I2S_Rx_DataWidth_8Bits = 0x3,
    CODEC_I2S_Rx_DataWidth_MAX,
} CodecI2SRxDatalenSel_TypeDef;

/** End of I2S_Rx_Data_Width
  * \}
  */

/**
 * \defgroup    I2S_Tx_Data_Width I2S Tx Data Width
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    CODEC_I2S_Tx_DataWidth_16Bits,
    CODEC_I2S_Tx_DataWidth_24Bits = 0x2,
    CODEC_I2S_Tx_DataWidth_8Bits = 0x3,
    CODEC_I2S_Tx_DataWidth_MAX,
} CodecI2STxDatalenSel_TypeDef;

/** End of I2S_Tx_Data_Width
  * \}
  */

/**
 * \defgroup    I2S_Channel_Lenth I2S Channel Lenth
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    I2S_CHANNELLEN_16,
    I2S_CHANNELLEN_32,
    I2S_CHANNELLEN_24,
    I2S_CHANNELLEN_8,
    I2S_CHANNELLEN_MAX,
} CodecI2SChannellenSel_TypeDef;

/** End of I2S_Channel_Lenth
  * \}
  */

/**
 * \defgroup    I2S_Data_Format I2S Data Format
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    CODEC_I2S_DataFormat_I2S,
    CODEC_I2S_DataFormat_LeftJustified,
    CODEC_I2S_DataFormat_PCM_A,
    CODEC_I2S_DataFormat_PCM_B,
    CODEC_I2S_DataFormat_MAX,
} CodecI2SFormatSel_TypeDef;

#define IS_CODEC_I2S_DATA_FORMAT(FORMAT) (((FORMAT) == CODEC_I2S_DataFormat_I2S) || \
                                          ((FORMAT) == CODEC_I2S_DataFormat_LeftJustified) || \
                                          ((FORMAT) == CODEC_I2S_DataFormat_PCM_A) || \
                                          ((FORMAT) == CODEC_I2S_DataFormat_PCM_B))

/** End of I2S_Data_Format
  * \}
  */

/**
 * \defgroup    CODEC_DAC_Dither CODEC DAC Dither
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    DAC_DA_DITHER_DISABLE = 0,
    DAC_DA_DITHER_LSB = 1,
    DAC_DA_DITHER_LSB_PLUS_1 = 2,
    DAC_DA_DITHER_LSB_PLUS_2 = 3,
} CodecDacDADitherSel_TypeDef;

/** End of CODEC_DAC_Dither
  * \}
  */

/**
 * \defgroup    CODEC_I2S_CH CODEC I2S CH
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    CODEC_I2S_CH_L_R,
    CODEC_I2S_CH_R_L,
    CODEC_I2S_CH_L_L,
    CODEC_I2S_CH_R_R,
    CODEC_I2S_CH_SEL_MAX,
} CodecI2SCHSequenceSel_TypeDef;

#define IS_CODEC_I2S_CH(CH)             (((CH) == CODEC_I2S_CH_L_R) || \
                                         ((CH) == CODEC_I2S_CH_R_L) || \
                                         ((CH) == CODEC_I2S_CH_L_L) || \
                                         ((CH) == CODEC_I2S_CH_R_R))

/** End of CODEC_I2S_CH
  * \}
  */

/**
 * \defgroup    Sample_Rate_Definition Sample Rate Definition
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    SAMPLE_RATE_48KHz,      // 0: 48KHz
    SAMPLE_RATE_96KHz,      // 1: 96KHz
    SAMPLE_RATE_192KHz,     // 2: 192KHz
    SAMPLE_RATE_32KHz,      // 3: 32KHz
    SAMPLE_RATE_176KHz,     // 4: 176.4KHz
    SAMPLE_RATE_16KHz,      // 5: 16KHz
    SAMPLE_RATE_RSV_1,
    SAMPLE_RATE_8KHz,       // 7: 8KHz
    SAMPLE_RATE_44100Hz,    // 8: 44.1KHz
    SAMPLE_RATE_88200Hz,    // 9: 88.2KHz
    SAMPLE_RATE_24KHz,      // 10: 24KHz
    SAMPLE_RATE_12KHz,      // 11: 12KHz
    SAMPLE_RATE_22050Hz,   // 12: 22.05KHz
    SAMPLE_RATE_11025Hz,  // 13: 11.025KHz
    SAMPLE_RATE_NUM,
} CodecSampleRate_TypeDef;

#define IS_SAMPLE_RATE(RATE) (((RATE) == SAMPLE_RATE_48KHz)   || ((RATE) == SAMPLE_RATE_96KHz)   || \
                              ((RATE) == SAMPLE_RATE_192KHz)   || ((RATE) == SAMPLE_RATE_176KHz)   || \
                              ((RATE) == SAMPLE_RATE_32KHz)   || ((RATE) == SAMPLE_RATE_16KHz)   || \
                              ((RATE) == SAMPLE_RATE_8KHz)    || ((RATE) == SAMPLE_RATE_44100Hz) || \
                              ((RATE) == SAMPLE_RATE_88200Hz) || ((RATE) == SAMPLE_RATE_24KHz)   || \
                              ((RATE) == SAMPLE_RATE_12KHz)   || ((RATE) == SAMPLE_RATE_22050Hz) || \
                              ((RATE) == SAMPLE_RATE_11025Hz))

/** End of Sample_Rate_Definition
  * \}
  */

/**
 * \defgroup    CODEC_Sample_Rate_SRC CODEC Sample Rate SRC
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    SAMPLE_RATE_SRC0 = 0,
    SAMPLE_RATE_SRC1 = 1,
    SAMPLE_RATE_SRC2 = 2,
    SAMPLE_RATE_SRC3_DA = 3,
    SAMPLE_RATE_SRC_MAX = 4,
} CodecSampleRateSrc_TypeDef;

/** End of CODEC_Sample_Rate_SRC
  * \}
  */

/**
 * \defgroup    DMIC_Clock_Type_Definition DMIC Clock Type Definition
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    DMIC_Clock_5MHz,
    DMIC_Clock_2500KHz,
    DMIC_Clock_1250KHz,
    DMIC_Clock_625KHz,
    DMIC_Clock_312500Hz,
    DMIC_CLK_SEL_MAX,
} CodecDmicClkSel_TypeDef;

#define IS_CODEC_DMIC_CLOCK(CLOCK) (((CLOCK) == DMIC_Clock_5MHz) || ((CLOCK) == DMIC_Clock_2500KHz) || \
                                    ((CLOCK) == DMIC_Clock_1250KHz) || ((CLOCK) == DMIC_Clock_625KHz) || \
                                    ((CLOCK) == DMIC_Clock_312500Hz))

/** End of DMIC_Clock_Type_Definition
  * \}
  */

/**
 * \defgroup    CODEC_MIC_BIAS_Config CODEC MIC BIAS Config
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    MICBIAS_VOLTAGE_1_507,
    MICBIAS_VOLTAGE_1_62,
    MICBIAS_VOLTAGE_1_705,
    MICBIAS_VOLTAGE_1_8,
    MICBIAS_VOLTAGE_1_906,
    MICBIAS_VOLTAGE_2_025,
    MICBIAS_VOLTAGE_2_16,
    MICBIAS_VOLTAGE_2_314,
} MicbiasVoltageSel_TypeDef;

#define IS_MICBIAS_CONFIG(CONFIG) (((CONFIG) == MICBIAS_VOLTAGE_1_507) || ((CONFIG) == MICBIAS_VOLTAGE_1_62) || \
                                   ((CONFIG) == MICBIAS_VOLTAGE_1_705) || ((CONFIG) == MICBIAS_VOLTAGE_1_8) || \
                                   ((CONFIG) == MICBIAS_VOLTAGE_1_906) || ((CONFIG) == MICBIAS_VOLTAGE_2_025) || \
                                   ((CONFIG) == MICBIAS_VOLTAGE_2_16) || ((CONFIG) == MICBIAS_VOLTAGE_2_314))

/** End of CODEC_MIC_BIAS_Config
  * \}
  */

/**
 * \defgroup    MICBST_Gain MICBST Gain
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    MICBST_Gain_0dB,
    MICBST_Gain_20dB,
    MICBST_Gain_30dB,
    MICBST_Gain_40dB,
} MicbiasGainSel_TypeDef;

#define IS_MICBST_GAIN(GAIN)            (((GAIN) == MICBST_Gain_0dB) || \
                                         ((GAIN) == MICBST_Gain_20dB) || \
                                         ((GAIN) == MICBST_Gain_30dB) || \
                                         ((GAIN) == MICBST_Gain_40dB))

/** End of MICBST_Gain
  * \}
  */

/**
 * \defgroup    MICBST_Mode MICBST Mode
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    MICBST_Mode_Single,
    MICBST_Mode_Differential,
} MicbiasModeSel_TypeDef;

#define IS_MICBST_MODE(MODE)            (((MODE) == MICBST_Mode_Single) || \
                                         ((MODE) == MICBST_Mode_Differential))

/** End of MICBST_Mode
  * \}
  */

/**
 * \defgroup    CODEC_Mute_Type CODEC Mute Type
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    CODEC_UNMUTE,
    CODEC_MUTE,
} CodecMuteSel_TypeDef;

#define IS_CODEC_MIC_MUTE(MUTE) (((MUTE) == CODEC_MUTE) || ((MUTE) == CODEC_UNMUTE))

/** End of CODEC_Mute_Type
  * \}
  */

/**
 * \defgroup    CODEC_Ch_Mic_Type
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    CODEC_CH_DMIC,
    CODEC_CH_AMIC,
} CodecCHMicSel_TypeDef;

#define IS_CODEC_CH_MIC_TYPE(TYPE) (((TYPE) == CODEC_CH_AMIC) || ((TYPE) == CODEC_CH_DMIC))

/** End of CODEC_Ch_Mic_Type
  * \}
  */

/**
 * \defgroup    DMIC_Data_Latch_Edge DMIC Data Latch Edge
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    DMIC_Ch_Rising_Latch,
    DMIC_Ch_Falling_Latch,
} CdecCHLatchEdgeSel_TypeDef;

#define IS_DMIC_CH_LATCH_EDGE(EDGE) (((EDGE) == DMIC_Ch_Rising_Latch) || ((EDGE) == DMIC_Ch_Falling_Latch))

/** End of DMIC_Data_Latch_Edge
  * \}
  */

/**
 * \defgroup    Ch_ADC_Zero_Detection_Timeout Ch ADC Zero Detection Timeout
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
typedef enum
{
    Ch_ADC_Zero_DetTimeout_1024_16_Sample,
    Ch_ADC_Zero_DetTimeout_1024_32_Sample,
    Ch_ADC_Zero_DetTimeout_1024_64_Sample,
    Ch_ADC_Zero_DetTimeout_64_Sample,
} CodecCHZDTSel_TypeDef;

#define IS_CH_ADC_ZERO_DET_TIMEOUT(TIMEOUT) (((TIMEOUT) == Ch_ADC_Zero_DetTimeout_1024_16_Sample) || \
                                             ((TIMEOUT) == Ch_ADC_Zero_DetTimeout_1024_32_Sample) || \
                                             ((TIMEOUT) == Ch_ADC_Zero_DetTimeout_1024_64_Sample) || \
                                             ((TIMEOUT) == Ch_ADC_Zero_DetTimeout_64_Sample))

/** End of Ch_ADC_Zero_Detection_Timeout
  * \}
  */

/**
 * \defgroup    Codec_Ad_Gain Codec Ad Gain
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define IS_AD_GAIN(GAIN)            (((GAIN)<= 0x7F))

/** End of Codec_Ad_Gain
  * \}
  */

/** End of CODEC_Exported_Constants
  * \}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** \defgroup CODEC_Exported_Types CODEC Exported Types
  * \brief
  * \{
  */

/**
 * \brief       CODEC initialize parameters.
 *
 * \ingroup     CODEC_Exported_Types
 */
typedef struct
{
    /* Basic parameters section */
    CodecSampleRateSrc_TypeDef CODEC_AdSampleRateSrc; /*!< Specifies the ADC path sample rate source. */
    CodecSampleRateSrc_TypeDef CODEC_DaSampleRateSrc; /*!< Specifies the DAC path sample rate source. */
    CodecSampleRate_TypeDef CODEC_SampleRate0; /*!< Specifies the sample rate 0. */
    CodecSampleRate_TypeDef CODEC_SampleRate1; /*!< Specifies the sample rate 1. */
    CodecDmicClkSel_TypeDef CODEC_DmicClock; /*!< Specifies the dmic clock. */

    CodecI2SFormatSel_TypeDef CODEC_I2SFormat;  /*!< Specifies the I2S Tx/Rx format of codec port. */
    CodecI2STxDatalenSel_TypeDef
    CODEC_I2STxDataWidth;   /*!< Specifies the I2S Tx data width of codec port. */
    CodecI2SRxDatalenSel_TypeDef
    CODEC_I2SRxDataWidth;   /*!< Specifies the I2S Rx data width of codec port. */

    CodecI2SChannellenSel_TypeDef
    CODEC_I2SChannelLen; /*!< Specifies the I2S Tx/Rx channel width of codec port. */
    CodecI2SCHSequenceSel_TypeDef CODEC_I2SChSequence; /*!< Specifies the I2S Tx/Rx channel sequence. */
    uint8_t CODEC_I2SSameLrcEn;

    MicbiasVoltageSel_TypeDef CODEC_MicBIAS; /*!< Specifies the MICBIAS voltage. */
    MicbiasGainSel_TypeDef CODEC_MicBstGain; /*!< Specifies the MICBST gain. */
    MicbiasModeSel_TypeDef CODEC_MicBstMode; /*!< Specifies the MICBST mode. */

    /* MIC channel 0 initialization parameters section */
    CodecMuteSel_TypeDef CODEC_Ch0Mute; /*!< Specifies the channel 0 dmic mute status. */
    CodecCHMicSel_TypeDef
    CODEC_Ch0MicType; /*!< Specifies the channel 0 mic type, which can be dmic or amic. */
    CdecCHLatchEdgeSel_TypeDef
    CODEC_Ch0DmicDataLatch; /*!< Specifies the channel 0 dmic data latch type. */
    uint32_t CODEC_Ch0AdGain;           /*!< Specifies the channel 0 ADC digital volume. */
    CodecCHZDTSel_TypeDef
    CODEC_Ch0ZeroDetTimeout;  /*!< Specifies the channel 0 zero detection timeout mode control. */

    /* MIC channel 1 initialization parameters section */
    CodecMuteSel_TypeDef CODEC_Ch1Mute; /*!< Specifies the channel 1 dmic mute status. */
    CodecCHMicSel_TypeDef
    CODEC_Ch1MicType; /*!< Specifies the channel 1 mic type, which can be dmic or amic. */
    CdecCHLatchEdgeSel_TypeDef
    CODEC_Ch1DmicDataLatch; /*!< Specifies the channel 1 dmic data latch type. */
    uint32_t CODEC_Ch1AdGain; /*!< Specifies the channel 1 ADC digital volume. */
    CodecCHZDTSel_TypeDef
    CODEC_Ch1ZeroDetTimeout;  /*!< Specifies the channel 0 zero detection timeout mode control. */

    /* PDM initialization parameters section */
    CodecMuteSel_TypeDef CODEC_DaMute; /*!< Specifies the DAC mute status. */
    uint32_t CODEC_DaGain; /*!< Specifies the DAC gain control. */
    CodecDacDADitherSel_TypeDef CODEC_DaC_Dither;
} CODEC_InitTypeDef;


/**
 * \brief       CODEC EQ part initialize parameters.
 *
 * \ingroup     CODEC_Exported_Types
 */
typedef struct
{
    uint32_t CODEC_EQChCmd;             /*!< Specifies the EQ channel status */
    uint32_t CODEC_EQCoefH0;            /*!< Specifies the EQ coef.h0. This value can be 0 to 0x7FFFF,
                                                 whose physical meaning represents a range of-8 to 7.99 */
    uint32_t CODEC_EQCoefB1;            /*!< Specifies the EQ coef.b1. This value can be 0 to 0x7FFFF,
                                                 whose physical meaning represents a range of-8 to 7.99 */
    uint32_t CODEC_EQCoefB2;            /*!< Specifies the EQ coef.b2. This value can be 0 to 0x7FFFF,
                                                 whose physical meaning represents a range of-8 to 7.99 */
    uint32_t CODEC_EQCoefA1;            /*!< Specifies the EQ coef.a1. This value can be 0 to 0x7FFFF,
                                                 whose physical meaning represents a range of-8 to 7.99 */
    uint32_t CODEC_EQCoefA2;            /*!< Specifies the EQ coef.a2. This value can be 0 to 0x7FFFF,
                                                 whose physical meaning represents a range of-8 to 7.99 */
} CODEC_EQInitTypeDef;

/** End of CODEC_Exported_Types
  * \}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** \defgroup CODEC_Exported_Functions CODEC Exported Functions
  * \brief
  * \{
  */

/**
 * \brief   Deinitializes the CODEC peripheral registers to their default reset values(turn off CODEC clock).
 * \param[in] CODECx: CODEC peripheral selected .
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_codec_init(void)
 * {
 *     CODEC_DeInit(CODEC);
 * }
 * \endcode
 */
void CODEC_DeInit(CODEC_TypeDef *CODECx);
/**
  * \brief    Initialize the CODEC analog registers in AON area.
  * \param  None.
  * \retval None
*
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_codec_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_CODEC, APBPeriph_CODEC_CLOCK, ENABLE);
 *     CODEC_AnalogCircuitInit();
 *     CODEC_InitTypeDef CODEC_InitStruct;

 *     CODEC_StructInit(&CODEC_InitStruct);
 *     CODEC_InitStruct.CODEC_Ch0MicType       = CODEC_CH0_AMIC;
 *     CODEC_InitStruct.CODEC_MicBstMode       = MICBST_Mode_Differential;
 *     CODEC_InitStruct.CODEC_SampleRate       = SAMPLE_RATE_16KHz;
 *     CODEC_InitStruct.CODEC_I2SFormat        = CODEC_I2S_DataFormat_I2S;
 *     CODEC_InitStruct.CODEC_I2SDataWidth     = CODEC_I2S_DataWidth_16Bits;
 *     CODEC_Init(CODEC, &CODEC_InitStruct);
 * }
 * \endcode
 */
void CODEC_AnalogCircuitInit(void);
/**
 * \brief   Initializes the CODEC peripheral according to the specified
 *          parameters in the CODEC_InitStruct.
 * \param[in] CODECx: CODEC peripheral selected.
 * \param[in] CODEC_InitStruct: Pointer to a CODEC_InitTypeDef structure that
 *            contains the configuration information for the specified CODEC peripheral
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_codec_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_CODEC, APBPeriph_CODEC_CLOCK, ENABLE);
 *
 *     CODEC_InitTypeDef CODEC_InitStruct;

 *     CODEC_StructInit(&CODEC_InitStruct);
 *     CODEC_InitStruct.CODEC_Ch0MicType       = CODEC_CH0_AMIC;
 *     CODEC_InitStruct.CODEC_MicBstMode       = MICBST_Mode_Differential;
 *     CODEC_InitStruct.CODEC_SampleRate       = SAMPLE_RATE_16KHz;
 *     CODEC_InitStruct.CODEC_I2SFormat        = CODEC_I2S_DataFormat_I2S;
 *     CODEC_InitStruct.CODEC_I2SDataWidth     = CODEC_I2S_DataWidth_16Bits;
 *     CODEC_Init(CODEC, &CODEC_InitStruct);
 * }
 * \endcode
 */
void CODEC_Init(CODEC_TypeDef *CODECx, CODEC_InitTypeDef *CODEC_InitStruct);

/**
 * \brief   Fills each CODEC_InitStruct member with its default value.
 * \param[in] CODEC_InitStruct: Pointer to a CODEC_InitTypeDef structure which will be initialized.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_codec_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_CODEC, APBPeriph_CODEC_CLOCK, ENABLE);
 *
 *     CODEC_InitTypeDef CODEC_InitStruct;

 *     CODEC_StructInit(&CODEC_InitStruct);
 *     CODEC_InitStruct.CODEC_Ch0MicType       = CODEC_CH0_AMIC;
 *     CODEC_InitStruct.CODEC_MicBstMode       = MICBST_Mode_Differential;
 *     CODEC_InitStruct.CODEC_SampleRate       = SAMPLE_RATE_16KHz;
 *     CODEC_InitStruct.CODEC_I2SFormat        = CODEC_I2S_DataFormat_I2S;
 *     CODEC_InitStruct.CODEC_I2SDataWidth     = CODEC_I2S_DataWidth_16Bits;
 *     CODEC_Init(CODEC, &CODEC_InitStruct);
 * }
 * \endcode
 */
void CODEC_StructInit(CODEC_InitTypeDef *CODEC_InitStruct);

/**
 * \brief   Initializes the CODEC EQ module according to the specified
 *          parameters in the CODEC_EQInitStruct.
 * \param[in] CODEC_EQx: CODEC EQ channel selected.
 * \param[in] CODEC_EQInitStruct: Pointer to a CODEC_EQInitTypeDef structure that
 *            contains the configuration information for the specified CODEC EQ channel.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_codec_eq_init(void)
 * {
 *     CODEC_EQInitTypeDef  CODEC_EQInitStruct;
 *     CODEC_StructInit(&CODEC_EQInitStruct);
 *     CODEC_EQInitStruct. CODEC_EQChCmd    = EQ_CH_Cmd_ENABLE;
 *     CODEC_EQInitStruct.CODEC_EQCoefH0    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefB1    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefB2    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefA1    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefA2    = 0xFF;
 *     CODEC_EQInit(CODEC_EQ1, CODEC_EQInitStruct);
 * }
 * \endcode
 */
void CODEC_EQInit(CODEC_EQTypeDef *CODEC_EQx, CODEC_EQInitTypeDef *CODEC_EQInitStruct);

/**
 * \brief  Fills each CODEC_EQInitStruct member with its default value.
 * \param[in] CODEC_EQInitStruct: Pointer to a CODEC_EQInitTypeDef structure which will be initialized.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_codec_eq_init(void)
 * {
 *     CODEC_EQInitTypeDef  CODEC_EQInitStruct;
 *     CODEC_StructInit(&CODEC_EQInitStruct);
 *     CODEC_EQInitStruct. CODEC_EQChCmd    = EQ_CH_Cmd_ENABLE;
 *     CODEC_EQInitStruct.CODEC_EQCoefH0    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefB1    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefB2    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefA1    = 0xFF;
 *     CODEC_EQInitStruct.CODEC_EQCoefA2    = 0xFF;
 *     CODEC_EQInit(CODEC_EQ1, CODEC_EQInitStruct);
 * }
 * \endcode
 */
void CODEC_EQStructInit(CODEC_EQInitTypeDef *CODEC_EQInitStruct);

/**
 * \brief  Enable or disable mic_bias output.
 * \param[in] CODECx: CODEC peripheral selected.
 * \param[in] NewState: New state of MICBIAS.
 *      This parameter can be: ENABLE or DISABLE.
 * \return none.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void codec_demo(void)
 * {
 *     CODEC_MICBIASCmd(CODEC, ENABLE);
 * }
 * \endcode
 */
void CODEC_MICBIASCmd(CODEC_TypeDef *CODECx, FunctionalState NewState);

/**
 * \brief  Reset CODEC.
 * \param[in] CODECx: CODEC peripheral selected.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void codec_demo(void)
 * {
 *     CODEC_Reset(CODEC);
 * }
 * \endcode
 */
__STATIC_INLINE void CODEC_Reset(CODEC_TypeDef *CODECx)
{
    /* Check the parameters */
    assert_param(IS_CODEC_PERIPH(CODECx));
    AUDIO_CONTROL_0_t codec_0x100 = {.d32 = CODEC->AUDIO_CONTROL_0};

    /* CODEC IP reset */
    codec_0x100.b.audio_ip_en = 0;
    CODEC->AUDIO_CONTROL_0 = codec_0x100.d32;
    codec_0x100.b.audio_ip_en = 1;
    CODEC->AUDIO_CONTROL_0 = codec_0x100.d32;
}

/**
 * \brief   Config MIC BIAS Vref voltage.
 * \param[in] CODECx: CODEC peripheral selected.
 * \param[in] data: New value of MIC BIAS.
 *   This parameter can be one of the following values:
 *     \arg MICBIAS_VOLTAGE_1_507: Vref voltage is 1.507V.
 *     \arg MICBIAS_VOLTAGE_1_62:  Vref voltage is 1.62V.
 *     \arg MICBIAS_VOLTAGE_1_705: Vref voltage is 1.705V.
 *     \arg MICBIAS_VOLTAGE_1_8: Vref voltage is 1.8V.
 *     \arg MICBIAS_VOLTAGE_1_906: Vref voltage is 1.906V.
 *     \arg MICBIAS_VOLTAGE_2_025:  Vref voltage is 2.025V.
 *     \arg MICBIAS_VOLTAGE_2_16: Vref voltage is 2.16V.
 *     \arg MICBIAS_VOLTAGE_2_314: Vref voltage is 2.314V.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void codec_demo(void)
 * {
 *     CODEC_SetMICBIAS(CODEC, MICBIAS_VOLTAGE_1_507);
 * }
 * \endcode
 */
__STATIC_INLINE void CODEC_SetMICBIAS(CODEC_TypeDef *CODECx,  uint16_t data)

{
    /* Check the parameters */
    assert_param(IS_CODEC_PERIPH(CODECx));
    assert_param(IS_MICBIAS_CONFIG(data));
    CODEC_ANA_01_BUS_t codec_0x004 = {.d32 = CODEC_ANA->CODEC_ANA_01_BUS};

    codec_0x004.b.MICBIAS_VSET =  data;

    CODEC_ANA->CODEC_ANA_01_BUS = codec_0x004.d32;
}

/** End of CODEC_Exported_Functions
  * \}
  */

/** End of CODEC
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif /* RTL_CODEC_H */
