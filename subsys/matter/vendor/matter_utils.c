/**************************
* Matter Utility Functions
**************************/
#include <stdio.h>
#include <mbedtls/rsa.h>
#include <mbedtls/pk.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "trace.h"
#include "flash_nor_device.h"
#include "flash_map.h"
#include "factory_data/factory.pb.h"
#include "factory_data/pb_encode.h"
#include "factory_data/pb_decode.h"
#include "aes_api.h"
#include "matter_utils.h"


#if (CONFIG_DAC_KEY_ENC) && (!defined(FEATURE_TRUSTZONE_ENABLE) || (FEATURE_TRUSTZONE_ENABLE != 1))
const char private_key[] =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQC2/En1Wp8OXujD\n"
    "J3gNB9FDB0TdEcRRVgUDc0ZIAZcVVspSSX6GL+yTVkbhMt59A/somGaHvMw5Bnz8\n"
    "P7jiVWWojKdUTUyXHI02omQN+ub9YSVHa2CrW4TT+odaQiWXMcSOvR8W71Cs65Wm\n"
    "Pc84+TfS1um12OJV/lT+QpiwWNeAFc8t87AnfYsEAHFg34M3l6NKaHnL1QmHgozY\n"
    "aODoCoti+wt8/YyZJj6NNl6gpOFcjq//zHVJmfKa3zmn9tGiYuy6cScKav9v5dAi\n"
    "3tVYAJua6mOzcbS7XuawDA3i7YwfXSiN16DJUyny0M0XhJGeDrESV7MlaoFAjffC\n"
    "Y8yB+rO/AgMBAAECggEAATY2fyt0djHmtLQkNWHh3uHDcPtXI3rAWb9/raZlC70h\n"
    "Zd4aumTC2k7t9TvTAVzvYttvgmKbr5z9YuZBIS6Tl8Qmr4Y7hZyoyr3Uj8sjkHX+\n"
    "OzTY1QmsHYNBNjXN7exkctWJySsYi6ybtxgdsTDlT74gmhWvmgYvJefm/vP3qV94\n"
    "uYGrxX188TbI+Uit1mHBWNCLfoXhcG41DGpaxl0puBYVnjUeyLM6eHvP/O/MPeNx\n"
    "R2mUx0c8t+UNLpYrRsUMga48oyasqtX4XpwpJMTkHabhMF5joyowkLEFNzPSrGLE\n"
    "cB6+DYKXLT6GWQTDSVSWWeo0SfNxJcvelFaT+OpPkQKBgQDHm/zE2ezF17pXn0HC\n"
    "Ert8inbcPnOUE34roq+nEidEt7gZlnnaXDoD3nF+ODy4kskVvbKpHLoQUMW+KoX9\n"
    "9oU5KCDkZPzAKN9OmJH/gM+PkCnQ/glJFrex6y+zlx0RQ4t9RnqKOvRXURZFBZ4f\n"
    "Acz9hCbl4r2ozeIUmYH2/m2D5QKBgQDqrgtmfTD1IKMwIWDtWQIP/7Zyc8un/27c\n"
    "tqpd+6z1tf6yBe53W5aZOv05n/rt/K+tzykFeYwgJan0CQfpiEhNr146v8pfaziF\n"
    "6rmD5CXoVHzVN20iw22PFoGRnJpJL+jHEZRu4u5YCbhpqpvx9yt5c+i37xLVW6gg\n"
    "YgGA32cm0wKBgBLeUOd+vh35hXjQ6hNhV+2u3Qt2r9A4g322dCoz0hhx0WpByiT1\n"
    "ANrqxQmsYTkFTxLKCsBnocL3D6opNN9L1U4Q2AMe9LFpa6pvS6W+VGB6vSkvSktp\n"
    "ZVXhKHIuf6XpH59o8R5t8jjqxlMh8890iR8RxeeHJmx1gcB2vk1NwaZtAoGBANcO\n"
    "gWH9TZUEwKbFn8ZBb9NreS0vEi00tcpLFpqjnN9aVTmcl+gHJV3sIcvh4BihG/jC\n"
    "Q4xB03hOK413jSmj+uY907MwuGpynkySSkkkFNzwN6lIbpmRz191t2zRvs1cuwtZ\n"
    "/9UR3msNFVK3xsqYv90/q76Kto6QvORYC5qmnyeDAoGAHEGAApHsiR1nNxqpcULd\n"
    "cIYMRsSjHKRc2sfYBjcFijQK9x+rFG0RXuspAaNoJ7f1g5RaTxdWnhCe6gdpZaxW\n"
    "Cy9qIuUL/Or4OMQKajMLn77rfkYyLL5sCWUuS21fUF+cvcyjkXGqzs1wAscBnzvZ\n"
    "6P5WYy5hWEptVtLhEQonXVg=\n"
    "-----END PRIVATE KEY-----\n";
