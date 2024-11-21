/**
*****************************************************************************************
*     Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     io_enhtim_pwm.c
* @brief    This file provides demo code of pwm.
* @author   Grace_yan
* @date     2023-03-24
* @version  v1.0
*****************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "rtl_rcc.h"
#include "rtl_pinmux.h"

#include "rtl_enh_tim.h"
#include "io_enhtim_pwm.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PWM_OUT_P_PIN        P0_0
#define PWM_PINMUX_OUT_P     ENPWM0_P
#define PWM_OUT_N_PIN        P0_1
#define PWM_PINMUX_OUT_N     ENPWM0_N

#define Enhance_Timer_Num              0
#define Enhance_Timer                  ENH_TIM0

#define ENHTIM_PWM_MANUAL_MODE   0
#define ENHTIM_PWM_AUTO_MODE     1
/* Gloables define -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialization of pinmux settings and pad settings.
  * @param  None.
  * @retval None.
  */
void board_enhance_pwm_init(void)
{
    Pad_Config(PWM_OUT_P_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);
    Pad_Config(PWM_OUT_N_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);

    Pinmux_Config(PWM_OUT_P_PIN, PWM_PINMUX_OUT_P);
    Pinmux_Config(PWM_OUT_N_PIN, PWM_PINMUX_OUT_N);
}

/**
  * @brief  Initialize GPIO peripheral.
  * @param  None.
  * @retval None.
  */
void enhance_timer_init(void)
{
    /* open enhtimer clock */
    RCC_PeriphClockCmd(APBPeriph_ENHTIMER, APBPeriph_ENHTIMER_CLOCK, ENABLE);

    ENHTIM_InitTypeDef ENHTIM_InitStruct;
    ENHTIM_StructInit(&ENHTIM_InitStruct);
    ENHTIM_InitStruct.ENHTIM_ClockDiv = ENHTIM_CLOCK_DIVIDER_1;

#if ENHTIM_PWM_AUTO_MODE
    ENHTIM_InitStruct.ENHTIM_Mode               = ENHTIM_MODE_PWM_AUTO;
    ENHTIM_InitStruct.ENHTIM_PWMOutputEn        = ENABLE;
    ENHTIM_InitStruct.ENHTIM_PWMStartPolarity   = ENHTIM_PWM_START_WITH_HIGH;
    ENHTIM_InitStruct.ENHTIM_MaxCount           = 100;
#endif

#if ENHTIM_PWM_MANUAL_MODE
    ENHTIM_InitStruct.ENHTIM_Mode = ENHTIM_MODE_PWM_MANUAL;
    ENHTIM_InitStruct.ENHTIM_PWMOutputEn = ENABLE;
    ENHTIM_InitStruct.ENHTIM_PWMStartPolarity = ENHTIM_PWM_START_WITH_HIGH;
    ENHTIM_InitStruct.ENHTIM_MaxCount = 4000;
    ENHTIM_InitStruct.ENHTIM_CCValue = 2000;
#endif

    ENHTIM_InitStruct.ENHTIM_PWMDeadZoneClockSource = ENHTIM_PWM_DZCLKSRCE_32K;
    ENHTIM_InitStruct.ENHTIM_PWMDeadZoneEn = ENABLE;
    ENHTIM_InitStruct.ENHTIM_PWMStopStateP = ENHTIM_PWM_STOP_AT_HIGH;
    ENHTIM_InitStruct.ENHTIM_PWMStopStateN = ENHTIM_PWM_STOP_AT_LOW;
    ENHTIM_InitStruct.ENHTIM_DeadZoneSize = 0x0;

    ENHTIM_Init(Enhance_Timer, &ENHTIM_InitStruct);

#if ENHTIM_PWM_AUTO_MODE
    /* After the waveform is transformed 8 times,
    the PWM maintains the last change and continues to output */
    ENHTIM_WriteCCFIFO(Enhance_Timer, 0); //Output high level with a duty cycle of 100%
    ENHTIM_WriteCCFIFO(Enhance_Timer, 20); //Output high level with a duty cycle of 80%
    ENHTIM_WriteCCFIFO(Enhance_Timer, 40); //Output high level with a duty cycle of 60%
    ENHTIM_WriteCCFIFO(Enhance_Timer, 50); //Output high level with a duty cycle of 50%
    ENHTIM_WriteCCFIFO(Enhance_Timer, 70); //Output high level with a duty cycle of 30%
    ENHTIM_WriteCCFIFO(Enhance_Timer, 80); //Output high level with a duty cycle of 20%
    ENHTIM_WriteCCFIFO(Enhance_Timer, 90); //Output high level with a duty cycle of 10%
    ENHTIM_WriteCCFIFO(Enhance_Timer, 100); //Output low level with a duty cycle of 100%
#endif

    ENHTIM_Cmd(Enhance_Timer, ENABLE);
}

/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/
