#ifndef MATTER_GPIO_H_
#define MATTER_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "matter_types.h"

#define MATTER_GPIO_KEY_STATE_RELEASE 0
#define MATTER_GPIO_KEY_STATE_PRESS   1

typedef void (*matter_gpio_callback)(uint8_t index, uint8_t state);
void matter_gpio_init(matter_gpio_callback func);

#ifdef __cplusplus
}
#endif

#endif // MATTER_UART_H_
