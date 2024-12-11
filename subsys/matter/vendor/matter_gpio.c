/**************************
* Matter UART Functions
**************************/

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdio.h>
#include "rtl_pinmux.h"
#include "rtl_rcc.h"
#include "rtl_gpio.h"
#include "rtl_nvic.h"
#include "vector_table.h"
#include "os_task.h"
#include "os_msg.h"
#include "matter_gpio.h"

#define GPIO_KEY_LEVEL_LOW                  0  /* low level */
#define GPIO_KEY_LEVEL_HIGH                 1  /* high level */

#define RX_BUF_SIZE 16
static void *matter_gpio_queue = NULL;
static void *matter_gpio_handle = NULL;
static matter_gpio_callback gpio_callback = NULL;

#ifdef BOARD_RTL8777G
#define GPIO_NUM  2
const uint8_t GPIO_PIN[GPIO_NUM] = {P9_1, P9_0};
const uint8_t GPIO_PIN_IRQN[GPIO_NUM] = {GPIOB22_IRQn, GPIOB21_IRQn};
#define GPIO_Pin_Key0_Handler    GPIOB22_Handler
#define GPIO_Pin_Key1_Handler    GPIOB21_Handler
#elif BOARD_EVB
#define GPIO_NUM  5
const uint8_t GPIO_PIN[GPIO_NUM] = {P4_0, P4_1, P2_4, P2_3, P3_5};
const uint8_t GPIO_PIN_IRQN[GPIO_NUM] = {GPIOB5_IRQn, GPIOB6_IRQn, GPIOA25_IRQn, GPIOA24_IRQn, GPIOB2_IRQn};
#define GPIO_Pin_Key0_Handler    GPIOB5_Handler
#define GPIO_Pin_Key1_Handler    GPIOB6_Handler
#define GPIO_Pin_Key2_Handler    GPIOA25_Handler
#define GPIO_Pin_Key3_Handler    GPIOA24_Handler
#define GPIO_Pin_Key4_Handler    GPIOB2_Handler
#else//to add
#endif
uint8_t key_trigger_level[GPIO_NUM];

static void gpio_key_interrupt_handler(uint8_t index)
{
    GPIO_INTConfig(GPIO_GetPort(GPIO_PIN[index]), GPIO_GetPin(GPIO_PIN[index]), DISABLE);
    GPIO_MaskINTConfig(GPIO_GetPort(GPIO_PIN[index]), GPIO_GetPin(GPIO_PIN[index]), ENABLE);

    uint16_t msg;
    uint8_t state;
    if (key_trigger_level[index] == GPIO_KEY_LEVEL_LOW)
    {
        //key press
        GPIO_SetITPolarity(GPIO_GetPort(GPIO_PIN[index]), GPIO_GetPin(GPIO_PIN[index]),
                           GPIO_INT_POLARITY_ACTIVE_HIGH);
        key_trigger_level[index] = GPIO_KEY_LEVEL_HIGH;
        state = MATTER_GPIO_KEY_STATE_PRESS;
    }
    else
    {
        //key release
        GPIO_SetITPolarity(GPIO_GetPort(GPIO_PIN[index]), GPIO_GetPin(GPIO_PIN[index]),
                           GPIO_INT_POLARITY_ACTIVE_LOW);
        key_trigger_level[index] = GPIO_KEY_LEVEL_LOW;
        state = MATTER_GPIO_KEY_STATE_RELEASE;
    }

    msg = index << 8 | state;
    os_msg_send(matter_gpio_queue, &msg, 0);

    GPIO_ClearINTPendingBit(GPIO_GetPort(GPIO_PIN[index]), GPIO_GetPin(GPIO_PIN[index]));
    GPIO_MaskINTConfig(GPIO_GetPort(GPIO_PIN[index]), GPIO_GetPin(GPIO_PIN[index]), DISABLE);
    GPIO_INTConfig(GPIO_GetPort(GPIO_PIN[index]), GPIO_GetPin(GPIO_PIN[index]), ENABLE);
}

