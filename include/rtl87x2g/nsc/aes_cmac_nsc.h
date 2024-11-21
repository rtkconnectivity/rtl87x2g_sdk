/**
*****************************************************************************************
*     Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    aes_cmac_nsc.h
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
#ifndef _AES_CMAC_H_
#define _AES_CMAC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/** @defgroup AES_CMAC AES CMAC API Sets
  * @brief API sets for aes CMAC encryption implementation
  * @{
  */
/*============================================================================*
  *                                Functions
  *============================================================================*/
/** @defgroup AES_CMAC_API_Exported_Functions AES CMAC API Sets Exported Functions
  * @{
  */
/**
* @brief  Calculate the AES CMAC for a given message
* @param  key          A 16-byte AES key
* @param  msg          pointer to the message data to be authenticated
* @param  msg_len      length of the message data
* @param  res          An array for storing the resulting 16-byte CMAC value
* @return void
*/
void aes_cmac(uint8_t key[16], uint8_t *msg,
              size_t msg_len, uint8_t res[16]);

/** End of AES_CMAC_API_Exported_Functions
  * @}
  */

/** End of AES_CMAC
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* _AES_CMAC_H_ */
