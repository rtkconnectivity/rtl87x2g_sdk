/**
*********************************************************************************************************
*               Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl_tim.h
* \brief    The header file of the peripheral TIMER driver.
* \details  This file provides all TIMER firmware functions.
* \author   Grace_yan
* \date     2023-10-17
* \version  v1.0
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef RTL_TIM_H
#define RTL_TIM_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "utils/rtl_utils.h"
#if defined (CONFIG_SOC_SERIES_RTL87X2G)
#include "tim/src/rtl87x2g/rtl_tim_def.h"
#elif defined (CONFIG_SOC_SERIES_RTL87X3E)
#include "tim/src/rtl87x3e/rtl_tim_def.h"
#elif defined (CONFIG_SOC_SERIES_RTL87X3D)
#include "tim/src/rtl87x3d/rtl_tim_def.h"
#elif defined (CONFIG_SOC_SERIES_RTL8762J)
#include "tim/src/rtl87x2j/rtl_tim_def.h"
#endif

/** \defgroup TIM         TIM
  * \brief
  * \{
  */
/*============================================================================*
 *                         Constants
 *============================================================================*/
/** \defgroup TIM_Exported_Constants TIM Exported Constants
  * \brief
  * \{
  */

/**
 * \defgroup    TIM_Private_Defines TIM Private Defines
 * \{
 * \ingroup     TIM_Exported_Constants
 */
#define TIM_0X08_TIMERENABLE      BIT0

/** End of TIM_Private_Defines
  * \}
  */

/**
 * \defgroup    PWM_DeadZone_Clock_Source PWM DeadZone Clock Source
 * \{
 * \ingroup     TIM_Exported_Constants
 */
typedef enum
{
    PWM_CK_5M_TIMER = 0x0,
    PWM_CK_32K_TIMER = 0x1,
} PWMDZClockSrc_TypeDef;

#define IS_TIM_PWM_DZ_SOURCE(PERIPH)     (((PERIPH) == PWM_CK_5M_TIMER) || \
                                          ((PERIPH) == PWM_CK_32K_TIMER))

/** End of PWM_DeadZone_Clock_Source
  * \}
  */

/**
 * \defgroup    TIM_Mode TIM Mode
 * \{
 * \ingroup     TIM_Exported_Constants
 */
typedef enum
{
    TIM_Mode_FreeRun = 0x0,
    TIM_Mode_UserDefine = 0x1,
} TIMMode_TypeDef;

#define IS_TIM_MODE(mode) (((mode) == TIM_Mode_FreeRun) || \
                           ((mode) == TIM_Mode_UserDefine))

/** End of TIM_Mode
  * \}
  */

/**
 * \defgroup    TIM_PWM_Mode_Enable TIM PWM Mode Enable
 * \{
 * \ingroup     TIM_Exported_Constants
 */
#define IS_TIM_PWM_En(mode) (((mode) == ENABLE) || \
                             ((mode) == DISABLE))

/** End of TIM_PWM_Mode_Enable
  * \}
  */

/**
 * \defgroup    PWM_DeadZone_Enable PWM DeadZone Enable
 * \{
 * \ingroup     TIM_Exported_Constants
 */
#define IS_TIM_PWM_DeadZone_En(mode) (((mode) == ENABLE) || \
                                      ((mode) == DISABLE))

/** End of PWM_DeadZone_Enable
  * \}
  */

/**
 * \defgroup    PWM_DeadZone_Stop_State PWM DeadZone Stop State
 * \{
 * \ingroup     TIM_Exported_Constants
 */
typedef enum
{
    PWM_STOP_AT_LOW = 0x0,
    PWM_STOP_AT_HIGH = 0x1,
} PWMDZStopState_TypeDef;

#define IS_PWM_STOP_STATE(STATE) (((STATE) == PWM_STOP_AT_LOW) || \
                                  ((STATE) == PWM_STOP_AT_HIGH))

/** End of PWM_DeadZone_Stop_State
  * \}
  */

#if TIM_SUPPORT_EVENT_DURATION
/**
 * \brief       TIM_Event_Duration TIM Event Duration
 *
 * \ingroup     TIM_Exported_Constants
 */
