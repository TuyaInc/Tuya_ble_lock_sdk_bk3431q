/**
 * \file tuya_ble_utils.h
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

#ifndef TUYA_BLE_UTILS_H_
#define TUYA_BLE_UTILS_H_

#include <stdbool.h>
#include <stdint.h>

typedef unsigned char u8 ;
typedef signed char s8;

typedef unsigned short u16;
typedef signed short s16;

typedef int s32;
typedef unsigned int u32;

typedef long long s64;
typedef unsigned long long u64;


uint8_t check_sum(u8 *pbuf,uint16_t len);

u8 tuya_ble_check_num(u8 *buf,u8 num);

void tuya_ble_hextoascii(u8 *hexbuf,u8 len,u8 *ascbuf);

void tuya_ble_asciitohex(u8 *ascbuf,u8 *hexbuf);

u8 tuya_ble_char_2_ascii(u8 data);

void tuya_ble_str_to_hex(u8 *str_buf,u8 str_len,u8 *hex_buf);

bool tuya_ble_is_word_aligned_tuya(void const* p);

u16 tuya_ble_crc16_compute(u8 * p_data, u16 size, u16 * p_crc);

uint32_t tuya_ble_crc32_compute(uint8_t const * p_data, uint32_t size, uint32_t const * p_crc);

void tuya_ble_device_id_20_to_16(uint8_t *in,uint8_t *out);

void tuya_ble_device_id_16_to_20(uint8_t *in,uint8_t *out);

#endif /* TUYA_UTILS_H_ */