void GPIO_Pin_Key0_Handler(void)
{
    gpio_key_interrupt_handler(0);
}

void GPIO_Pin_Key1_Handler(void)
{
    gpio_key_interrupt_handler(1);
}

#ifdef BOARD_EVB
void GPIO_Pin_Key2_Handler(void)
{
    gpio_key_interrupt_handler(2);
}

void GPIO_Pin_Key3_Handler(void)
{
    gpio_key_interrupt_handler(3);
}

void GPIO_Pin_Key4_Handler(void)
{
    gpio_key_interrupt_handler(4);
}
#endif

void matter_gpio_task(void *param)
{
    uint16_t msg;
    uint8_t index;
    uint8_t state;

    while (1)
    {
        if (os_msg_recv(matter_gpio_queue, &msg, 0xffffffff))
        {
            if (gpio_callback)
            {
                index = (msg >> 8) & 0xFF;
                state = msg & 0xFF;
                gpio_callback(index, state);
            }
        }
    }
}

void matter_gpio_init(matter_gpio_callback func)
{
    uint8_t i;

    for (i = 0; i < GPIO_NUM; i++)
    {
        //PAD_OUT_HIGH?
        Pad_Config(GPIO_PIN[i], PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
        Pinmux_Config(GPIO_PIN[i], DWGPIO);
    }

    /* GPIO peripheral clock enable */
    RCC_PeriphClockCmd(APBPeriph_GPIOA, APBPeriph_GPIOA_CLOCK, ENABLE);
    RCC_PeriphClockCmd(APBPeriph_GPIOB, APBPeriph_GPIOB_CLOCK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode       = GPIO_DIR_IN;
    GPIO_InitStruct.GPIO_ITCmd      = ENABLE;
    GPIO_InitStruct.GPIO_ITTrigger  = GPIO_INT_TRIGGER_LEVEL;
    GPIO_InitStruct.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
    /* Enable GPIO doubonce function, Set doubonce time is 1ms,
     * debounce time = (CntLimit + 1) * DEB_CLK = 32 / 32000 = 1 */
    GPIO_InitStruct.GPIO_ITDebounce        = GPIO_INT_DEBOUNCE_ENABLE;
    GPIO_InitStruct.GPIO_DebounceClkSource = GPIO_DEBOUNCE_32K;
    GPIO_InitStruct.GPIO_DebounceClkDiv    = GPIO_DEBOUNCE_DIVIDER_1;
    GPIO_InitStruct.GPIO_DebounceCntLimit  = 32 - 1;
    for (i = 0; i < GPIO_NUM; i++)
    {
        key_trigger_level[i] = GPIO_KEY_LEVEL_LOW;

        GPIO_InitStruct.GPIO_Pin = GPIO_GetPin(GPIO_PIN[i]);
        GPIO_Init(GPIO_GetPort(GPIO_PIN[i]), &GPIO_InitStruct);
    }

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    for (i = 0; i < GPIO_NUM; i++)
    {
        /* Enable NVIC*/
        NVIC_InitStruct.NVIC_IRQChannel = GPIO_PIN_IRQN[i];
        NVIC_Init(&NVIC_InitStruct);

        /* Enable interrupt */
        GPIO_ClearINTPendingBit(GPIO_GetPort(GPIO_PIN[i]), GPIO_GetPin(GPIO_PIN[i]));
        GPIO_MaskINTConfig(GPIO_GetPort(GPIO_PIN[i]), GPIO_GetPin(GPIO_PIN[i]), DISABLE);
        GPIO_INTConfig(GPIO_GetPort(GPIO_PIN[i]), GPIO_GetPin(GPIO_PIN[i]), ENABLE);
    }

    gpio_callback = func;
    os_msg_queue_create(&matter_gpio_queue, "matter_gpio_queue", RX_BUF_SIZE, sizeof(uint16_t));
    os_task_create(&matter_gpio_handle, "matter_gpio_test", matter_gpio_task, 0, 2048, 1);
}

#ifdef __cplusplus
}
#endif
