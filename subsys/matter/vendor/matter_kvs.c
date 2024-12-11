#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"
#include "string.h"
#include "chip_porting.h"
#include "mbedtls/sha256.h"
#include "iot_persistent_config.h"
#include <openthread/cli.h>
#include "kvmgr.h"
#include "aes_api.h"


uint32_t __attribute__((weak)) kvs_addr_get(void)
{
    return KVS_ADDR;
}

extern otError kv_cmd_handler(void *aContext, uint8_t argc, char *argv[]);

int32_t matter_kvs_cmd_handle(void *aContext, int argc, char *argv[])
{
    return kv_cmd_handler(aContext, argc, argv);
}

int32_t matter_kvs_init(void)
{
    uint32_t kvs_addr = kvs_addr_get();
    return aos_kv_init(kvs_addr);
}

void matter_kvs_clean(void)
{
    uint32_t kvs_addr = kvs_addr_get();
    aos_kv_deinit(kvs_addr);
    aos_kv_delall();
}

#define CONFIG_ENABLE_KVS_ENCRYPTION 0
#if CONFIG_ENABLE_KVS_ENCRYPTION

#define KVS_ENCRYPTION_DBG 0

#define AES128_KEY_ID (0)

static int32_t kvs_encrypt(unsigned char *input_to_encrypt, int input_len,
                           unsigned char *encrypt_output)
{
    aes128_ctr_encrypt_with_load_key(input_to_encrypt, AES128_KEY_ID, encrypt_output, input_len);

    return 0;
}

static int32_t kvs_decrypt(unsigned char *input_to_decrypt, int input_len,
                           unsigned char *decrypt_output)
{
    aes128_ctr_decrypt_with_load_key(input_to_decrypt, AES128_KEY_ID, decrypt_output, input_len);
    return 0;
}

int32_t matter_kvs_put(const char *key, const void *value, size_t valueSize)
{
    int32_t ret = MATTER_KVS_STATUS_NO_ERROR;

    void *encrypt_output = calloc(1, valueSize);
    if (encrypt_output == NULL)
    {
        goto exit;
    }

    kvs_encrypt(value, valueSize, encrypt_output);

#if KVS_ENCRYPTION_DBG
    uint8_t *plaintext = calloc(1, valueSize);
    kvs_decrypt(encrypt_output, valueSize, plaintext);
    MATTER_PRINT_INFO2("matter_kvs_put: plaintext %b, decrypted plaintext %b", TRACE_BINARY(16, value),
                       TRACE_BINARY(16, plaintext));

    free(plaintext);
#endif


    if (0 != aos_kv_set(key, encrypt_output, valueSize, 1))
    {
        //APP_PRINT_TRACE1("Bee4_KvsPut kv_set %s failed", TRACE_STRING(key));
        ret = MATTER_KVS_LOOKUP_NOT_FOUND;
    }
    else
    {
        ret = MATTER_KVS_STATUS_NO_ERROR;
    }

exit:
    if (encrypt_output)
    {
        free(encrypt_output);
    }

    return ret;
}

int32_t matter_kvs_get(const char *key, void *buffer, size_t buffer_size, size_t *read_bytes_size)
{
    int32_t ret = MATTER_KVS_STATUS_NO_ERROR;
    int len = buffer_size;

    void *decrypt_input = calloc(1, buffer_size);
    if (decrypt_input == NULL)
    {
        goto exit;
    }

    if (0 != aos_kv_get(key, decrypt_input, &len))
    {
        //APP_PRINT_TRACE1("Bee4_KvsGet kv_get %s failed", TRACE_STRING(key));
        ret = MATTER_KVS_LOOKUP_NOT_FOUND;
    }
    else
    {
        kvs_decrypt(decrypt_input, len, buffer);
        *read_bytes_size = len;
        ret = MATTER_KVS_STATUS_NO_ERROR;
    }

exit:
    if (decrypt_input)
    {
        free(decrypt_input);
    }

    return ret;
}

#else

int32_t matter_kvs_put(const char *key, const void *value, size_t valueSize)
{
    if (0 != aos_kv_set(key, value, valueSize, 1))
    {
        // APP_PRINT_TRACE1("matter_kvs_put: key %s failed", TRACE_STRING(key));
        return MATTER_KVS_LOOKUP_NOT_FOUND;
    }

    return MATTER_KVS_STATUS_NO_ERROR;
}

int32_t matter_kvs_get(const char *key, void *buffer, size_t buffer_size, size_t *read_bytes_size)
{
    int len = buffer_size;

    if (0 != aos_kv_get(key, buffer, &len))
    {
        // APP_PRINT_TRACE1("matter_kvs_get: key %s failed", TRACE_STRING(key));
        return MATTER_KVS_LOOKUP_NOT_FOUND;
    }

    *read_bytes_size = len;
    return MATTER_KVS_STATUS_NO_ERROR;
}
#endif

int32_t matter_kvs_key_delete(const char *key)
{
    if (0 != aos_kv_del(key))
    {
        // APP_PRINT_TRACE1("matter_kvs_key_delete: key %s failed", TRACE_STRING(key));
        return MATTER_KVS_LOOKUP_NOT_FOUND;
    }

    return MATTER_KVS_STATUS_NO_ERROR;
}

bool matter_kvs_key_find(const char *key)
{
    return (aos_key_find(key) != NULL) ? true : false;
}

#ifdef __cplusplus
}
#endif
