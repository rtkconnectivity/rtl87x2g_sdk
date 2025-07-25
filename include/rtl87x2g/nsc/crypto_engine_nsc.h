/**
*****************************************************************************************
*     Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    crypto_engine_nsc.h
  * @brief
  * @author
  * @date    2024.7.17
  * @version v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2024 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */


/*============================================================================*
 *                      Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _CRYPTO_ENGINE_NSC_H_
#define _CRYPTO_ENGINE_NSC_H_

#include <stdint.h>
#include <stdbool.h>

/** @defgroup CRYPTO_ENGINE Crypto Engine
  * @brief
  * @{
  */

/*============================================================================*
 *                              Types
 *============================================================================*/

/**
  * @cond     private
  * @defgroup CRYPTO_ENGINE_Private_Types
  * @{
  */

/* public key engine */
typedef enum
{
    ERR_NONE            = 0x0,
    ERR_PRIME           = 0x1,
    ERR_R2_MOD_N        = 0x2,
    ERR_ECC_ODD_POINT   = 0x4,
    ERR_ECC_Z           = 0x8,
    ERR_MODULAR_INV     = 0x10,
    ERR_N_ST_INPUT      = 0x20,
    ERR_NO_VALID_EXP    = 0x40,
    ERR_INVALID_INPUT   = 0x80,
} ERR_CODE;

typedef enum
{
    ECC_PRIME_MODE       = 0,   // 3'b000
    ECC_BINARY_MODE      = 1,   // 3'b001
    RSA_MODE             = 2,   // 3'b010
    ECC_EDWARDS_CURVE    = 3,   // 3'b011
    ECC_MONTGOMERY_CURVE = 7,   // 3'b111
} PKE_MODE;

typedef struct
{
    uint32_t x[16];
    uint32_t y[16];
    uint32_t z[16];
} ECC_POINT;

typedef struct
{
    uint32_t *N;    // modular, also called p
    uint32_t *A;    // curve parameter a
    uint32_t *B;    // curve parameter b
    uint32_t *n;    // order of G
    ECC_POINT G;    // base point

    uint32_t key_bits;
    PKE_MODE mode;

} ECC_GROUP;

/** End of CRYPTO_ENGINE_Private_Types
  * @}
  * @endcond
  */

/** @defgroup CRYPTO_ENGINE_Exported_Types Crypto Engine Exported Types
  * @brief
  * @{
  */

/**
 * @brief  Use mode for HW sha256.
 */
typedef enum
{
    HW_SHA256_CPU_MODE,         /*!< Use CPU mode for HW sha256.  */
    HW_SHA256_DMA_MODE          /*!< Use DMA mode for HW sha256.  */
} HW_SHA256_ACCESS_MODE;

/**
 * @brief  Context for HW sha256.
 */
typedef struct
{
    uint32_t total[2];          /*!< The number of Bytes processed.  */
    uint32_t state[8];          /*!< The intermediate digest state.  */
    uint8_t buffer[64];         /*!< The data block being processed. */
    int is224;                  /*!< Unused, just align mbedtls structure */
} HW_SHA256_CTX;