#endif

#define MEMBER_SIZE(type, member) sizeof(((type *)0)->member)

#define BUFFER_SIZE (1024)

bool store_string_spake2_salt(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData **) arg;
    bool ret = false;
    uint8_t *buffer;

    buffer = calloc(1, BUFFER_SIZE);
    if (buffer == NULL)
    {
        goto exit;
    }

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > BUFFER_SIZE - 1)
    {
        goto exit;
    }

    if (!pb_read(stream, buffer, stream->bytes_left))
    {
        goto exit;
    }

    if (data_length > sizeof(fdp->cdata.spake2_salt.value))
    {
        goto exit;
    }

    memcpy(fdp->cdata.spake2_salt.value, buffer, data_length);
    fdp->cdata.spake2_salt.len = data_length;

    ret = true;

exit:
    if (buffer)
    {
        free(buffer);
    }
    return ret;
}

bool store_string_spake2_verifier(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData **) arg;
    bool ret = false;
    uint8_t *buffer;

    buffer = calloc(1, BUFFER_SIZE);
    if (buffer == NULL)
    {
        goto exit;
    }

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > BUFFER_SIZE - 1)
    {
        goto exit;
    }

    if (!pb_read(stream, buffer, stream->bytes_left))
    {
        goto exit;
    }

    if (data_length > sizeof(fdp->cdata.spake2_verifier.value))
    {
        goto exit;
    }

    memcpy(fdp->cdata.spake2_verifier.value, buffer, data_length);
    fdp->cdata.spake2_verifier.len = data_length;

    ret = true;

exit:
    if (buffer)
    {
        free(buffer);
    }
    return ret;
}

bool store_string_dac_cert(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData **) arg;
    bool ret = false;
    uint8_t *buffer;

    buffer = calloc(1, BUFFER_SIZE);
    if (buffer == NULL)
    {
        goto exit;
    }

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > BUFFER_SIZE - 1)
    {
        goto exit;
    }

    if (!pb_read(stream, buffer, stream->bytes_left))
    {
        goto exit;
    }

    if (data_length > sizeof(fdp->dac.dac_cert.value))
    {
        goto exit;
    }

    memcpy(fdp->dac.dac_cert.value, buffer, data_length);
    fdp->dac.dac_cert.len = data_length;

    ret = true;

exit:
    if (buffer)
    {
        free(buffer);
    }
    return ret;
}


int app_entropy_source(void *data, unsigned char *output, size_t len, size_t *olen)
{
    uint32_t rnd_num;
    uint32_t index = 0;
    uint32_t *pbuf;
    pbuf = (uint32_t *)output;

    if (len == 0)
    {
        return (0);
    }

    *olen = len;

    while (len > 0)
    {
        rnd_num = platform_random(0xffffffff);

        if (len > sizeof(uint32_t))
        {
            *pbuf = rnd_num;
            pbuf++;
            len -= sizeof(uint32_t);
        }
        else
        {
            index = (uint32_t)pbuf - (uint32_t)output;

            while (len > 0)
            {
                output[index] = rnd_num & 0xFF;
                rnd_num = rnd_num >> 8;
                index++;
                len--;
            }
        }
    }

    return 0;
}


