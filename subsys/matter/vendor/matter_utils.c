/************************** 
* Matter Utility Functions 
**************************/
#include <stdio.h>

#ifdef __cplusplus
 extern "C" {
#endif

#include "chip_porting.h"
#include "flash_nor_device.h"
#include "../../../matter/vendor/factory_data/factory.pb.h"
#include "flash_map.h"
#include "../../../matter/vendor/factory_data/pb_encode.h"
#include "../../../matter/vendor/factory_data/pb_decode.h"
#include "aes_api.h"
#include "trace.h"

#ifndef CONFIG_ENABLE_FACTORY_DATA_ENCRYPTION
#define CONFIG_ENABLE_FACTORY_DATA_ENCRYPTION 0
#endif

bool store_string_spake2_salt(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData**) arg;
    uint8_t buffer[1024] = {0};

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;

    if(data_length > sizeof(fdp->cdata.spake2_salt.value))
        return false;

    memcpy(fdp->cdata.spake2_salt.value, buffer, data_length);
    fdp->cdata.spake2_salt.len = data_length;
    return true;
}

bool store_string_spake2_verifier(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData**) arg;
    uint8_t buffer[1024] = {0};

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;

    if(data_length > sizeof(fdp->cdata.spake2_verifier.value))
        return false;

    memcpy(fdp->cdata.spake2_verifier.value, buffer, data_length);
    fdp->cdata.spake2_verifier.len = data_length;
    return true;
}

bool store_string_dac_cert(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData**) arg;
    uint8_t buffer[1024] = {0};

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;

    if(data_length > sizeof(fdp->dac.dac_cert.value))
        return false;

    memcpy(fdp->dac.dac_cert.value, buffer, data_length);
    fdp->dac.dac_cert.len = data_length;
    return true;
}

bool store_string_dac_key(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData**) arg;
    uint8_t buffer[1024] = {0};

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;

    if(data_length > sizeof(fdp->dac.dac_key.value))
        return false;

    memcpy(fdp->dac.dac_key.value, buffer, data_length);
    fdp->dac.dac_key.len = data_length;
    return true;
}

bool store_string_pai_cert(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData**) arg;
    uint8_t buffer[1024] = {0};

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;

    if(data_length > sizeof(fdp->dac.pai_cert.value))
        return false;

    memcpy(fdp->dac.pai_cert.value, buffer, data_length);
    fdp->dac.pai_cert.len = data_length;
    return true;
}

bool store_string_cd(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData**) arg;
    uint8_t buffer[1024] = {0};

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;

    if(data_length > sizeof(fdp->dac.cd.value))
        return false;

    memcpy(fdp->dac.cd.value, buffer, data_length);
    fdp->dac.cd.len = data_length;
    return true;
}

bool store_string_vendor_name(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData**) arg;
    uint8_t buffer[1024] = {0};

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;

    if(data_length > sizeof(fdp->dii.vendor_name.value))
        return false;

    memcpy(fdp->dii.vendor_name.value, buffer, data_length);
    fdp->dii.vendor_name.len = data_length;
    return true;
}

bool store_string_product_name(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData**) arg;
    uint8_t buffer[1024] = {0};

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;
    
    if(data_length > sizeof(fdp->dii.product_name.value))
        return false;

    memcpy(fdp->dii.product_name.value, buffer, data_length);
    fdp->dii.product_name.len = data_length;
    return true;
}

bool store_string_hw_ver_string(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData**) arg;
    uint8_t buffer[1024] = {0};

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;

    if(data_length > sizeof(fdp->dii.hw_ver_string.value))
        return false;

    memcpy(fdp->dii.hw_ver_string.value, buffer, data_length);
    fdp->dii.hw_ver_string.len = data_length;
    return true;
}

bool store_string_mfg_date(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData**) arg;
    uint8_t buffer[1024] = {0};

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;

    if(data_length > sizeof(fdp->dii.mfg_date.value))
        return false;

    memcpy(fdp->dii.mfg_date.value, buffer, data_length);
    fdp->dii.mfg_date.len = data_length;
    return true;
}

bool store_string_serial_num(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData**) arg;
    uint8_t buffer[1024] = {0};

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;

    if(data_length > sizeof(fdp->dii.serial_num.value))
        return false;

    memcpy(fdp->dii.serial_num.value, buffer, data_length);
    fdp->dii.serial_num.len = data_length;
    return true;
}

bool store_string_rd_id_uid(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    FactoryData *fdp = *(FactoryData**) arg;
    uint8_t buffer[1024] = {0};

    /* We could read block-by-block to avoid the large buffer... */
    size_t data_length = stream->bytes_left;
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;

    if(data_length > sizeof(fdp->dii.rd_id_uid.value))
        return false;

    memcpy(fdp->dii.rd_id_uid.value, buffer, data_length);
    fdp->dii.rd_id_uid.len = data_length;
    return true;
}


int32_t ReadFactory(uint8_t *buffer, uint32_t buffer_len, uint16_t *pfactorydata_len)
{
    #define LENGTH_BYTES 2
    uint32_t ret = 0;

#if CONFIG_ENABLE_FACTORY_DATA_ENCRYPTION
    union 
    {
        uint16_t length;
        uint8_t buf[LENGTH_BYTES];
    } __attribute__((packed)) img_hdr = {} ;

    #define AES128_KEY_ID (0)

    const uint8_t* img_ptr = FACTORY_DATA_ADDR;

    MATTER_PRINT_INFO1("ReadFactory: encrypted data %b", TRACE_BINARY(16, img_ptr));


    aes128_ctr_decrypt_with_load_key(img_ptr, AES128_KEY_ID, img_hdr.buf, LENGTH_BYTES);
    

    MATTER_PRINT_INFO2("ReadFactory: length %d, block data %b", img_hdr.length, TRACE_BINARY(LENGTH_BYTES, img_hdr.buf));

    if(img_hdr.length + 2 > buffer_len)
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
