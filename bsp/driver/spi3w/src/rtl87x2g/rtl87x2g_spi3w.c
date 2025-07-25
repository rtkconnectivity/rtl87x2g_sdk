/**
*********************************************************************************************************
*               Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* \file     rtl87x2g_spi3w.c
* \brief    This file provides all the SPI firmware internal functions.
* \details
* \author   Bert
* \date     2023-10-17
* \version  v1.0
*********************************************************************************************************
*/

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "rtl_spi3w.h"
#include "rtl_rcc.h"
#include "app_section.h"

/*============================================================================*
 *                        Public Functions
 *============================================================================*/
/**
  * \brief  Store SPI3W register values when system enter DLPS.
  * \param  PeriReg: Specifies to select the SPI3W peripheral.
  * \param  StoreBuf: Store buffer to store SPI3W register data.
  * \return None.
  */
RAM_FUNCTION
void SPI3W_DLPSEnter(void *PeriReg, void *StoreBuf)
{
    SPI3WStoreReg_Typedef *store_buf = (SPI3WStoreReg_Typedef *)StoreBuf;

    RCC_PeriphClockCmd(APBPeriph_SPI3W, APBPeriph_SPI3W_CLOCK, ENABLE);

    store_buf->spi3w_reg[0] = SPI3W->SPI3W_CONFIG;        /*!< 0x30 */
}

/**
  * \brief  Restore SPI3W register values when system enter DLPS.
  * \param  PeriReg: Specifies to select the SPI3W peripheral.
  * \param  StoreBuf: Restore buffer to restore SPI3W register data.
  * \return None
  */
RAM_FUNCTION
void SPI3W_DLPSExit(void *PeriReg, void *StoreBuf)
{
    SPI3WStoreReg_Typedef *store_buf = (SPI3WStoreReg_Typedef *)StoreBuf;

    RCC_PeriphClockCmd(APBPeriph_SPI3W, APBPeriph_SPI3W_CLOCK, ENABLE);

    SPI3W->SPI3W_CONFIG = store_buf->spi3w_reg[0];
}

/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/

