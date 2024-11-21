/**
*****************************************************************************************
*     Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     io_spi.h
* @brief    This file provides demo code of spi flash.
* @author
* @date     2023-03-24
* @version  v1.0
*****************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Defines ------------------------------------------------------------------*/
/* SPI1 MASTER pin define*/
#define SPI_MASTER_SCK_PIN               P4_0
#define SPI_MASTER_MOSI_PIN              P4_1
#define SPI_MASTER_MISO_PIN              P4_2
#define SPI_MASTER_CS0_PIN               P2_3
#define SPI_MASTER_CS1_PIN               P2_4
#define SPI_MASTER_CS2_PIN               P2_5

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

void spi_demo(void);
void board_spi_init(void);

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/
