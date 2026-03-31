#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include "trace.h"
#include "bt_types.h"
#include "os_mem.h"
#ifdef BOARD_RTL8771HTV
#include "dfu_flash.h"
#else
#include "fmc_api.h"
#include "dfu_common.h"
#endif

#include "matter_ota.h"

#define MATTER_OTA_BUF_SIZE       (2048)

#define MP_HEADER_SIZE            (512)
#define PACK_HEADER_INDICATOR_LEN (32)
#define PACK_HEADER_LEN1          (40 + PACK_HEADER_INDICATOR_LEN)
#define SUB_FILE_HEADER_LEN       (12)

//MP header item id
#define MP_HEADER_BIN_ID           0x0001
#define MP_HEADER_VERSION          0x0002
#define MP_HEADER_PART_NUM         0x0003
#define MP_HEADER_DATA_LEN         0x0004
#define MP_HEADER_BIN_DEPEND       0x0020
#define MP_HEADER_DATE             0x0052
#define MP_HEADER_OTA_VER          0x0011
#define MP_HEADER_IMAGE_ID         0x0012
#define MP_HEADER_FLASH_OTA_ADDR   0x0013
#define MP_HEADER_IMAGE_SIZE       0x0014
#define MP_HEADER_SECURE_VER       0x0015
#define MP_HEADER_IMAGE_VER        0x0016
#define MP_HEADER_VER_VER          0x0022
#define MP_HEADER_IC_TYPE          0x0100

#ifdef BOARD_RTL8771HTV
#define dfu_write_data_to_flash   dfu_update
#define dfu_checksum              dfu_check_checksum

typedef enum
{
    OTA_BANK0,
    OTA_BANK1,
    OTA_BANK_MAX,
} T_ACTIVE_BANK_NUM;
#endif

typedef enum t_matter_ota_state
{
    MATTER_OTA_STATE_PACK_HEADER,
    MATTER_OTA_STATE_IMAGE_HEADER,
    MATTER_OTA_STATE_IMAGE_PAYLOAD,
    MATTER_OTA_STATE_MP_INFO,
} T_MATTER_OTA_STATE;

typedef struct t_sub_image_info
{
    IMG_ID   image_id;
    uint32_t download_addr;
    uint32_t image_size;
    uint32_t image_offset;
} T_SUB_IMAGE_INFO;

typedef struct t_matter_ota_db
{
    uint8_t            *p_buf;
    uint16_t            buf_offset;
    T_SUB_IMAGE_INFO   *p_sub_image_info;
    uint8_t             sub_image_num;
    uint8_t             cur_image_index;
    uint32_t            cur_download_size;
    uint16_t            cur_image_id;
    uint32_t            cur_image_payload_size;
    bool                is_data_valid;
    T_ACTIVE_BANK_NUM   active_bank;
    T_MATTER_OTA_STATE  state;
} T_MATTER_OTA_DB;

T_MATTER_OTA_DB *matter_ota_db = NULL;

#ifdef BOARD_RTL8771HTV
T_ACTIVE_BANK_NUM get_active_bank_num()
{
    T_ACTIVE_BANK_NUM bank_num = OTA_BANK_MAX;

    uint32_t ota_bank0_addr = flash_nor_get_bank_addr(FLASH_OTA_BANK_0);
    uint32_t ota_bank1_addr = flash_nor_get_bank_addr(FLASH_OTA_BANK_1);
    uint32_t ota_header_addr = get_active_ota_bank_addr();
    if (ota_bank0_addr == ota_header_addr)
    {
        bank_num = OTA_BANK0;
    }
    else if (ota_bank1_addr == ota_header_addr)
    {
        bank_num = OTA_BANK1;
    }
    else
    {
        /* invalid case */
        //bank_num = OTA_BANK_MAX;
    }

    return bank_num;
}
#endif

int32_t matter_ota_get_item_pos_from_header(uint16_t item_id, uint8_t *p_header)
{
    int32_t i;

    for (i = 0; i < MP_HEADER_SIZE;)
    {
        uint16_t id;
        uint8_t len;

        LE_ARRAY_TO_UINT16(id, &p_header[i]);
        len = p_header[i + 2];

        if (id == item_id)
        {
            return i;
        }
        else
        {
            i += (3 + len);
        }
    }

    return -1;
}

