/**
*********************************************************************************************************
*               Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl_lpc.h
* \brief    The header file of the peripheral LPC driver.
* \details  This file provides all LPC firmware functions.
* \author   echo
* \date     2023-10-17
* \version  v1.0
* *******************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _RTL_LPC_H_
#define _RTL_LPC_H_

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "utils/rtl_utils.h"
#if defined (CONFIG_SOC_SERIES_RTL87X2G)
#include "lpc/src/rtl87x2g/rtl_lpc_def.h"
#elif defined (CONFIG_SOC_SERIES_RTL8762J)
#include "lpc/src/rtl87x2j/rtl_lpc_def.h"
#endif


/** \defgroup LPC         LPC
  * \brief
  * \{
  */

/*============================================================================*
 *                         Constants
 *============================================================================*/
/** \defgroup LPC_Exported_Constants LPC Exported Constants
  * \brief
  * \{
  */

/**
 * \defgroup    LPC_Channel LPC Channel
 * \{
 * \ingroup     LPC_Exported_Constants
 */
typedef enum
{
    LPC_CHANNEL_ADC0,
    LPC_CHANNEL_ADC1,
    LPC_CHANNEL_ADC2,
    LPC_CHANNEL_ADC3,
    LPC_CHANNEL_ADC4,
    LPC_CHANNEL_ADC5,
    LPC_CHANNEL_ADC6,
    LPC_CHANNEL_ADC7,
    LPC_CHANNEL_VBAT,
    LPC_CHANNEL_MAX,
} LPCChannelSel_TypeDef;

#define IS_LPC_CHANNEL(CHANNEL) (((CHANNEL) < LPC_CHANNEL_MAX))

/** End of LPC_Channel
  * \}
  */

/**
 * \defgroup    LPC_Edge LPC Edge
 * \{
 * \ingroup     LPC_Exported_constants
 */
typedef enum
{
    LPC_Vin_Below_Vth,
    LPC_Vin_Over_Vth,
} LPCEdge_TypeDef;

#define IS_LPC_EDGE(EDGE) (((EDGE) == LPC_Vin_Below_Vth) || \
                           ((EDGE) == LPC_Vin_Over_Vth))

/** End of LPC_Edge
  * \}
  */

#if LPC_SUPPORT_TRIGER_MODE
/**
 * \defgroup    LPC_Mode LPC Mode
 * \{
 * \ingroup     LPC_Exported_constants
 */
typedef enum
{
    LPC_DIVIDE_MODE,
    LPC_BYPASS_MODE,
} LPCMode_TypeDef;

#define IS_LPC_MODE(MODE) (((MODE) == LPC_BYPASS_MODE) || \
                           ((MODE) == LPC_DIVIDE_MODE))

/** End of LPC_Mode
  * \}
  */

/**
 * \defgroup    LPC_Trigger_Mode LPC Trigger Mode
 * \{
 * \ingroup     LPC_Exported_constants
 */
typedef enum
{
    LPC_SINGLE_OUTPUT,
    LPC_DUAL_OUTPUT,
} LPCTriggerMode_TypeDef;

#define IS_LPC_TRIGGER_MODE(MODE) (((MODE) == LPC_SINGLE_OUTPUT) || \
                                   ((MODE) == LPC_DUAL_OUTPUT))

/** End of LPC_Trigger_Mode
  * \}
  */

#endif

/**
 * \defgroup    LPC_Debounce_Div LPC Debounce Div
 * \{
 * \ingroup     LPC_Exported_constants
 */
typedef enum
{
    LPC_DEBOUNCE_DIV_1,
    LPC_DEBOUNCE_DIV_2,
    LPC_DEBOUNCE_DIV_4,
    LPC_DEBOUNCE_DIV_8,
    LPC_DEBOUNCE_DIV_16,
    LPC_DEBOUNCE_DIV_32,
    LPC_DEBOUNCE_DIV_40,
    LPC_DEBOUNCE_DIV_64,
} LPCDebDiv_TypeDef;

