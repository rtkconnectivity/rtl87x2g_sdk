/**
  ******************************************************************************
  * @file    patch_header_check.h
  * @author
  * @version V0.0.1
  * @date    2017-09-01
  * @brief   This file contains all the functions regarding patch header check.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _PATCH_HEADER_CHECK_NSC_H_
#define _PATCH_HEADER_CHECK_NSC_H_
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "flash_nor_device.h"
#include "rtl876x_lib_platform.h"
#include "rom_uuid.h"
#include "trace.h"

/** @addtogroup  FLASH_DEVICE    Flash Device
    * @{
    */
/*============================================================================*
  *                                   Constants
  *============================================================================*/
/** @defgroup FLASH_DEVICE_Exported_Constants Flash Device Exported Constants
  * @brief
  * @{
  */

#define OTA_HEADER_SIZE                     0x1000
#define OTA_HDR_IMG_INFO_DEFAULT_VALUE      0xFFFFFFFF
#define UUID_SIZE                           16
#define DEFAULT_HEADER_SIZE                 (0x400 + 256)
/*IC Type refer to WIKI: https://wiki.realtek.com/display/Bee1/BT+SOC+IC+Type*/
#define IMG_IC_TYPE                         0xF
#define FLASH_NOR_TABLE_MAGIC_PATTERN       (0x5A5A12A5)

#define BOOT_EXT_PATTERN                    0x736c

#define CMAC_BYTE_SIZE                      (16)

#define RSA_PUBLIC_KEY_BYTE_SIZE            (384)

#define ECDSA_PUBLIC_KEY_BYTE_SIZE          (32)
// ecdsa uncompressed key len = (public key byte len * 2) + 1
#define ECDSA_UNCOMPRESSED_PUBLIC_KEY_LEN   (65)

#define ED25519_PUBLIC_KEY_BYTE_SIZE        (32)

#define SHA256_BYTE_SIZE                    (32)

#define RSA_SIGNATURE_BYTE_SIZE             (RSA_PUBLIC_KEY_BYTE_SIZE)
// *INDENT-OFF*
#define RTK_MBEDTLS_ECDSA_MAX_SIG_LEN( bits )                               \
        ( /*T,L of SEQUENCE*/ ( ( bits ) >= 61 * 8 ? 3 : 2 ) +              \
          /*T,L of r,s*/        2 * ( ( ( bits ) >= 127 * 8 ? 3 : 2 ) +     \
          /*V of r,s*/                ( ( bits ) + 8 ) / 8 ) )

#define ECDSA_SIGNATURE_BYTE_SIZE           (RTK_MBEDTLS_ECDSA_MAX_SIG_LEN(ECDSA_PUBLIC_KEY_BYTE_SIZE << 3))

#define ED25519_SIGNATURE_BYTE_SIZE         (ED25519_PUBLIC_KEY_BYTE_SIZE * 2)

#define VP_PARTITION_NAME "VP"

/*
        - signature range   = hash + header content + payload
        - hash range        = header content + payload
*/
#define SIGNATURE_RANGE(header)     (((T_IMG_HEADER_FORMAT *)(header))->ctrl_header.payload_len + header_size_rom - RSA_SIGNATURE_BYTE_SIZE)

#define HASH_RANGE(header)          (((T_IMG_HEADER_FORMAT *)(header))->ctrl_header.payload_len + header_size_rom - RSA_SIGNATURE_BYTE_SIZE - SHA256_SIZE)

#define GET_SIGNATURE(auth_type)    ((auth_type == AUTH_ECDSA) ?   header->auth.ecdsa_image_signature :   \
                                     (auth_type == AUTH_ED25519) ? header->auth.ed25519_image_signature : \
                                                                   header->auth.rsa_image_signature)

#define GET_COMPUTE_PUBLIC_KEY_HASH_LEN(auth_type)                                   \
                ((auth_type == AUTH_ECDSA) ?   (ECDSA_UNCOMPRESSED_PUBLIC_KEY_LEN) : \
                 (auth_type == AUTH_ED25519) ? (ED25519_PUBLIC_KEY_BYTE_SIZE) :      \
                                               (RSA_PUBLIC_KEY_BYTE_SIZE + 4))
// *INDENT-ON*
#define SYS_CFG_SIGNATURE           0x12345bb3
/* Config Format: offset(2 bytes) + length(1 byte) + data(1 byte) + mask(1 byte)*/
#define MIN_ENTRY_SIZE              5

