/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      bt_matter_adapter_app_task.h
   * @brief     Routines to create App task and handle events & messages
   * @author
   * @date
   * @version
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */
#ifndef _BT_MATTER_ADAPTER_APP_TASK_H_
#define _BT_MATTER_ADAPTER_APP_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

void bt_matter_adapter_send_msg(uint16_t sub_type, uint32_t param);

void bt_matter_adapter_app_task_init(void);

void bt_matter_adapter_app_task_deinit(void);

void bt_matter_adapter_adv_start();

void bt_matter_adapter_adv_stop();

void bt_matter_adapter_adv_config(uint16_t adv_int_min,
                                  uint16_t adv_int_max,
                                  uint8_t  *adv_data,
                                  uint8_t  adv_data_len);

uint8_t bt_matter_adapter_le_disconnect(uint8_t conn_id);

#ifdef __cplusplus
}
#endif
#endif