typedef enum
{
    TIM_EventDuration_32us  = 0x00,
    TIM_EventDuration_64us  = 0x01,
    TIM_EventDuration_128us = 0x02,
    TIM_EventDuration_256us = 0x03,
} TIMEventDuration_TypeDef;
#define IS_TIM_Event_DURATION(duration) (((duration) == TIM_EventDuration_32us) || \
                                         ((duration) == TIM_EventDuration_64us) || \
                                         ((duration) == TIM_EventDuration_128us) || \
                                         ((duration) == TIM_EventDuration_256us))
#endif

/** End of TIM_Exported_Constants
  * \}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** \defgroup TIM_Exported_Types TIM Exported Types
  * \brief
  * \{
  */

/**
 * \brief       TIM init structure definition.
 *
 * \ingroup     TIM_Exported_Types
 */
typedef struct
{
    TIMClockSrc_TypeDef TIM_ClockSrc;          /*!< Specifies the clock source. */

    TIMClockDiv_TypeDef TIM_SOURCE_DIV;        /*!< Specifies the clock source div.
                                                    This parameter can be a value of ENABLE or DISABLE. */

    FunctionalState TIM_SOURCE_DIV_En;         /*!< Timer source clock div enable. */

    TIMMode_TypeDef TIM_Mode;                  /*!< Specifies the counter mode. */

    FunctionalState TIM_PWM_En;                /*!< Specifies the PWM mode.
                                                    This parameter can be a value of DISABLE or ENABLE. */

    uint32_t TIM_Period;                       /*!< Specifies the period value to be loaded into the active
                                                    Auto-Reload Register at the next update event.
                                                    This parameter must range from 0x0000 to 0xFFFFFFFF. */

    uint32_t TIM_PWM_High_Count;               /*!< Specifies the PWM High Count.
                                                    This parameter must range from 0x0000 to 0xFFFFFFFF. */

    uint32_t TIM_PWM_Low_Count;                /*!< Specifies the PWM Low Count.
                                                    This parameter must range from 0x0000 to 0xFFFFFFFF. */

#if TIM_SUPPORT_EVENT_DURATION
    uint32_t TIM_EventMode;                    /*!< Specifies the TIM event mode. */

    uint32_t TIM_EventIndex;                   /*!< Specifies the TIM event index. */

    uint32_t TIM_EventDuration;                /*!< Specifies the TIM event duration. */
#endif

#if TIM_SUPPORT_CLOCK_DEPEND
    uint8_t  ClockDepend;                      /*!< Specifies TIM Source depend.
                                                   timer3 depend timer2, timer5 depend timer4, timer7 depend timer6.
                                                   This parameter can be a value of ENABLE or DISABLE. */
#endif

    PWMDZClockSrc_TypeDef PWM_Deazone_ClockSrc; /*!< Specifies the pwm deazone clock source. */

    TIMClockDiv_TypeDef PWM_Deadzone_DIV;      /*!< Specifies the clock source div. */

    uint32_t PWM_Deazone_Size;                 /*!< Size of deadzone time, DeadzoneTime=deadzonesize/(32000 or 32768).
                                                    This parameter must range from 1 to 0xff. */

    FunctionalState PWMDeadZone_En;            /*!< PWM Deadzone enable.
                                                    This parameter can be a value of ENABLE or DISABLE. */

    PWMDZStopState_TypeDef PWM_Stop_State_P;   /*!< Specifies the PWM P stop state. */

    PWMDZStopState_TypeDef PWM_Stop_State_N;   /*!< Specifies the PWM N stop state. */

} TIM_TimeBaseInitTypeDef;

/** End of TIM_Exported_Types
  * \}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** \defgroup TIM_Exported_Functions TIM Exported Functions
  * \brief
  * \{
  */

/**
 * \brief   Deinitialize the TIMx peripheral registers to their default reset values.
 *
 * \return  None.
 */
void TIM_DeInit(TIM_TypeDef *TIMx);

/**
 * \brief   Initialize the TIMx time base unit peripheral according to
 *          the specified parameters in TIM_TimeBaseInitStruct.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \param[in] TIM_TimeBaseInitStruct: Pointer to a TIM_TimeBaseInitTypeDef
 *            structure that contains the configuration information for the selected TIM peripheral.
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_PWM_En = DISABLE;
 *     TIM_InitStruct.TIM_Period = 1000000 - 1 ;
 *     TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
 *     TIM_TimeBaseInit(TIM6, &TIM_InitStruct);
 * }
 * \endcode
 */
