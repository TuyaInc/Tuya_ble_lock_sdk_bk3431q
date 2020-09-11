#include "app_common.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION
 */
static void app_common_tuya_ble_sdk_callback(tuya_ble_cb_evt_param_t* param);
static void app_common_evt_process(void *p_param);

/*********************************************************************
 * VARIABLES
 */
//tuya_ble_sdk init param
tuya_ble_device_param_t tuya_ble_device_param = 
{
    .device_id_len    = DEVICE_ID_LEN,
    .p_type           = TUYA_BLE_PRODUCT_ID_TYPE_PID,
    .product_id_len   = APP_PORT_PID_LEN,
    .bound_flag       = 0,
    .firmware_version = TUYA_DEVICE_FVER_NUM,
    .hardware_version = TUYA_DEVICE_HVER_NUM,
};





/*********************************************************
FN: 
*/
void app_common_init(void)
{
    lock_flash_init();
    
    //tuya_ble_sdk init
    memcpy(tuya_ble_device_param.device_id,   TUYA_DEVICE_DID,      DEVICE_ID_LEN);    //16
    memcpy(tuya_ble_device_param.product_id,  TUYA_DEVICE_PID,      APP_PORT_PID_LEN); //8
    memcpy(tuya_ble_device_param.auth_key,    TUYA_DEVICE_AUTH_KEY, AUTH_KEY_LEN);     //32
    app_port_ble_sdk_init(&tuya_ble_device_param);
    
    //register tuya_ble_sdk callback
    app_port_ble_callback_queue_register(app_common_tuya_ble_sdk_callback);
    
    app_test_init();
}

/*********************************************************
FN: 
*/
static void app_common_tuya_ble_sdk_callback(tuya_ble_cb_evt_param_t* param)
{
//    APP_DEBUG_PRINTF("callback: %02x", param->evt);
    switch(param->evt)
    {
        //connect status
        case TUYA_BLE_CB_EVT_CONNECTE_STATUS: {
            if(param->connect_status == BONDING_CONN) {
                APP_DEBUG_PRINTF("bonding and connecting");
                lock_timer_stop(LOCK_TIMER_CONN_MONITOR);
                app_active_report_stop(ACTICE_REPORT_STOP_STATE_BONDING);
                lock_timer_start(LOCK_TIMER_CONN_PARAM_UPDATE);
            }
        } break;
        
        //dp parser
        case TUYA_BLE_CB_EVT_DP_WRITE: {
            if(param->dp_write_data.p_data[2] == (param->dp_write_data.data_len - 3))
            {
                lock_dp_parser_handler(param->dp_write_data.p_data);
            }
        } break;
        
        //response - dp report
        case TUYA_BLE_CB_EVT_DP_DATA_REPORT_RESPONSE: {
            if(g_sync_new.flag == 1)
            {
                lock_open_meth_sync_new_report(param->dp_response_data.status);
            }
        } break;
        
        //response - dp report with timestamp
        case TUYA_BLE_CB_EVT_DP_DATA_WTTH_TIME_REPORT_RESPONSE: {
            if(param->dp_with_time_response_data.status != 0xFF)
            {
                lock_offline_evt_report(param->dp_with_time_response_data.status);
            }
        } break;
        
        //timestamp synch
        case TUYA_BLE_CB_EVT_TIME_STAMP: {
            APP_DEBUG_PRINTF("TUYA_BLE_CB_EVT_TIME_STAMP - time_zone: %d", param->timestamp_data.time_zone);
            APP_DEBUG_PRINTF("TUYA_BLE_CB_EVT_TIME_STAMP - timestamp: %d", param->timestamp_data.timestamp);
            app_port_update_timestamp(param->timestamp_data.timestamp);
            lock_timer_start(LOCK_TIMER_BONDING_CONN);
        } break;
        
        //unbond
        case TUYA_BLE_CB_EVT_UNBOUND:
        //unexpected unbond /restore factory setting
        case TUYA_BLE_CB_EVT_ANOMALY_UNBOUND: {
            lock_factory_handler(false);
            lock_timer_start(LOCK_TIMER_RESET_WITH_DISCONN);
        } break;
        
        //restore factory setting
        case TUYA_BLE_CB_EVT_DEVICE_RESET: {
            lock_factory_handler(false);
            lock_timer_start(LOCK_TIMER_RESET_WITH_DISCONN);
        } break;
        
        //ota
        case TUYA_BLE_CB_EVT_OTA_DATA: {
            app_ota_handler(&param->ota_data);
        } break;
        
        default: {
        } break;
    }
}


