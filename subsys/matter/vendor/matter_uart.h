/**************************
* Matter UART Functions
**************************/
#ifndef MATTER_UART_H_
#define MATTER_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "matter_types.h"

//#define OT_UART_TX		P3_0
//#define OT_UART_RX		P3_1
#define MATTER_UART         UART3
#define MATTER_UART_APB     APBPeriph_UART3
#define MATTER_UART_APBCLK  APBPeriph_UART3_CLOCK
#define MATTER_UART_IRQn    UART3_IRQn
#define MATTER_UART_VECTORn UART3_VECTORn

uint32_t MatterPlatUartEnable(void);
uint16_t MatterPlatUartSend(const char *aBuf, uint16_t aBufLength);
uint8_t MatterPlatUartRecv(void);

#ifdef __cplusplus
}
#endif

#endif // MATTER_UART_H_