#define IS_LPC_DEB_DIV(DIV) (((DIV) == LPC_DEBOUNCE_DIV_1) || \
                             ((DIV) == LPC_DEBOUNCE_DIV_2) || \
                             ((DIV) == LPC_DEBOUNCE_DIV_4) || \
                             ((DIV) == LPC_DEBOUNCE_DIV_8) || \
                             ((DIV) == LPC_DEBOUNCE_DIV_16) || \
                             ((DIV) == LPC_DEBOUNCE_DIV_32) || \
                             ((DIV) == LPC_DEBOUNCE_DIV_40) || \
                             ((DIV) == LPC_DEBOUNCE_DIV_64))

/** End of LPC_Debounce_Div
  * \}
  */

/**
 * \defgroup    LPC_Threshold LPC Threshold
 * \{
 * \ingroup     LPC_Exported_constants
 */
typedef enum
{
    LPC_INPUT_RANGE
} LPCThreshod_TypeDef;

#define IS_LPC_THRESHOLD(THRESHOLD) ((THRESHOLD) < LPC_THRESHOLD_MAX)

/** End of LPC_Threshold
  * \}
  */



#if LPC_SUPPORT_RAP_MODE
/**
 * \defgroup    LPC_Task TIM Task
 * \{
 * \ingroup     LPC_Exported_Constants
 */
typedef enum
{
    LPC_TASK_START = 0,
    LPC_TASK_STOP = 1,
} LPCTask_TypeDef;
/** End of LPC_Task
  * \}
  */

/**
 * \defgroup    LPC_Shortcut_Task LPC Shortcut Task
 * \{
 * \ingroup     LPC_Exported_Constants
 */
typedef enum
{
    LPC_SHORTCUT_TASK_STOP = 1,
} LPCShortcutTask_TypeDef;

/** End of LPC_Shortcut_Task
  * \}
  */

/**
 * \defgroup    LPC_Shortcut_Event LPC Shortcut Event
 * \{
 * \ingroup     LPCExported_Constants
 */
typedef enum
{
    LPC_SHORTCUT_EVENT_STATUS = 0,
} LPCShortcutEvent_TypeDef;

/** End of LPC_Shortcut_Event
  * \}
  */
#endif
/** End of LPC_Exported_Constants
  * \}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** \defgroup LPC_Exported_Types LPC Exported Types
  * \brief
  * \{
  */

/**
 * \brief       LPC init structure definition
 *
 * \ingroup     LPC_Exported_Types
 */
typedef struct
{
    LPCChannelSel_TypeDef LPC_Channel;       /*!< Specifies the input pin.*/
    LPCEdge_TypeDef LPC_Edge;                /*!< Specifies the comparator output edge. */
    LPCThreshod_TypeDef
    LPC_Threshold;       /*!< Specifies the threshold value of comparator voltage. */
#if  LPC_SUPPORT_DEBOUNCE_MODE
    FunctionalState LPC_DebouncEn;           /*!< Specifies the LPC Debounce. */
    LPCDebDiv_TypeDef LPC_DebouncDiv;        /*!< Specifies the LPC Debounce Divider. */
    uint32_t LPC_DebouncCnt;                 /*!< Set lpcomp debounce time debounce. debounce time=(LPC_CLK/(reg_lpcomp0_deb_cnt+1)). LPC_DebouncCnt <=255.*/
#endif
#if  LPC_SUPPORT_TRIGER_MODE
    LPCMode_TypeDef LPC_Mode;                /*!< Specifies the LPC mode.*/
    uint8_t LPC_ThresholdL;                  /*!< Specifies the threshold value of comparator voltage. */
    LPCTriggerMode_TypeDef LPC_TrigerMode;   /*!< Specifies the LPC0 triger mode.
                                                  This parameter can be a value of \ref LPC Trigger Mode. */
#endif
} LPC_InitTypeDef;

/** End of LPC_Exported_Types
  * \}
  */