void TIM_TimeBaseInit(TIM_TypeDef *TIMx, TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct);

/**
 * \brief   Fills each TIM_InitStruct member with its default value.
 *
 * \param[in] TIM_TimeBaseInitStruct: Pointer to a TIM_TimeBaseInitTypeDef structure which will be initialized.
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_PWM_En = DISABLE;
 *     TIM_InitStruct.TIM_Period = 1000000 - 1;
 *     TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
 *     TIM_TimeBaseInit(TIM6, &TIM_InitStruct);
 * }
 * \endcode
 */
void TIM_StructInit(TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct);

/**
 * \brief   Enable or disable the specified TIM peripheral.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \param[in] NewState: New state of the TIMx peripheral.
 *            This parameter can be: ENABLE or DISABLE.
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_PWM_En = DISABLE;
 *     TIM_InitStruct.TIM_Period = 1000000 - 1;
 *     TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
 *     TIM_TimeBaseInit(TIM6, &TIM_InitStruct);
 *     TIM_Cmd(TIM6, ENABLE);
 * }
 * \endcode
 */
void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * \brief       Enable or disable the specified TIMx interrupt.
 *
 * \param[in]   TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \param[in]   NewState: New state of the TIMx peripheral.
 *              This parameter can be: ENABLE or DISABLE.
 *
 * \return      None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_PWM_En = DISABLE;
 *     TIM_InitStruct.TIM_Period = 1000000 - 1;
 *     TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
 *     TIM_TimeBaseInit(TIM6, &TIM_InitStruct);
 *     TIM_ClearINT(TIM6);
 *     TIM_INTConfig(TIM6, ENABLE);
 */
void TIM_INTConfig(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * \brief       Change TIM period value.
 *
 * \param[in]   TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \param[in]   period: Period value to be changed.
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     uint32_t new_period = 1000000 - 1;
 *     TIM_Cmd(TIM6, DISABLE);
 *     TIM_ChangePeriod(TIM6, new_period);
 *
 * }
 * \endcode
 */
void TIM_ChangePeriod(TIM_TypeDef *TIMx, uint32_t period);

/**
 * \brief   Change PWM freq and duty according high_cnt and low_cnt
 *
 * \param[in]   TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \param[in]   high_count: This parameter can be 0x00~0xFFFFFFFF.
 * \param[in]  low_count: This parameter can be 0x00~0xFFFFFFFF.
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define TIM_DEMO TIMB_6
 *
 * void timer_demo(void)
 * {
 *     uint32_t high_count = 1000000 - 1;
 *     uint32_t low_count = 1000000 - 1;
 *     TIM_Cmd(TIM_DEMO, DISABLE);
 *     TIM_ChangePeriod(TIM_DEMO, high_count, low_count);
 * }
 * \endcode
 */
void TIM_PWMChangeFreqAndDuty(TIM_TypeDef *TIMx, uint32_t high_count, uint32_t low_count);

/**
 * \brief   Get TIMx current value when timer is running.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 *
 * \return  The counter value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     uint32_t cur_value = TIM_GetCurrentValue(TIM6);
 * }
 * \endcode
 */
uint32_t TIM_GetCurrentValue(TIM_TypeDef *TIMx);

/**
 * \brief   Check whether the TIM interrupt has occurred or not.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 *
 * \return  The new state of the TIM_IT(SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     ITStatus int_status = TIM_GetINTStatus(TIM6);
 * }
 * \endcode
 */
ITStatus TIM_GetINTStatus(TIM_TypeDef *TIMx);

/**
 * \brief   Check whether the TIM is in operation or not.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 *
 * \return  The new state of the timer operation status (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     ITStatus intstatus = TIM_GetOperationStatus(TIM6);
 * }
 * \endcode
 */
ITStatus TIM_GetOperationStatus(TIM_TypeDef *TIMx);

/**
 * \brief Get TIMx elapsed value when timer is running.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 *
 * \return The elapsed counter value.
 */
uint32_t TIM_GetElapsedValue(TIM_TypeDef *TIMx);

/**
 * \brief   Clear TIM interrupt.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     TIM_ClearINT(TIM6);
 * }
 * \endcode
 */
