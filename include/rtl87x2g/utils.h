/**
************************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
************************************************************************************************************
* @file     utils.h
* @brief    utility helper function for user application
* @author   lory_xu
* @date     2017-02
* @version  v1.0
*************************************************************************************************************
*/

#ifndef _UTILS_NS_H_
#define _UTILS_NS_H_

#ifdef __cplusplus
extern "C" {
#endif
/*============================================================================*
 *                               Header Files
*============================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include "nsc/utils_nsc.h"

/** @defgroup PLATFORM_UTILS  Platform Utils
  * @brief
  * @{
  */

/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup PLATFORM_UTILS_Exported_Macros Platform Utils Exported Macros
  * @brief
  * @{
  */

#ifndef ABS
#define ABS(x)     (((x) > 0) ? (x) : ((~(x))+1))
#endif

#define BIT64(_n)   (1ULL << (_n))

#define DIVIDE_AND_ROUND(dividend, divisor)     (((dividend) + ((divisor) >> 1)) / (divisor))

#define DIVIDE_AND_ROUNDUP(dividend, divisor)   (((dividend) + ((divisor) - 1)) / (divisor))

#define CLOCK_COUNT_DIFF(clk_begin, clk_end, bit_mask) (((((uint64_t)clk_end & ((uint64_t)bit_mask - 1)) | (uint64_t)bit_mask) - ((uint64_t)clk_begin & ((uint64_t)bit_mask - 1))) & ((uint64_t)bit_mask - 1))

#define VENDOR_COUNTER_BASE_ADDRESS 0x40006000

#define VENDOR_COUNTER_READ(Vendor_offset) \
    ((uint32_t)*((volatile uint32_t*)(VENDOR_COUNTER_BASE_ADDRESS+(Vendor_offset))))
#define VENDOR_COUNTER_WRITE(Vendor_offset, Value) \
    ((*((volatile uint32_t*)(VENDOR_COUNTER_BASE_ADDRESS + (Vendor_offset)))) = (Value))

#define CPU_CNT_BIT_NUM             BIT64(32)

#define PF_RTC_BIT_NUM              BIT64(32)

#define PLATFORM_ASSERT(x) if( ( x ) == 0 ) { vAssertHandler(__FUNCTION__, __LINE__); }

#define BUILD_BUG_ON_ZERO(e)  (sizeof(struct { int _; int:-!!(e); }) - sizeof(struct { int _; }))
#ifndef SAME_TYPE
#define SAME_TYPE(a, b)  __builtin_types_compatible_p(typeof(a), typeof(b))
#endif
#define MUST_BE_ARRAY(a)  BUILD_BUG_ON_ZERO(SAME_TYPE((a), &(*a)))

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) ((sizeof(a) / sizeof(*a)) + MUST_BE_ARRAY(a))
#endif
#ifndef hex2char
#define hex2char(val) ("0123456789abcdef"[(val) & 0xf])
#endif

#define count_1bits(x) (uint8_t)__builtin_popcount(x)
#define count_0bits(x) (32 - count_1bits(x))
#define check_inverse(data, inverse_data, mask) ((data ^ inverse_data) == mask)
#define SET_BIT(val, mask)          ((val) |=  (mask))
#define CLR_BIT(val, mask)          ((val)  &= ~(mask))

#define US_TO_CPU_CLK(us, residual)\
    cal_quotient_remainder(1000000, get_SystemCpuClock(), us, &residual)

#define CPU_CLK_TO_US(cpu_clk)\
    cal_quotient_remainder(get_SystemCpuClock(), 1000000, cpu_clk, NULL)

/** End of PLATFORM_UTILS_Exported_Macros
  * @}
  */

/*============================================================================*
 *                              Types
 *============================================================================*/
/** @defgroup PLATFORM_UTILS_Exported_Types Platform Utils Exported Types
  * @brief
  * @{
  */

typedef enum
{
    ROUND                   = 0,
    ROUNDUP                 = 1,
    ROUNDDOWN               = 2,
} RoundType;

typedef enum
{
    SYSTICK_EXTERNAL_CLOCK   = 0,
    SYSTICK_PROCESSOR_CLOCK  = 1
} SYSTICK_CLK_SRC_TYPE;

/** End of PLATFORM_UTILS_Exported_Types
  * @}
  */

/*============================================================================*
*                              Functions
*============================================================================*/
/** @defgroup PLATFORM_UTILS_Exported_Functions Platform Utils Exported Functions
  * @brief
  * @{
  */

#if   defined ( __CC_ARM )
/**
 * @brief Busy delay for specified micro second
 * @param t   to specify t micro seconds to delay
 * @return none
 */
extern volatile void (*platform_delay_ms)(uint32_t t);
/**
 * @brief Busy delay for specified millisecond
 * @param t   to specify t milliseconds to delay
 * @return none
 */
extern volatile void (*platform_delay_us)(uint32_t t);
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) || defined (__GNUC__)
/**
 * @brief Busy delay for specified micro second
 * @param t   to specify t micro seconds to delay
 * @return none
 */