/*============================================================================*
 *                         Constants
 *============================================================================*/
/** \defgroup LPC_Exported_Constants LPC Exported Constants
  * \brief
  * \{
  */

/**
 * \defgroup    LPC_Flags_Definition    LPC Flags Definition
 * \{
 * \ingroup     LPC_Exported_Constants
 */
#define LPC_FLAG_LPCOMP                     (BIT0)
#define LPC_FLAG_LPCOMP_AON                 (BIT1)

#define IS_LPC_FLAG(FLAG) (((FLAG) == LPC_FLAG_LPCOMP_AON) || \
                           ((FLAG) == LPC_FLAG_LPCOMP))
#define IS_LPC_CLEAR_FLAG(FLAG) ((FLAG) == LPC_FLAG_LPCOMP)

/** End of LPC_Flags_Definition
  * \}
  */

/** End of LPC_Exported_Constants
  * \}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** \defgroup LPC_Exported_Functions LPC Exported Functions
  * \brief
  * \{
  */

/**
  * \brief  Reset LPC.
  * \param[in] LPCx: Select the LPC peripheral. \ref LPC_Declaration
  * \return None
  */
void LPC_DeInit(LPC_TypeDef *LPCx);

/**
 * \brief   Initializes LPC peripheral according to
 *          the specified parameters in LPC_InitStruct.
 * \param[in] LPCx: Select the LPC peripheral. \ref LPC_Declaration
 * \param[in] LPC_InitStruct: Pointer to a LPC_InitTypeDef structure that contains
 *                            the configuration information for the specified LPC peripheral.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lpc_init(void)
 * {
 *     LPC_InitTypeDef LPC_InitStruct;
 *
 *     LPC_StructInit(&LPC_InitStruct);
 *     LPC_InitStruct.LPC_Channel   = LPC_CAPTURE_CHANNEL;
 *     LPC_InitStruct.LPC_Edge      = LPC_VOLTAGE_DETECT_EDGE;
 *     LPC_InitStruct.LPC_Threshold = LPC_VOLTAGE_DETECT_THRESHOLD;
 *     LPC_Init(LPC0,&LPC_InitStruct);
 *     LPC_Cmd(LPC0,ENABLE);
 *
 *     LPC_INTConfig(LPC_INT_LPCOMP, ENABLE);
 *
 *     NVIC_InitTypeDef NVIC_InitStruct;
 *     NVIC_InitStruct.NVIC_IRQChannel = LPC_IRQn;
 *     NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
 *     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 *     NVIC_Init(&NVIC_InitStruct);

 *     LPC_NvCmd(LPC0,ENABLE);
 * }
 * \endcode
 */
void LPC_Init(LPC_TypeDef *LPCx, LPC_InitTypeDef *LPC_InitStruct);

/**
 * \brief  Fills each LPC_InitStruct member with its default value.
 * \param[in]  LPCx: Select the LPC peripheral. \ref LPC_Declaration
 * \param[in]  LPC_InitStruct : Pointer to a LPC_InitTypeDef structure which will be initialized.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lpc_init(void)
 * {
 *     LPC_InitTypeDef LPC_InitStruct;
 *
 *     LPC_StructInit(&LPC_InitStruct);
 *     LPC_InitStruct.LPC_Channel   = LPC_CAPTURE_CHANNEL;
 *     LPC_InitStruct.LPC_Edge      = LPC_VOLTAGE_DETECT_EDGE;
 *     LPC_InitStruct.LPC_Threshold = LPC_VOLTAGE_DETECT_THRESHOLD;
 *     LPC_Init(LPC0,&LPC_InitStruct);
 * }
 * \endcode
 */
void LPC_StructInit(LPC_InitTypeDef *LPC_InitStruct);

