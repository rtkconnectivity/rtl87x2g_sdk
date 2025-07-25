/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    aes_api.h
  * @brief   AES APIs abstracted for encryption related.
  * @details AES encryption APIs which deliver HW/SW implemented reliable and safe AES solution.
  * @author  Abel
  * @date    2017.5.17
  * @version v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */


/*============================================================================*
 *                      Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _AES_API_H_
#define _AES_API_H_

/*============================================================================*
 *                               Header Files
*============================================================================*/
#include <stdint.h>
#include <stdbool.h>

#define AES256 1
#define ECB    1
#define CBC    1
/** @defgroup AES_API AES API Sets
  * @brief API sets for AES encryption implementation
  * @{
  */

/*============================================================================*
  *                                Structures
  *============================================================================*/
/** @defgroup AES_API_Exported_Types AES API Sets Exported Types
  * @brief
  * @{
  */
typedef struct
{
    uint8_t *input;
    uint8_t *iv;
    uint32_t word_len;
} AES_CBC_CTX;

typedef enum
{
    CRYPTO_ERR_SUCCESS = 0,
    CRYPTO_ERR_MODE_NOT_SUPPORT,
    CRYPTO_ERR_KEY_IV_NOT_EXIST,
    CRYPTO_ERR_KEY_LEN_MISMATCH,
    CRYPTO_ERR_DATA_LEN_NOT_16B_ALIGNED,

    CRYPTO_ERR_MAX = 0xFF
} crypto_err_t;

/** End of AES_API_Exported_Types
  * @}
  */

/*============================================================================*
  *                                Functions
  *============================================================================*/
/** @defgroup AES_API_Exported_Functions AES API Sets Exported Functions
  * @{
  */
/**
    * @brief  byte swap
    * @param  src          source address
    * @param  dst          destination address
    * @param  len          swap size
    */

void swap_buf(const uint8_t *src, uint8_t *dst, uint16_t len);

/**
    * @brief  128 bit AES ECB encryption on specified plaintext and keys
    * @note   least significant octet of encrypted data corresponds to encrypted[0]
    * @param  plaintext    specified plain text to be encrypted
    * @param  key          keys to encrypt the plaintext
    * @param  encrypted    output buffer to store encrypted data
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    */
bool aes128_ecb_encrypt(uint8_t plaintext[16], const uint8_t key[16], uint8_t *encrypted);
/**
    * @brief  128 bit AES ECB encryption on specified plaintext and keys
    * @note   most significant octet of encrypted data corresponds to encrypted[0]
    * @param  plaintext    specified plain text to be encrypted
    * @param  key          keys to encrypt the plaintext
    * @param  encrypted    output buffer to store encrypted data
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    */
bool aes128_ecb_encrypt_msb2lsb(uint8_t plaintext[16], const uint8_t key[16],
                                uint8_t *encrypted);

/**
    * @brief  128 bit AES ECB decryption on specified encrypted data and keys
    * @note   least significant octet of encrypted data corresponds to encrypted[0]
    * @param  input    specified encrypted data to be decrypted
    * @param  key          keys to decrypt the data
    * @param  output    output buffer to store plain data
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    */
bool aes128_ecb_decrypt(uint8_t *input, const uint8_t *key, uint8_t *output);
/**
    * @brief  128 bit AES ECB decryption on specified encrypted data and keys
    * @note   most significant octet of encrypted data corresponds to encrypted[0]
    * @param  input    specified encrypted data to be decrypted
    * @param  key          keys to decrypt the data
    * @param  output    output buffer to store plain data
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    */
bool aes128_ecb_decrypt_msb2lsb(uint8_t *input, const uint8_t *key, uint8_t *output);

/**
    * @brief  128 bit AES CBC encryption on specified plaintext and keys
    * @note   least significant octet of encrypted data corresponds to encrypted[0]
    * @param  cbc_ctx   specified plain text to be encrypted, iv and length in words
    * @param  key          keys to decrypt the data
    * @param  encrypted    output buffer to store encrypted data
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    */
bool aes128_cbc_encrypt(AES_CBC_CTX *cbc_ctx, const uint8_t *key, uint8_t *encrypted);

/**
    * @brief  128 bit AES CBC decryption on specified encrypted data and keys
    * @note   least significant octet of encrypted data corresponds to encrypted[0]
    * @param  cbc_ctx   specified encrypted data to be decrypted, iv and length in words
    * @param  key       keys to decrypt the data
    * @param  output    output buffer to store plain data
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    */
bool aes128_cbc_decrypt(AES_CBC_CTX *cbc_ctx, const uint8_t *key, uint8_t *output);

/**
    * @brief  256 bit AES ECB encryption on specified plaintext and keys
    * @note   least significant octet of encrypted data corresponds to encrypted[0]
    * @param  plaintext    specified plain text to be encrypted
    * @param  key          keys to encrypt the plaintext
    * @param  encrypted    output buffer to store encrypted data
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    */
bool aes256_ecb_encrypt(uint8_t plaintext[16], const uint8_t key[32], uint8_t *encrypted);
/**
    * @brief  256 bit AES ECB encryption on specified plaintext and keys
    * @note   most significant octet of encrypted data corresponds to encrypted[0]
    * @param  plaintext    specified plain text to be encrypted
    * @param  key          keys to encrypt the plaintext
    * @param  encrypted    output buffer to store encrypted data
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    */
bool aes256_ecb_encrypt_msb2lsb(uint8_t plaintext[16], const uint8_t key[32],
                                uint8_t *encrypted);
/**
    * @brief  256 bit AES ECB decryption on specified encrypted data and keys
    * @note   least significant octet of encrypted data corresponds to encrypted[0]
    * @param  input    specified encrypted data to be decrypted
    * @param  key          keys to decrypt the data
    * @param  output    output buffer to store plain data
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    */
bool aes256_ecb_decrypt(uint8_t *input, const uint8_t *key, uint8_t *output);
/**
    * @brief  256 bit AES ECB decryption on specified encrypted data and keys
    * @note   most significant octet of encrypted data corresponds to encrypted[0]
    * @param  input    specified encrypted data to be decrypted
    * @param  key          keys to decrypt the data
    * @param  output    output buffer to store plain data
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    */
bool aes256_ecb_decrypt_msb2lsb(uint8_t *input, const uint8_t *key, uint8_t *output);

/**
    * @brief  256 bit AES CBC encryption on specified plaintext and keys
    * @note   least significant octet of encrypted data corresponds to encrypted[0]
    * @param  cbc_ctx   specified plain text to be encrypted, iv and length in words
    * @param  key          keys to decrypt the data
    * @param  encrypted    output buffer to store encrypted data
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    */
bool aes256_cbc_encrypt(AES_CBC_CTX *cbc_ctx, const uint8_t *key, uint8_t *encrypted);

/**
    * @brief  256 bit AES CBC decryption on specified encrypted data and keys
    * @note   least significant octet of encrypted data corresponds to encrypted[0]
    * @param  cbc_ctx   specified encrypted data to be decrypted, iv and length in words
    * @param  key       keys to decrypt the data
    * @param  output    output buffer to store plain data
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    */
bool aes256_cbc_decrypt(AES_CBC_CTX *cbc_ctx, const uint8_t *key, uint8_t *output);

/**
    * @brief  128 bit AES CTR encryption on specified plaintext and load key id
    * @note   least significant octet of encrypted data corresponds to data[0]
    * @param  input    input buffer to store plain data
    * @param  key_material_id   represents a key that is stored in the first 4KB of the flash, and it needs to be programmed using the MP Tool.
    * @param  output    output buffer to store encrypted data
    * @param  data_len    length of data
    * @return crypto_err_t results
    */
crypto_err_t aes128_ctr_encrypt_with_load_key(const unsigned char *input, uint8_t key_material_id,
                                              unsigned char *output, uint32_t data_len);

/**
    * @brief  128 bit AES CTR decryption on specified encrypted data and load key id
    * @note   least significant octet of encrypted data corresponds to data[0]
    * @param  input    input buffer to store encrypted data
    * @param  key_material_id   represents a key that is stored in the first 4KB of the flash, and it needs to be programmed using the MP Tool.
    * @param  output    output buffer to store plain data
    * @param  data_len    length of data
    * @return crypto_err_t results
    */
