/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
  ******************************************************************************
  * @file    bt_matter_adapter_app_main.h
  * @author
  * @version
  * @brief
  ******************************************************************************
  */
#ifndef __BT_MATTER_ADAPTER_APP_MAIN_H_
#define __BT_MATTER_ADAPTER_APP_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

int bt_matter_adapter_init(uint8_t link_num);

void bt_matter_adapter_device_name_set(void *device_name, uint8_t device_name_len);

uint16_t bt_matter_adapter_mtu_get(uint16_t conn_id);

#ifdef __cplusplus
}
#endif
#endif // __BT_MATTER_ADAPTER_APP_MAIN_H_