/**
 * \brief  Enable or disable LPC peripheral.
 * \param[in]  LPCx: Select the LPC peripheral. \ref LPC_Declaration
 * \param[in]  NewState: New state of LPC peripheral.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lpc_init(void)
 * {
 *     LPC_InitTypeDef LPC_InitStruct;
 *
 *     LPC_StructInit(&LPC_InitStruct);
 *     LPC_InitStruct.LPC_Channel   = LPC_CAPTURE_CHANNEL;
 *     LPC_InitStruct.LPC_Edge      = LPC_VOLTAGE_DETECT_EDGE;
 *     LPC_InitStruct.LPC_Threshold = LPC_VOLTAGE_DETECT_THRESHOLD;
 *     LPC_Init(LPC0, &LPC_InitStruct);
 *     LPC_Cmd(LPC0,ENABLE);
 * }
 * \endcode
 */
void LPC_Cmd(LPC_TypeDef *LPCx, FunctionalState NewState);

/**
 * \brief  Enable or disable the specified LPC interrupts.
 * \param[in]  LPCx: Select the LPC peripheral. \ref LPC_Declaration
 * \param[in] LPC_INT: Specifies the LPC interrupt source to be enabled or disabled.
 *      This parameter can be one of the following values:
 *      \arg LPC_INT_LPCOMP: voltage detection interrupt.
 *      \arg LPC_INT_LPCOMP_AON: wakeup voltage detection interrupt.
 * \param[in]  NewState: New state of the specified LPC interrupt.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lpc_init(void)
 * {
 *     LPC_InitTypeDef LPC_InitStruct;
 *
 *     LPC_StructInit(&LPC_InitStruct);
 *     LPC_InitStruct.LPC_Channel   = LPC_CAPTURE_CHANNEL;
 *     LPC_InitStruct.LPC_Edge      = LPC_VOLTAGE_DETECT_EDGE;
 *     LPC_InitStruct.LPC_Threshold = LPC_VOLTAGE_DETECT_THRESHOLD;
 *     LPC_Init(&LPC_InitStruct);
 *     LPC_Cmd(LPC0,ENABLE);
 *
 *     LPC_INTConfig(LPC0,LPC_INT_LPCOMP, ENABLE);
 *
 *     NVIC_InitTypeDef NVIC_InitStruct;
 *     NVIC_InitStruct.NVIC_IRQChannel = LPC0_IRQn;
 *     NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
 *     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 *     NVIC_Init(&NVIC_InitStruct);

 *     LPC_NvCmd(LPC0,ENABLE);
 * }
 * \endcode
 */
void LPC_INTConfig(LPC_TypeDef *LPCx, uint32_t LPC_INT, FunctionalState NewState);

/**
 * \brief     Enable wakeup signal to power sequence.
 * \param[in] LPCx: Select the LPC peripheral. \ref LPC_Declaration
 * \param[in] NewState: Enable or disable LPC wakeup signal to power sequence.
 *            This parameter can be: ENABLE or DISABLE..
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lpc_init(void)
 * {
 *     LPC_InitTypeDef LPC_InitStruct;
 *
 *     LPC_StructInit(&LPC_InitStruct);
 *     LPC_InitStruct.LPC_Channel   = LPC_CAPTURE_CHANNEL;
 *     LPC_InitStruct.LPC_Edge      = LPC_VOLTAGE_DETECT_EDGE;
 *     LPC_InitStruct.LPC_Threshold = LPC_VOLTAGE_DETECT_THRESHOLD;
 *     LPC_Init(LPC0,&LPC_InitStruct);
 *     LPC_Cmd(LPC0,ENABLE);
 *
 *     LPC_INTConfig(LPC0, LPC_INT_LPCOMP, ENABLE);
 *     LPC_INTCmd(LPC0,ENABLE);
 *     LPC_WKCmd(LPC0,ENABLE);
 *
 *     NVIC_InitTypeDef NVIC_InitStruct;
 *     NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
 *     NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
 *     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 *     NVIC_Init(&NVIC_InitStruct);
 * }
 * \endcode
 */
void LPC_WKCmd(LPC_TypeDef *LPCx, FunctionalState NewState);

