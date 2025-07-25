/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file      main.c
* @brief     Source file for BLE peripheral project, mainly used for initialize modules
* @author    jane
* @date      2017-06-12
* @version   v1.0
**************************************************************************************
* @attention
* <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
**************************************************************************************
*/

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <stdlib.h>
#include <string.h>

#include "trace.h"
#include "utils.h"

#include "io_ir.h"

/**
  * @brief  Demo code of ir send data.
  * @param  No parameter.
  * @return Void
*/
void ir_demo(void)
{
    board_ir_init();
    board_pwm_init();
    driver_ir_init();
    driver_pwm_init();
    driver_ir_gdma_init();

}

/**
  * @brief    Entry of APP code
  * @return   int (To avoid compile warning)
*/
int main(void)
{
    extern uint32_t random_seed_value;
    srand(random_seed_value);
    __enable_irq();

    ir_demo();


    while (1)
    {
//        DBG_DIRECT("GDMA_GetTransferLen(IO_TEST_GDMA_Channel) = %d",
//                   GDMA_GetTransferLen(IO_TEST_GDMA_Channel));
        platform_delay_ms(100);
    }
}
/** @} */ /* End of group PERIPH_DEMO_MAIN */