#define IMG_VER_TO_LITTLE_ENDIAN(git_ver)       (((git_ver & 0xf) << 28 ) + (((git_ver & 0xff0) >> 4) << 20) + (((git_ver & 0x07fff000) >> 12) << 5) + ((git_ver & 0xf8000000) >> 27))

/** End of FLASH_DEVICE_Exported_Constants
  * @}
  */
/*============================================================================*
  *                                   Types
  *============================================================================*/
/** @defgroup FLASH_DEVICE_Exported_Types Flash Device Exported Types
  * @brief
  * @{
  */

typedef enum
{
    IMG_SCCD             = 0x379D,
    IMG_OCCD             = 0x379E,
    IMG_BOOTPATCH        = 0x379F,          /*!< KM4 boot patch */
    IMG_DFU_FIRST        = IMG_BOOTPATCH,
    IMG_OTA              = 0x37A0,          /*!< OTA header */
    IMG_SECUREMCUAPP     = 0x37A2,          /*!< KM4 secure app */
    IMG_SECUREMCUAPPDATA = 0x37A3,          /*!< KM4 secure app data */
    IMG_BT_STACKPATCH    = 0x37A6,          /*!< BT stack patch */
    IMG_BANK_FIRST       = IMG_BT_STACKPATCH,
    IMG_MCUPATCH         = 0x37A7,          /*!< KM4 non-secure rom patch */
    IMG_UPPERSTACK       = 0x37A8,          /*!< KM4 non-secure Upperstack */
    IMG_MCUAPP           = 0x37A9,          /*!< KM4 non-secure app */
    IMG_MCUCFGDATA       = 0x37AA,          /*!< KM4 MCUConfigData */
    IMG_MCUAPPDATA1      = 0x37AE,
    IMG_MCUAPPDATA2      = 0x37AF,
    IMG_MCUAPPDATA3      = 0x37B0,
    IMG_MCUAPPDATA4      = 0x37B1,
    IMG_MCUAPPDATA5      = 0x37B2,
    IMG_MCUAPPDATA6      = 0x37B3,
    IMG_ZIGBEESTACK      = 0x37B4,          /*!< KM4 Zigbee stack */
    IMG_MAX              = 0x37B5,
    IMG_DFU_MAX          = IMG_MAX,

    IMG_RO_DATA1         = 0x3A81,  /*!< FOR vp */
    IMG_RO_DATA2         = 0x3A82,
    IMG_RO_DATA3         = 0x3A83,
    IMG_RO_DATA4         = 0x3A84,
    IMG_RO_DATA5         = 0x3A85,
    IMG_RO_DATA6         = 0x3A86,

    IMG_USER_DATA8       = 0xFFF7,    /*!< the image data only support unsafe single bank ota */
    IMG_USER_DATA_FIRST  = IMG_USER_DATA8,
    IMG_USER_DATA7       = 0xFFF8,    /*!< the image data only support unsafe single bank ota */
    IMG_USER_DATA6       = 0xFFF9,    /*!< the image data only support unsafe single bank ota */
    IMG_USER_DATA5       = 0xFFFA,    /*!< the image data only support unsafe single bank ota */
    IMG_USER_DATA4       = 0xFFFB,    /*!< the image data only support unsafe single bank ota */
    IMG_USER_DATA3       = 0xFFFC,    /*!< the image data only support unsafe single bank ota */
    IMG_USER_DATA2       = 0xFFFD,    /*!< the image data only support unsafe single bank ota */
    IMG_USER_DATA1       = 0xFFFE,    /*!< the image data only support unsafe single bank ota */
    IMG_USER_DATA_MAX    = 0xFFFF,    /*!< the image data only support unsafe single bank ota */
} IMG_ID;


typedef enum _ENC_KEY_SELECT
{
    ENC_KEY_SCEK = 0,
    ENC_KEY_SCEK_WITH_RTKCONST,
    ENC_KEY_OCEK,
    ENC_KEY_OCEK_WITH_OEMCONST,
    ENC_KEY_ON_FLASH,
    ENC_KEY_MAX,
} ENC_KEY_SELECT;

