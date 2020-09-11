/**
 * \file tuya_ble_api.h
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


#ifndef TUYA_BLE_API_H__
#define TUYA_BLE_API_H__

#include "tuya_ble_type.h"
#include "tuya_ble_internal_config.h"
#include "tuya_ble_port.h"

#if (TUYA_BLE_USE_OS==0)
/**
 * @brief   Function for executing all enqueued tasks.
 *
 * @note    This function must be called from within the main loop. It will 
 * execute all events scheduled since the last time it was called.
 * */
void tuya_ble_main_tasks_exec(void);
#endif

/**
 * @brief   Function for transmit ble data from peer devices to tuya sdk.
 *
 * @note    This function must be called from where the ble data is received. 
 *.
 * */
tuya_ble_status_t tuya_ble_gatt_receive_data(uint8_t *p_data,uint16_t len);


/**
 * @brief   Function for transmit ble data from peer devices to tuya sdk.
 *
 * @note    This function must be called from where the ble data is received. 
 *.
 * */
tuya_ble_status_t tuya_ble_uart_receive_data(uint8_t *p_data,uint16_t len);

/**
 * @brief   Function for update the device id to tuya sdk.
 *
 * @note    the following id of the device must be update immediately when changed.
 *.
 * */

tuya_ble_status_t tuya_ble_device_update_product_id(tuya_ble_product_id_type_t type, uint8_t len, uint8_t* p_buf);

tuya_ble_status_t tuya_ble_device_update_login_key(uint8_t* p_buf, uint8_t len);

tuya_ble_status_t tuya_ble_device_update_bound_state(uint8_t state);

/**
 * @brief   Function for initialize the tuya sdk.
 *
 * @note    appliction should call this after all platform init complete.
 *.
 * */

tuya_ble_status_t tuya_ble_sdk_init(tuya_ble_device_param_t * param_data);



/**
 * @brief   Function for report the dp point data.
 *
 * @note    
 *.
 * */

tuya_ble_status_t tuya_ble_dp_data_report(uint8_t *p_data,uint32_t len); 

/**
 * @brief   Function for report the dp point data with time.
 *
 * @note    
 *.
 * */

tuya_ble_status_t tuya_ble_dp_data_with_time_report(uint32_t timestamp,uint8_t *p_data,uint32_t len);


/**
 * @brief   Function for process the internal state of tuya sdk, application should  call this in connect handler.
 *
 * @note    
 *.
 * */
void tuya_ble_connected_handler(void);


/**
 * @brief   Function for process the internal state of tuya sdk, application should  call this in disconnect handler.
 *
 * @note    
 *.
 * */
void tuya_ble_disconnected_handler(void);


/**
 * @brief   Function for data passthrough.
 *
 * @note    The tuya sdk will forwards the data to the app.
 *.
 * */
tuya_ble_status_t tuya_ble_data_passthrough(uint8_t *p_data,uint32_t len);


/**
 * @brief   Function for response for the net config req.
 *
 * @note    
 *.
 * */
tuya_ble_status_t tuya_ble_net_config_response(int16_t result_code);


#if (!TUYA_BLE_DEVICE_REGISTER_FROM_BLE)  
/**
 * @brief   Function for response for ubound req.
 *
 * @note
 *.
 * */
tuya_ble_status_t tuya_ble_ubound_response(uint8_t result_code);


/**
 * @brief   Function for response for anomaly ubound req.
 *
 * @note
 *.
 * */
tuya_ble_status_t tuya_ble_anomaly_ubound_response(uint8_t result_code);

/**
 * @brief   Function for response for device reset req.
 *
 * @note
 *.
 * */
tuya_ble_status_t tuya_ble_device_reset_response(uint8_t result_code);

#endif

/**
 * @brief   Function for get the ble connet status.
 *
 * @note    
 *.
 * */

tuya_ble_connect_status_t tuya_ble_connect_status_get(void);

/**
 * @brief   Function for notify the sdk the device has resumes factory Settings.
 *
 * @note    When the device resumes factory Settings,shoule notify the sdk.
 *.
 * */

tuya_ble_status_t tuya_ble_device_factory_reset(void);

/**
 * @brief   Function for response the ota req.
 *
 * @note    response the ota data req from call back event.
 *.
 * */

tuya_ble_status_t tuya_ble_ota_response(tuya_ble_ota_data_response_t *p_data);



/**
 * @brief   Function for send custom event to main process of ble sdk.
 *
 * @note    
 *.
 * */
uint8_t tuya_ble_custom_event_send(tuya_ble_custom_evt_t evt);



#if TUYA_BLE_USE_OS

/**
 * @brief   Function for registe queue to receive call back evt when use os
 *
 * @note    
 * 
 * */
tuya_ble_status_t tuya_ble_callback_queue_register(void *cb_queue);

/**
 * @brief   Function for response the event.
 *
 * @note    if use os,must be sure to call this function after process one event in queue.
 * 
 * */
tuya_ble_status_t tuya_ble_event_response(tuya_ble_cb_evt_param_t *param);

#else
	
/**
 * @brief   Function for registe call back functions.
 *
 * @note    appliction should receive the message from the call back registed by this function.
 * 
 * */	
tuya_ble_status_t tuya_ble_callback_queue_register(tuya_ble_callback_t cb);

#endif


#endif