bool store_string_dac_key(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData **) arg;
    bool ret = false;
    uint8_t *buffer;

    buffer = malloc(BUFFER_SIZE);
    if (buffer)
    {
        /* We could read block-by-block to avoid the large buffer... */
        size_t data_length = stream->bytes_left;
        if (stream->bytes_left > BUFFER_SIZE - 1)
        {
            goto exit;
        }

        if (!pb_read(stream, buffer, stream->bytes_left))
        {
            goto exit;
        }

        MATTER_PRINT_INFO2("store_string_dac_key: len %d, key_encrypted %b", data_length,
                           TRACE_BINARY(16, buffer));

#define OUT_MAX_LEN (sizeof(fdp->dac.dac_key.value))

#if (CONFIG_DAC_KEY_ENC) && (!defined(FEATURE_TRUSTZONE_ENABLE) || (FEATURE_TRUSTZONE_ENABLE != 1))
        mbedtls_pk_context pk;
        mbedtls_entropy_context entropy;
        mbedtls_ctr_drbg_context ctr_drbg;
        size_t olen = 0;
        char dac_key_decrypted[OUT_MAX_LEN] = {0};

        mbedtls_ctr_drbg_init(&ctr_drbg);
        mbedtls_entropy_init(&entropy);
        mbedtls_pk_init(&pk);

        mbedtls_entropy_add_source(&entropy, app_entropy_source, NULL, 1, MBEDTLS_ENTROPY_SOURCE_STRONG);

        int ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                                        (const unsigned char *) "Realtek", strlen("Realtek"));

        int key_parse_ret = mbedtls_pk_parse_key(&pk, private_key, sizeof(private_key), NULL, 0,
                                                 mbedtls_ctr_drbg_random, &ctr_drbg);

        int decrypt_ret = mbedtls_rsa_pkcs1_decrypt(mbedtls_pk_rsa(pk), mbedtls_ctr_drbg_random, &ctr_drbg,
                                                    &olen, buffer, dac_key_decrypted, OUT_MAX_LEN);

        if (olen < OUT_MAX_LEN)
        {
            memcpy(fdp->dac.dac_key.value, dac_key_decrypted, olen);
        }
        else
        {
            memcpy(fdp->dac.dac_key.value, dac_key_decrypted, OUT_MAX_LEN);
        }

        fdp->dac.dac_key.len = olen;

        MATTER_PRINT_INFO3("store_string_dac_key: ret %d, key_parse_ret %d, decrypt_ret %d", ret,
                           key_parse_ret, decrypt_ret);

        MATTER_PRINT_INFO2("store_string_dac_key: len %d, key_decrypted %b", olen, TRACE_BINARY(16,
                           fdp->dac.dac_key.value));

        mbedtls_entropy_free(&entropy);
        mbedtls_pk_free(&pk);
        mbedtls_ctr_drbg_free(&ctr_drbg);
#else
        if (data_length < OUT_MAX_LEN)
        {
            memcpy(fdp->dac.dac_key.value, buffer, data_length);
        }
        else
        {
            memcpy(fdp->dac.dac_key.value, buffer, OUT_MAX_LEN);
        }

        fdp->dac.dac_key.len = data_length;
#endif //CONFIG_DAC_KEY_ENC
    }

    ret = true;

exit:
    if (buffer)
    {
        free(buffer);
    }
    return ret;
}

bool store_string_pai_cert(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData **) arg;
    bool ret = false;
    uint8_t *buffer;

    buffer = calloc(1, BUFFER_SIZE);
    if (buffer == NULL)
    {
        goto exit;
    }

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > BUFFER_SIZE - 1)
    {
        goto exit;
    }

    if (!pb_read(stream, buffer, stream->bytes_left))
    {
        goto exit;
    }

    if (data_length > sizeof(fdp->dac.pai_cert.value))
    {
        goto exit;
    }

    memcpy(fdp->dac.pai_cert.value, buffer, data_length);
    fdp->dac.pai_cert.len = data_length;

    ret = true;

exit:
    if (buffer)
    {
        free(buffer);
    }
    return ret;
}

bool store_string_cd(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData **) arg;
    bool ret = false;
    uint8_t *buffer;

    buffer = calloc(1, BUFFER_SIZE);
    if (buffer == NULL)
    {
        goto exit;
    }

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > BUFFER_SIZE - 1)
    {
        goto exit;
    }

    if (!pb_read(stream, buffer, stream->bytes_left))
    {
        goto exit;
    }

    if (data_length > sizeof(fdp->dac.cd.value))
    {
        goto exit;
    }

    memcpy(fdp->dac.cd.value, buffer, data_length);
    fdp->dac.cd.len = data_length;

    ret = true;

exit:
    if (buffer)
    {
        free(buffer);
    }
    return ret;
}

bool store_string_vendor_name(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData **) arg;
    bool ret = false;
    uint8_t *buffer;

    buffer = calloc(1, BUFFER_SIZE);
    if (buffer == NULL)
    {
        goto exit;
    }

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > BUFFER_SIZE - 1)
    {
        goto exit;
    }

    if (!pb_read(stream, buffer, stream->bytes_left))
    {
        goto exit;
    }

    if (data_length > sizeof(fdp->dii.vendor_name.value))
    {
        goto exit;
    }

    memcpy(fdp->dii.vendor_name.value, buffer, data_length);
    fdp->dii.vendor_name.len = data_length;

    ret = true;

