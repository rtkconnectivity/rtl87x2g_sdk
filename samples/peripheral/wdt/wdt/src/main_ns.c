/**
*****************************************************************************************
*     Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file
* @brief
* @author
* @date
* @version
**************************************************************************************

*/
/* Demo includes. */
#include "stdlib.h"
#include "app_section.h"
#include "app_task.h"
#include "board.h"

#include "trace.h"
#include "rtl_pinmux.h"
#include "io_gpio.h"
#include "io_wdt.h"


/******************************************************************/

void driver_init(void)
{
    driver_gpio_init();
    driver_wdt_init();
}

/* Non-Secure main. */
int main(void)
{
    DBG_DIRECT("Start wdt test!");
    extern uint32_t random_seed_value;
    srand(random_seed_value);
    board_gpio_init();
    app_task_init();

//  driver_init();
    /* Start scheduler. */
    os_sched_start();


    return 0;
}

/*-----------------------------------------------------------*/
