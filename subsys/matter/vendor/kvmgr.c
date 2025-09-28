/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <errno.h>
#include <string.h>
#include "os_queue.h"
#include "kvmgr.h"

#if defined(CONFIG_ENABLE_RAM_REDUCE) && (CONFIG_ENABLE_RAM_REDUCE == 1)
#define MAPPING_TABLE_ENABLE 0
#else
#define MAPPING_TABLE_ENABLE 1
#endif

#ifdef MAPPING_TABLE_ENABLE
#define MAPPING_TABLE_ITEM_MAX 200

typedef struct _mapping_table_item_t
{
    struct _mapping_table_item_t      *p_next;
    char                              *p_key;          /* The store buffer for key */
    uint8_t                            key_len;        /* The length of the key */
    uint16_t                           val_len;        /* The length of the value */
    uint16_t                           pos;            /* The store position of the key-value item */
} mapping_table_item_t;
#endif

/* Key-value function return code description */
typedef enum
{
    RES_OK              = 0,        /* Successed */
    RES_CONT            = -EAGAIN,  /* Loop continued */
    RES_NO_SPACE        = -ENOSPC,  /* The space is out of range */
    RES_INVALID_PARAM   = -EINVAL,  /* The parameter is invalid */
    RES_MALLOC_FAILED   = -ENOMEM,  /* Error related to malloc */
    RES_ITEM_NOT_FOUND  = -ENOENT,  /* Could not find the key-value item */
    RES_FLASH_READ_ERR  = -EIO,     /* The flash read operation failed */
    RES_FLASH_WRITE_ERR = -EIO,     /* The flash write operation failed */
    RES_FLASH_EARSE_ERR = -EIO      /* The flash earse operation failed */
} result_e;

/* Defination of block information */
#define BLK_BITS                12                          /* The number of bits in block size */
#define BLK_SIZE                (1 << BLK_BITS)             /* Block size, current is 4k bytes */
#define BLK_NUMS                (KV_TOTAL_SIZE >> BLK_BITS) /* The number of blocks, must be bigger than KV_GC_RESERVED */
#define BLK_OFF_MASK            ~(BLK_SIZE - 1)             /* The mask of block offset in key-value store */
#define BLK_STATE_USED          0xCC                        /* Block state: USED --> block is inused and without dirty data */
#define BLK_STATE_CLEAN         0xEE                        /* Block state: CLEAN --> block is clean, ready for used */
#define BLK_STATE_DIRTY         0x44                        /* Block state: DIRTY --> block is inused and with dirty data */
#define BLK_HEADER_SIZE         4                           /* The block header size 4bytes */

#define INVALID_BLK_STATE(state) \
    (((state) != BLK_STATE_USED) && \
     ((state) != BLK_STATE_CLEAN) && \
     ((state) != BLK_STATE_DIRTY))

/* Defination of key-value item information */
#define ITEM_HEADER_SIZE        8                           /* The key-value item header size 8bytes */
#define ITEM_STATE_NORMAL       0xEE                        /* Key-value item state: NORMAL --> the key-value item is valid */
#define ITEM_STATE_DELETE       0                           /* Key-value item state: DELETE --> the key-value item is deleted */
#define ITEM_MAX_KEY_LEN        255                         /* The max key length for key-value item */
#define ITEM_MAX_VAL_LEN        2048                         /* The max value length for key-value item */
#define ITEM_MAX_LEN            (ITEM_HEADER_SIZE + ITEM_MAX_KEY_LEN + ITEM_MAX_VAL_LEN)

/* Defination of key-value store information */
#define KV_STATE_OFF            1                           /* The offset of block/item state in header structure */
#define KV_ALIGN_MASK           ~(sizeof(void *) - 1)       /* The mask of key-value store alignment */
#define KV_GC_RESERVED          1                           /* The reserved block for garbage collection */
#define KV_GC_STACK_SIZE        1024

#define KV_SELF_REMOVE          0
#define KV_ORIG_REMOVE          1
/* Flash block header description */
typedef struct _block_header_t
{
    uint8_t     magic;          /* The magic number of block */
    uint8_t     state;          /* The state of the block */
    uint8_t     reserved[2];
} __attribute__((packed)) block_hdr_t;

/* Key-value item header description */
typedef struct _item_header_t
{
    uint8_t     magic;          /* The magic number of key-value item */
    uint8_t     state;          /* The state of key-value item */
    uint8_t     crc;            /* The crc-8 value of key-value item */
    uint8_t     key_len;        /* The length of the key */
    uint16_t    val_len;        /* The length of the value */
    uint16_t    origin_off;     /* The origin key-value item offset, it will be used when updating */
} __attribute__((packed)) item_hdr_t;

/* Key-value item description */
typedef struct _kv_item_t
{
    item_hdr_t  hdr;            /* The header of the key-value item */
    char       *store;          /* The store buffer for key-value */
    uint16_t    len;            /* The length of the buffer */
    uint16_t    pos;            /* The store position of the key-value item */
} kv_item_t;

/* Block information structure for management */
typedef struct _block_info_t
{
    uint16_t    space;          /* Free space in current block */
    uint8_t     state;          /* The state of current block */
} block_info_t;