bool rtk_matter_ota_append(uint8_t *p_data, uint16_t len)
{
    if (matter_ota_db->buf_offset + len > MATTER_OTA_BUF_SIZE)
    {
        MATTER_PRINT_INFO0("rtk_matter_ota_append: failed");
        return false;
    }

    memcpy(matter_ota_db->p_buf + matter_ota_db->buf_offset, p_data, len);
    matter_ota_db->buf_offset += len;

    return true;
}

uint32_t rtk_matter_ota_get_int_count(uint32_t file_indicator)
{
    file_indicator = (file_indicator & 0x55555555) + ((file_indicator >> 1) & 0x55555555);
    file_indicator = (file_indicator & 0x33333333) + ((file_indicator >> 2) & 0x33333333);
    file_indicator = (file_indicator & 0x0F0F0F0F) + ((file_indicator >> 4) & 0x0F0F0F0F);
    file_indicator = (file_indicator & 0x00FF00FF) + ((file_indicator >> 8) & 0x00FF00FF);
    file_indicator = (file_indicator & 0x0000FFFF) + ((file_indicator >> 16) & 0x0000FFFF);

    return file_indicator;
}

uint8_t rtk_matter_ota_get_indicator_count()
{
    uint8_t count;
    uint8_t i;

    count = 0;
    for (i = 0; i < PACK_HEADER_INDICATOR_LEN; i += 4)
    {
        count += rtk_matter_ota_get_int_count(*(uint32_t *)(matter_ota_db->p_buf + 40 + i));
    }

    return count;
}

bool rtk_matter_ota_process_pack_header()
{
    uint8_t ret = 0;

    if (matter_ota_db->state == MATTER_OTA_STATE_PACK_HEADER)
    {
        if (matter_ota_db->buf_offset >= PACK_HEADER_LEN1)
        {
            uint8_t pack_header_len;

            if (matter_ota_db->sub_image_num == 0)
            {
                matter_ota_db->sub_image_num = rtk_matter_ota_get_indicator_count();
            }

            MATTER_PRINT_INFO1("rtk_matter_ota_process_pack_header: sub image num %d",
                               matter_ota_db->sub_image_num);

            if (matter_ota_db->sub_image_num == 0)
            {
                ret = 1;
                goto fail_invalid_image_num;
            }

            if (matter_ota_db->p_sub_image_info == NULL)
            {
                matter_ota_db->p_sub_image_info = os_mem_zalloc(RAM_TYPE_DATA_ON,
                                                                sizeof(T_SUB_IMAGE_INFO) * matter_ota_db->sub_image_num);
                if (matter_ota_db->p_sub_image_info == NULL)
                {
                    ret = 2;
                    goto fail_alloc_buf;
                }
            }

            pack_header_len = PACK_HEADER_LEN1 + SUB_FILE_HEADER_LEN * matter_ota_db->sub_image_num;
            if (matter_ota_db->buf_offset >= pack_header_len)
            {
                uint16_t tmp_buf_left;
                uint8_t *p_buf = matter_ota_db->p_buf + PACK_HEADER_LEN1;

                for (uint8_t i = 0; i < matter_ota_db->sub_image_num; i++)
                {
                    LE_ARRAY_TO_UINT32(matter_ota_db->p_sub_image_info[i].download_addr, p_buf);
                    p_buf += SUB_FILE_HEADER_LEN;
                }

                tmp_buf_left = matter_ota_db->buf_offset - pack_header_len;
                memmove(matter_ota_db->p_buf, matter_ota_db->p_buf + pack_header_len, tmp_buf_left);
                matter_ota_db->buf_offset = tmp_buf_left;

                matter_ota_db->cur_image_index = 0;
                matter_ota_db->state = MATTER_OTA_STATE_IMAGE_HEADER;
            }
            /*else buffer too small*/
        }
        /*else buffer too small*/
    }

    return true;

fail_alloc_buf:
fail_invalid_image_num:
    MATTER_PRINT_ERROR1("rtk_matter_ota_process_pack_header: failed %d", -ret);
    return false;
}