typedef union _PUBLIC_KEY
{
    struct
    {
        uint8_t N[RSA_PUBLIC_KEY_BYTE_SIZE];
        uint8_t E[4];
    };

    struct
    {
        uint8_t ecdsa_key[ECDSA_UNCOMPRESSED_PUBLIC_KEY_LEN];
    };

    struct
    {
        uint8_t eddsa_key[ED25519_PUBLIC_KEY_BYTE_SIZE];
    };

} PUBLIC_KEY;

typedef struct _IMG_CTRL_HEADER_FORMAT
{
    uint16_t crc16;
    uint8_t ic_type;
    uint8_t secure_version;
    union
    {
        uint16_t value;
        struct
        {
            uint16_t xip: 1; // payload is executed on flash
            uint16_t enc: 1; // all the payload is encrypted
            uint16_t load_when_boot: 1; // load image when boot
            uint16_t enc_load: 1; // encrypt load part or not
            uint16_t enc_key_select: 3; // referenced to ENC_KEY_SELECT
            uint16_t not_ready: 1; //for copy image in ota
            uint16_t not_obsolete: 1; //for copy image in ota
            uint16_t integrity_check_en_in_boot: 1; // enable image integrity check in boot flow
//            uint16_t header_size: 4; // unit:KB, set for tool
//            uint16_t rsvd: 2;
            uint16_t compressed_not_ready: 1;
            uint16_t compressed_not_obsolete: 1;
            uint16_t rsvd: 1;
            uint16_t image_type: 3; /*for app 000b: normal image, 001b:compressed image, other for more types
            for patch in temp bank consist of 001b: patch+fsbl, 010b: patch+app, 011b: patch+fsbl+app*/
        };
    } ctrl_flag;
    uint16_t image_id;
    uint32_t payload_len;
} T_IMG_CTRL_HEADER_FORMAT;

typedef struct
{
    union
    {
        uint32_t version;
        struct
        {
            uint32_t _version_reserve: 8;   //!< reserved
            uint32_t _version_revision: 8; //!< revision version
            uint32_t _version_minor: 8;     //!< minor version
            uint32_t _version_major: 8;     //!< major version
        } header_sub_version; //!< ota header sub version
        struct
        {
            uint32_t _version_major: 4;     //!< major version
            uint32_t _version_minor: 8;     //!< minor version
            uint32_t _version_revision: 15; //!< revision version
            uint32_t _version_reserve: 5;   //!< reserved
        } img_sub_version; //!< other image sub version including patch, app, app data1-6

    } ver_info;
} T_IMAGE_VERSION;

typedef struct
{
    union
    {
        uint32_t version;
        struct
        {
            uint32_t _version_major: 4;     //!< major version
            uint32_t _version_minor: 8;     //!< minor version
            uint32_t _version_revision: 15; //!< revision version
            uint32_t _version_reserve: 5;   //!< reserved
        } sub_version;
    } ver_info;
    uint32_t _version_commitid;     //!< git commit id
    uint8_t _customer_name[8];      //!< branch name for customer patch
} T_VERSION_FORMAT;

typedef union
{
    uint8_t bytes[24];
    struct
    {
        uint32_t app_ram_data_addr;
        uint32_t app_ram_data_size;
        uint32_t app_heap_data_on_size;
        uint32_t ext_data_sram_size;
        uint32_t rsvd[2];
    } app_ram_info;
} T_EXTRA_INFO_FORMAT;

typedef struct _AUTH_HEADER_FORMAT
{
    union
    {
        uint8_t image_mac[CMAC_BYTE_SIZE];
        uint8_t rsa_image_signature[RSA_SIGNATURE_BYTE_SIZE];
        struct
        {
            uint8_t ecdsa_image_signature[ECDSA_SIGNATURE_BYTE_SIZE];
            uint8_t ecdsa_image_signature_len;
        };
        uint8_t ed25519_image_signature[ED25519_SIGNATURE_BYTE_SIZE];
    };
    uint8_t image_hash[SHA256_BYTE_SIZE];
} T_AUTH_HEADER_FORMAT;

typedef struct _FLASH_SEC_FORMAT
{
    union
    {
        uint32_t value;
        struct
        {
            uint32_t enable: 1;
            uint32_t mode: 2; // gcm mode:0b00, ctr mode:0b01, mix mode:0b10
            uint32_t key_select: 3;
            uint32_t rsvd: 26;
        };
    } ctrl_flag;
    uint32_t base_addr;
    uint32_t region_size;
    uint8_t iv_high[4];
    uint8_t iv_low[4];
} T_FLASH_SEC_FORMAT;