exit:
    if (buffer)
    {
        free(buffer);
    }
    return ret;
}

bool store_string_product_name(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData **) arg;
    bool ret = false;
    uint8_t *buffer;

    buffer = calloc(1, BUFFER_SIZE);
    if (buffer == NULL)
    {
        goto exit;
    }

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > BUFFER_SIZE - 1)
    {
        goto exit;
    }

    if (!pb_read(stream, buffer, stream->bytes_left))
    {
        goto exit;
    }

    if (data_length > sizeof(fdp->dii.product_name.value))
    {
        goto exit;
    }

    memcpy(fdp->dii.product_name.value, buffer, data_length);
    fdp->dii.product_name.len = data_length;

    ret = true;

exit:
    if (buffer)
    {
        free(buffer);
    }
    return ret;
}

bool store_string_hw_ver_string(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData **) arg;
    bool ret = false;
    uint8_t *buffer;

    buffer = calloc(1, BUFFER_SIZE);
    if (buffer == NULL)
    {
        goto exit;
    }

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > BUFFER_SIZE - 1)
    {
        goto exit;
    }

    if (!pb_read(stream, buffer, stream->bytes_left))
    {
        goto exit;
    }

    if (data_length > sizeof(fdp->dii.hw_ver_string.value))
    {
        goto exit;
    }

    memcpy(fdp->dii.hw_ver_string.value, buffer, data_length);
    fdp->dii.hw_ver_string.len = data_length;

    ret = true;

exit:
    if (buffer)
    {
        free(buffer);
    }
    return ret;
}

bool store_string_mfg_date(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData **) arg;
    bool ret = false;
    uint8_t *buffer;

    buffer = calloc(1, BUFFER_SIZE);
    if (buffer == NULL)
    {
        goto exit;
    }

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > BUFFER_SIZE - 1)
    {
        goto exit;
    }

    if (!pb_read(stream, buffer, stream->bytes_left))
    {
        goto exit;
    }

    if (data_length > sizeof(fdp->dii.mfg_date.value))
    {
        goto exit;
    }

    memcpy(fdp->dii.mfg_date.value, buffer, data_length);
    fdp->dii.mfg_date.len = data_length;

    ret = true;

exit:
    if (buffer)
    {
        free(buffer);
    }
    return ret;
}

bool store_string_serial_num(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData **) arg;
    bool ret = false;
    uint8_t *buffer;

    buffer = calloc(1, BUFFER_SIZE);
    if (buffer == NULL)
    {
        goto exit;
    }

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > BUFFER_SIZE - 1)
    {
        goto exit;
    }

    if (!pb_read(stream, buffer, stream->bytes_left))
    {
        goto exit;
    }

    if (data_length > sizeof(fdp->dii.serial_num.value))
    {
        goto exit;
    }

    memcpy(fdp->dii.serial_num.value, buffer, data_length);
    fdp->dii.serial_num.len = data_length;

    ret = true;

exit:
    if (buffer)
    {
        free(buffer);
    }
    return ret;
}

bool store_string_rd_id_uid(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData **) arg;
    bool ret = false;
    uint8_t *buffer;

    buffer = calloc(1, BUFFER_SIZE);
    if (buffer == NULL)
    {
        goto exit;
    }

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > BUFFER_SIZE - 1)
    {
        goto exit;
    }

    if (!pb_read(stream, buffer, stream->bytes_left))
    {
        goto exit;
    }

    if (data_length > sizeof(fdp->dii.rd_id_uid.value))
    {
        goto exit;
    }

    memcpy(fdp->dii.rd_id_uid.value, buffer, data_length);
    fdp->dii.rd_id_uid.len = data_length;

    ret = true;

exit:
    if (buffer)
    {
        free(buffer);
    }
    return ret;
}


