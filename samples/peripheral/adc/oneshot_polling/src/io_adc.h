/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file     io_adc.h
* @brief
* @details
* @author   yuan
* @date     2019-01-11
* @version  v1.0
*********************************************************************************************************
*/

#ifndef __IO_ADC_H
#define __IO_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "rtl876x.h"

/* Change the ADC sampling pin here! */

#define ADC_SAMPLE_PIN                    P2_4
#define ADC_SAMPLE_CHANNEL                ADC_Channel_Index_4


#define ADC_SAMPLE_PIN_0                    P2_6
#define ADC_SAMPLE_PIN_1                    P2_7
#define ADC_SAMPLE_CHANNEL_0                ADC_Channel_Index_6
#define ADC_SAMPLE_CHANNEL_1                ADC_Channel_Index_7
#define ADC_CHANNEL_NUM                     2

void adc_demo(void);
void adc_sample_demo(void);

#ifdef __cplusplus
}
#endif

#endif

