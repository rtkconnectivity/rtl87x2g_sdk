#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "board.h"
#include "trace.h"
#include "wdt.h"
#include "os_msg.h"
#include "os_timer.h"
#include "app_section.h"
#include "app_msg.h"
#include "matter_wdt.h"

typedef void *TimerHandle_t;

static void *matter_wdt_io_queue_handle;
static TimerHandle_t watch_dog_reset_dlps_timer;
static bool is_aon_wdg_enable = false;

extern void otSysEventSignalPending(void);

RAM_FUNCTION
static void matter_wdt_timer_callback(TimerHandle_t p_timer)
{
    T_IO_MSG io_msg;

    io_msg.type = IO_MSG_TYPE_RESET_WDG_TIMER;

    //push msg
    if (os_msg_send(matter_wdt_io_queue_handle, &io_msg, 0) == false)
    {
        APP_PRINT_ERROR1("matter_wdt_send_msg fail: subtype 0x%x", io_msg.subtype);
    }

    otSysEventSignalPending();
}

void matter_wdt_init(void *io_queue)
{
    matter_wdt_io_queue_handle = io_queue;

    if (false == os_timer_create(&watch_dog_reset_dlps_timer, "watch_dog_reset_dlps_timer",
                                 1, \
                                 WATCH_DOG_TIMEOUT_MS - 1000, true, matter_wdt_timer_callback))
    {
        MATTER_PRINT_INFO0("init watch_dog_reset_dlps_timer failed");
    }
    else
    {
        os_timer_start(&watch_dog_reset_dlps_timer);
        MATTER_PRINT_INFO0("Start watch_dog_reset_dlps_timer!");
    }
}

void matter_wdt_watchdog_open()
{
    if (!is_aon_wdg_enable)
    {
        WDT_Start(WATCH_DOG_TIMEOUT_MS, RESET_ALL_EXCEPT_AON);
        is_aon_wdg_enable = true;
        os_timer_restart(&watch_dog_reset_dlps_timer, WATCH_DOG_TIMEOUT_MS - 1000);
    }
}

void matter_wdt_watchdog_close(void)
{
    if (is_aon_wdg_enable)
    {
        WDT_Disable();
        is_aon_wdg_enable = false;
        os_timer_stop(&watch_dog_reset_dlps_timer);
    }
}

void matter_wdt_watchdog_feed(void)
{
    WDT_Kick();
}

#ifdef __cplusplus
}
#endif