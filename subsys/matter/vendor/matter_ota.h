#ifndef MATTER_OTA_H_
#define MATTER_OTA_H_

#ifdef __cplusplus
extern "C" {
#endif

uint8_t rtk_matter_ota_prepare(void);

uint8_t rtk_matter_ota_finalize(void);

void rtk_matter_ota_apply(void);

uint8_t rtk_matter_ota_abort(void);

uint8_t rtk_matter_ota_process_block(uint8_t *p_data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif
