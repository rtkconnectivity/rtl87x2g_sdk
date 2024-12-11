#ifndef MATTER_LED_H_
#define MATTER_LED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "matter_types.h"

typedef void *T_MATTER_LED_HANDLE;

bool matter_led_init(void);

T_MATTER_LED_HANDLE matter_led_create(uint8_t index);

bool matter_led_release(T_MATTER_LED_HANDLE handle);

bool matter_led_state_set(T_MATTER_LED_HANDLE handle, bool state);

/**
 * matter_led.h
 *
 * \brief Start LED blink.
 *
 * \param[in]  handle        Matter LED instance \ref T_MATTER_LED_HANDLE.
 * \param[in]  on_time_ms    The LED on time in milliseconds.
 * \param[in]  off_time_ms   The LED off time in milliseconds.
 * \param[in]  count         The target blink count.
 *      \arg \c 0xFFFF       Blink infinitely until timer is stopped.
 *      \arg \c others       The target blink count.
 *
 * \return          The result of LED blink.
 * \retval true     LED blink was started successfully.
 * \retval false    LED was failed to blink.
 *
 * \ingroup MATTER_LED
 */
bool matter_led_blink_start(T_MATTER_LED_HANDLE handle,
                            uint32_t            on_time_ms,
                            uint32_t            off_time_ms,
                            uint16_t            count);

bool matter_led_blink_stop(T_MATTER_LED_HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif
