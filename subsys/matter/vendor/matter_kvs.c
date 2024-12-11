/**************************
* Matter FTL Related
**************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"
#include "string.h"
#include "stdbool.h"
#include "chip_porting.h"
#include "mbedtls/sha256.h"
#include "mbedtls/aes.h"
#include "iot_persistent_config.h"
#include "os_mem.h"
#include "os_sync.h"
#include <openthread/cli.h>

#include "kvmgr.h"

uint32_t __attribute__((weak)) kvs_addr_get(void)
{
    return KVS_ADDR;
}

extern otError kv_cmd_handler(void *aContext, uint8_t argc, char *argv[]);

int32_t Bee4_KvsCmd(void *aContext, int argc, char *argv[])
{
    return kv_cmd_handler(aContext, argc, argv);
}

int32_t Bee4_KvsInit(void)
{
    uint32_t kvs_addr = kvs_addr_get();
    return aos_kv_init(kvs_addr);
}

int32_t Bee4_KvsClean(void)
{
    uint32_t kvs_addr = kvs_addr_get();
    aos_kv_deinit(kvs_addr);
    aos_kv_delall();
    return 0;
}

#define CONFIG_ENABLE_KVS_ENCRYPTION 0
#if CONFIG_ENABLE_KVS_ENCRYPTION

static mbedtls_aes_context aes;
// key length 32 bytes for 256 bit encrypting, it can be 16 or 24 bytes for 128 and 192 bits encrypting mode
static unsigned char key[] = {0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                              0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                              0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                              0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
                             };

static int32_t kvs_encrypt(unsigned char *input_to_encrypt, int input_len,
                           unsigned char *encrypt_output)
{
    size_t nc_off = 0;

    unsigned char nonce_counter[16] = {0};
    unsigned char stream_block[16] = {0};
    int ret = mbedtls_aes_crypt_ctr(&aes, input_len, &nc_off, nonce_counter, stream_block,
                                    input_to_encrypt, encrypt_output);
    return ret;
}

static int32_t kvs_decrypt(unsigned char *input_to_decrypt, int input_len,
                           unsigned char *decrypt_output)
{
    size_t nc_off1 = 0;
    unsigned char nonce_counter1[16] = {0};
    unsigned char stream_block1[16] = {0};
    int ret = mbedtls_aes_crypt_ctr(&aes, input_len, &nc_off1, nonce_counter1, stream_block1,
                                    input_to_decrypt, decrypt_output);
    return ret;
}

int32_t Bee4_KvsPut(const char *key, const void *value, size_t valueSize)
{
    int32_t ret = BEE_KVS_STATUS_NO_ERROR;

    void *encrypt_output = calloc(1, valueSize);
    if (encrypt_output == NULL)
    {
        goto exit;
    }

    kvs_encrypt(value, valueSize, encrypt_output);

    if (0 != aos_kv_set(key, encrypt_output, valueSize, 1))
    {
        //APP_PRINT_TRACE1("Bee4_KvsPut kv_set %s failed", TRACE_STRING(key));
        ret = BEE_KVS_LOOKUP_NOT_FOUND;
    }
    else
    {
        ret = BEE_KVS_STATUS_NO_ERROR;
    }

exit:
    if (encrypt_output)
    {
        free(encrypt_output);
    }

    return ret;
}

int32_t Bee4_KvsGet(const char *key, void *value, size_t valueSize, size_t *readBytesSize)
{
    int32_t ret = BEE_KVS_STATUS_NO_ERROR;
    int len = valueSize;

    void *decrypt_input = calloc(1, valueSize);
    if (decrypt_input == NULL)
    {
        goto exit;
    }

    if (0 != aos_kv_get(key, decrypt_input, &len))
    {
        //APP_PRINT_TRACE1("Bee4_KvsGet kv_get %s failed", TRACE_STRING(key));
        ret = BEE_KVS_LOOKUP_NOT_FOUND;
    }
    else
    {
        kvs_decrypt(decrypt_input, len, value);
        *readBytesSize = len;
        ret = BEE_KVS_STATUS_NO_ERROR;
    }

exit:
    if (decrypt_input)
    {
        free(decrypt_input);
    }

    return ret;
}

#else

int32_t Bee4_KvsPut(const char *key, const void *value, size_t valueSize)
{
    if (0 != aos_kv_set(key, value, valueSize, 1))
    {
        //APP_PRINT_TRACE1("Bee4_KvsPut kv_set %s failed", TRACE_STRING(key));
        return BEE_KVS_LOOKUP_NOT_FOUND;
    }
    else
    {
        return BEE_KVS_STATUS_NO_ERROR;
    }
}

int32_t Bee4_KvsGet(const char *key, void *value, size_t valueSize, size_t *readBytesSize)
{
    int len = valueSize;
    if (0 != aos_kv_get(key, value, &len))
    {
        //APP_PRINT_TRACE1("Bee4_KvsGet kv_get %s failed", TRACE_STRING(key));
        return BEE_KVS_LOOKUP_NOT_FOUND;
    }
    else
    {
        *readBytesSize = len;
        return BEE_KVS_STATUS_NO_ERROR;
    }
}

#endif

int32_t Bee4_KvsDeleteKey(const char *key)
{
    if (0 != aos_kv_del(key))
    {
        //APP_PRINT_TRACE1("Bee4_KvsDeleteKey kv_del %s failed", TRACE_STRING(key));
        return BEE_KVS_LOOKUP_NOT_FOUND;
    }
    else
    {
        return BEE_KVS_STATUS_NO_ERROR;
    }
}

bool checkExist(const char *domain, const char *key)
{
    return aos_key_find(key) ? true : false;
}

#ifdef __cplusplus
}
#endif
