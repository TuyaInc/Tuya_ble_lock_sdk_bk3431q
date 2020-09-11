/**
 * \file tuya_ble_app_production_test.h
 *
 * \brief 
 */
/*
 *  Copyright (C) 2014-2019, Tuya Inc., All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of tuya ble sdk 
 */

#ifndef _TUYA_BLE_APP_PRODUCTION_TEST_H_
#define _TUYA_BLE_APP_PRODUCTION_TEST_H_

#include <stdint.h>
#include "tuya_ble_internal_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define      TUYA_BLE_AUC_CMD_ENTER              0x00
#define      TUYA_BLE_AUC_CMD_QUERY_HID          0x01
#define      TUYA_BLE_AUC_CMD_GPIO_TEST          0x02
#define      TUYA_BLE_AUC_CMD_WRITE_AUTH_INFO    0x03
#define      TUYA_BLE_AUC_CMD_QUERY_INFO         0x04
#define      TUYA_BLE_AUC_CMD_RESET              0x05
#define      TUYA_BLE_AUC_CMD_QUERY_FINGERPRINT  0x06
#define      TUYA_BLE_AUC_CMD_WRITE_HID          0x07
#define      TUYA_BLE_AUC_CMD_RSSI_TEST          0x08
#define      TUYA_BLE_AUC_CMD_WRITE_OEM_INFO     0x09

void tuya_ble_app_production_test_process(uint8_t *p_in_data,uint16_t in_len,uint8_t *p_out_data,uint16_t *out_len);

#ifdef __cplusplus
}
#endif

#endif // _TUYA_BLE_APP_PRODUCTION_TEST_H_