extern void (*platform_delay_ms)(uint32_t t);
/**
 * @brief Busy delay for specified millisecond
 * @param t   to specify t milliseconds to delay
 * @return none
 */
extern void (*platform_delay_us)(uint32_t t);
#endif

/**
 * @brief Function pointer to calculate quotient and remainder of two 64-bit integers.
 * @return The quotient
 */
extern uint32_t (*cal_quotient_remainder)(const uint64_t, const uint64_t, const uint32_t,
                                          uint32_t *);

/**
 * @brief An empty function that performs no operations.
 * @param  void
 * @return void
 */
void empty_function(void);

/**
 * @brief A function that always returns true.
 * @param  void
 * @return true
 */
bool true_function(void);

/**
 * @brief A function that returns a void pointer.
 * @param  void
 * @return A void pointer.
 */
void *void_ptr_function(void);

/**
 * @brief Assert handler function for handling assertion failures.
 *
 * @param pFuncName The name of the function where the assert failed.
 * @param funcLine The line number where the assert failed.
 * @return void
 */
extern void vAssertHandler(const char *pFuncName, uint32_t funcLine);

/**
 * @brief Reverse the bits in a 32-bit word.
 * @param num The 32-bit integer whose bits are to be reversed.
 * @return The 32-bit integer with reversed bits.
 */
uint32_t reverse_bits_in_word(uint32_t num);

/**
 * @brief Check if two ranges overlap.
 * @param range0 An array of two 32-bit integers representing the first range.
 * @param range1 An array of two 32-bit integers representing the second range.
 * @return true if the ranges overlap, false otherwise.
 */
bool is_overlapped(uint32_t range0[2], uint32_t range1[2]);

/**
 * @brief Calculate the difference between two clock counts considering a bit mask.
 * @param clk_begin The starting clock count.
 * @param clk_end The ending clock count.
 * @param bit_mask The bit mask to apply to the clock counts.
 * @return The difference between the two clock counts.
 */
uint64_t clk_cnt_diff(uint64_t clk_begin, uint64_t clk_end, uint64_t bit_mask);

/**
 * @brief Compare two clock counts considering a third comparison clock count and a bit mask.
 * @param clk_begin The base clock count.
 * @param clk_end_1 The first clock count to compare.
 * @param clk_end_2 The second clock count to compare.
 * @param bit_mask The bit mask to apply to the clock counts.
 * @return true if clk_end_1 is closer to clk_begin than clk_end_2, false otherwise.
 */
bool clk_cnt_cmp(uint64_t clk_begin, uint64_t clk_end_1, uint64_t clk_end_2, uint64_t bit_mask);

/**
 * @brief Convert microseconds to RTC clock ticks.
 * @param us The time in microseconds.
 * @param round_type The rounding type to apply (e.g., round up or down).
 * @return The equivalent time in RTC clock ticks.
 */
uint32_t us_to_rtc_clk(uint32_t us, RoundType round_type);

/**
 * @brief Convert RTC clock ticks to microseconds.
 * @param rtc_clk The time in RTC clock ticks.
 * @param round_type The rounding type to apply (e.g., round up or down).
 * @return The equivalent time in microseconds.
 */
uint32_t rtc_clk_to_us(uint32_t rtc_clk, RoundType round_type);

/**
 * @brief Initialize the DWT cycle counter.
 * @param  void
 * @return void
 */
void dwt_cyccnt_init(void);

/**
 * @brief Read the CPU counter value.
 * @param  void
 * @return The current value of the CPU counter.
 */
uint32_t read_cpu_counter(void);

/**
 * @brief Get the source type of the system tick clock.
 * @param  void
 * @return The source type of the system tick clock.
 */
SYSTICK_CLK_SRC_TYPE get_sys_tick_clk_type(void);

/**
 * @brief Get the current system core clock frequency.
 * @param  void
 * @return The system core clock frequency in Hz.
 */
uint32_t get_SystemCoreClock(void);

/**
 * @brief Get the current system cpu clock frequency.
 * @param  void
 * @return The system cpu clock frequency in Hz.
 */
extern uint32_t (*get_SystemCpuClock)(void);

/**
 * @brief Function pointer to get the stack enable status.
 * @param  void
 * @return The stack enable status.
 */
extern uint32_t (*get_stack_en)(void);

/**
 * @brief Update the CPU tick to microsecond conversion factor based on the CPU frequency.
 * @param cpu_freq The CPU frequency in Hz.
 * @return void
 */
void update_cpu_tick_to_us(uint32_t cpu_freq);


#define IS_USE_VHCI (get_stack_en())

/**
 * @brief Generate random number given max number allowed
 * @param max   to specify max number that allowed
 * @return random number
 */

uint32_t platform_random(uint32_t max);

/** End of PLATFORM_UTILS_Exported_Functions
  * @}
  */

/** End of PLATFORM_UTILS
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif

