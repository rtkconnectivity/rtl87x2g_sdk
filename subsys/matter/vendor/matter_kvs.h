/**
*********************************************************************************************************
*               Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* \file     matter_kvs.h
* \brief    This file provides key value storage functions.
* \details
* \author   rock ding
* \date     2024-04-17
* \version  v1.0
*********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef MATTER_KVS_H
#define MATTER_KVS_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "matter_types.h"

/*============================================================================*
 *                         Constants
 *============================================================================*/
/** \defgroup Matter_KVS_Exported_Constants Matter KVS Exported Constants
  * \brief
  * \{
  */

/**
 * \defgroup    Matter_KVS_Status Matter KVS Status
 * \{
 * \ingroup     Matter_KVS_Exported_Constants
 */
#define MATTER_KVS_STATUS_NO_ERROR         0
#define MATTER_KVS_HASH_INVALID            1
#define MATTER_KVS_LOOKUP_NOT_FOUND        2
#define MATTER_KVS_BUFFER_TOO_SMALL        3

/** End of Matter_KVS_Status
  * \}
  */

/** End of Matter_KVS_Exported_Constants
  * \}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** \defgroup Matter_KVS_Exported_Functions Matter KVS Exported Functions
  * \brief
  * \{
  */

/**
  * @brief  Initialize the matter kvs module.
  *
  * @retval 0  On success.
  * @retval -1 On failure.
  */
int32_t matter_kvs_init(void);

/**
  * @brief  Clean the matter kvs module, all the key-value entries will be deleted.
  *
  * @retval none.
  */
void matter_kvs_clean(void);

/**
  * @brief  Adds a key-value entry to the KVS. If the key was already present, its value is overwritten.
  * @param[in]  key          The name of the key to update, this is a null-terminated string.
  * @param[in]  value        Pointer to the data.
  * @param[in]  valueSize    Size of the data.
  *
  * @retval #MATTER_KVS_STATUS_NO_ERROR On success.
  * @retval Other value on failure.
  */
int32_t matter_kvs_put(const char *key, const void *value, size_t valueSize);

/**
  * @brief  Reads the value of an entry in the KVS. The value is read into the
  *         provided buffer and the number of bytes read is returned.
  * @param[in]      key                 The name of the key to get, this is a null-terminated string.
  * @param[in,out]  buffer              A buffer to read the value into.
  * @param[in]      buffer_size         The size of the buffer in bytes.
  * @param[out]     read_bytes_size     The number of bytes which were copied into the buffer.
  *
  * @retval #MATTER_KVS_STATUS_NO_ERROR   On success.
  * @retval #MATTER_KVS_LOOKUP_NOT_FOUND  The key is not present in the KVS.
  */
int32_t matter_kvs_get(const char *key, void *buffer, size_t buffer_size, size_t *read_bytes_size);

/**
  * @brief  Removes a key-value entry from the KVS.
  * @param[in]  key   The name of the key to delete, this is a null-terminated string.
  *
  * @retval #MATTER_KVS_STATUS_NO_ERROR   On success.
  * @retval #MATTER_KVS_LOOKUP_NOT_FOUND  The key is not present in the KVS.
  */
int32_t matter_kvs_key_delete(const char *key);

/**
  * @brief  Check if a key exists in the KVS.
  * @param[in]  key   The name of the key to find, this is a null-terminated string.
  *
  * @retval #true  On success.
  * @retval #false The key is not present in the KVS.
  */
bool matter_kvs_key_find(const char *key);



/**
  * Dispatch and execute the command for the given argument list.
  *
  * @param[in] aContext           No use
  * @param[in] argc               Number of arguments in argv.
  * @param[in] argv               Array of arguments in the tokenized command line to execute.
  *
  * @retval                       success or failure
  */
int32_t matter_kvs_cmd_handle(void *aContext, int argc, char *argv[]);

/** End of Matter_KVS_Exported_Functions
  * \}
  */


#ifdef __cplusplus
}
#endif

#endif /*MATTER_KVS_H*/


/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/
