/**
*****************************************************************************************
*     Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     io_wdt.c
* @brief    This file provides demo code of wdt.
* @author   Grace_yan
* @date     2023-03-24
* @version  v1.0
*****************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl_wdt.h"
#include "rtl_aon_wdt.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define USE_WDT                 1
#define USE_AON_WDT             0
#define USE_WDT_SELECT          USE_WDT

/* Gloables define -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialize wdt peripheral.
  * @param  None.
  * @return None.
  */
void driver_wdt_init(void)
{
    /* WDT timing 10s. */
#if USE_WDT_SELECT
    WDT_Start(10000, RESET_ALL);
#else
    AON_WDT_Start(AON_WDT, 10000, RESET_ALL);
#endif
}

/**
  * @brief  Feeding dog.
  * @param  None.
  * @return None
  */
void wdt_feed(void)
{
#if USE_WDT_SELECT
    WDT_Kick();
#else
    AON_WDT_Kick(AON_WDT);
#endif
}

/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/