void TIM_ClearINT(TIM_TypeDef *TIMx);

/**
 * \brief   PWM complementary output emergency stop and resume.
 *          PWM_P emergency stop level state is configured by PWM_Stop_State_P,
 *          PWM_N emergency stop level state is configured by PWM_Stop_State_N.
 *
 * \param[in] PWMx: Select the PWM peripheral. \ref PWM_Declaration
 * \param[in] NewState: New state of complementary output.
 *            \ref DISABLE: Resume PWM complementary output.
 *            \ref ENABLE: PWM complementary output emergency stop.
 *
 * \return  None.
 * \note    To use this function, need to configure the corresponding timer.
 *          PWM2 ->> TIM2, PWM3 ->> TIM3.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_pwm_init(void)
 * {
 *     Pad_Config(P0_1, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
 *     Pad_Config(P0_2, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
 *                PAD_OUT_HIGH);
 *     Pad_Config(P2_2, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
 *                PAD_OUT_HIGH);
 *
 *     Pinmux_Config(P0_1, PWM_OUT_PIN_PINMUX);
 *     Pinmux_Config(P0_2, PWM_OUT_P_PIN_PINMUX);
 *     Pinmux_Config(P2_2, PWM_OUT_N_PIN_PINMUX);
 * }
 *
 * void driver_pwm_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_Mode             = TIM_Mode_UserDefine;
 *     TIM_InitStruct.TIM_PWM_En           = ENABLE;
 *     TIM_InitStruct.TIM_PWM_High_Count   = PWM_HIGH_COUNT;
 *     TIM_InitStruct.TIM_PWM_Low_Count    = PWM_LOW_COUNT;
 *     TIM_InitStruct.PWM_Stop_State_P     = PWM_STOP_AT_HIGH;
 *     TIM_InitStruct.PWM_Stop_State_N     = PWM_STOP_AT_LOW;
 *     TIM_InitStruct.PWMDeadZone_En       = ENABLE;  //enable to use pwn p/n output
 *     TIM_InitStruct.PWM_Deazone_Size     = 255;
 *     TIM_TimeBaseInit(PWM2, &TIM_InitStruct);
 *
 *     TIM_Cmd(PWM2, ENABLE);
 * }
 *
 * void pwm_demo(void)
 * {
 *    board_pwm_init();
 *    driver_pwm_init();
 *    //Add delay.
 *    TIM_PWMComplOutputEMCmd(PWM2,ENABLE);
 * }
 * \endcode
 */
void TIM_PWMComplOutputEMCmd(PWM_TypeDef *PWMx, FunctionalState NewState);

/**
 * \brief   Enable or disable bypass dead zone function of PWM complementary output.
 *          After enabling, PWM_P = ~PWM_N.
 *
 * \param[in] PWMx: Select the PWM peripheral. \ref PWM_Declaration
 * \param[in] NewState: New state of the PWMx peripheral.
 *            \ref DISABLE: Disable bypass dead zone function.
 *            \ref ENABLE: Enable bypass dead zone function.
 *
 * \return  None.
 * \note    To use this function, need to configure the corresponding timer.
 *          PWM2 ->> TIM2, PWM3 ->> TIM3.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_pwm_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_Mode             = TIM_Mode_UserDefine;
 *     TIM_InitStruct.TIM_PWM_En           = ENABLE;
 *     TIM_InitStruct.TIM_PWM_High_Count   = PWM_HIGH_COUNT;
 *     TIM_InitStruct.TIM_PWM_Low_Count    = PWM_LOW_COUNT;
 *     TIM_InitStruct.PWM_Stop_State_P     = PWM_STOP_AT_HIGH;
 *     TIM_InitStruct.PWM_Stop_State_N     = PWM_STOP_AT_LOW;
 *     TIM_InitStruct.PWMDeadZone_En       = ENABLE;  //enable to use pwn p/n output
 *     TIM_InitStruct.PWM_Deazone_Size     = 255;
 *     TIM_TimeBaseInit(PWM2, &TIM_InitStruct);
 *
 *     TIM_Cmd(PWM2, ENABLE);
 *     TIM_PWMDZBypassCmd(PWM2, ENABLE);
 * }
 * \endcode
 */
void TIM_PWMDZBypassCmd(PWM_TypeDef *PWMx, FunctionalState NewState);

