#include "lock_hard.h"




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

/*********************************************************************
 * VARIABLES
 */




/*****************************************************   -password-   ******************************************************/

/*********************************************************
FN: 
*/




/*****************************************************   -doorcard-   ******************************************************/

/*********************************************************
FN: 
*/
void lock_hard_doorcard_start_reg(void* buf, uint32_t size)
{
//    open_meth_creat_t* cmd = buf;
//    uint32_t cmd_len = size;
    
    //start reg doorcard
}

/*********************************************************
FN: 
*/
void lock_hard_doorcard_cancel_reg(void)
{
    //cancel reg doorcard
}

/*********************************************************
FN: 
*/
uint32_t lock_hard_doorcard_delete(uint8_t hardid)
{
    //delete card in card model by hard id
    
    //if delete success, return 0
    return 0;
}




/*****************************************************   -finger-   ******************************************************/

/*********************************************************
FN: 
*/
void lock_hard_finger_start_reg(void* buf, uint32_t size)
{
//    open_meth_creat_t* cmd = buf;
//    uint32_t cmd_len = size;
    
    //start reg finger
}

/*********************************************************
FN: 
*/
void lock_hard_finger_cancel_reg(void)
{
    //cancel reg finger
}

/*********************************************************
FN: 
*/
uint32_t lock_hard_finger_delete(uint8_t hardid)
{
    //delete finger in finger model by hard id
    
    //if delete success, return 0
    return 0;
}




/*****************************************************   -face-   ******************************************************/

/*********************************************************
FN: 
*/
void lock_hard_face_start_reg(void* buf, uint32_t size)
{
//    open_meth_creat_t* cmd = buf;
//    uint32_t cmd_len = size;
    
    //start reg face
}

/*********************************************************
FN: 
*/
void lock_hard_face_cancel_reg(void)
{
    //cancel reg face
}

/*********************************************************
FN: 
*/
uint32_t lock_hard_face_delete(uint8_t hardid)
{
    //delete face in face model by hard id
    
    //if delete success, return 0
    return 0;
}




/*****************************************************   -simulate-   ******************************************************/

volatile lock_hard_uart_simulate_auto_switch_t g_auto_switch = {0};

