/**
 * \file tuya_ble_config.h
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


#ifndef TUYA_BLE_CONFIG_H__
#define TUYA_BLE_CONFIG_H__


/*
 * If using an OS, be sure to call the tuya api functions and SDK-related queues within the same task
 */
#define TUYA_BLE_USE_OS 0

/*
 * If using an OS, tuya ble sdk  will create a task autonomously to process ble event.
 */

#if TUYA_BLE_USE_OS

#define TUYA_BLE_TASK_PRIORITY  1

#endif
/*
 * TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_BLE                   ble normal
 * TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_REGISTER_FROM_BLE     device register from ble 
 * TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_MESH                  ble mesh
 * TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_WIFI_24G              wifi
 * TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_WIFI_5G               wifi 
 * TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_ZIGBEE                zigbee
 * TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_NB                    nb-iot
 * @note: 
 * for example (TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_BLE|TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_MESH|TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_WIFI_24G)
 */
//#define TUYA_BLE_DEVICE_COMMUNICATION_ABILITY  (TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_BLE|TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_WIFI_24G|TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_WIFI_5G)

#define TUYA_BLE_DEVICE_COMMUNICATION_ABILITY  (TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_BLE|TUYA_BLE_DEVICE_COMMUNICATION_ABILITY_REGISTER_FROM_BLE)

 /*
  * TUYA_BLE_SECURE_CONNECTION_WITH_AUTH_KEY     encrypt with auth key
  * TUYA_BLE_SECURE_CONNECTION_WITH_ECC          encrypt with ECDH
  * TUYA_BLE_SECURE_CONNECTION_WTIH_PASSTHROUGH  no encrypt 
  * @note : only choose one 
 */
#define  TUYA_BLE_SECURE_CONNECTION_TYPE  TUYA_BLE_SECURE_CONNECTION_WITH_AUTH_KEY
 
 
 
/*
 * if defined ,include UART module
 */
//#define TUYA_BLE_UART 


/*
 * if defined ,include product test module
 */
//#define TUYA_BLE_PRODUCTION_TEST 


/*
 * gatt mtu max sizes 
 */
#define TUYA_BLE_DATA_MTU_MAX  20


/*
 * if defined ,enable log output
 */
//#define TUYA_BLE_LOG_ENABLED


/*
 * MACRO for advanced encryption,if 1 will use user rand check.
 */
#define TUYA_BLE_ADVANCED_ENCRYPTION_DEVICE 0




//nv
/* The minimum size of flash erasure. May be a flash sector size. */
#define TUYA_NV_ERASE_MIN_SIZE         (0x1000)

/* the flash write granularity, unit: byte*/
#define TUYA_NV_WRITE_GRAN             (4)

/* start address */
#define TUYA_NV_START_ADDR              BK_FLASH_OTA_END_ADDR

/* area size. */
#define TUYA_NV_AREA_SIZE              (4*TUYA_NV_ERASE_MIN_SIZE)



#endif