/* image_info[] is used to record image address and size in OTA bank
 * ex. non-secure patch's addr = image_info[(IMG_MCUPATCH - IMG_OTA - 1)*2 + 0]
 *     non-secure patch's size = image_info[(IMG_MCUPATCH - IMG_OTA - 1)*2 + 1]
 *     ...
 *  => image_id's addr = image_info[(image_id - IMG_OTA - 1)*2 + 0]
 *     image_id's size = image_info[(image_id - IMG_OTA - 1)*2 + 1]
 */
typedef union _IMG_HEADER_FORMAT
{
    uint8_t bytes[DEFAULT_HEADER_SIZE];
    struct
    {
        T_AUTH_HEADER_FORMAT auth;
        T_IMG_CTRL_HEADER_FORMAT ctrl_header;
        uint8_t uuid[16];
        uint32_t exe_base;
        uint32_t load_src;
        uint32_t load_len;
        uint32_t image_base;
        uint16_t dev_id; // for AMZN
        uint16_t flash_layout_size_4k; // ex. 1MB flash used size => flash_layout_size_4k = 1MB/4KB = 256
        uint32_t magic_pattern;
        uint8_t dec_key[16];
        uint32_t load_dst;
        T_EXTRA_INFO_FORMAT ex_info;
        T_VERSION_FORMAT git_ver;
        PUBLIC_KEY PubKey;
        T_FLASH_SEC_FORMAT flash_sec_cfg;
        union
        {
            uint8_t reserved_data[344];
            struct
            {
                uint32_t ver_val;
                uint32_t image_info[(IMG_MAX - IMG_OTA - 1) * 2];
                uint32_t reserved0[45];
            };
            struct __attribute__((packed))
            {
                uint32_t tool_version;
                uint32_t timestamp;
                uint16_t boot_ext_img_id;
                uint16_t boot_ext_load_pattern;
                uint16_t reserved1[166];
            };
        };
    };
} T_IMG_HEADER_FORMAT;

typedef struct _ROM_HEADER_FORMAT
{
    uint8_t uuid[UUID_SIZE];
    void *init_ptr;
    void *entry_ptr;
} T_ROM_HEADER_FORMAT;

typedef enum
{
    IMG_CHECK_PASS,
    IMG_CHECK_ERR_HEADER_ADDR,
    IMG_CHECK_ERR_NOT_READY,
    IMG_CHECK_ERR_ROM_UUID,
    IMG_CHECK_ERR_MAGIC_PATTERN,
    IMG_CHECK_ERR_SIZE,
    IMG_CHECK_ERR_ID,
    IMG_CHECK_ERR_ENTRY_RETURN,
    IMG_CHECK_ERR_VERIFY,
    IMG_CHECK_ERR_EXE_BASE,
    IMG_CHECK_ERR_IC_TYPE,
    IMG_CHECK_ERR_MAX,
} IMG_CHECK_ERR_TYPE;

/** End of FLASH_DEVICE_Exported_Types
  * @}
  */
/*************************************************************************************************
*                          Constants
*************************************************************************************************/


/*************************************************************************************************
*                          functions
*************************************************************************************************/
/** @defgroup FLASH_DEVICE_Exported_Functions Flash Device Exported Functions
    * @brief
    * @{
    */
/**
    * @brief    check whether ota support bank switch
    * @param    void
    * @return   true: support bank switch, false: not support bank switch
    */
bool is_ota_support_bank_switch(void);

/**
    * @brief    get image header format size
    * @param    void
    * @return   image header format size
    */
uint32_t get_img_header_format_size(void);

/**
    * @brief    get image header extra infomation
    * @param    void
    * @return   pointer to T_EXTRA_INFO_FORMAT
    */
T_EXTRA_INFO_FORMAT *get_extra_info(void);

/**
    * @brief    get active boot patch address
    * @param    void
    * @return   active boot patch address
    */
uint32_t get_active_boot_patch_addr(void);

/**
    * @brief    get active secure app address
    * @param    void
    * @return   active secure app address
    */
uint32_t get_active_secure_app_addr(void);

/**
    * @brief    get active ota bank address
    * @param    void
    * @return   active ota bank address
    */
uint32_t get_active_ota_bank_addr(void);