/**
 * \brief   Change the PWM dead time clock source.
 *
 * \param[in] PWMx: Select the PWM peripheral. \ref PWM_Declaration
 * \param[in] PWM_Deazone_ClockSrc: New state of the PWMx peripheral.
 *            \ref PWM_CK_5M_TIMER: Use 5M clock source.
 *            \ref PWM_CK_32K_TIMER: Use 32k clock source.
 *
 * \return  None.
 * \note    To use this function, need to configure the corresponding timer.
 *          PWM2 ->> TIM2, PWM3 ->> TIM3.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_pwm_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *     //Open 32k clock source.
 *     RCC_ClockSrc5MCmd();
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_Mode             = TIM_Mode_UserDefine;
 *     TIM_InitStruct.TIM_PWM_En           = ENABLE;
 *     TIM_InitStruct.TIM_PWM_High_Count   = PWM_HIGH_COUNT;
 *     TIM_InitStruct.TIM_PWM_Low_Count    = PWM_LOW_COUNT;
 *     TIM_InitStruct.PWM_Stop_State_P     = PWM_STOP_AT_HIGH;
 *     TIM_InitStruct.PWM_Stop_State_N     = PWM_STOP_AT_LOW;
 *     TIM_InitStruct.PWMDeadZone_En       = ENABLE;  //enable to use pwn p/n output
 *     TIM_InitStruct.PWM_Deazone_Size     = PWM_DEAD_ZONE_SIZE;
 *     TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
 *
 *     //Use 32k clock source.
 *     TIM_PWMChangeDZClockSrc(TIM2, PWM_CK_32K_TIMER);
 *
 *     TIM_Cmd(TIM2, ENABLE);
 * }
 * \endcode
 */
void TIM_PWMChangeDZClockSrc(PWM_TypeDef *PWMx, PWMDZClockSrc_TypeDef PWM_Deazone_ClockSrc);

/**
 * \brief  TIM clock config.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \param[in] ClockSrc: specifies the PLL clock source.
 * \param[in] ClockDiv: specifies the APB peripheral to gates its clock.
 *
 * \return None
 */
void TIM_ClkConfig(TIM_TypeDef *TIMx, TIMClockSrc_TypeDef ClockSrc,
                   TIMClockDiv_TypeDef ClockDiv);

/**
 * \brief  Get TIM clock config.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \param[out] ClockSrc: specifies the PLL clock source.
 * \param[out] ClockDiv: specifies the APB peripheral to gates its clock.
 *
 * \return The status of get clock
 */
bool TIM_ClkGet(TIM_TypeDef *TIMx, TIMClockSrc_TypeDef *ClockSrc, TIMClockDiv_TypeDef *ClockDiv);

#if RCC_SUPPORT_TIMSOURCECONFIG_API
/**
 * rtl876x_rcc.h
 *
 * \brief     Select the specified timer clock divider and source.
 *
 * \param[in] clocklevel: Timer clock divider.
 *            This parameter can be one of the following values:
 *            \arg TIM2TO7_CLOCK_DIV_x: TIM2TO7 clock divider, where x can be 1 ~ 4, 6, 8, 16.
 * \param[in] clocksource: Timer clock Source.
 *            This parameter can be one of the following values:
 *            \arg TIM_CLOCK_SOURCE_SYSTEM_CLOCK: Select timer clock source of system clock.
 *            \arg TIM_CLOCK_SOURCE_40MHZ: Select timer clock source of 40MHz.
 *            \arg TIM_CLOCK_SOURCE_PLL: Select timer clock source of PLL.
 * \param[in] NewState: New state of the specified RCC Clock Source.
 *            This parameter can be one of the following values:
 *            \arg ENABLE: Enable the specified RCC Clock Source.
 *            \arg DISABLE: Disable the specified RCC Clock Source.
 *
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_xx_init(void)
 * {
 *     TIM_SourceConfig(TIM2TO7_CLOCK_DIV_1, TIM_CLOCK_SOURCE_40MHZ, ENABLE);
 * }
 * \endcode
 */
void TIM_SourceConfig(uint16_t clocklevel, uint16_t clocksource, FunctionalState NewState);
#endif

/** End of TIM_Exported_Functions
  * \}
  */

/** End of TIM
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif /*_RTL_TIM_H_*/


/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/