/**
  * \brief  Checks whether the specified LPC interrupt is set or not.
  * \param[in] LPCx: Select the LPC peripheral. \ref LPC_Declaration
  * \param  LPC_FLAG: specifies the LPC interrupt to check.
  *     This parameter can be one of the following values:
  *     \arg LPC_INT_LPCOMP: voltage detection interrupt.
  *     \arg LPC_INT_LPCOMP_AON: wakeup voltage detection interrupt.
  * \return The new state of SPI_IT (SET or RESET).
  */
FlagStatus LPC_GetFlagStatus(LPC_TypeDef *LPCx, uint32_t LPC_FLAG);

/**
  * \brief  Clear the specified LPC flag.
  * \param[in] LPCx: Select the LPC peripheral. \ref LPC_Declaration
  * \param  LPC_FLAG: specifies the LPC flag to clear.
  *   This parameter can be one of the following values:
  *     \arg LPC_FLAG_LPCOMP: LPC NVIC flag.
  * \return None
  */
void LPC_ClearFlag(LPC_TypeDef *LPCx, uint32_t LPC_FLAG);

/**
 * \brief  Checks whether the specified LPC interrupt is set or not.
 * \param[in] LPCx: Select the LPC peripheral. \ref LPC_Declaration
 * \param[in]  LPC_INT: specifies the LPC interrupt to check.
 *   This parameter can be one of the following values:
 *     \arg LPC_INT_LPCOMP: voltage detection interrupt.
 *     \arg LPC_INT_LPCOMP_AON: wakeup voltage detection interrupt(for RTL8772F).
 * \return  The new state of SPI_IT (SET or RESET).
 *
 */
ITStatus LPC_GetINTStatus(LPC_TypeDef *LPCx, uint32_t LPC_INT);

/**
 * \brief  Configure LPCOMP counter's comparator value.
 * \param[in] LPCx: Select the LPC peripheral. \ref LPC_Declaration
 * \param[in]  data: LPCOMP counter's comparator value, which can be 0 to 0xfff.
 * \return None.
 *
 */
void LPC_WriteComparator(LPC_TypeDef *LPCx, uint32_t data);


/**
 * \brief  Read LPCOMP comparator value.
 *
 * \param[in] LPCx: Select the LPC peripheral. \ref LPC_Declaration
 * \return  LPCOMP comparator value.
 */
uint16_t LPC_ReadComparator(LPC_TypeDef *LPCx);


/**
 * \brief  Read LPC counter value.
 * \param[in] LPCx: Select the LPC peripheral. \ref LPC_Declaration
 * \return  LPCOMP comparator value.
 *
 */
uint16_t LPC_ReadCounter(LPC_TypeDef *LPCx);


/**
  * @brief Reset the LPC counter.
  * \param  LPCx: Select the LPC peripheral. \ref LPC_Declaration
  */
void LPC_CounterReset(LPC_TypeDef *LPCx);

/**
  * @brief Start or stop the LPC counter.
  * \param  LPCx: Select the LPC peripheral. \ref LPC_Declaration
  * @param  NewState: new state of the LPC counter.
  *   This parameter can be one of the following values:
  *     @arg ENABLE: Start LPCOMP counter.
  *     @arg DISABLE: Stop LPCOMP counter.
  * @retval None
  */
void LPC_CounterCmd(LPC_TypeDef *LPCx, FunctionalState NewState);


#if LPC_SUPPORT_RAP_MODE

void LPC_TaskTrigger(LPC_TypeDef *LPCx, LPCTask_TypeDef LPC_Task);
void LPC_RAPModeCmd(LPC_TypeDef *LPCx, FunctionalState NewState);
void LPC_RAPQactiveForceCmd(LPC_TypeDef *LPCx, FunctionalState NewState);
void LPC_ShortcutCmd(LPC_TypeDef *LPCx, uint32_t Task, uint32_t Event, FunctionalState NewState);

#endif
/** End of LPC_Exported_Functions
  * \}
  */

/** End of LPC
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif /* _RTL_LPC_H_ */

/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/