/**
    * @brief    get header address by image id
    * @param    image_id  image id
    * @return   header address
    */
uint32_t get_header_addr_by_img_id(IMG_ID image_id);

/**
    * @brief    get active bank image size by image id
    * @param    image_id  image id
    * @return   image size
    */
uint32_t get_active_bank_image_size_by_img_id(IMG_ID image_id);

/**
    * @brief    get active ota bank size
    * @param    void
    * @return   active ota bank size
    */
uint32_t get_active_ota_bank_size(void);

/**
    * @brief    get inactive ota bank size
    * @param    void
    * @return   inactive ota bank address
    */
uint32_t get_temp_ota_bank_size(void);

/**
    * @brief    get inactive ota bank image address by image id
    * @param    image_id  image id
    * @return   inactive ota bank image address
    */
uint32_t get_temp_ota_bank_img_addr_by_img_id(IMG_ID image_id);

/**
    * @brief    header valid check
    * @param    header_addr  header address
    * @param    image_id  image id
    * @return   A @ref IMG_CHECK_ERR_TYPE type value
    */
static IMG_CHECK_ERR_TYPE check_header_valid(uint32_t header_addr, IMG_ID image_id)
{
    IMG_CHECK_ERR_TYPE ret = IMG_CHECK_PASS;

    if (header_addr == 0 || header_addr > (FMC_MAIN0_ADDR + FMC_MAIN0_SIZE))
    {
        ret = IMG_CHECK_ERR_HEADER_ADDR;
        goto EXIT;
    }

    T_IMG_HEADER_FORMAT *header = (T_IMG_HEADER_FORMAT *)header_addr;
    if (header->ctrl_header.image_id != image_id)
    {
        ret = IMG_CHECK_ERR_ID;
        goto EXIT;
    }

    if (header->ctrl_header.payload_len > FMC_MAIN0_SIZE)
    {
        ret = IMG_CHECK_ERR_SIZE;
        goto EXIT;
    }

    if (image_id == IMG_OTA)
    {
        if (header->magic_pattern != FMC_CAL_PATTERN)
        {
            ret = IMG_CHECK_ERR_MAGIC_PATTERN;
            goto EXIT;
        }
    }
    else if (image_id == IMG_BOOTPATCH || (image_id > IMG_OTA && image_id < IMG_MAX))
    {
        if (header->ctrl_header.ctrl_flag.not_ready)
        {
            ret = IMG_CHECK_ERR_NOT_READY;
            goto EXIT;
        }

        if (IMG_IC_TYPE != header->ctrl_header.ic_type)
        {
            ret = IMG_CHECK_ERR_IC_TYPE;
            goto EXIT;
        }


        char c_rom_uuid[UUID_SIZE]  = DEFINE_symboltable_uuid;
        bool is_uuid_valid = (memcmp(header->uuid, c_rom_uuid, 16) == 0);

        if (is_uuid_valid == false)
        {
            ret = IMG_CHECK_ERR_ROM_UUID;
            goto EXIT;
        }
    }
    else
    {
        ret = IMG_CHECK_ERR_ID;
        goto EXIT;
    }

    return ret;

EXIT:
    DBG_DIRECT("check_header_valid: image_id 0x%x, addr 0x%x, ret %d", 3, image_id, header_addr, ret);

    return ret;
}

/**
    * @brief    get inactive ota bank image size by image id
    * @param    image_id  image id
    * @return   inactive ota bank image size
    */
