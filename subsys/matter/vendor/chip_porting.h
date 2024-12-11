#ifndef _CHIP_PORTING_H_
#define _CHIP_PORTING_H_

#ifdef __cplusplus
extern "C" {
#endif


//
#include "matter_kvs.h"
#include "matter_gpio.h"
#include "matter_utils.h"
#include "matter_uart.h"
#include <cmsis_compiler.h>
#include <trace.h>
#include "wdt.h"

//extern void dbg_send(const uint8_t *aBuf, uint16_t aBufLength);

#ifdef __cplusplus
}
#endif

#endif