bool rtk_matter_ota_process_mp_header()
{
    uint8_t ret = 0;

    if (matter_ota_db->state == MATTER_OTA_STATE_IMAGE_HEADER)
    {
        if (matter_ota_db->buf_offset >= MP_HEADER_SIZE)
        {
            uint16_t tmp_buf_left;
            int32_t pos;

            pos = matter_ota_get_item_pos_from_header(MP_HEADER_IMAGE_ID, matter_ota_db->p_buf);
            if (pos == -1)
            {
                ret = 1;
                goto fail_check_id;
            }
            LE_ARRAY_TO_UINT16(matter_ota_db->cur_image_id, &(matter_ota_db->p_buf[pos + 3]));

            pos = matter_ota_get_item_pos_from_header(MP_HEADER_DATA_LEN, matter_ota_db->p_buf);
            if (pos == -1)
            {
                ret = 2;
                goto fail_check_len;
            }
            LE_ARRAY_TO_UINT32(matter_ota_db->cur_image_payload_size, &(matter_ota_db->p_buf[pos + 3]));

            matter_ota_db->p_sub_image_info[matter_ota_db->cur_image_index].image_id =
                matter_ota_db->cur_image_id;
            matter_ota_db->p_sub_image_info[matter_ota_db->cur_image_index].image_size =
                matter_ota_db->cur_image_payload_size;
            if (matter_ota_db->cur_image_id == IMG_BOOTPATCH)
            {
                matter_ota_db->p_sub_image_info[matter_ota_db->cur_image_index].image_size = 0;
                matter_ota_db->p_sub_image_info[matter_ota_db->cur_image_index].image_offset = 0;
            }
            else
            {
                if (matter_ota_db->cur_image_index == 0)
                {
                    matter_ota_db->p_sub_image_info[matter_ota_db->cur_image_index].image_offset = 0;
                }
                else
                {
                    matter_ota_db->p_sub_image_info[matter_ota_db->cur_image_index].image_offset =
                        matter_ota_db->p_sub_image_info[matter_ota_db->cur_image_index - 1].image_offset +
                        matter_ota_db->p_sub_image_info[matter_ota_db->cur_image_index - 1].image_size;
                }
            }

            MATTER_PRINT_INFO3("rtk_matter_ota_process_mp_header: image id 0x%04x, payload size 0x%x, offset 0x%x",
                               matter_ota_db->p_sub_image_info[matter_ota_db->cur_image_index].image_id,
                               matter_ota_db->p_sub_image_info[matter_ota_db->cur_image_index].image_size,
                               matter_ota_db->p_sub_image_info[matter_ota_db->cur_image_index].image_offset);

            tmp_buf_left = matter_ota_db->buf_offset - MP_HEADER_SIZE;
            memmove(matter_ota_db->p_buf, matter_ota_db->p_buf + MP_HEADER_SIZE, tmp_buf_left);
            matter_ota_db->buf_offset = tmp_buf_left;

            if (is_ota_support_bank_switch())
            {
                if ((matter_ota_db->active_bank == OTA_BANK0 &&
                     matter_ota_db->cur_image_index < matter_ota_db->sub_image_num / 2) ||
                    (matter_ota_db->active_bank == OTA_BANK1 &&
                     matter_ota_db->cur_image_index >= matter_ota_db->sub_image_num / 2))
                {
                    matter_ota_db->is_data_valid = false;
                }
                else
                {
                    matter_ota_db->is_data_valid = true;
                }
            }

            matter_ota_db->cur_download_size = 0;
            matter_ota_db->state = MATTER_OTA_STATE_IMAGE_PAYLOAD;
        }
        /*else buffer too small*/
    }

    return true;

fail_check_len:
fail_check_id:
    MATTER_PRINT_ERROR1("rtk_matter_ota_process_mp_header: failed %d", -ret);
    return false;
}

