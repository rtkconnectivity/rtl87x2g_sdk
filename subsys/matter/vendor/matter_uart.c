/**************************
* Matter UART Functions
**************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "matter_uart.h"
#include "utils/uart.h"

uint32_t MatterPlatUartEnable(void)
{
    otPlatUartEnable();
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
