/**************************
* Matter I2C Functions
**************************/

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdio.h>
#include "rtl_pinmux.h"
#include "rtl_rcc.h"
#include "rtl_i2c.h"
#include "rtl_nvic.h"
#include "vector_table.h"
#include "matter_i2c.h"
#include "trace.h"

/* Private define ------------------------------------------------------------*/
#define I2C0_SCL_PIN               P2_1
#define I2C0_SDA_PIN               P2_2
#define I2C0_ADDR                  (0xC8 >> 1)


/**
  * @brief  Initialization of pinmux settings and pad settings.
  * @param  No parameter.
  * @return void
*/
static void board_i2c_init(void)
{
    Pad_Config(I2C0_SCL_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);
    Pad_Config(I2C0_SDA_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);

    Pinmux_Config(I2C0_SCL_PIN, I2C0_CLK);
    Pinmux_Config(I2C0_SDA_PIN, I2C0_DAT);
}

/**
  * @brief  Initialize I2C Master peripheral.
  * @param  No parameter.
  * @return void
  */
static void driver_i2c_init(void)
{
    I2C_InitTypeDef I2C_InitStruct;

    /* Initialize I2C peripheral */
    RCC_PeriphClockCmd(APBPeriph_I2C0, APBPeriph_I2C0_CLOCK, ENABLE);

    I2C_StructInit(&I2C_InitStruct);

    I2C_InitStruct.I2C_ClockSpeed       = 100000;
    I2C_InitStruct.I2C_DeviveMode       = I2C_DeviveMode_Master;
    I2C_InitStruct.I2C_AddressMode      = I2C_AddressMode_7BIT;
    I2C_InitStruct.I2C_SlaveAddress     = I2C0_ADDR;
    I2C_InitStruct.I2C_RxThresholdLevel = 8;
    I2C_InitStruct.I2C_Ack              = ENABLE;

    I2C_Init(I2C0, &I2C_InitStruct);
    I2C_Cmd(I2C0, ENABLE);
}

/**
  * @brief  Config i2c nvic.
  * @param  No parameter.
  * @return void
  */
static void nvic_i2c_config(void)
{
    /* Detect stop signal */
    I2C_ClearINTPendingBit(I2C0, I2C_INT_STOP_DET);

    I2C_INTConfig(I2C0, I2C_INT_STOP_DET, ENABLE);

    /* Config I2C interrupt */
    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannel = I2C0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

void I2C0_Handler(void)
{
    if (I2C_GetINTStatus(I2C0, I2C_INT_STOP_DET) == SET)
    {
        /* Notes: DBG_DIRECT is only used for debug demo, do not use in app!
            In app project, the APP_PRINT_INFO can be used.
        */
        // DBG_DIRECT("I2C0 Stop signal detected");
        I2C_ClearINTPendingBit(I2C0, I2C_INT_STOP_DET);
    }
}

int matter_i2c_init(void)
{
    /* Configure pad and pinmux firstly! */
    board_i2c_init();

    /* Initialize i2c peripheral */
    driver_i2c_init();

    /* Config i2c nvic */
    nvic_i2c_config();

    return 0;
}

int matter_i2c_release(void)
{
    I2C_INTConfig(I2C0, I2C_INT_STOP_DET, DISABLE);
    I2C_Cmd(I2C0, DISABLE);
    I2C_DeInit(I2C0);

    return 0;
}

int matter_i2c_write(uint8_t *p_data, uint16_t length)
{
    int ret = 0;
    if (I2C_Success != (ret = I2C_MasterWrite(I2C0, p_data, length)))
    {
        DBG_DIRECT("I2C_MasterWrite error %d", ret);
        return 1;
    }

    return 0;   // if no i2c bus error
}

int matter_i2c_read(uint8_t *p_data, uint16_t length)
{
    int ret = 0;
    if (I2C_Success != (ret = I2C_MasterRead(I2C0, p_data, length)))
    {
        DBG_DIRECT("I2C_MasterRead error %d", ret);
        return 1;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif

