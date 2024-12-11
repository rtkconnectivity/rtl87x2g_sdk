/**************************
* Matter UART Functions
**************************/

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include "rtl_rcc.h"
#include "rtl_gpio.h"
#include "rtl_nvic.h"
#include "rtl_uart.h"
#include "rtl_gdma.h"
#include "matter_uart.h"
#include "vector_table.h"
#include "os_sync.h"
#include "utils/uart.h"

#define UART_TX_GDMA_CHANNEL_NUM    GDMA_CH_NUM0
#define UART_TX_GDMA_CHANNEL        GDMA_Channel0

uint32_t MatterPlatUartEnable(void)
{
    return 0;
}

uint16_t MatterPlatUartSend(const char *aBuf, uint16_t aBufLength)
{
    otPlatUartSend(aBuf, aBufLength);
    return aBufLength;
}

uint8_t MatterPlatUartRecv(void)
{
    return 0;
}

#ifdef __cplusplus
}
#endif