/*********************************************************
FN: 
*/
void app_common_evt_send(app_evt_param_t* param)
{
    tuya_ble_custom_evt_t custom_evt;
    
    custom_evt.data = param;
    custom_evt.custom_event_handler = app_common_evt_process;
    
    app_port_ble_custom_event_send(custom_evt);
}

/*********************************************************
FN: 
*/
static void app_common_evt_process(void *p_param)
{
    app_evt_param_t* param = p_param;
    
    switch(param->evt)
    {
        case APP_EVT_START_REG_CARD: {
            lock_hard_doorcard_start_reg(param->data.value, param->data.len);
        } break;
        
        case APP_EVT_CANCEL_REG_CARD: {
            lock_hard_doorcard_cancel_reg();
        } break;
        
        case APP_EVT_START_REG_FINGER: {
            lock_hard_finger_start_reg(param->data.value, param->data.len);
        } break;
        
        case APP_EVT_CANCEL_REG_FINGER: {
            lock_hard_finger_cancel_reg();
        } break;
        
        case APP_EVT_START_REG_FACE: {
            lock_hard_face_start_reg(param->data.value, param->data.len);
        } break;
        
        case APP_EVT_CANCEL_REG_FACE: {
            lock_hard_face_cancel_reg();
        } break;
        
        case APP_EVT_BLE_GAP_EVT_CONNECTED: {
            lock_timer_start(LOCK_TIMER_CONN_MONITOR);
        } break;
        
        case APP_EVT_BLE_GAP_EVT_DISCONNECTED: {
            app_ota_disconn_handler();
            lock_timer_stop(LOCK_TIMER_CONN_MONITOR);
            app_active_report_finished_and_disconnect_handler();
        } break;
        
        case APP_EVT_TIMER_0: {
            conn_param_update_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_1: {
            delay_report_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_2: {
            bonding_conn_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_3: {
            reset_with_disconn_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_4: {
            conn_monitor_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_5: {
            app_test_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_6: {
            app_test_reset_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_7: {
            app_active_report_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_8: {
            reset_with_disconn2_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_9: {
        } break;
        
        case APP_EVT_TIMER_10: {
        } break;
        
        case APP_EVT_TIMER_11: {
        } break;
        
        case APP_EVT_TIMER_12: {
        } break;
        
        case APP_EVT_TIMER_13: {
        } break;
        
        case APP_EVT_TIMER_14: {
        } break;
        
        case APP_EVT_TIMER_15: {
        } break;
        
        case APP_EVT_TIMER_16: {
        } break;
        
        case APP_EVT_TIMER_17: {
        } break;
        
        case APP_EVT_TIMER_18: {
        } break;
        
        case APP_EVT_TIMER_19: {
        } break;
        
        default: {
        } break;
    }
    
    if(param != NULL)
    {
        app_port_free(param);
    }
}

/*********************************************************
FN: no data
*/
void app_common_evt_send_only_evt(app_evt_t evt)
{
    app_evt_param_t* param = app_port_malloc(sizeof(app_evt_param_t));
    if(param)
    {
        param->evt = evt;
        app_common_evt_send(param);
    }
}

/*********************************************************
FN: 
*/
void app_common_evt_send_with_data(app_evt_t evt, void* buf, uint32_t len)
{
    app_evt_param_t* param = app_port_malloc(sizeof(app_evt_param_t) + len);
    if(param)
    {
        param->evt = evt;
        param->data.len = len;
        memcpy(param->data.value, buf, len);
        app_common_evt_send(param);
    }
}