static __inline uint32_t get_temp_ota_bank_img_size_by_img_id(IMG_ID image_id)
{
    uint32_t image_size = 0;
    uint32_t temp_bank_addr;
    uint32_t temp_bank_size;
    uint32_t ota_bank0_addr = flash_nor_get_bank_addr(FLASH_OTA_BANK_0);

    if (image_id < IMG_DFU_FIRST || ((image_id >= IMG_MAX)))
    {
        return image_size;
    }

    if (image_id == IMG_BOOTPATCH)
    {
        uint32_t boot_patch0_addr = flash_nor_get_bank_addr(FLASH_BOOT_PATCH0);
        uint32_t boot_patch1_addr = flash_nor_get_bank_addr(FLASH_BOOT_PATCH1);
        if (get_active_boot_patch_addr() == boot_patch0_addr)
        {
            return  flash_nor_get_bank_size(FLASH_BOOT_PATCH1);
        }
        else if (get_active_boot_patch_addr() == boot_patch1_addr)
        {
            return  flash_nor_get_bank_size(FLASH_BOOT_PATCH0);;
        }
        else
        {
            return 0;
        }
    }
    else if (image_id == IMG_SECUREMCUAPP || image_id == IMG_SECUREMCUAPPDATA)
    {
        uint32_t secure_app0_addr = flash_nor_get_bank_addr(FLASH_SECURE_APP0);
        uint32_t secure_app1_addr = flash_nor_get_bank_addr(FLASH_SECURE_APP1);
        if (get_active_secure_app_addr() == secure_app0_addr)
        {
            return (image_id == IMG_SECUREMCUAPP) ? flash_nor_get_bank_size(FLASH_SECURE_APP1) :
                   flash_nor_get_bank_size(FLASH_SECURE_APP_DATA1);
        }
        else if (get_active_secure_app_addr() == secure_app1_addr)
        {
            return (image_id == IMG_SECUREMCUAPP) ? flash_nor_get_bank_size(FLASH_SECURE_APP0) :
                   flash_nor_get_bank_size(FLASH_SECURE_APP_DATA0);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        /* IMG_BANK_FIRST <= image_id < IMG_MAX, image in bank */
        if (!is_ota_support_bank_switch())
        {
            if (image_id == IMG_OTA)
            {
                return 0;
            }

            image_size = flash_nor_get_bank_size(FLASH_OTA_TMP);
        }
        else
        {
            if (ota_bank0_addr == get_active_ota_bank_addr())
            {
                temp_bank_addr = flash_nor_get_bank_addr(FLASH_OTA_BANK_1);
                temp_bank_size = flash_nor_get_bank_size(FLASH_OTA_BANK_1);
            }
            else
            {
                temp_bank_addr = ota_bank0_addr;
                temp_bank_size = flash_nor_get_bank_size(FLASH_OTA_BANK_0);
            }

            if (image_id == IMG_OTA)
            {
                if (is_ota_support_dual_bank_merge())
                {
                    image_size = temp_bank_size;
                }
                else
                {
                    image_size = OTA_HEADER_SIZE;
                }
            }
            else
            {
                if (is_ota_support_dual_bank_merge())
                {
                    return 0;
                }
                else
                {
                    if (IMG_CHECK_PASS != check_header_valid(temp_bank_addr, IMG_OTA))
                    {
                        return 0;
                    }
                    image_size = ((T_IMG_HEADER_FORMAT *)temp_bank_addr)->image_info[(image_id - IMG_OTA - 1) * 2 +
                                                                                                              1];
                }
            }
        }
    }

    if (OTA_HDR_IMG_INFO_DEFAULT_VALUE == image_size)
    {
        image_size = 0;
    }

    return image_size;
}

/**
    * @brief    get ota bank image version
    * @param    is_active_bank  whether active bank
    * @param    image_id  image id
    * @param    p_image_version  pointer to p_image_version
    * @return   ture: success ; false: fail
    */
bool get_ota_bank_image_version(bool is_active_bank, IMG_ID image_id,
                                T_IMAGE_VERSION *p_image_version);

/**
    * @brief    get ota header item
    * @param    is_active_bank  whether active bank
    * @param    offset  ota header offset
    * @param    data  pointer to data
    * @return   ture: success ; false: fail
    */
bool get_ota_header_item(bool is_active_bank, uint32_t offset, uint32_t *data);

/**
    * @brief    image entry
    * @param    image_id  image id
    * @param    image_addr  image address
    * @return   A @ref IMG_CHECK_ERR_TYPE type value
    */
IMG_CHECK_ERR_TYPE image_entry(uint16_t image_id, uint32_t image_addr);

/**
    * @brief    image entry check
    * @param    rom_header  pointer to rom_header
    * @param    patch_header  pointer to patch_header
    * @return   A @ref IMG_CHECK_ERR_TYPE type value
    */
IMG_CHECK_ERR_TYPE image_entry_check(T_ROM_HEADER_FORMAT *rom_header,
                                     T_ROM_HEADER_FORMAT *patch_header);


/** @} */ /* End of group FLASH_DEVICE_Exported_Functions */
/** @} */ /* End of group FLASH_DEVICE */

#endif // _PATCH_HEADER_CHECK_H_
