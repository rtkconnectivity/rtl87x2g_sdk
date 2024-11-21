/**
*****************************************************************************************
*     Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     main.c
* @brief    This file provides demo code of rtc comparator.
* @author
* @date     2023-03-24
* @version  v1.0
*****************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>

#include "trace.h"
#include "io_ir.h"

/**
  * @brief  Entry of APP code
  * @return int (To avoid compile warning)
*/
int main(void)
{
    extern uint32_t random_seed_value;
    srand(random_seed_value);
    __enable_irq();

    ir_demo();

    while (1)
    {
        ;

    }
}



/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/