bool rtk_matter_ota_process_payload()
{
    uint8_t ret = 0;

    if (matter_ota_db->state == MATTER_OTA_STATE_IMAGE_PAYLOAD)
    {
        if (matter_ota_db->cur_download_size + matter_ota_db->buf_offset <
            matter_ota_db->cur_image_payload_size)
        {
            if (is_ota_support_bank_switch())
            {
                if (matter_ota_db->is_data_valid)
                {
                    if (dfu_write_data_to_flash(matter_ota_db->cur_image_id,
                                                matter_ota_db->cur_download_size,
                                                0,
                                                matter_ota_db->buf_offset,
                                                matter_ota_db->p_buf) != 0)
                    {
                        ret = 1;
                        goto fail_write_data;
                    }
                }
            }
            else
            {
                //OTA temp
                if (dfu_write_data_to_flash(matter_ota_db->cur_image_id,
                                            matter_ota_db->cur_download_size,
                                            matter_ota_db->p_sub_image_info[matter_ota_db->cur_image_index].image_offset,
                                            matter_ota_db->buf_offset,
                                            matter_ota_db->p_buf) != 0)
                {
                    ret = 1;
                    goto fail_write_data;
                }
            }

            matter_ota_db->cur_download_size += matter_ota_db->buf_offset;
            matter_ota_db->buf_offset = 0;
        }
        else
        {
            //last block
            uint16_t last_pkt_len;
            uint16_t tmp_buf_left;

            last_pkt_len = matter_ota_db->cur_image_payload_size - matter_ota_db->cur_download_size;

            if (is_ota_support_bank_switch())
            {
                if (matter_ota_db->is_data_valid)
                {
                    if (dfu_write_data_to_flash(matter_ota_db->cur_image_id,
                                                matter_ota_db->cur_download_size,
                                                0,
                                                last_pkt_len,
                                                matter_ota_db->p_buf) != 0)
                    {
                        ret = 2;
                        goto fail_write_data;
                    }

                    if (dfu_checksum(matter_ota_db->cur_image_id, 0) == false)
                    {
                        ret = 3;
                        goto fail_check_crc;
                    }
                }
            }
            else
            {
                //OTA temp
                if (dfu_write_data_to_flash(matter_ota_db->cur_image_id,
                                            matter_ota_db->cur_download_size,
                                            matter_ota_db->p_sub_image_info[matter_ota_db->cur_image_index].image_offset,
                                            last_pkt_len,
                                            matter_ota_db->p_buf) != 0)
                {
                    ret = 2;
                    goto fail_write_data;
                }

                if (dfu_checksum(matter_ota_db->cur_image_id,
                                 matter_ota_db->p_sub_image_info[matter_ota_db->cur_image_index].image_offset) == false)
                {
                    ret = 3;
                    goto fail_check_crc;
                }
            }

            matter_ota_db->cur_download_size += last_pkt_len;

            //memmove
            tmp_buf_left = matter_ota_db->buf_offset - last_pkt_len;
            memmove(matter_ota_db->p_buf, matter_ota_db->p_buf + last_pkt_len, tmp_buf_left);
            matter_ota_db->buf_offset = tmp_buf_left;

            //change state & image index
            matter_ota_db->cur_image_index++;
            if (matter_ota_db->cur_image_index < matter_ota_db->sub_image_num)
            {
                matter_ota_db->state = MATTER_OTA_STATE_IMAGE_HEADER;
            }
            else
            {
                matter_ota_db->state = MATTER_OTA_STATE_MP_INFO;
            }
        }

        MATTER_PRINT_INFO1("rtk_matter_ota_process_payload: image offset 0x%x",
                           matter_ota_db->cur_download_size);
    }

    return true;

fail_check_crc:
fail_write_data:
    MATTER_PRINT_ERROR1("rtk_matter_ota_process_payload: failed %d", -ret);
    return false;
}

uint8_t rtk_matter_ota_prepare(void)
{
    uint8_t ret = 0;

    if (matter_ota_db != NULL)
    {
        ret = 1;
        goto fail_db_not_cleared;
    }

    matter_ota_db = os_mem_alloc(RAM_TYPE_DATA_ON, sizeof(T_MATTER_OTA_DB));
    if (matter_ota_db == NULL)
    {
        ret = 2;
        goto fail_alloc_db;
    }

    matter_ota_db->p_buf = os_mem_alloc(RAM_TYPE_DATA_ON, MATTER_OTA_BUF_SIZE);
    if (matter_ota_db->p_buf == NULL)
    {
        ret = 3;
        goto fail_alloc_buf;
    }

    matter_ota_db->state = MATTER_OTA_STATE_PACK_HEADER;
    matter_ota_db->buf_offset = 0;
    matter_ota_db->p_sub_image_info = NULL;
    matter_ota_db->sub_image_num = 0;
    matter_ota_db->cur_image_index = 0;
    matter_ota_db->is_data_valid = false;
    matter_ota_db->active_bank = get_active_bank_num();

    MATTER_PRINT_INFO1("rtk_matter_ota_prepare: active bank %d", matter_ota_db->active_bank);

    return 0;

fail_alloc_buf:
    os_mem_free(matter_ota_db);
    matter_ota_db = NULL;
fail_alloc_db:
fail_db_not_cleared:
    MATTER_PRINT_ERROR1("rtk_matter_ota_prepare: failed %d", -ret);
    return ret;
}

