#ifndef _GATT_H_
#define _GATT_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/*============================================================================*
*                         Macros
*============================================================================*/

/** @addtogroup  GATT_SERVER_API GATT Server API
  * @defgroup    GATT_ATTRIBUTE  GATT Attribute
  * @brief       GATT Attribute Definitions
  *
  * @{
  */

/** @defgroup  GATT_DECLARATIONS_UUID    GATT Declarations UUID
  * @brief     GATT declarations UUID.
  * @{
  */
#define GATT_UUID_PRIMARY_SERVICE       0x2800  /**<  GATT Primary Service Declaration. */
#define GATT_UUID_SECONDARY_SERVICE     0x2801  /**<  GATT Secondary Service Declaration. */
#define GATT_UUID_INCLUDE               0x2802  /**<  GATT Include Declaration. */
#define GATT_UUID_CHARACTERISTIC        0x2803  /**<  GATT Characteristic Declaration. */
/** End of GATT_DECLARATIONS_UUID
 * @}
 */

/** @defgroup  GATT_DESCRIPTORS_UUID     GATT Descriptors UUID
  * @brief     GATT descriptors UUID.
  * @{
  */
#define GATT_UUID_CHAR_EXTENDED_PROP                    0x2900  /**<  Characteristic Extended Properties. */
#define GATT_UUID_CHAR_USER_DESCR                       0x2901  /**<  Characteristic User Description. */
#define GATT_UUID_CHAR_CLIENT_CONFIG                    0x2902  /**<  Client Characteristic Configuration. */
#define GATT_UUID_CHAR_SERVER_CONFIG                    0x2903  /**<  Server Characteristic Configuration. */
#define GATT_UUID_CHAR_FORMAT                           0x2904  /**<  Characteristic Presentation Format. */
#define GATT_UUID_CHAR_AGGR_FORMAT                      0x2905  /**<  Characteristic Aggregate Format. */
#define GATT_UUID_CHAR_VALID_RANGE                      0x2906  /**<  Valid Range. */
#define GATT_UUID_CHAR_EXTERNAL_REPORT_REFERENCE        0x2907  /**<  External Report Reference. */
#define GATT_UUID_CHAR_REPORT_REFERENCE                 0x2908  /**<  Report Reference. */
#define GATT_UUID_CHAR_DESCRIPTOR_NUM_OF_DIGITALS       0x2909  /**<  Number of Digitals. */
#define GATT_UUID_CHAR_DESCRIPTOR_VALUE_TRIGGER_SETTING 0x290A  /**<  Value Trigger Setting. */
#define GATT_UUID_CHAR_SENSING_CONFIGURATION            0x290B  /**<  Environmental Sensing Configuration. */
#define GATT_UUID_CHAR_SENSING_MEASUREMENT              0x290C  /**<  Environmental Sensing Measurement. */
#define GATT_UUID_CHAR_SENSING_TRIGGER_SETTING          0x290D  /**<  Environmental Sensing Trigger Setting. */
#define GATT_UUID_CHAR_DESCRIPTOR_TIME_TRIGGER_SETTING  0x290E  /**<  Time Trigger Setting. */
/** End of GATT_DESCRIPTORS_UUID
 * @}
 */

/** @defgroup  GATT_CHARACTERISTICS_UUID     GATT Characteristics UUID
  * @brief     GATT characteristics UUID.
  * @{
  */
#define GATT_UUID_CHAR_DEVICE_NAME                     0x2A00  /**<  Device Name. */
#define GATT_UUID_CHAR_APPEARANCE                      0x2A01  /**<  Appearance. */
#define GATT_UUID_CHAR_PER_PRIV_FLAG                   0x2A02  /**<  Peripheral Privacy Flag. */
#define GATT_UUID_CHAR_RECONN_ADDRESS                  0x2A03  /**<  Reconnection Address. */
#define GATT_UUID_CHAR_PER_PREF_CONN_PARAM             0x2A04  /**<  Peripheral Preferred Connection Parameters. */
#define GATT_UUID_CHAR_SERVICE_CHANGED                 0x2A05  /**<  Service Changed. */
#define GATT_UUID_CHAR_CENTRAL_ADDRESS_RESOLUTION      0x2AA6  /**<  Central Address Resolution. */
#define GATT_UUID_CHAR_RESOLVABLE_PRIVATE_ADDRESS_ONLY 0x2AC9  /**<  Resolvable Private Address Only. */
#define GATT_UUID_CHAR_CLIENT_SUPPORTED_FEATURES       0x2B29  /**<  Client Supported Features. */
#define GATT_UUID_CHAR_DATABASE_HASH                   0x2B2A  /**<  Database Hash. */
#define GATT_UUID_CHAR_SERVER_SUPPORTED_FEATURES       0x2B3A  /**<  Server Supported Features. */