/** End of CRYPTO_ENGINE_Exported_Types
  * @}
  */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup CRYPTO_ENGINE_Exported_Functions Crypto Engine Exported Functions
  * @brief
  * @{
  */
/**
 * @brief  Hardware sha256 process.
 * @param[in]  input    The addr of input buffer.
 * @param[in]  byte_len The size of input buffer, unit is byte.
 * @param[out] result   The addr of output buffer.
 * @param[in]  mode     Use mode for Hardware sha256, @ref HW_SHA256_ACCESS_MODE . If mode is DMA, Channel 3 is used default.
 *
 * @return          Result of Hardware sha256 process.
 * @retval true     Hardware sha256 process is successful.
 * @retval false    Hardware sha256 fails to process. When the parameters are invalid,
 *                  it can lead to calculation errors. If an error is returned,
 *                  users need to recheck whether the parameters are valid.
 * <b>Example usage</b>
 * \code{.c}
 *
 * // HW Sha256 test.
 * bool sha256_test(void)
 * {
 *     int input_len = 0;
 *     uint8_t  *input_buf;
 *     uint32_t sha256sum[32];
 *
 *     input_buf = (uint8_t *)TEST_BUF_ADDR;
 *     input_len = TEST_BUF_SIZE;
 *
 *     bool ret = hw_sha256(input_buf, input_len, sha256sum, HW_SHA256_CPU_MODE);
 *
 *     return ret;
 * }
 * \endcode
 *
 */
bool hw_sha256(uint8_t *input, uint32_t byte_len, uint32_t *result, int mode);

/**
 * @brief  Hardware sha256 init.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * // HW Sha256 test.
 * bool sha256_test(void)
 * {
 *     int input_len = 0;
 *     uint8_t  *input_buf;
 *     uint32_t sha256sum[32];
 *
 *     uint16_t loop_cnt, remain_size;
 *     uint8_t *pdata = (uint8_t *)TEST_BUF_ADDR;
 *
 *     input_len = TEST_BUF_SIZE;
 *     loop_cnt = input_len / SHA256_BUFFER_SIZE;
 *     remain_size = input_len % SHA256_BUFFER_SIZE;
 *
 *     HW_SHA256_CTX hw_sha256_ctx = {NULL};
 *
 *     hw_sha256_init();
 *
 *     hw_sha256_start(hw_sha256_ctx, NULL);
 *
 *     for (i = 0; i < loop_cnt; ++i)
 *     {
 *         memcpy(input_buf, pdata, SHA256_BUFFER_SIZE);
 *         hw_sha256_cpu_update(&hw_sha256_ctx, input_buf, SHA256_BUFFER_SIZE);
 *         pdata += SHA256_BUFFER_SIZE;
 *     }
 *
 *     if (remain_size)
 *     {
 *         memcpy(input_buf, pdata, remain_size);
 *         hw_sha256_cpu_update(&hw_sha256_ctx, input_buf, remain_size);
 *     }
 *
 *     hw_sha256_finish(&hw_sha256_ctx, sha256sum);
 *
 *     return ret;
 * }
 * \endcode
 *
 */
void hw_sha256_init(void);

/**
 * @brief  Hardware sha256 start.
 * @param[in]  ctx      The pointer of Hardware sha256 context.
 * @param[in]  iv       The pointer of iv for Hardware sha256. In sha256 start process, it should always be NULL.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * // HW Sha256 test.
 * bool sha256_test(void)
 * {
 *     int input_len = 0;
 *     uint8_t  *input_buf;
 *     uint32_t sha256sum[32];
 *
 *     uint16_t loop_cnt, remain_size;
 *     uint8_t *pdata = (uint8_t *)TEST_BUF_ADDR;
 *
 *     input_len = TEST_BUF_SIZE;
 *     loop_cnt = input_len / SHA256_BUFFER_SIZE;
 *     remain_size = input_len % SHA256_BUFFER_SIZE;
 *
 *     HW_SHA256_CTX hw_sha256_ctx = {NULL};
 *
 *     hw_sha256_init();
 *
 *     hw_sha256_start(hw_sha256_ctx, NULL);
 *
 *     for (i = 0; i < loop_cnt; ++i)
 *     {
 *         memcpy(input_buf, pdata, SHA256_BUFFER_SIZE);
 *         hw_sha256_cpu_update(&hw_sha256_ctx, input_buf, SHA256_BUFFER_SIZE);
 *         pdata += SHA256_BUFFER_SIZE;
 *     }
 *
 *     if (remain_size)
 *     {
 *         memcpy(input_buf, pdata, remain_size);
 *         hw_sha256_cpu_update(&hw_sha256_ctx, input_buf, remain_size);
 *     }
 *
 *     hw_sha256_finish(&hw_sha256_ctx, sha256sum);
 *
 *     return ret;
 * }
 * \endcode
 *
 */
void hw_sha256_start(HW_SHA256_CTX *ctx, uint32_t *iv);

/**
 * @brief  Hardware sha256 cpu update.
 * @param[in]  ctx      The pointer of Hardware sha256 context.
 * @param[in]  input    The addr of input buffer.
 * @param[in]  byte_len The size of input buffer, unit is byte.
 *
 * @return          Result of Hardware sha256 update.
 * @retval true     Hardware sha256 cpu update is successful.
 * @retval false    Hardware sha256 cpu update fails to process. When the parameters are invalid,
 *                  it can lead to calculation errors. If an error is returned,
 *                  users need to recheck whether the parameters are valid.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * // HW Sha256 test.
 * bool sha256_test(void)
 * {
 *     int input_len = 0;
 *     uint8_t  *input_buf;
 *     uint32_t sha256sum[32];
 *
 *     uint16_t loop_cnt, remain_size;
 *     uint8_t *pdata = (uint8_t *)TEST_BUF_ADDR;
 *
 *     input_len = TEST_BUF_SIZE;
 *     loop_cnt = input_len / SHA256_BUFFER_SIZE;
 *     remain_size = input_len % SHA256_BUFFER_SIZE;
 *
 *     HW_SHA256_CTX hw_sha256_ctx = {NULL};
 *
 *     hw_sha256_init();
 *
 *     hw_sha256_start(hw_sha256_ctx, NULL);
 *
 *     for (i = 0; i < loop_cnt; ++i)
 *     {
 *         memcpy(input_buf, pdata, SHA256_BUFFER_SIZE);
 *         hw_sha256_cpu_update(&hw_sha256_ctx, input_buf, SHA256_BUFFER_SIZE);
 *         pdata += SHA256_BUFFER_SIZE;
 *     }
 *
 *     if (remain_size)
 *     {
 *         memcpy(input_buf, pdata, remain_size);
 *         hw_sha256_cpu_update(&hw_sha256_ctx, input_buf, remain_size);
 *     }
 *
 *     hw_sha256_finish(&hw_sha256_ctx, sha256sum);
 *
 *     return ret;
 * }
 * \endcode
 *
 */
bool hw_sha256_cpu_update(HW_SHA256_CTX *ctx, uint8_t *input, uint32_t byte_len);

/**
 * @brief  Hardware sha256 finish.
 * @param[in]  ctx      The pointer of Hardware sha256 context.
 * @param[out]  result  The addr of output buffer.
 *
 * @return          Result of Hardware sha256 finish.
 * @retval true     Hardware sha256 finish is successful.
 * @retval false    Hardware sha256 finish fails to process. When the parameters are invalid,
 *                  it can lead to calculation errors. If an error is returned,
 *                  users need to recheck whether the parameters are valid.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * // HW Sha256 test.
 * bool sha256_test(void)
 * {
 *     int input_len = 0;
 *     uint8_t  *input_buf;
 *     uint32_t sha256sum[32];
 *
 *     uint16_t loop_cnt, remain_size;
 *     uint8_t *pdata = (uint8_t *)TEST_BUF_ADDR;
 *
 *     input_len = TEST_BUF_SIZE;
 *     loop_cnt = input_len / SHA256_BUFFER_SIZE;
 *     remain_size = input_len % SHA256_BUFFER_SIZE;
 *
 *     HW_SHA256_CTX hw_sha256_ctx = {NULL};
 *
 *     hw_sha256_init();
 *
 *     hw_sha256_start(hw_sha256_ctx, NULL);
 *
 *     for (i = 0; i < loop_cnt; ++i)
 *     {
 *         memcpy(input_buf, pdata, SHA256_BUFFER_SIZE);
 *         hw_sha256_cpu_update(&hw_sha256_ctx, input_buf, SHA256_BUFFER_SIZE);
 *         pdata += SHA256_BUFFER_SIZE;
 *     }
 *
 *     if (remain_size)
 *     {
 *         memcpy(input_buf, pdata, remain_size);
 *         hw_sha256_cpu_update(&hw_sha256_ctx, input_buf, remain_size);
 *     }
 *
 *     hw_sha256_finish(&hw_sha256_ctx, sha256sum);
 *
 *     return ret;
 * }
 * \endcode
 *
 */
bool hw_sha256_finish(HW_SHA256_CTX *ctx, uint32_t *result);

/** End of CRYPTO_ENGINE_Exported_Functions
  * @}
  */

/** End of CRYPTO_ENGINE
  * @}
  */

#endif