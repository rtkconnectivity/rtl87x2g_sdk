#ifdef __cplusplus
extern "C" {
#endif

#include "matter_types.h"

#define MATTER_KVS_ENABLE_BACKUP        0
#define MATTER_KVS_ENABLE_WEAR_LEVELING 0
#define MATTER_KVS_BEGIN_ADDR           FTL_ADDR
/*1024 elements per page, every element occupy 4 bytes*/
#define PAGE_ELEMENT_NUM            (4096 / 4)
#define PAGE_NUM  (FTL_SIZE/4096 - 1)
#define MAX_NUM     (511*PAGE_NUM - PAGE_ELEMENT_NUM /2 - 2)
#define MAX_PAGE_ELEMENT_NUM    511
#define MAX_ALL_PAGE_ELEMENT_NUM    (MAX_PAGE_ELEMENT_NUM * 20)

#define BEE_KVS_STATUS_NO_ERROR         0
#define BEE_KVS_HASH_INVALID            9
#define BEE_KVS_LOOKUP_FOUND            10
#define BEE_KVS_LOOKUP_NOT_FOUND        11
#define BEE_KVS_LOOKUP_NOT_FOUND_MAX    12
#define BEE_KVS_NO_MEM                  13

/*
 * Debug level definiation
 */
#define FTL_DBG_LV_ERR 0x01
#define FTL_DBG_LV_INFO 0x02
#define FTL_DBG_LV_ALL (FTL_DBG_LV_ERR | FTL_DBG_LV_INFO)

#define CFG_FTL_DEBUG 1
#define FTL_DBG_LEVEL FTL_DBG_LV_ERR

#ifdef CFG_FTL_DEBUG
#   define FTL_DBG(lv, fmt, args...) \
    do { \
        if (lv & FTL_DBG_LEVEL) { \
            DBG_DIRECT("[FTL]" fmt , ##args); \
        } \
    } while(0)
#else
#   define FTL_DBG(lv, fmt, args...) do { } while(0)
#endif


int32_t Bee4_KvsInit(void);
int32_t Bee4_KvsClean(void);
int32_t Bee4_lookup_key(uint8_t *hask_key, uint32_t key_len, uint16_t *lookup_idx);
int32_t Bee4_KvsPut(const char *key, const void *value, size_t valueSize);
int32_t Bee4_KvsGet(const char *key, void *value, size_t valueSize, size_t *readBytesSize);

// for BeeConfig
int32_t Bee4_KvsDeleteKey(const char *key);
bool checkExist(const char *domain, const char *key);
int32_t setPref_new(const char *domain, const char *key, uint8_t *value, size_t byteCount);
int32_t getPref_bool_new(const char *domain, const char *key, uint8_t *val);
int32_t getPref_u32_new(const char *domain, const char *key, uint32_t *val);
int32_t getPref_u64_new(const char *domain, const char *key, uint64_t *val);
int32_t getPref_str_new(const char *domain, const char *key, char *buf, size_t bufSize,
                        size_t *outLen);
int32_t getPref_bin_new(const char *domain, const char *key, uint8_t *buf, size_t bufSize,
                        size_t *outLen);

int32_t Bee4_KvsCmd(void *aContext, int argc, char *argv[]);
#ifdef __cplusplus
}
#endif