/* GATT Service Client Supported Features bit field. */
#define GATTS_CLIENT_SUPPORTED_FEATURES_DEFAULT_VALUE       0x00 /**< Server shall not use any of the features associated with that bit when communicating with this client. */
#define GATTS_CLIENT_SUPPORTED_FEATURES_ROBUST_CACHING_BIT  0x01 /**< Client supports robust caching. */
#define GATTS_CLIENT_SUPPORTED_FEATURES_EATT_BEARER_BIT     0x02 /**< Client supports Enhanced ATT bearer. */
#define GATTS_CLIENT_SUPPORTED_FEATURES_MULTI_NOTIF_BIT     0x04 /**< Client supports receiving ATT_MULTIPLE_HANDLE_VALUE_NTF PDUs. */

/* GATT Service Server Supported Features bit field. */
#define GATTS_SERVER_SUPPORTED_FEATURES_EATT_BIT  0x01 /**< Server supports Enhanced ATT feature. */

#define GATTS_CLIENT_SUPPORTED_FEATURES_LEN   1        /**< GATT Service Client Supported Features characteristic value length.
                                                            Variable length, 1 is used in Core Spec v5.2. */

#define GATTS_DATABASE_HASH_LEN               16       /**< GATT Service Database Hash characteristic value length. */
/** End of GATT_CHARACTERISTICS_UUID
 * @}
 */

/** @defgroup  GATT_BASED_SERVICE_UUID     GATT Based Service UUID
  * @brief     GATT based service UUID.
  * @{
  */
#define GATT_UUID_GAP                       0x1800  /**<  GAP Service. */
#define GATT_UUID_GATT                      0x1801  /**<  GATT Service. */
/** End of GATT_BASED_SERVICE_UUID
 * @}
 */

/** @defgroup  GATT_ATTRIBUTE_PERMISSIONS    GATT Attribute Permissions
  * @brief Attribute read and/or write permissions, encryption key size.
  * @{
  */
#define GATT_PERM_NONE                   0x00  /**< Read or write is not permitted. */
#define GATT_PERM_ALL                    0x01  /**< Authentication not necessary. */
#define GATT_PERM_AUTHEN_REQ             0x02  /**< Permissions require authentication, MITM protection not necessary. */
#define GATT_PERM_AUTHEN_MITM_REQ        0x03  /**< Permissions require authentication and MITM protection. */
#define GATT_PERM_AUTHOR_REQ             0x04  /**< Permissions require authorization. */
#define GATT_PERM_ENCRYPTED_REQ          0x08  /**< Permissions require encryption. */
#define GATT_PERM_AUTHEN_SC_REQ          0x00010000   /**< Permissions require authentication of Secure Connections,
                                                           MITM protection not necessary. */

/* read (bits 0..3 & bit 16) */
#define GATT_PERM_READ                   GATT_PERM_ALL
#define GATT_PERM_READ_AUTHEN_REQ        GATT_PERM_AUTHEN_REQ
#define GATT_PERM_READ_AUTHEN_MITM_REQ   GATT_PERM_AUTHEN_MITM_REQ
#define GATT_PERM_READ_AUTHOR_REQ        GATT_PERM_AUTHOR_REQ
#define GATT_PERM_READ_ENCRYPTED_REQ     GATT_PERM_ENCRYPTED_REQ
#define GATT_PERM_READ_AUTHEN_SC_REQ     GATT_PERM_AUTHEN_SC_REQ

#define GATT_PERM_READ_AUTHEN_GET(x)     (x & 0x03)
#define GATT_PERM_READ_AUTHOR_GET(x)     (x & 0x04)
#define GATT_PERM_READ_ENCRYPT_GET(x)    (x & 0x08)
#define GATT_PERM_READ_AUTHEN_SC_GET(x)  (x & 0x00010000)