crypto_err_t aes128_ctr_decrypt_with_load_key(const unsigned char *input, uint8_t key_material_id,
                                              unsigned char *output, uint32_t data_len);

/**
    * @brief  256 bit AES CTR encryption on specified plaintext and load key id
    * @note   least significant octet of encrypted data corresponds to data[0]
    * @param  input    input buffer to store plain data
    * @param  key_material_id   represents a key that is stored in the first 4KB of the flash, and it needs to be programmed using the MP Tool.
    * @param  output    output buffer to store encrypted data
    * @param  data_len    length of data
    * @return crypto_err_t results
    */
crypto_err_t aes256_ctr_encrypt_with_load_key(const unsigned char *input, uint8_t key_material_id,
                                              unsigned char *output, uint32_t data_len);

/**
    * @brief  256 bit AES CTR decryption on specified encrypted data and load key id
    * @note   least significant octet of encrypted data corresponds to data[0]
    * @param  input    input buffer to store encrypted data
    * @param  key_material_id   represents a key that is stored in the first 4KB of the flash, and it needs to be programmed using the MP Tool.
    * @param  output    output buffer to store plain data
    * @param  data_len    length of data
    * @return crypto_err_t results
    */
crypto_err_t aes256_ctr_decrypt_with_load_key(const unsigned char *input, uint8_t key_material_id,
                                              unsigned char *output, uint32_t data_len);

/**
    * @brief  128 bit AES CBC encryption on specified plaintext and load key id
    * @note   least significant octet of encrypted data corresponds to data[0]
    * @param  input    input buffer to store plain data
    * @param  key_material_id   represents a key that is stored in the first 4KB of the flash, and it needs to be programmed using the MP Tool.
    * @param  output    output buffer to store encrypted data
    * @param  data_len    length of data which should be 16 bytes aligned
    * @return crypto_err_t results
    */
crypto_err_t aes128_cbc_encrypt_with_load_key(const unsigned char *input, uint8_t key_material_id,
                                              unsigned char *output, uint32_t data_len);

/**
    * @brief  128 bit AES CBC decryption on specified encrypted data and load key id
    * @note   least significant octet of encrypted data corresponds to data[0]
    * @param  input    input buffer to store encrypted data
    * @param  key_material_id   represents a key that is stored in the first 4KB of the flash, and it needs to be programmed using the MP Tool.
    * @param  output    output buffer to store plain data
    * @param  data_len    length of data which should be 16 bytes aligned
    * @return crypto_err_t results
    */
crypto_err_t aes128_cbc_decrypt_with_load_key(const unsigned char *input, uint8_t key_material_id,
                                              unsigned char *output, uint32_t data_len);

/**
    * @brief  256 bit AES CBC encryption on specified plaintext and load key id
    * @note   least significant octet of encrypted data corresponds to data[0]
    * @param  input    input buffer to store plain data
    * @param  key_material_id   represents a key that is stored in the first 4KB of the flash, and it needs to be programmed using the MP Tool.
    * @param  output    output buffer to store encrypted data
    * @param  data_len    length of data which should be 16 bytes aligned
    * @return crypto_err_t results
    */
crypto_err_t aes256_cbc_encrypt_with_load_key(const unsigned char *input, uint8_t key_material_id,
                                              unsigned char *output, uint32_t data_len);

/**
    * @brief  256 bit AES CBC decryption on specified encrypted data and load key id
    * @note   least significant octet of encrypted data corresponds to data[0]
    * @param  input    input buffer to store encrypted data
    * @param  key_material_id   represents a key that is stored in the first 4KB of the flash, and it needs to be programmed using the MP Tool.
    * @param  output    output buffer to store plain data
    * @param  data_len    length of data which should be 16 bytes aligned
    * @return crypto_err_t results
    */
crypto_err_t aes256_cbc_decrypt_with_load_key(const unsigned char *input, uint8_t key_material_id,
                                              unsigned char *output, uint32_t data_len);

/** End of AES_API_Exported_Functions
  * @}
  */

/** End of AES_API
  * @}
  */

#endif
