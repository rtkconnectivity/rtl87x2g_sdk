#ifndef MATTER_WDT_H_
#define MATTER_WDT_H_

#ifdef __cplusplus
extern "C" {
#endif

void matter_wdt_init(void *io_queue);

void matter_wdt_watchdog_open();

void matter_wdt_watchdog_close(void);

void matter_wdt_watchdog_feed(void);

#ifdef __cplusplus
}
#endif

#endif // MATTER_WDT_H_