/* write (bits 4..7 & bit 20) */
#define GATT_PERM_WRITE                  (GATT_PERM_ALL << 4)
#define GATT_PERM_WRITE_AUTHEN_REQ       (GATT_PERM_AUTHEN_REQ << 4)
#define GATT_PERM_WRITE_AUTHEN_MITM_REQ  (GATT_PERM_AUTHEN_MITM_REQ << 4)
#define GATT_PERM_WRITE_AUTHOR_REQ       (GATT_PERM_AUTHOR_REQ << 4)
#define GATT_PERM_WRITE_ENCRYPTED_REQ    (GATT_PERM_ENCRYPTED_REQ << 4)
#define GATT_PERM_WRITE_AUTHEN_SC_REQ    (GATT_PERM_AUTHEN_SC_REQ<<4)

#define GATT_PERM_WRITE_AUTHEN_GET(x)    ((x >> 4) & 0x03)
#define GATT_PERM_WRITE_AUTHOR_GET(x)    ((x >> 4) & 0x04)
#define GATT_PERM_WRITE_ENCRYPT_GET(x)   ((x >> 4) & 0x08)
#define GATT_PERM_WRITE_AUTHEN_SC_GET(x) ((x>>4) & 0x00010000)

/* notification/indication (bits 8..11 & bit 24) */
#define GATT_PERM_NOTIF_IND                  (GATT_PERM_ALL << 8)
#define GATT_PERM_NOTIF_IND_AUTHEN_REQ       (GATT_PERM_AUTHEN_REQ << 8)
#define GATT_PERM_NOTIF_IND_AUTHEN_MITM_REQ  (GATT_PERM_AUTHEN_MITM_REQ << 8)
#define GATT_PERM_NOTIF_IND_AUTHOR_REQ       (GATT_PERM_AUTHOR_REQ << 8)
#define GATT_PERM_NOTIF_IND_ENCRYPTED_REQ    (GATT_PERM_ENCRYPTED_REQ << 8)
#define GATT_PERM_NOTIF_IND_AUTHEN_SC_REQ    (GATT_PERM_AUTHEN_SC_REQ<<8)

#define GATT_PERM_NOTIF_IND_AUTHEN_GET(x)    ((x >> 8) & 0x03)
#define GATT_PERM_NOTIF_IND_AUTHOR_GET(x)    ((x >> 8) & 0x04)
#define GATT_PERM_NOTIF_IND_ENCRYPT_GET(x)   ((x >> 8) & 0x08)
#define GATT_PERM_NOTIF_IND_AUTHEN_SC_GET(x) ((x>>8) & 0x00010000)

/* key size - 1 (bits 12..15) */
#define GATT_PERM_KEYSIZE(size)          ((size-1) << 12)
#define GATT_PERM_KEYSIZE_GET(x, size)   {                             \
        size = ((x >> 12) & 0x0F);  \
        if ( size > 0 )             \
            size++;                   \
    }
/** End of GATT_ATTRIBUTE_PERMISSIONS
 * @}
 */

/** @defgroup  GATT_CHARACTERISTIC_PROPERTIES    GATT Characteristic Properties
  * @brief     GATT characteristic properties.
  * @{
  */
#define GATT_CHAR_PROP_BROADCAST           0x01  /**< If set, permits broadcasts of the Characteristic Value using
Server Characteristic Configuration Descriptor. */
#define GATT_CHAR_PROP_READ                0x02  /**< If set, permits reads of the Characteristic Value. */
#define GATT_CHAR_PROP_WRITE_NO_RSP        0x04  /**< If set, permits writes of the Characteristic Value without response. */
#define GATT_CHAR_PROP_WRITE               0x08  /**< If set, permits writes of the Characteristic Value with response. */
#define GATT_CHAR_PROP_NOTIFY              0x10  /**< If set, permits notifications of a Characteristic Value without acknowledgment. */
#define GATT_CHAR_PROP_INDICATE            0x20  /**< If set, permits indications of a Characteristic Value with acknowledgment. */
#define GATT_CHAR_PROP_WRITE_AUTHEN_SIGNED 0x40  /**< If set, permits signed writes to the Characteristic Value. */
#define GATT_CHAR_PROP_EXT_PROP            0x80  /**< If set, additional characteristic properties are defined in the Characteristic
Extended Properties Descriptor. */