/*********************************************************
FN: 
*/
void lock_hard_uart_simulate(uint8_t cmd, uint8_t* data, uint16_t len)
{
    if(len > 200) {
        APP_DEBUG_PRINTF("Error: lock_hard_uart_simulate length error");
        return;
    }
    
    APP_DEBUG_PRINTF("uart simulate lock cmdid: 0x%02x  data_len: %d", cmd, len);
    APP_DEBUG_HEXDUMP("uart simulate lock data", data, len);
    
	switch(cmd)
	{
		case UART_SIMULATE_REG_PASSWORD: {
            //auto reg switch
            if(data[0] == 0x00)
            {
                g_auto_switch.creat_pw_flag = data[1];
                APP_DEBUG_PRINTF("g_auto_switch.creat_pw_flag: %d", data[1]);
            }
            //reg complete
            else if(data[0] == 0x01)
            {
                APP_DEBUG_PRINTF("OPEN_METH_PASSWORD creat complete");
                lock_hard_creat_sub_report(OPEN_METH_PASSWORD, REG_STAGE_COMPLETE, lock_get_hardid(OPEN_METH_PASSWORD), REG_NOUSE_DEFAULT_VALUE, REG_NOUSE_DEFAULT_VALUE);
                lock_hard_save_in_local_flash(OPEN_METH_PASSWORD);
            }
            //reg fail, data[1] = reg_stage_t, data[2] = reg_failed_reason_t
            else if(data[0] == 0x02)
            {
                APP_DEBUG_PRINTF("OPEN_METH_PASSWORD creat fail");
                lock_hard_creat_sub_report(OPEN_METH_PASSWORD, REG_STAGE_FAILED, lock_get_hardid(OPEN_METH_PASSWORD), data[1], data[2]);
            }
        } break;
        
		case UART_SIMULATE_REG_DOORCARD: {
            //reg complete
            if(data[0] == 0x01)
            {
                APP_DEBUG_PRINTF("OPEN_METH_DOORCARD creat complete");
                lock_hard_creat_sub_report(OPEN_METH_DOORCARD, REG_STAGE_COMPLETE, lock_get_hardid(OPEN_METH_DOORCARD), REG_NOUSE_DEFAULT_VALUE, REG_NOUSE_DEFAULT_VALUE);
                lock_hard_save_in_local_flash(OPEN_METH_DOORCARD);
            }
            //reg fail, data[1] = reg_stage_t, data[2] = reg_failed_reason_t
            else if(data[0] == 0x02)
            {
                APP_DEBUG_PRINTF("OPEN_METH_DOORCARD creat fail");
                lock_hard_creat_sub_report(OPEN_METH_DOORCARD, REG_STAGE_FAILED, lock_get_hardid(OPEN_METH_DOORCARD), data[1], data[2]);
            }
        } break;
        
		case UART_SIMULATE_REG_FINGER: {
            //reg count
            if(data[0] == 0x00)
            {
                //reg normal
                if(data[1] == 0x00)
                {
                    lock_hard_creat_sub_report(OPEN_METH_FINGER, REG_STAGE_RUNNING, lock_get_hardid(OPEN_METH_FINGER), data[2], REG_ABNORMAL_NONE);
                }
                //reg fingerprint incomplete
                else if(data[1] == 0x01)
                {
                    lock_hard_creat_sub_report(OPEN_METH_FINGER, REG_STAGE_RUNNING, lock_get_hardid(OPEN_METH_FINGER), data[2], REG_ABNORMAL_FP_INCOMPLETE);
                }
            }
            //reg complete
            else if(data[0] == 0x01)
            {
                APP_DEBUG_PRINTF("OPEN_METH_FINGER creat complete");
                lock_hard_creat_sub_report(OPEN_METH_FINGER, REG_STAGE_COMPLETE, lock_get_hardid(OPEN_METH_FINGER), REG_NOUSE_DEFAULT_VALUE, REG_NOUSE_DEFAULT_VALUE);
                lock_hard_save_in_local_flash(OPEN_METH_FINGER);
            }
            //reg fail, data[1] = reg_stage_t, data[2] = reg_failed_reason_t
            else if(data[0] == 0x02)
            {
                APP_DEBUG_PRINTF("OPEN_METH_FINGER creat fail");
                lock_hard_creat_sub_report(OPEN_METH_FINGER, REG_STAGE_FAILED, lock_get_hardid(OPEN_METH_FINGER), data[1], data[2]);
            }
        } break;
        
		case UART_SIMULATE_REG_FACE: {
            //reg complete
            if(data[0] == 0x01)
            {
                APP_DEBUG_PRINTF("OPEN_METH_FACE creat complete");
                lock_hard_creat_sub_report(OPEN_METH_FACE, REG_STAGE_COMPLETE, lock_get_hardid(OPEN_METH_FACE), REG_NOUSE_DEFAULT_VALUE, REG_NOUSE_DEFAULT_VALUE);
                lock_hard_save_in_local_flash(OPEN_METH_FACE);
            }
            //reg fail, data[1] = reg_stage_t, data[2] = reg_failed_reason_t
            else if(data[0] == 0x02)
            {
                APP_DEBUG_PRINTF("OPEN_METH_FACE creat fail");
                lock_hard_creat_sub_report(OPEN_METH_FACE, REG_STAGE_FAILED, lock_get_hardid(OPEN_METH_FACE), data[1], data[2]);
            }
        } break;
        
		case UART_SIMULATE_REPORT_OPEN_RECORD: {
            //data[0] = dp_id, data[1] = hardid
            if(data[0] == OR_LOG_OPEN_WITH_COMBINE) {
                lock_open_record_combine_report(data[1], len-2, &data[2]);
            } else {
                lock_open_record_report(data[0], data[1]);
            }
//            lock_open_record_report(OR_OPEN_WITH_FINGER, lock_get_hardid(OPEN_METH_FINGER));
        } break;
        
		case UART_SIMULATE_REPORT_ALARM: {
            //data[0] = alarm reason
            lock_alarm_record_report((alarm_reason_t)data[0]);
//            lock_alarm_record_report(ALARM_WRONG_FINGER);
        } break;
        
		case UART_SIMULATE_STATE_SYNC: {
            uint32_t value;
            value = (data[1]<<24) + (data[2]<<16) + (data[3]<<8) + data[4];
            lock_state_sync_report(data[0], value);
        } break;
        
		case UART_SIMULATE_DYNAMIC_PWD: {
            if(DYNAMIC_PWD_VERIFY_SUCCESS == lock_dynamic_pwd_verify(&data[0], 8)) {
                APP_DEBUG_PRINTF("lock_open_with_dynamic_pwd_success");
            } else {
                APP_DEBUG_PRINTF("lock_open_with_dynamic_pwd_fail");
            }
        } break;
        
		case UART_SIMULATE_OFFLINE_PWD: {
            uint8_t plain_pwd[OFFLINE_PWD_LEN+6] = {0};
            uint8_t plain_pwd_len = 0;
            
            uint8_t key[16];
            memset(key, '0', sizeof(key));
            memcpy(&key[10], tuya_ble_current_para.sys_settings.login_key, LOGIN_KEY_LEN);
            
            uint32_t timestamp = app_port_get_timestamp();
            uint32_t ret = lock_offline_pwd_verify(key, sizeof(key),
                                                    &data[0], OFFLINE_PWD_LEN, timestamp,
                                                    plain_pwd+6, &plain_pwd_len);
            
            for(uint32_t idx=0; idx<OFFLINE_PWD_LEN+6; idx++) {
                plain_pwd[idx] += 0x30;
            }
            uint8_t encrypt_pwd[OFFLINE_PWD_LEN+6] = {0};
            uint8_t iv[16] = {0x00};
            app_port_aes128_cbc_encrypt(key, iv, plain_pwd, sizeof(plain_pwd), encrypt_pwd);
            
            if(ret == OFFLINE_PWD_VERIFY_SUCCESS) {
                lock_open_record_report_offline_pwd(OR_LOG_OPEN_WITH_OFFLINE_PWD, encrypt_pwd);
                APP_DEBUG_PRINTF("lock_open_with_offline_pwd_success");
            }
            else if(ret == OFFLINE_PWD_CLEAR_SINGLE_SUCCESS) {
                lock_open_record_report_offline_pwd(OR_LOG_ALARM_OFFLINE_PWD_CLEAR_SINGLE, encrypt_pwd);
                APP_DEBUG_PRINTF("clear_single_with_offline_pwd_success");
            }
            else if(ret == OFFLINE_PWD_CLEAR_ALL_SUCCESS) {
                lock_open_record_report_offline_pwd(OR_LOG_ALARM_OFFLINE_PWD_CLEAR_ALL, encrypt_pwd);
                APP_DEBUG_PRINTF("clear_all_with_offline_pwd_success");
            }
            else {
                APP_DEBUG_PRINTF("lock_open_with_offline_pwd_fail");
            }
        } break;
        
		case UART_SIMULATE_FACTORY_RESET: {
            lock_factory_handler(false);
            lock_timer_start(LOCK_TIMER_RESET_WITH_DISCONN);
        } break;
        
		case UART_SIMULATE_COMMON_DP: {
            app_port_dp_data_report(data, len);
        } break;
        
		case UART_SIMULATE_COMMON_DP_WITH_TIMESTAMP: {
            uint32_t timestamp = (data[0]<<24) + (data[1]<<16) + (data[2]<<8) + data[3];
            app_port_dp_data_with_time_report(timestamp, &data[4], len-4);
        } break;
        
		case UART_SIMULATE_SET_FLAG: {
            //open with bt flag
            if(data[0] == 0x01)
            {
                g_auto_switch.open_with_bt_flag = data[1];
                APP_DEBUG_PRINTF("g_auto_switch.open_with_bt_flag: %d", data[1]);
            }
            //
            else if(data[0] == 0x02)
            {
            }
        } break;
        
		case UART_SIMULATE_ACTIVE_REPORT: {
            if(data[0] == 0x01) {
                app_active_report_start();
            }
            else if(data[0] == 0x02) {
                app_active_report_finished_and_disconnect();
            }
            
        } break;
        
		case UART_SIMULATE_DELETE_FLASH: {
            app_port_nv_set_default();
            APP_DEBUG_PRINTF("app_port_nv_set_default");
        } break;
        
		default: {
        } break;
    }
}




