typedef struct _kv_mgr_t
{
    uint8_t      kv_initialize;        /* The flag to indicate the key-value store is initialized */
    uint8_t      gc_triggered;         /* The flag to indicate garbage collection is triggered */
    uint8_t      gc_waiter;            /* The number of thread wait for garbage collection finished */
    uint8_t      clean_blk_nums;       /* The number of block which state is clean */
    uint16_t     write_pos;            /* Current write position for key-value item */
    uint32_t     kv_addr;
    void        *gc_sem;
    void        *kv_mutex;
    block_info_t block_info[BLK_NUMS]; /* The array to record block management information */
    T_OS_QUEUE   mapping_table;
    void        *gc_task_handle;
} kv_mgr_t;

static kv_mgr_t g_kv_mgr;

static const uint8_t BLK_MAGIC_NUM  = 'K';    /* The block header magic number */
static const uint8_t ITEM_MAGIC_NUM = 'I';    /* The key-value item header magic number */

void aos_kv_gc(void *arg);

/* CRC-8: the poly is 0x31 (x^8 + x^5 + x^4 + 1) */
static uint8_t utils_crc8(uint8_t *buf, uint16_t length)
{
    uint8_t crc = 0x00;
    uint8_t i;

    while (length--)
    {
        crc ^= *buf++;
        for (i = 8; i > 0; i--)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ 0x31;
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

#define ROUND_DOWN(a,b) (((a) / (b)) * (b))
#define MIN(a,b)        (((a) < (b)) ? (a) : (b))
#define FLASH_PAGE_SIZE 4096
#define FLASH_WORD_SIZE 4

static int raw_read(uint32_t offset, void *buf, size_t size)
{
    if (size > 0)
    {
        flash_nor_read_locked(g_kv_mgr.kv_addr + offset, (uint8_t *)buf, size);
    }

    return 0;
}

static int raw_write(uint32_t offset, const void *data, size_t size)
{
    if (size > 0)
    {
        flash_nor_write_locked(g_kv_mgr.kv_addr + offset, (uint8_t *)data, size);
    }

    return RES_OK;
}

static int raw_erase(uint32_t offset, uint32_t size)
{
    uint32_t dst_addr = g_kv_mgr.kv_addr + offset;
    uint32_t fl_addr = ROUND_DOWN(dst_addr, FLASH_PAGE_SIZE);
    flash_nor_erase_locked(fl_addr, FLASH_NOR_ERASE_SECTOR);
    return 0;
}

static void trigger_gc(void)
{
    if (g_kv_mgr.gc_triggered)
    {
        return;
    }

    g_kv_mgr.gc_waiter = 0;
    g_kv_mgr.gc_triggered = 1;
    //APP_PRINT_INFO0(">> aos_kv_gc");
    aos_kv_gc(NULL);
    //APP_PRINT_INFO0("<< aos_kv_gc");
}

static void kv_item_free(kv_item_t *item)
{
    if (item)
    {
        if (item->store)
        {
            free(item->store);
        }
        free(item);
    }
}

static int kv_state_set(uint16_t pos, uint8_t state)
{
    return raw_write(pos + KV_STATE_OFF, &state, 1);
}

static int kv_block_format(uint8_t index)
{
    block_hdr_t hdr;
    uint16_t pos = index << BLK_BITS;

    memset(&hdr, 0, sizeof(hdr));
    hdr.magic = BLK_MAGIC_NUM;
    if (!raw_erase(pos, BLK_SIZE))
    {
        hdr.state = BLK_STATE_CLEAN;
    }
    else
    {
        return RES_FLASH_EARSE_ERR;
    }

    if (raw_write(pos, &hdr, BLK_HEADER_SIZE) != RES_OK)
    {
        return RES_FLASH_WRITE_ERR;
    }

    g_kv_mgr.block_info[index].state = BLK_STATE_CLEAN;
    g_kv_mgr.block_info[index].space = BLK_SIZE - BLK_HEADER_SIZE;
    (g_kv_mgr.clean_blk_nums)++;
    return RES_OK;
}

static uint16_t kv_item_calc_pos(uint16_t len)
{
    block_info_t *blk_info;
    uint8_t blk_index = (g_kv_mgr.write_pos) >> BLK_BITS;

    blk_info = &(g_kv_mgr.block_info[blk_index]);
    if (blk_info->space > len)
    {
        if (((blk_info->space - len) < ITEM_MAX_LEN) && (g_kv_mgr.clean_blk_nums <= KV_GC_RESERVED))
        {
            trigger_gc();
        }
        return g_kv_mgr.write_pos;
    }

#if BLK_NUMS > KV_GC_RESERVED + 1
    uint8_t i;
    for (i = blk_index + 1; i != blk_index; i++)
    {
        if (i == BLK_NUMS)
        {
            i = 0;
        }

        blk_info = &(g_kv_mgr.block_info[i]);
        if ((blk_info->space) > len)
        {
            g_kv_mgr.write_pos = (i << BLK_BITS) + BLK_SIZE - blk_info->space;
            if (blk_info->state == BLK_STATE_CLEAN)
            {
                if (kv_state_set((i << BLK_BITS), BLK_STATE_USED) != RES_OK)
                {
                    return 0;
                }
                blk_info->state = BLK_STATE_USED;
                (g_kv_mgr.clean_blk_nums)--;
            }
            return g_kv_mgr.write_pos;
        }
    }
#endif

    trigger_gc();
    return 0;
}

#if (MAPPING_TABLE_ENABLE == 1)
static int __item_mapping_table_init_cb(kv_item_t *item, const char *key)
{
    mapping_table_item_t *p_mapping_item = (mapping_table_item_t *)malloc(sizeof(mapping_table_item_t));
    if (!p_mapping_item)
    {
        return RES_MALLOC_FAILED;
    }
    memset(p_mapping_item, 0, sizeof(mapping_table_item_t));

    p_mapping_item->p_key = (char *)malloc(item->hdr.key_len);
    if (!p_mapping_item->p_key)
    {
        free(p_mapping_item);
        return RES_MALLOC_FAILED;
    }
    memset(p_mapping_item->p_key, 0, item->hdr.key_len);

    p_mapping_item->key_len = item->hdr.key_len;
    p_mapping_item->val_len = item->hdr.val_len;
    p_mapping_item->pos     = item->pos;
    raw_read(item->pos + ITEM_HEADER_SIZE, p_mapping_item->p_key, item->hdr.key_len);

    os_queue_in(&g_kv_mgr.mapping_table, p_mapping_item);

    return RES_CONT;
}

static int kv_mapping_table_item_add(const char *key, uint8_t key_len, uint16_t val_len,
                                     uint16_t offset)
{
    mapping_table_item_t *p_mapping_item;
    int ret;

    p_mapping_item = os_queue_peek(&g_kv_mgr.mapping_table, 0);
    while (p_mapping_item)
    {
        if ((p_mapping_item->key_len == key_len) &&
            (memcmp(p_mapping_item->p_key, key, key_len) == 0))
        {
            break;
        }

        p_mapping_item = p_mapping_item->p_next;
    }

    if (p_mapping_item)
    {
        p_mapping_item->val_len = val_len;
        p_mapping_item->pos = offset;
    }
    else
    {
        if (g_kv_mgr.mapping_table.count >= MAPPING_TABLE_ITEM_MAX)
        {
            ret = RES_NO_SPACE;
            goto fail_item_enqueue;
        }

        p_mapping_item = (mapping_table_item_t *)malloc(sizeof(mapping_table_item_t));
        if (!p_mapping_item)
        {
            ret = RES_MALLOC_FAILED;
            goto fail_alloc_item;
        }

        p_mapping_item->p_key = (char *)malloc(key_len);
        if (!p_mapping_item->p_key)
        {
            ret = RES_MALLOC_FAILED;
            goto fail_alloc_key;
        }

        //APP_PRINT_INFO2("kv_mapping_table_item_add: key %s, offset 0x%x", TRACE_STRING(key), offset);

        memcpy(p_mapping_item->p_key, key, key_len);
        p_mapping_item->key_len = key_len;
        p_mapping_item->val_len = val_len;
        p_mapping_item->pos = offset;

        os_queue_in(&g_kv_mgr.mapping_table, p_mapping_item);
    }

    return RES_OK;

fail_alloc_key:
    free(p_mapping_item);
fail_alloc_item:
fail_item_enqueue:
    APP_PRINT_ERROR2("kv_mapping_table_item_add: failed key %s, ret %d", TRACE_STRING(key), ret);
    return ret;
}

static int kv_mapping_table_item_del(uint16_t offset)
{
    mapping_table_item_t *p_mapping_item;

    p_mapping_item = os_queue_peek(&g_kv_mgr.mapping_table, 0);
    while (p_mapping_item)
    {
        if (p_mapping_item->pos == offset)
        {
            break;
        }

        p_mapping_item = p_mapping_item->p_next;
    }

    if (p_mapping_item)
    {
        //APP_PRINT_INFO1("kv_mapping_table_item_del: key %s", TRACE_STRING(p_mapping_item->p_key));

        os_queue_delete(&g_kv_mgr.mapping_table, p_mapping_item);
        if (p_mapping_item->p_key)
        {
            free(p_mapping_item->p_key);
        }
        free(p_mapping_item);

        return RES_OK;
    }

    return RES_ITEM_NOT_FOUND;
}
#endif

static int kv_item_del(kv_item_t *item, int mode)
{
    int ret = RES_OK;
    item_hdr_t hdr;
    uint8_t i;
    uint16_t offset;

    if (mode == KV_SELF_REMOVE)
    {
        offset = item->pos;
    }
    else if (mode == KV_ORIG_REMOVE)
    {
        offset = item->hdr.origin_off;
        memset(&hdr, 0, ITEM_HEADER_SIZE);
        if (raw_read(offset, &hdr, ITEM_HEADER_SIZE) != RES_OK)
        {
            return RES_FLASH_READ_ERR;
        }

        if ((hdr.magic != ITEM_MAGIC_NUM) ||
            (hdr.state != ITEM_STATE_NORMAL) ||
            (hdr.key_len != item->hdr.key_len))
        {
            return RES_OK;
        }

        char *origin_key = (char *)malloc(hdr.key_len);
        if (!origin_key)
        {
            return RES_MALLOC_FAILED;
        }
        char *new_key = (char *)malloc(hdr.key_len);
        if (!new_key)
        {
            free(origin_key);
            return RES_MALLOC_FAILED;
        }

        raw_read(offset + ITEM_HEADER_SIZE, origin_key, hdr.key_len);
        raw_read(item->pos + ITEM_HEADER_SIZE, new_key, hdr.key_len);
        if (memcmp(origin_key, new_key, hdr.key_len) != 0)
        {
            free(origin_key);
            free(new_key);
            return RES_OK;
        }

        free(origin_key);
        free(new_key);
    }
    else
    {
        return RES_INVALID_PARAM;
    }

    if ((ret = kv_state_set(offset, ITEM_STATE_DELETE)) != RES_OK)
    {
        return ret;
    }

#if (MAPPING_TABLE_ENABLE == 1)
    kv_mapping_table_item_del(offset);
#endif

    i = offset >> BLK_BITS;
    if (g_kv_mgr.block_info[i].state == BLK_STATE_USED)
    {
        if ((ret = kv_state_set((offset & BLK_OFF_MASK), BLK_STATE_DIRTY)) != RES_OK)
        {
            return ret;
        }
        g_kv_mgr.block_info[i].state = BLK_STATE_DIRTY;
    }

    return ret;
}

/*the function to be invoked while polling the used block*/
typedef int (*item_func)(kv_item_t *item, const char *key);

static int __item_recovery_cb(kv_item_t *item, const char *key)
{
    char *p = (char *)malloc(item->len);
    if (!p)
    {
        return RES_MALLOC_FAILED;
    }

    if (raw_read(item->pos + ITEM_HEADER_SIZE, p, item->len) != RES_OK)
    {
        free(p);
        return RES_FLASH_READ_ERR;
    }

    if (item->hdr.crc == utils_crc8((uint8_t *)p, item->len))
    {
        if ((item->hdr.origin_off != 0) && (item->pos != item->hdr.origin_off))
        {
            kv_item_del(item, KV_ORIG_REMOVE);
        }
    }
    else
    {
        kv_item_del(item, KV_SELF_REMOVE);
    }

    free(p);
    return RES_CONT;
}

static int __item_find_cb(kv_item_t *item, const char *key)
{
    if (item->hdr.key_len != strlen(key))
    {
        return RES_CONT;
    }

    item->store = (char *)malloc(item->hdr.key_len + item->hdr.val_len);
    if (!item->store)
    {
        return RES_MALLOC_FAILED;
    }

    if (raw_read(item->pos + ITEM_HEADER_SIZE, item->store, item->len) != RES_OK)
    {
        return RES_FLASH_READ_ERR;
    }

    if (memcmp(item->store, key, strlen(key)) == 0)
    {
        return RES_OK;
    }

    return RES_CONT;
}

static int __item_gc_cb(kv_item_t *item, const char *key)
{
    char *p;
    int ret;
    uint16_t len;
    uint8_t index;

    len = (ITEM_HEADER_SIZE + item->len + ~KV_ALIGN_MASK) & KV_ALIGN_MASK;
    p = (char *)malloc(len);
    if (!p)
    {
        return RES_MALLOC_FAILED;
    }

    if (raw_read(item->pos, p, len) != RES_OK)
    {
        ret = RES_FLASH_READ_ERR;
        goto err;
    }

    if (raw_write(g_kv_mgr.write_pos, p, len) != RES_OK)
    {
        ret = RES_FLASH_WRITE_ERR;
        goto err;
    }

#if (MAPPING_TABLE_ENABLE == 1)
    kv_mapping_table_item_add(p + ITEM_HEADER_SIZE, item->hdr.key_len, item->hdr.val_len,
                              g_kv_mgr.write_pos);
#endif

    g_kv_mgr.write_pos += len;
    index = (g_kv_mgr.write_pos) >> BLK_BITS;
    g_kv_mgr.block_info[index].space -= len;
    ret = RES_CONT;

err:
    free(p);
    return ret;
}

static kv_item_t *kv_item_traverse(item_func func, uint8_t blk_index, const char *key)
{
    kv_item_t *item;
    item_hdr_t *hdr;
    uint16_t pos = (blk_index << BLK_BITS) + BLK_HEADER_SIZE;
    uint16_t end = (blk_index << BLK_BITS) + BLK_SIZE;
    uint16_t len = 0;

    do
    {
        item = (kv_item_t *)malloc(sizeof(kv_item_t));
        if (!item)
        {
            return NULL;
        }
        memset(item, 0, sizeof(kv_item_t));
        hdr = &(item->hdr);

        if (raw_read(pos, hdr, ITEM_HEADER_SIZE) != RES_OK)
        {
            kv_item_free(item);
            return NULL;
        }

        if (hdr->magic != ITEM_MAGIC_NUM)
        {
            if ((hdr->magic == 0xFF) && (hdr->state == 0xFF))
            {
                kv_item_free(item);
                break;
            }
            hdr->val_len = 0xFFFF;
        }

        if (hdr->val_len > ITEM_MAX_VAL_LEN || hdr->key_len > ITEM_MAX_KEY_LEN)
        {
            pos += ITEM_HEADER_SIZE;
            kv_item_free(item);
            if (g_kv_mgr.block_info[blk_index].state == BLK_STATE_USED)
            {
                kv_state_set((blk_index << BLK_BITS), BLK_STATE_DIRTY);
                g_kv_mgr.block_info[blk_index].state = BLK_STATE_DIRTY;
            }
            continue;
        }

        len = (ITEM_HEADER_SIZE + hdr->key_len + hdr->val_len + ~KV_ALIGN_MASK) & KV_ALIGN_MASK;

        if (hdr->state == ITEM_STATE_NORMAL)
        {
            item->pos = pos;
            item->len = hdr->key_len + hdr->val_len;
            int ret = func(item, key);
            if (ret == RES_OK)
            {
                return item;
            }
            else if (ret != RES_CONT)
            {
                kv_item_free(item);
                return NULL;
            }
        }
        else
        {
            if (g_kv_mgr.block_info[blk_index].state == BLK_STATE_USED)
            {
                kv_state_set((blk_index << BLK_BITS), BLK_STATE_DIRTY);
                g_kv_mgr.block_info[blk_index].state = BLK_STATE_DIRTY;
            }
        }

        kv_item_free(item);
        pos += len;
    }
    while (end > (pos + ITEM_HEADER_SIZE));

    g_kv_mgr.block_info[blk_index].space = (end > pos) ? (end - pos) : ITEM_HEADER_SIZE;
    return NULL;
}

static kv_item_t *kv_item_get(const char *key)
{
    kv_item_t *item;
    uint8_t i;

    for (i = 0; i < BLK_NUMS; i++)
    {
        if (g_kv_mgr.block_info[i].state != BLK_STATE_CLEAN)
        {
            item = kv_item_traverse(__item_find_cb, i, key);
            if (item)
            {
                return item;
            }
        }
    }

    return NULL;
}

typedef struct
{
    char *p;
    int ret;
    uint16_t len;
} kv_storeage_t;
static int kv_item_store(const char *key, const void *val, int len, uint16_t origin_off)
{
    kv_storeage_t store;
    item_hdr_t hdr;
    char *p;
    uint16_t pos;

    hdr.magic = ITEM_MAGIC_NUM;
    hdr.state = ITEM_STATE_NORMAL;
    hdr.key_len = strlen(key);
    hdr.val_len = len;
    hdr.origin_off = origin_off;

    store.len = (ITEM_HEADER_SIZE + hdr.key_len + hdr.val_len + ~KV_ALIGN_MASK) & KV_ALIGN_MASK;
    store.p = (char *)malloc(store.len);
    if (!store.p)
    {
        APP_PRINT_INFO0("RES_MALLOC_FAILED");
        return RES_MALLOC_FAILED;
    }

    memset(store.p, 0, store.len);
    p = store.p + ITEM_HEADER_SIZE;
    memcpy(p, key, hdr.key_len);
    p += hdr.key_len;
    memcpy(p, val, hdr.val_len);
    p -= hdr.key_len;
    hdr.crc = utils_crc8((uint8_t *)p, hdr.key_len + hdr.val_len);
    memcpy(store.p, &hdr, ITEM_HEADER_SIZE);

    pos = kv_item_calc_pos(store.len);
    if (pos > 0)
    {
        store.ret = raw_write(pos, store.p, store.len);
        if (store.ret == RES_OK)
        {
            g_kv_mgr.write_pos = pos + store.len;
            uint8_t index = g_kv_mgr.write_pos >> BLK_BITS;
            g_kv_mgr.block_info[index].space -= store.len;

#if (MAPPING_TABLE_ENABLE == 1)
            kv_mapping_table_item_add(key, hdr.key_len, hdr.val_len, pos);
#endif
        }
    }
    else
    {
        APP_PRINT_INFO0("RES_NO_SPACE");
        store.ret = RES_NO_SPACE;
    }

    if (store.p)
    {
        free(store.p);
    }
    return store.ret;
}

static int kv_item_update(kv_item_t *item, const char *key, const void *val, int len)
{
    int ret;

    if (item->hdr.val_len == len)
    {
        if (!memcmp(item->store + item->hdr.key_len, val, len))
        {
            return RES_OK;
        }
    }

    ret = kv_item_store(key, val, len, item->pos);
    if (ret != RES_OK)
    {
        return ret;
    }

    ret = kv_item_del(item, KV_SELF_REMOVE);

    return ret;
}

static int kv_init(void)
{
    block_hdr_t hdr;
    int ret, nums = 0;
    uint8_t i, next;
    uint8_t unclean[BLK_NUMS] = {0};

    for (i = 0; i < BLK_NUMS; i++)
    {
        memset(&hdr, 0, sizeof(block_hdr_t));
        raw_read((i << BLK_BITS), &hdr, BLK_HEADER_SIZE);
        if (hdr.magic == BLK_MAGIC_NUM)
        {
            if (INVALID_BLK_STATE(hdr.state))
            {
                if ((ret = kv_block_format(i)) != RES_OK)
                {
                    return ret;
                }
                else
                {
                    continue;
                }
            }

            g_kv_mgr.block_info[i].state = hdr.state;
            kv_item_traverse(__item_recovery_cb, i, NULL);
            if (hdr.state == BLK_STATE_CLEAN)
            {
                if (g_kv_mgr.block_info[i].space != (BLK_SIZE - BLK_HEADER_SIZE))
                {
                    unclean[nums] = i;
                    nums++;
                }
                else
                {
                    (g_kv_mgr.clean_blk_nums)++;
                }
            }
        }
        else
        {
            if ((ret = kv_block_format(i)) != RES_OK)
            {
                return ret;
            }
        }
    }

    while (nums > 0)
    {
        i = unclean[nums - 1];
        if (g_kv_mgr.clean_blk_nums >= KV_GC_RESERVED)
        {
            if ((ret = kv_state_set((i << BLK_BITS), BLK_STATE_DIRTY)) != RES_OK)
            {
                return ret;
            }
            g_kv_mgr.block_info[i].state = BLK_STATE_DIRTY;
        }
        else
        {
            if ((ret = kv_block_format(i)) != RES_OK)
            {
                return ret;
            }
        }
        nums--;
    }

    if (g_kv_mgr.clean_blk_nums == 0)
    {
        if ((ret = kv_block_format(0)) != RES_OK)
        {
            return ret;
        }
    }

    if (g_kv_mgr.clean_blk_nums == BLK_NUMS)
    {
        g_kv_mgr.write_pos = BLK_HEADER_SIZE;
        if (!kv_state_set((g_kv_mgr.write_pos & BLK_OFF_MASK), BLK_STATE_USED))
        {
            g_kv_mgr.block_info[0].state = BLK_STATE_USED;
            (g_kv_mgr.clean_blk_nums)--;
        }
    }
    else
    {
        for (i = 0; i < BLK_NUMS; i++)
        {
            if ((g_kv_mgr.block_info[i].state == BLK_STATE_USED) ||
                (g_kv_mgr.block_info[i].state == BLK_STATE_DIRTY))
            {
                next = ((i + 1) == BLK_NUMS) ? 0 : (i + 1);
                if (g_kv_mgr.block_info[next].state == BLK_STATE_CLEAN)
                {
                    g_kv_mgr.write_pos = (i << BLK_BITS) + BLK_SIZE - g_kv_mgr.block_info[i].space;
                    break;
                }
            }
        }
    }

    return RES_OK;
}

void aos_kv_gc(void *arg)
{
    uint32_t notify;
    uint8_t i;
    uint8_t gc_index;
    uint8_t gc_copy = 0;
    uint16_t origin_pos;

    os_mutex_take(g_kv_mgr.kv_mutex, 0xffffffff);

    origin_pos = g_kv_mgr.write_pos;
    if (g_kv_mgr.clean_blk_nums == 0)
    {
        goto exit;
    }

    for (gc_index = 0; gc_index < BLK_NUMS; gc_index++)
    {
        if (g_kv_mgr.block_info[gc_index].state == BLK_STATE_CLEAN)
        {
            g_kv_mgr.write_pos = (gc_index << BLK_BITS) + BLK_HEADER_SIZE;
            break;
        }
    }

    if (gc_index == BLK_NUMS)
    {
        goto exit;
    }

    i = (origin_pos >> BLK_BITS) + 1;
    while (1)
    {
        if (i == BLK_NUMS)
        {
            i = 0;
        }

        if (g_kv_mgr.block_info[i].state == BLK_STATE_DIRTY)
        {
            kv_item_traverse(__item_gc_cb, i, NULL);

            gc_copy = 1;
            if (kv_block_format(i) != RES_OK)
            {
                goto exit;
            }

            kv_state_set((g_kv_mgr.write_pos & BLK_OFF_MASK), BLK_STATE_USED);
            g_kv_mgr.block_info[gc_index].state = BLK_STATE_USED;
            (g_kv_mgr.clean_blk_nums)--;
            break;
        }
        if (i == (origin_pos >> BLK_BITS))
        {
            break;
        }
        i++;
    }

    if (gc_copy == 0)
    {
        g_kv_mgr.write_pos = origin_pos;
    }

exit:
    g_kv_mgr.gc_triggered = 0;
    os_mutex_give(g_kv_mgr.kv_mutex);
    if (g_kv_mgr.gc_waiter > 0)
    {
        os_sem_give(g_kv_mgr.gc_sem);
    }
}

int aos_kv_del(const char *key)
{
    kv_item_t *item;
    int ret;
    os_mutex_take(g_kv_mgr.kv_mutex, 0xffffffff);

    item = kv_item_get(key);
    if (!item)
    {
        os_mutex_give(g_kv_mgr.kv_mutex);
        return RES_ITEM_NOT_FOUND;
    }

    ret = kv_item_del(item, KV_SELF_REMOVE);
    kv_item_free(item);
    os_mutex_give(g_kv_mgr.kv_mutex);
    return ret;
}

int aos_kv_set(const char *key, const void *val, int len, int sync)
{
    kv_item_t *item;
    int ret;
    if (!key || !val || len < 0 || strlen(key) > ITEM_MAX_KEY_LEN || len > ITEM_MAX_VAL_LEN)
    {
        return RES_INVALID_PARAM;
    }

    if (g_kv_mgr.gc_triggered)
    {
        (g_kv_mgr.gc_waiter)++;
        os_sem_take(g_kv_mgr.gc_sem, 0xffffffff);
    }

    os_mutex_take(g_kv_mgr.kv_mutex, 0xffffffff);

    item = kv_item_get(key);
    if (item)
    {
        ret = kv_item_update(item, key, val, len);
        kv_item_free(item);
    }
    else
    {
        ret = kv_item_store(key, val, len, 0);
    }

    os_mutex_give(g_kv_mgr.kv_mutex);
    return ret;
}

#if (MAPPING_TABLE_ENABLE == 1)
void *aos_key_find(const char *key)
{
    mapping_table_item_t *p_mapping_item;

    p_mapping_item = os_queue_peek(&g_kv_mgr.mapping_table, 0);
    while (p_mapping_item)
    {
        if ((p_mapping_item->key_len == strlen(key)) &&
            (memcmp(p_mapping_item->p_key, key, strlen(key)) == 0))
        {
            break;
        }

        p_mapping_item = p_mapping_item->p_next;
    }

    // if (p_mapping_item == NULL)
    // {
    //     APP_PRINT_ERROR1("aos_key_find: key %s failed", TRACE_STRING(key));
    // }

    return p_mapping_item;
}

int aos_kv_get(const char *key, void *buffer, int *buffer_len)
{
    mapping_table_item_t *p_mapping_item;

    if (!key || !buffer || !buffer_len || *buffer_len <= 0)
    {
        return RES_INVALID_PARAM;
    }

    os_mutex_take(g_kv_mgr.kv_mutex, 0xffffffff);

    p_mapping_item = os_queue_peek(&g_kv_mgr.mapping_table, 0);
    while (p_mapping_item)
    {
        if ((p_mapping_item->key_len == strlen(key)) &&
            (memcmp(p_mapping_item->p_key, key, strlen(key)) == 0))
        {
            break;
        }

        p_mapping_item = p_mapping_item->p_next;
    }

    os_mutex_give(g_kv_mgr.kv_mutex);

    if (p_mapping_item)
    {
        if (*buffer_len < p_mapping_item->val_len)
        {
            *buffer_len = p_mapping_item->val_len;
            return RES_NO_SPACE;
        }

        raw_read(p_mapping_item->pos + ITEM_HEADER_SIZE + p_mapping_item->key_len, buffer,
                 p_mapping_item->val_len);
        *buffer_len = p_mapping_item->val_len;

        return RES_OK;
    }

    return RES_ITEM_NOT_FOUND;
}

void kv_mapping_table_init(void)
{
    os_queue_init(&g_kv_mgr.mapping_table);

    for (int i = 0; i < BLK_NUMS; i++)
    {
        kv_item_traverse(__item_mapping_table_init_cb, i, NULL);
    }
}

void kv_mapping_table_deinit()
{
    if (g_kv_mgr.kv_initialize)
    {
        mapping_table_item_t *p_mapping_item;
        mapping_table_item_t *p_next;

        p_mapping_item = os_queue_peek(&g_kv_mgr.mapping_table, 0);
        while (p_mapping_item)
        {
            p_next = p_mapping_item->p_next;

            os_queue_delete(&g_kv_mgr.mapping_table, p_mapping_item);
            if (p_mapping_item->p_key)
            {
                free(p_mapping_item->p_key);
            }
            free(p_mapping_item);

            p_mapping_item = p_next;
        }
    }
}
#else
static int __item_peek_cb(kv_item_t *item, const char *key)
{
    char *p_key = (char *)malloc(item->hdr.key_len);
    if (!p_key)
    {
        return RES_MALLOC_FAILED;
    }

    memset(p_key, 0, item->hdr.key_len);
    if (raw_read(item->pos + ITEM_HEADER_SIZE, p_key, item->hdr.key_len) != RES_OK)
    {
        free(p_key);
        return RES_FLASH_READ_ERR;
    }

    if (memcmp(p_key, key, strlen(key)) == 0)
    {
        free(p_key);
        return RES_OK;
    }
    else
    {
        free(p_key);
        return RES_CONT;
    }
}

void *aos_key_find(const char *key)
{
    kv_item_t *item;
    uint8_t i;

    for (i = 0; i < BLK_NUMS; i++)
    {
        if (g_kv_mgr.block_info[i].state != BLK_STATE_CLEAN)
        {
            item = kv_item_traverse(__item_peek_cb, i, key);
            if (item)
            {
                return item;
            }
        }
    }

    //APP_PRINT_ERROR1("aos_key_find: key %s failed", TRACE_STRING(key));
    return NULL;
}

int aos_kv_get(const char *key, void *buffer, int *buffer_len)
{
    kv_item_t *item = NULL;
    int ret;

    if (!key || !buffer || !buffer_len || *buffer_len <= 0)
    {
        return RES_INVALID_PARAM;
    }

    os_mutex_take(g_kv_mgr.kv_mutex, 0xffffffff);

    item = kv_item_get(key);

    os_mutex_give(g_kv_mgr.kv_mutex);

    if (!item)
    {
        return RES_ITEM_NOT_FOUND;
    }

    if (*buffer_len < item->hdr.val_len)
    {
        *buffer_len = item->hdr.val_len;
        kv_item_free(item);
        return RES_NO_SPACE;
    }
    else
    {
        memcpy(buffer, (item->store + item->hdr.key_len), item->hdr.val_len);
        *buffer_len = item->hdr.val_len;
    }

    kv_item_free(item);
    return RES_OK;
}
#endif

/* CLI Support */
#if 1
static int __item_print_cb(kv_item_t *item, const char *key)
{
    char *p_key = (char *)malloc(item->hdr.key_len + 1);
    if (!p_key)
    {
        return RES_MALLOC_FAILED;
    }
    memset(p_key, 0, item->hdr.key_len + 1);
    raw_read(item->pos + ITEM_HEADER_SIZE, p_key, item->hdr.key_len);

    char *p_val = (char *)malloc(item->hdr.val_len + 1);
    if (!p_val)
    {
        free(p_key);
        return RES_MALLOC_FAILED;
    }
    memset(p_val, 0, item->hdr.val_len + 1);
    raw_read(item->pos + ITEM_HEADER_SIZE + item->hdr.key_len, p_val, item->hdr.val_len);

    APP_PRINT_INFO1("%s = ", TRACE_STRING(p_key));
    for (uint16_t i = 0; i < item->hdr.val_len; i++)
    {
        APP_PRINT_TRACE1("%02x", p_val[i]);
    }

    free(p_key);
    free(p_val);

    return RES_CONT;
}

otError kv_cmd_handler(void *aContext, uint8_t argc, char *argv[])
{
    int ret = 0;
    char *buffer = NULL;

    if (!argc)
    {
        return OT_ERROR_INVALID_ARGS;
    }

    if (strncmp(argv[0], "set", 3) == 0)
    {
        if (argc != 3)
        {
            return OT_ERROR_INVALID_ARGS;
        }
        ret = aos_kv_set(argv[1], argv[2], strlen(argv[2]), 1);
        if (ret != 0)
        {
            APP_PRINT_INFO0("cli set kv failed");
        }
    }
    else if (strncmp(argv[0], "get", 3) == 0)
    {
        if (argc != 2)
        {
            return OT_ERROR_INVALID_ARGS;
        }

        buffer = malloc(ITEM_MAX_VAL_LEN);
        if (!buffer)
        {
            APP_PRINT_INFO0("cli get kv failed");
            return OT_ERROR_NO_BUFS;
        }

        memset(buffer, 0, ITEM_MAX_VAL_LEN);
        int len = ITEM_MAX_VAL_LEN;

        ret = aos_kv_get(argv[1], buffer, &len);
        if (ret != 0)
        {
            APP_PRINT_INFO0("cli: no paired kv");
        }
        else
        {
            APP_PRINT_INFO0("value is ");
            for (int i = 0; i < len; i++)
            {
                APP_PRINT_TRACE1("%02x", buffer[i]);
            }
        }

        if (buffer)
        {
            free(buffer);
        }
    }
    else if (strncmp(argv[0], "del", 3) == 0)
    {
        if (argc != 2)
        {
            return OT_ERROR_INVALID_ARGS;
        }
        ret = aos_kv_del(argv[1]);
        if (ret != 0)
        {
            APP_PRINT_INFO0("cli kv del failed");
        }
    }
    else if (strncmp(argv[0], "list", 4) == 0)
    {
        APP_PRINT_INFO0("list begin");

        for (int i = 0; i < BLK_NUMS; i++)
        {
            kv_item_traverse(__item_print_cb, i, NULL);
        }
        APP_PRINT_INFO0("list end");
    }
    return OT_ERROR_NONE;
}
#endif

int aos_kv_init(uint32_t kv_addr)
{
    uint8_t blk_index;
    int ret;

    if (g_kv_mgr.kv_initialize)
    {
        return RES_OK;
    }

    if (BLK_NUMS <= KV_GC_RESERVED)
    {
        return -EINVAL;
    }

    memset(&g_kv_mgr, 0, sizeof(g_kv_mgr));
    os_mutex_create(&g_kv_mgr.kv_mutex);

    g_kv_mgr.kv_addr = kv_addr;
    if ((ret = kv_init()) != RES_OK)
    {
        return ret;
    }

    os_sem_create(&g_kv_mgr.gc_sem, "gc_sem", 0, 1);

    g_kv_mgr.kv_initialize = 1;

#if (MAPPING_TABLE_ENABLE == 1)
    kv_mapping_table_init();
#endif

    blk_index = (g_kv_mgr.write_pos >> BLK_BITS);
    if (((g_kv_mgr.block_info[blk_index].space) < ITEM_MAX_LEN) &&
        (g_kv_mgr.clean_blk_nums < KV_GC_RESERVED + 1))
    {
        trigger_gc();
    }

    return RES_OK;
}


void aos_kv_deinit(uint32_t kv_addr)
{
#if (MAPPING_TABLE_ENABLE == 1)
    kv_mapping_table_deinit();
#endif

    g_kv_mgr.kv_initialize = 0;
    g_kv_mgr.kv_addr = kv_addr;
    if (g_kv_mgr.gc_sem)
    {
        os_sem_delete(g_kv_mgr.gc_sem);
    }
    if (g_kv_mgr.kv_mutex)
    {
        os_mutex_delete(g_kv_mgr.kv_mutex);
    }
}

void aos_kv_delall(void)
{
    for (uint32_t i = 0; i < BLK_NUMS; i++)
    {
        flash_nor_erase_locked((g_kv_mgr.kv_addr + (i * BLK_SIZE)), FLASH_NOR_ERASE_SECTOR);
    }
}