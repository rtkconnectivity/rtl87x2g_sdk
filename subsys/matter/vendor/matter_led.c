#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "trace.h"
#include "os_mem.h"
#include "os_queue.h"
#include "os_timer.h"
#include "matter_led.h"

#define LED_PIN_MAX_NUM 5

#define LED_PWM_COUNT       (40000000/LED_PWM_FREQ)

typedef struct t_matter_led
{
    struct t_matter_led      *p_next;
    uint8_t                   pin_index;
    uint16_t                  brightness;
    uint16_t                  current_count;
    uint16_t                  target_count;
    uint32_t                  blink_on_time_ms;
    uint32_t                  blink_off_time_ms;
    void                     *led_timer;
    bool                      state;
    bool                      dimmable;
} T_MATTER_LED;

typedef struct t_matter_led_db
{
    T_OS_QUEUE           leds;
    T_LED_PWM_PARAM      led_pwm_param[LED_PIN_MAX_NUM];
    uint8_t              led_num;
} T_MATTER_LED_DB;

static T_MATTER_LED_DB *matter_led_db = NULL;

void matter_led_state_do_set(T_MATTER_LED *matter_led, bool state)
{
    matter_led->state = state;

    if (true == state)
    {
        GPIO_WriteBit(GPIO_GetPort(matter_led_db->led_pwm_param[matter_led->pin_index].pin_num),
                      GPIO_GetPin(matter_led_db->led_pwm_param[matter_led->pin_index].pin_num), Bit_SET);
    }
    else
    {
        GPIO_WriteBit(GPIO_GetPort(matter_led_db->led_pwm_param[matter_led->pin_index].pin_num),
                      GPIO_GetPin(matter_led_db->led_pwm_param[matter_led->pin_index].pin_num), Bit_RESET);
    }
}

static void matter_led_timeout_cb(void *p_xtimer)
{
    T_MATTER_LED *matter_led;
    uint32_t timer_id;

    os_timer_id_get(&p_xtimer, &timer_id);
    matter_led = (T_MATTER_LED *)timer_id;

    if (matter_led->target_count == 0xFFFF ||
        matter_led->current_count < matter_led->target_count)
    {
        if (matter_led->state == true)
        {
            matter_led->current_count++;
        }

        matter_led_state_do_set(matter_led, !matter_led->state);
        os_timer_restart(&matter_led->led_timer,
                         matter_led->state ? matter_led->blink_on_time_ms : matter_led->blink_off_time_ms);
    }
    else
    {
        os_timer_stop(&matter_led->led_timer);
        matter_led->current_count = 0;
        matter_led->target_count = 0;
        matter_led_state_do_set(matter_led, false);
    }
}

bool matter_led_init(T_LED_PWM_PARAM *led_pwm_list, uint8_t led_pwm_num)
{
    if (led_pwm_num > LED_PIN_MAX_NUM)
    {
        return false;
    }

    if (matter_led_db == NULL)
    {
        RCC_PeriphClockCmd(APBPeriph_GPIOA, APBPeriph_GPIOA_CLOCK, ENABLE);

        matter_led_db = os_mem_alloc2(sizeof(T_MATTER_LED_DB));
        if (matter_led_db == NULL)
        {
            return false;
        }

        os_queue_init(&matter_led_db->leds);

        for (uint8_t i = 0; i < led_pwm_num; i++)
        {
            matter_led_db->led_pwm_param[i] = led_pwm_list[i];
        }
    }

    return true;
}