void rtk_matter_ota_clear()
{
    if (matter_ota_db)
    {
        if (matter_ota_db->p_sub_image_info)
        {
            os_mem_free(matter_ota_db->p_sub_image_info);
        }

        if (matter_ota_db->p_buf)
        {
            os_mem_free(matter_ota_db->p_buf);
        }

        os_mem_free(matter_ota_db);
        matter_ota_db = NULL;
    }
}

uint8_t rtk_matter_ota_finalize(void)
{
    uint8_t i;

    if (matter_ota_db == NULL)
    {
        MATTER_PRINT_ERROR0("rtk_matter_ota_finalize: not prepared");
        return 1;
    }

    for (i = 0; i < matter_ota_db->sub_image_num; i++)
    {
        if (is_ota_support_bank_switch())
        {
            if ((matter_ota_db->active_bank == OTA_BANK0 && i >= matter_ota_db->sub_image_num / 2) ||
                (matter_ota_db->active_bank == OTA_BANK1 && i < matter_ota_db->sub_image_num / 2))
            {
                dfu_set_ready((T_IMG_HEADER_FORMAT *)matter_ota_db->p_sub_image_info[i].download_addr);
            }
        }
        else
        {
            //OTA temp
#ifdef BOARD_RTL8771HTV
            uint32_t base_addr = flash_nor_get_bank_addr(FLASH_OTA_TMP);
#else
            uint32_t base_addr = flash_partition_addr_get(PARTITION_FLASH_OTA_TMP);
#endif
            T_IMG_HEADER_FORMAT *p_header;

            if (matter_ota_db->p_sub_image_info[i].image_id == IMG_BOOTPATCH)
            {
                p_header = (T_IMG_HEADER_FORMAT *)dfu_get_temp_ota_bank_img_addr_by_img_id(IMG_BOOTPATCH);
            }
            else
            {
                p_header = (T_IMG_HEADER_FORMAT *)(base_addr + matter_ota_db->p_sub_image_info[i].image_offset);
            }

            if (p_header)
            {
                dfu_set_ready(p_header);
            }
        }
    }

    rtk_matter_ota_clear();

    MATTER_PRINT_INFO0("rtk_matter_ota_finalize");

    return 0;
}

void rtk_matter_ota_apply(void)
{
    MATTER_PRINT_INFO0("rtk_matter_ota_apply");

#ifdef BOARD_RTL8771HTV
    dfu_fw_reboot(true);
#else
    dfu_fw_reboot(RESET_ALL, DFU_ACTIVE_RESET);
#endif
}

uint8_t rtk_matter_ota_abort(void)
{
    if (matter_ota_db == NULL)
    {
        MATTER_PRINT_ERROR0("rtk_matter_ota_abort: not prepared");
        return 1;
    }

    MATTER_PRINT_INFO0("rtk_matter_ota_abort");

    rtk_matter_ota_clear();

    return 0;
}

uint8_t rtk_matter_ota_process_block(uint8_t *p_data, uint16_t len)
{
    uint8_t ret = 0;

    if (matter_ota_db == NULL)
    {
        ret = 1;
        goto fail_db_not_prepared;
    }

    if (matter_ota_db->state == MATTER_OTA_STATE_MP_INFO)
    {
        //just drop mp info to save time
        return 0;
    }

    if (p_data == NULL || len == 0)
    {
        ret = 2;
        goto fail_input_validation;
    }

    if (!rtk_matter_ota_append(p_data, len))
    {
        ret = 3;
        goto fail_append_data;
    }

    if (!rtk_matter_ota_process_pack_header())
    {
        ret = 4;
        goto fail_process_pack_header;
    }

    if (!rtk_matter_ota_process_mp_header())
    {
        ret = 5;
        goto fail_process_mp_header;
    }

    if (!rtk_matter_ota_process_payload())
    {
        ret = 6;
        goto fail_process_payload;
    }

    return 0;

fail_process_payload:
fail_process_mp_header:
fail_process_pack_header:
fail_append_data:
fail_input_validation:
fail_db_not_prepared:
    MATTER_PRINT_INFO1("rtk_matter_ota_process_block: failed %d", -ret);
    return ret;
}

#ifdef __cplusplus
}
#endif