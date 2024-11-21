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

#include "trace.h"

#include "io_ir.h"

/**
  * @brief  Initialize global data.
  * @param  No parameter.
  * @return void
  */
void global_data_init(void)
{
    global_data_ir_init();
}

/**
  * @brief    Contains the initialization of pinmux settings and pad settings
  * @note     All the pinmux settings and pad settings shall be initiated in this function,
  *           but if legacy driver is used, the initialization of pinmux setting and pad setting
  *           should be peformed with the IO initializing.
  * @return   void
  */
void board_init(void)
{
    board_ir_init();
}

/**
 * @brief    Contains the initialization of peripherals
 * @note     Both new architecture driver and legacy driver initialization method can be used
 * @return   void
 */
void driver_init(void)
{
    driver_ir_init();
}

/**
 * @brief    Entry of APP code
 * @return   int (To avoid compile warning)
 */
int main(void)
{
    DBG_DIRECT("IR RX MAIN");
    __enable_irq();
    extern uint32_t random_seed_value;
    srand(random_seed_value);
    global_data_init();
    board_init();
    driver_init();
    ir_demo();
    while (1)
    {

    }

    //return 0;
}
/** @} */ /* End of group PERIPH_DEMO_MAIN */