T_MATTER_LED_HANDLE matter_led_create(uint8_t index, bool dimmable, uint16_t brightness)
{
    T_MATTER_LED *matter_led;
    int32_t ret = 0;

    if (matter_led_db == NULL)
    {
        ret = 1;
        goto fail_check_param;
    }

    matter_led = os_queue_peek(&matter_led_db->leds, 0);
    while (matter_led != NULL)
    {
        if (matter_led->pin_index == index)
        {
            ret = 2;
            goto fail_check_pin;
        }

        matter_led = matter_led->p_next;
    }

    matter_led = os_mem_zalloc2(sizeof(T_MATTER_LED));
    if (matter_led == NULL)
    {
        ret = 3;
        goto fail_alloc_led;
    }

    matter_led->pin_index = index;
    matter_led->state = false;
    matter_led->current_count = 0;
    matter_led->target_count = 0;
    matter_led->blink_on_time_ms = 0;
    matter_led->blink_off_time_ms = 0;
    matter_led->dimmable = dimmable;
    matter_led->brightness = brightness;

    if (dimmable)
    {
        Pad_Config(matter_led_db->led_pwm_param[index].pin_num, PAD_PINMUX_MODE, PAD_IS_PWRON,
                   PAD_PULL_DOWN, PAD_OUT_DISABLE,
                   PAD_OUT_HIGH);
        Pinmux_Config(matter_led_db->led_pwm_param[index].pin_num,
                      matter_led_db->led_pwm_param[index].pin_func);
        /* TIM */
        TIM_TimeBaseInitTypeDef TIM_InitStruct;
        TIM_StructInit(&TIM_InitStruct);
        TIM_InitStruct.TIM_PWM_En = ENABLE;
        /*<! PWM output freqency = 40M/(TIM_PWM_High_Count + TIM_PWM_Low_Count) */
        /*<! PWM duty cycle = TIM_PWM_High_Count/(TIM_PWM_High_Count + TIM_PWM_Low_Count) */
        uint32_t high_count;
        if (0xffff == brightness)
        {
            high_count = LED_PWM_COUNT;
        }
        else
        {
            high_count = (LED_PWM_COUNT / 65535.0) * brightness;
        }

        TIM_InitStruct.TIM_PWM_High_Count = high_count;
        TIM_InitStruct.TIM_PWM_Low_Count = LED_PWM_COUNT - high_count;
        TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
        TIM_TimeBaseInit(matter_led_db->led_pwm_param[index].timer_type, &TIM_InitStruct);
        /* Enable PWM output */
        TIM_Cmd(matter_led_db->led_pwm_param[index].timer_type, ENABLE);
    }
    else
    {
        Pad_Config(matter_led_db->led_pwm_param[index].pin_num, PAD_PINMUX_MODE, PAD_IS_PWRON,
                   PAD_PULL_DOWN, PAD_OUT_DISABLE,
                   PAD_OUT_HIGH);
        Pinmux_Config(matter_led_db->led_pwm_param[index].pin_num, DWGPIO);

        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_StructInit(&GPIO_InitStruct);
        GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
        GPIO_InitStruct.GPIO_ITCmd  = DISABLE;
        GPIO_InitStruct.GPIO_Pin    = GPIO_GetPin(matter_led_db->led_pwm_param[index].pin_num);
        GPIO_Init(GPIO_GetPort(matter_led_db->led_pwm_param[index].pin_num), &GPIO_InitStruct);
        GPIO_ResetBits(GPIOA, GPIO_GetPin(matter_led_db->led_pwm_param[index].pin_num));
    }

    os_timer_create(&matter_led->led_timer, "led timer", (uint32_t)matter_led,
                    1000, false, matter_led_timeout_cb);

    os_queue_in(&matter_led_db->leds, matter_led);

    return matter_led;

fail_alloc_led:
fail_check_pin:
fail_check_param:
    MATTER_PRINT_ERROR1("matter_led_create: failed %d", -ret);
    return NULL;
}

bool matter_led_release(T_MATTER_LED_HANDLE handle)
{
    T_MATTER_LED *matter_led;
    int32_t       ret = 0;

    matter_led = (T_MATTER_LED *)handle;
    if (os_queue_search(&matter_led_db->leds, matter_led) == false)
    {
        ret = 1;
        goto fail_check_handle;
    }

    os_timer_delete(&matter_led->led_timer);
    os_queue_delete(&matter_led_db->leds, matter_led);
    os_mem_free(matter_led);

    return true;

fail_check_handle:
    MATTER_PRINT_ERROR2("matter_led_release: handle %p, failed %d", matter_led, -ret);
    return false;
}