/* GATT client characteristic configuration bit field */
#define GATT_CLIENT_CHAR_CONFIG_DEFAULT         0x0000 /**< The Characteristic Value shall be neither indicated nor notified. */
#define GATT_CLIENT_CHAR_CONFIG_NOTIFY          0x0001 /**< The Characteristic Value shall be notified. */
#define GATT_CLIENT_CHAR_CONFIG_INDICATE        0x0002 /**< The Characteristic Value shall be indicated. */
#define GATT_CLIENT_CHAR_CONFIG_NOTIFY_INDICATE 0x0003 /**< The Characteristic Value shall be both indicated and notified. */

#define GATT_CLIENT_CHAR_NOTIF_IND_DATA_PENGDING 0x8000 /**< The Characteristic Value to be indicated or notified is pending. */

/* GATT server characteristic configuration bit field */
#define GATT_SERVER_CHAR_CONFIG_BROADCAST       0x0001 /**< The Characteristic Value shall be broadcast. */
/** End of GATT_CHARACTERISTIC_PROPERTIES
 * @}
 */


/** @defgroup  GATT_UUID_SIZE    GATT UUID Size
  * @brief     GATT UUID size.
  * @{
  */
#define UUID_16BIT_SIZE                 2   /**< 16 bits UUID size. */
#define UUID_128BIT_SIZE                16  /**< 128 bits UUID size. */

#define ATT_ATTRIBUTE_MAX_LENGTH        512 /**< Attribute Maximum length. */

#define HI_WORD(x)  ((uint8_t)((x & 0xFF00) >> 8))
#define LO_WORD(x)  ((uint8_t)(x))
/** End of GATT_UUID_SIZE
 * @}
 */


/*---------------------------------------------------------------------------
 * GATT server attribute descriptor
 *--------------------------------------------------------------------------*/

/** @defgroup  GATT_ATTRIBUTE_FLAG   GATT Attribute Flag
  * @brief     GATT attribute flag.
  * @{
  */
#define ATTRIB_FLAG_VOID           0x0000 /**< Attribute value neither supplied by application
nor included following 16bit UUID. Attribute value is pointed by p_value_context
and value_len shall be set to the length of attribute value. */
#define ATTRIB_FLAG_UUID_128BIT    0x0001 /**< Attribute uses 128 bit UUID. */
#define ATTRIB_FLAG_VALUE_INCL     0x0002 /**< Attribute value is included following 16 bit UUID. */
#define ATTRIB_FLAG_VALUE_APPL     0x0004 /**< Application has to supply write value. */
#define ATTRIB_FLAG_ASCII_Z        0x0008 /**< Attribute value is ASCII_Z string. */
#define ATTRIB_FLAG_CCCD_APPL      0x0010 /**< Application will be informed about CCCD value is changed. */
#define ATTRIB_FLAG_CCCD_NO_FILTER 0x0020 /**< Application will be informed about CCCD value
when CCCD is write by client, no matter it is changed or not. */
#define ATTRIB_FLAG_LE             0x0800 /**< Used only for primary service declaration attributes if GATT over Bluetooth LE is supported. */
#define ATTRIB_FLAG_BREDR          0x0400 /**< Used only for primary service declaration attributes if GATT over BR/EDR is supported. */
/** End of GATT_ATTRIBUTE_FLAG
 * @}
 */

/** @defgroup  GATT_ATTRIBUTE_APPL   GATT Attribute Definition
  * @brief     GATT attribute definition.
  * @{
  */
typedef struct
{
    uint16_t    flags;              /**< Attribute flags @ref GATT_ATTRIBUTE_FLAG. */
    uint8_t     type_value[2 + 14]; /**< 16 bit UUID + included value or 128 bit UUID. */
    uint16_t    value_len;          /**< Length of value. */
    void        *p_value_context;   /**< Pointer to value if @ref ATTRIB_FLAG_VALUE_INCL
    and @ref ATTRIB_FLAG_VALUE_APPL not set. */
    uint32_t    permissions;        /**< Attribute permission @ref GATT_ATTRIBUTE_PERMISSIONS. */
} T_ATTRIB_APPL;
/** End of GATT_ATTRIBUTE_APPL
 * @}
 */

/** End of GATT_ATTRIBUTE
 * @}
 */

/** End of GATT_SERVER_API
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _GATT_H_ */