int32_t ReadFactory(uint8_t *buffer, uint32_t buffer_len, uint16_t *pfactorydata_len)
{
#define LENGTH_BYTES 2
    uint32_t ret = 0;

#if CONFIG_FACTORY_DATA_ENCRYPTION
    union
    {
        uint16_t length;
        uint8_t buf[LENGTH_BYTES];
    } __attribute__((packed)) img_hdr = {} ;

#define AES128_KEY_ID (0)

    const uint8_t *img_ptr = (const uint8_t *) FACTORY_DATA_ADDR;

    MATTER_PRINT_INFO1("ReadFactory: encrypted data %b", TRACE_BINARY(16, img_ptr));

    aes128_ctr_decrypt_with_load_key(img_ptr, AES128_KEY_ID, img_hdr.buf, LENGTH_BYTES);

    MATTER_PRINT_INFO2("ReadFactory: length %d, block data %b", img_hdr.length,
                       TRACE_BINARY(LENGTH_BYTES, img_hdr.buf));

    if (img_hdr.length + 2 > buffer_len)
    {
        MATTER_PRINT_ERROR1("ReadFactory: factory data over buffer_len %d", buffer_len);
        return -1;
    }

    // uint16_t aligned_size = ALIGN_SIZE(img_hdr.length, 16);

    aes128_ctr_decrypt_with_load_key(img_ptr, AES128_KEY_ID, buffer, LENGTH_BYTES + img_hdr.length);

    MATTER_PRINT_INFO1("ReadFactory: data %b", TRACE_BINARY(16, &buffer[LENGTH_BYTES]));

    for (size_t i = 0; i < img_hdr.length; i++)
    {
        buffer[i] = buffer[i + LENGTH_BYTES];
    }

    *pfactorydata_len = img_hdr.length;

    MATTER_PRINT_INFO2("ReadFactory: buffer %p, data2 %b", buffer, TRACE_BINARY(16, buffer));
#else
    uint32_t address = FACTORY_DATA_ADDR;
    const uint8_t length_bytes = 2;

    flash_nor_read_locked(address, (uint8_t *)pfactorydata_len, length_bytes);

    flash_nor_read_locked(address + 2, buffer, *pfactorydata_len);
#endif
    return ret;
}

int32_t DecodeFactory(uint8_t *buffer, FactoryData *fdp, uint16_t data_len)
{
    int32_t ret = 0;
    pb_istream_t stream;
    FactoryDataProvider FDP = FactoryDataProvider_init_zero;

    stream = pb_istream_from_buffer(buffer, data_len);

    // Set the callbacks for these fields
    // Once decoding is done, the decoded data will be passed to the callback functions and handled there
    // TODO: Combine all the callbacks into a single switch case callback
    FDP.cdata.spake2_salt.value.funcs.decode = &store_string_spake2_salt;
    FDP.cdata.spake2_verifier.value.funcs.decode = &store_string_spake2_verifier;
    FDP.dac.dac_cert.value.funcs.decode = &store_string_dac_cert;
    FDP.dac.dac_key.value.funcs.decode = &store_string_dac_key;
    FDP.dac.pai_cert.value.funcs.decode = &store_string_pai_cert;
    FDP.dac.cd.value.funcs.decode = &store_string_cd;
    FDP.dii.vendor_name.value.funcs.decode = &store_string_vendor_name;
    FDP.dii.product_name.value.funcs.decode = &store_string_product_name;
    FDP.dii.hw_ver_string.value.funcs.decode = &store_string_hw_ver_string;
    FDP.dii.mfg_date.value.funcs.decode = &store_string_mfg_date;
    FDP.dii.serial_num.value.funcs.decode = &store_string_serial_num;
    FDP.dii.rd_id_uid.value.funcs.decode = &store_string_rd_id_uid;

    // Pass in fdp as an argument to the callback
    FDP.cdata.spake2_salt.value.arg = fdp;
    FDP.cdata.spake2_verifier.value.arg = fdp;
    FDP.dac.dac_cert.value.arg = fdp;
    FDP.dac.dac_key.value.arg = fdp;
    FDP.dac.pai_cert.value.arg = fdp;
    FDP.dac.cd.value.arg = fdp;
    FDP.dii.vendor_name.value.arg = fdp;
    FDP.dii.product_name.value.arg = fdp;
    FDP.dii.hw_ver_string.value.arg = fdp;
    FDP.dii.mfg_date.value.arg = fdp;
    FDP.dii.serial_num.value.arg = fdp;
    FDP.dii.rd_id_uid.value.arg = fdp;

    if (!pb_decode(&stream, FactoryDataProvider_fields, &FDP))
    {
        ret = -1;
        goto exit;
    }

    // We handle the integer fields here, don't need for callbacks
    fdp->cdata.passcode = FDP.cdata.passcode;
    fdp->cdata.discriminator = FDP.cdata.discriminator;
    fdp->cdata.spake2_it = FDP.cdata.spake2_it;
    fdp->dii.vendor_id = FDP.dii.vendor_id;
    fdp->dii.product_id = FDP.dii.product_id;
    fdp->dii.hw_ver = FDP.dii.hw_ver;

exit:
    return ret;
}

#ifdef __cplusplus
}
#endif