bool matter_led_state_set(T_MATTER_LED_HANDLE handle, bool state)
{
    T_MATTER_LED *matter_led;
    int32_t       ret = 0;

    matter_led = (T_MATTER_LED *)handle;
    if (os_queue_search(&matter_led_db->leds, matter_led) == false)
    {
        ret = 1;
        goto fail_check_handle;
    }

    os_timer_stop(&matter_led->led_timer);
    matter_led_state_do_set(matter_led, state);

    return true;

fail_check_handle:
    MATTER_PRINT_ERROR2("matter_led_state_set: handle %p, failed %d", matter_led, -ret);
    return false;
}

bool matter_led_blink_start(T_MATTER_LED_HANDLE handle,
                            uint32_t            on_time_ms,
                            uint32_t            off_time_ms,
                            uint16_t            count)
{
    T_MATTER_LED *matter_led;
    int32_t       ret = 0;

    matter_led = (T_MATTER_LED *)handle;
    if (os_queue_search(&matter_led_db->leds, matter_led) == false)
    {
        ret = 1;
        goto fail_check_handle;
    }

    os_timer_stop(&matter_led->led_timer);
    matter_led->current_count = 0;
    matter_led->target_count = count;
    matter_led->blink_on_time_ms = on_time_ms;
    matter_led->blink_off_time_ms = off_time_ms;

    if (on_time_ms != 0 && off_time_ms != 0)
    {
        matter_led_state_do_set(matter_led, !matter_led->state);
        os_timer_restart(&matter_led->led_timer, matter_led->state ? on_time_ms : off_time_ms);
    }

    return true;

fail_check_handle:
    MATTER_PRINT_ERROR2("matter_led_blink_start: handle %p, failed %d", matter_led, -ret);
    return false;
}

bool matter_led_blink_stop(T_MATTER_LED_HANDLE handle)
{
    T_MATTER_LED *matter_led;
    int32_t       ret = 0;

    matter_led = (T_MATTER_LED *)handle;
    if (os_queue_search(&matter_led_db->leds, matter_led) == false)
    {
        ret = 1;
        goto fail_check_handle;
    }

    os_timer_stop(&matter_led->led_timer);
    matter_led->current_count = 0;
    matter_led->target_count = 0;
    matter_led_state_do_set(matter_led, false);

    return true;

fail_check_handle:
    MATTER_PRINT_ERROR2("matter_led_blink_stop: handle %p, failed %d", matter_led, -ret);
    return false;
}

bool matter_led_brightness_get(T_MATTER_LED_HANDLE handle, uint16_t *brightness)
{
    T_MATTER_LED *matter_led;
    int32_t       ret = 0;

    matter_led = (T_MATTER_LED *)handle;
    if (os_queue_search(&matter_led_db->leds, matter_led) == false)
    {
        ret = 1;
        goto fail_check_handle;
    }

    *brightness = matter_led->brightness;

    return true;

fail_check_handle:
    MATTER_PRINT_ERROR2("matter_led_brightness_set: handle %p, failed %d", matter_led, -ret);
    return false;
}

bool matter_led_brightness_set(T_MATTER_LED_HANDLE handle, uint16_t brightness)
{
    T_MATTER_LED *matter_led;
    uint32_t      high_count;
    int32_t       ret = 0;

    matter_led = (T_MATTER_LED *)handle;
    if (os_queue_search(&matter_led_db->leds, matter_led) == false)
    {
        ret = 1;
        goto fail_check_handle;
    }

    if (0xffff == brightness)
    {
        high_count = LED_PWM_COUNT;
    }
    else
    {
        high_count = (LED_PWM_COUNT / 65535.0) * brightness;
    }
    TIM_PWMChangeFreqAndDuty(matter_led_db->led_pwm_param[matter_led->pin_index].timer_type, high_count,
                             LED_PWM_COUNT - high_count);

    matter_led->brightness = brightness;

    return true;

fail_check_handle:
    MATTER_PRINT_ERROR2("matter_led_brightness_set: handle %p, failed %d", matter_led, -ret);
    return false;
}

#ifdef __cplusplus
}
#endif