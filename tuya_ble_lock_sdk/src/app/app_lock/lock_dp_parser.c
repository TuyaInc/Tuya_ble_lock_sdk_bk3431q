#include "lock_dp_parser.h"




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
static uint32_t open_meth_creat_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len);
static uint32_t open_meth_delete_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len);
static uint32_t open_meth_modify_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len);
static uint32_t open_meth_freeze_or_unfreeze_handler(void* cmd_dp_data, uint8_t freeze_state);
static uint32_t user_freeze_or_unfreeze_handler(void* cmd_dp_data, uint8_t freeze_state);
static uint32_t open_with_bt_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len);
static uint32_t open_meth_sync_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len);
static uint32_t open_meth_sync_new_handler(uint8_t cmd_dp_data_len, void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len);
static uint32_t temp_pw_creat_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len);
static uint32_t temp_pw_delete_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len);
static uint32_t temp_pw_modify_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len);
static uint32_t open_with_nopwd_remote_setkey_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len);
static uint32_t open_with_nopwd_remote_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len);
static uint32_t offline_pwd_set_T0_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len);

/*********************************************************************
 * VARIABLES
 */
lock_dp_t g_cmd;
lock_dp_t g_rsp;




/*********************************************************
FN: 
*/
uint32_t lock_dp_parser_handler(void* dp_data)
{
    uint8_t rsp_flag = 1;
    
    //init cmd and rsp
    memcpy(&g_cmd, dp_data, sizeof(lock_dp_t));
    memcpy(&g_rsp, dp_data, sizeof(lock_dp_t));
    APP_DEBUG_HEXDUMP("dp_cmd", (void*)&g_cmd, g_cmd.dp_data_len+3);
    
    switch(g_cmd.dp_id)
    {
        case WR_BSC_OPEN_METH_CREATE: {
            open_meth_creat_handler(g_cmd.dp_data, g_rsp.dp_data, &g_rsp.dp_data_len);
        } break;
        
        case WR_BSC_OPEN_METH_DELETE: {
            open_meth_delete_handler(g_cmd.dp_data, g_rsp.dp_data, &g_rsp.dp_data_len);
        } break;
        
        case WR_BSC_OPEN_METH_MODIFY: {
            open_meth_modify_handler(g_cmd.dp_data, g_rsp.dp_data, &g_rsp.dp_data_len);
        } break;
        
        case WR_BSC_OPEN_METH_FREEZE: {
            if(g_cmd.dp_data_len == 4) {
                APP_DEBUG_PRINTF("OPEN_METH freeze start");
                open_meth_freeze_or_unfreeze_handler(g_cmd.dp_data, FREEZE_ON);
            }
        } break;
        
        case WR_BSC_OPEN_METH_UNFREEZE: {
            if(g_cmd.dp_data_len == 4) {
                APP_DEBUG_PRINTF("OPEN_METH unfreeze start");
                open_meth_freeze_or_unfreeze_handler(g_cmd.dp_data, FREEZE_OFF);
            }
        } break;
        
        case WR_SET_USER_FREEZE: {
            if(g_cmd.dp_data_len == 4) {
                APP_DEBUG_PRINTF("OPEN_METH freeze user start");
                user_freeze_or_unfreeze_handler(g_cmd.dp_data, FREEZE_ON);
            }
        } break;
        
        case WR_SET_USER_UNFREEZE: {
            if(g_cmd.dp_data_len == 4) {
                APP_DEBUG_PRINTF("OPEN_METH unfreeze user start");
                user_freeze_or_unfreeze_handler(g_cmd.dp_data, FREEZE_OFF);
            }
        } break;
        
        case WR_BSC_OPEN_METH_SYNC: {
            APP_DEBUG_PRINTF("OPEN_METH sync start");
            open_meth_sync_handler(g_cmd.dp_data, g_rsp.dp_data, &g_rsp.dp_data_len);
        } break;
        
        case WR_BSC_OPEN_METH_SYNC_NEW: {
            APP_DEBUG_PRINTF("OPEN_METH sync new start");
            open_meth_sync_new_handler(g_cmd.dp_data_len, g_cmd.dp_data, g_rsp.dp_data, &g_rsp.dp_data_len);
        } break;
        
        case OW_BSC_OPEN_WITH_BT: {
            open_with_bt_handler(g_cmd.dp_data, g_rsp.dp_data, &g_rsp.dp_data_len);
        } break;
        
        case WR_STS_REVERSE_LOCK: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] <= 0x01))
            {
                lock_remote_anti_lock(g_cmd.dp_data[0]);
            }
        } break;
        
        case WR_SET_MESSAGE_SWITCH: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] <= 0x01))
            {
                lock_settings.message_switch = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_MESSAGE_SWITCH SUCCESS");
                }
            }
        } break;
        
        case WR_SET_DOOR_BELL: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] <= 0x0A))
            {
                lock_settings.door_bell = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_DOOR_BELL SUCCESS");
                }
            }
        } break;
        
        case WR_SET_LOCK_VOLUME: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] <= 0x03))
            {
                lock_settings.lock_volume = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_LOCK_VOLUME SUCCESS");
                }
            }
        } break;
        
        case WR_SET_LOCK_LANGUAGE: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] <= 0x0A))
            {
                lock_settings.lock_language = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_LOCK_LANGUAGE SUCCESS");
                }
            }
        } break;
        
        case WR_SET_WELCOME_WORDS: {
            if((g_cmd.dp_data_len > 0) && (g_cmd.dp_data_len <= HARD_WELCOME_WORDS_MAX_LEN))
            {
                memcpy(lock_settings.welcome_words, g_cmd.dp_data, g_cmd.dp_data_len);
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_WELCOME_WORDS SUCCESS");
                }
            }
        } break;
        
        case WR_SET_KEY_TONE: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] <= 0x0A))
            {
                lock_settings.key_tone = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_KEY_TONE SUCCESS");
                }
            }
        } break;
        
        case WR_SET_NAVIGATE_VOLUME: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] <= 0x0A))
            {
                lock_settings.navigation_volume = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_NAVIGATE_VOLUME SUCCESS");
                }
            }
        } break;
        
        case WR_SET_AUTO_LOCK_SWITCH: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] <= 0x01))
            {
                lock_settings.auto_lock_switch = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_AUTO_LOCK_SWITCH SUCCESS");
                }
            }
        } break;
        
        case WR_SET_COMBINE_LOCK: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] <= 0x06))
            {
                lock_settings.combine_lock_switch = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_COMBINE_LOCK SUCCESS");
                }
            }
        } break;
        
        case WR_SET_TIMER_LOCK: {
            if(g_cmd.dp_data_len == 4)
            {
                uint32_t value;
                value = (g_cmd.dp_data[0]<<24) + (g_cmd.dp_data[1]<<16) + (g_cmd.dp_data[2]<<8) + g_cmd.dp_data[3];
                lock_settings.timer_lock = value;
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_TIMER_LOCK SUCCESS");
                }
            }
        } break;
        
        case WR_SET_TIMER_AUTO_LOCK: {
            if(g_cmd.dp_data_len == 4)
            {
                uint32_t value;
                value = (g_cmd.dp_data[0]<<24) + (g_cmd.dp_data[1]<<16) + (g_cmd.dp_data[2]<<8) + g_cmd.dp_data[3];
                lock_settings.timer_auto_lock = value;
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_TIMER_AUTO_LOCK SUCCESS");
                }
            }
        } break;
        
        case WR_SET_FINGER_NUM: {
            if(g_cmd.dp_data_len == 4)
            {
                uint32_t value;
                value = (g_cmd.dp_data[0]<<24) + (g_cmd.dp_data[1]<<16) + (g_cmd.dp_data[2]<<8) + g_cmd.dp_data[3];
                lock_settings.finger_number = value;
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_FINGER_NUM SUCCESS");
                }
            }
        } break;
        
        case WR_SET_HAND_LOCK: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] == 0x01))
            {
                lock_settings.hand_lock = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_HAND_LOCK SUCCESS");
                }
            }
        } break;
        
        case WR_SET_MOTOR_DIRECTION: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] <= 0x01))
            {
                lock_settings.motor_direction = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_MOTOR_DIRECTION SUCCESS");
                }
            }
        } break;
        
        case WR_SET_MOTOR_TORQUE: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] <= 0x02))
            {
                lock_settings.motor_torque = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_MOTOR_TORQUE SUCCESS");
                }
            }
        } break;
        
        case WR_SET_AWAYHOME_ARMING_SWITCH: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] <= 0x01))
            {
                lock_settings.awayhome_arming = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_SET_AWAYHOME_ARMING_SWITCH SUCCESS");
                }
            }
        } break;
        
        case WR_BSC_TEMP_PW_CREAT: {
            temp_pw_creat_handler(g_cmd.dp_data, g_rsp.dp_data, &g_rsp.dp_data_len);
        } break;
        
        case WR_BSC_TEMP_PW_DELETE: {
            temp_pw_delete_handler(g_cmd.dp_data, g_rsp.dp_data, &g_rsp.dp_data_len);
        } break;
        
        case WR_BSC_TEMP_PW_MODIFY: {
            temp_pw_modify_handler(g_cmd.dp_data, g_rsp.dp_data, &g_rsp.dp_data_len);
        } break;
        
        case WR_BSC_OPEN_WITH_NOPWD_REMOTE_SETKEY: {
            open_with_nopwd_remote_setkey_handler(g_cmd.dp_data, g_rsp.dp_data, &g_rsp.dp_data_len);
        } break;
        
        case WR_BSC_OPEN_WITH_NOPWD_REMOTE: {
            open_with_nopwd_remote_handler(g_cmd.dp_data, g_rsp.dp_data, &g_rsp.dp_data_len);
        } break;
        
        case WR_BSC_SET_T0: {
            offline_pwd_set_T0_handler(g_cmd.dp_data, g_rsp.dp_data, &g_rsp.dp_data_len);
        } break;
        
        default: {
            rsp_flag = 0;
        } break;
    }
    
    if(rsp_flag && (g_rsp.dp_data_len > 0))
    {
        app_port_dp_data_report((void*)&g_rsp, (3 + g_rsp.dp_data_len));
    }

    return 0;
}

/*********************************************************
FN: create open method
*/
static uint32_t open_meth_creat_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len)
{
    open_meth_creat_t* cmd = cmd_dp_data;
    open_meth_creat_result_t* rsp = rsp_dp_data;
    uint8_t* rsp_len = rsp_dp_data_len;
    
    switch(cmd->meth)
    {
        case OPEN_METH_PASSWORD: {
            if(cmd->stage == REG_STAGE_STSRT)
            {
                if(lock_get_hardid(cmd->meth) != HARD_ID_INVALID)
                {
                    APP_DEBUG_PRINTF("OPEN_METH_PASSWORD creat start");
                    if(g_auto_switch.creat_pw_flag == 0) {
                        lock_hard_creat_sub_report_with_delay(cmd->meth, REG_STAGE_COMPLETE, lock_get_hardid(cmd->meth), REG_NOUSE_DEFAULT_VALUE, REG_NOUSE_DEFAULT_VALUE);
                        lock_hard_save_in_local_flash(cmd->meth);
                    }
                }
                else
                {//hardid is used up
                    APP_DEBUG_PRINTF("OPEN_METH_PASSWORD hardid is used up");
                    lock_hard_creat_sub_report_with_delay(cmd->meth, REG_STAGE_FAILED, lock_get_hardid(cmd->meth), REG_STAGE_STSRT, REG_FAILD_NO_HARDID);
                }
                rsp->reg_num = 0x00; //password doesn't need this
            }
            else if(cmd->stage == REG_STAGE_CANCEL)
            {
                APP_DEBUG_PRINTF("OPEN_METH_PASSWORD REG_STAGE_CANCEL");
//                app_common_evt_send_only_evt(APP_EVT_CANCEL_REG_CARD);
                rsp->reg_num = REG_NOUSE_DEFAULT_VALUE;
            }
            rsp->result = REG_NOUSE_DEFAULT_VALUE; //default, no sence
        } break;
        
        case OPEN_METH_DOORCARD: {
            if(cmd->stage == REG_STAGE_STSRT)
            {
                if(lock_get_hardid(cmd->meth) != HARD_ID_INVALID)
                {
                    APP_DEBUG_PRINTF("OPEN_METH_DOORCARD creat start");
                    app_common_evt_send_with_data(APP_EVT_START_REG_CARD, cmd, sizeof(open_meth_creat_t));
                }
                else
                {//hardid is used up
                    APP_DEBUG_PRINTF("OPEN_METH_DOORCARD hardid is used up");
                    lock_hard_creat_sub_report_with_delay(cmd->meth, REG_STAGE_FAILED, lock_get_hardid(cmd->meth), REG_STAGE_STSRT, REG_FAILD_NO_HARDID);
                }

                rsp->reg_num = 0x01; //doorcard only need 1 time
            }
            else if(cmd->stage == REG_STAGE_CANCEL)
            {
                APP_DEBUG_PRINTF("OPEN_METH_DOORCARD REG_STAGE_CANCEL");
                app_common_evt_send_only_evt(APP_EVT_CANCEL_REG_CARD);
                rsp->reg_num = REG_NOUSE_DEFAULT_VALUE;
            }
            rsp->result = REG_NOUSE_DEFAULT_VALUE;
        } break;
        
        case OPEN_METH_FINGER: {
            if(cmd->stage == REG_STAGE_STSRT)
            {
                if(lock_get_hardid(cmd->meth) != HARD_ID_INVALID)
                {
                    APP_DEBUG_PRINTF("OPEN_METH_FINGER creat start");
                    app_common_evt_send_with_data(APP_EVT_START_REG_FINGER, cmd, sizeof(open_meth_creat_t));
                }
                else
                {//hardid is used up
                    APP_DEBUG_PRINTF("OPEN_METH_FINGER hardid is used up");
                    lock_hard_creat_sub_report_with_delay(cmd->meth, REG_STAGE_FAILED, lock_get_hardid(cmd->meth), REG_STAGE_STSRT, REG_FAILD_NO_HARDID);
                }
                
                rsp->reg_num = FINGER_REG_TOTAL_TIMES;
            }
            else if(cmd->stage == REG_STAGE_CANCEL)
            {
                APP_DEBUG_PRINTF("OPEN_METH_FINGER REG_STAGE_CANCEL");
                app_common_evt_send_only_evt(APP_EVT_CANCEL_REG_FINGER);
                rsp->reg_num = REG_NOUSE_DEFAULT_VALUE;
            }
            rsp->result = REG_NOUSE_DEFAULT_VALUE;
        } break;
        
        case OPEN_METH_FACE: {
            if(cmd->stage == REG_STAGE_STSRT)
            {
                if(lock_get_hardid(cmd->meth) != HARD_ID_INVALID)
                {
                    APP_DEBUG_PRINTF("OPEN_METH_FACE creat start");
                    app_common_evt_send_with_data(APP_EVT_START_REG_FACE, cmd, sizeof(open_meth_creat_t));
                }
                else
                {//hardid is used up
                    APP_DEBUG_PRINTF("OPEN_METH_FACE hardid is used up");
                    lock_hard_creat_sub_report_with_delay(cmd->meth, REG_STAGE_FAILED, lock_get_hardid(cmd->meth), REG_STAGE_STSRT, REG_FAILD_NO_HARDID);
                }

                rsp->reg_num = 0x01; //face only need 1 time
            }
            else if(cmd->stage == REG_STAGE_CANCEL)
            {
                APP_DEBUG_PRINTF("OPEN_METH_FACE REG_STAGE_CANCEL");
                app_common_evt_send_only_evt(APP_EVT_CANCEL_REG_FACE);
                rsp->reg_num = REG_NOUSE_DEFAULT_VALUE;
            }
            rsp->result = REG_NOUSE_DEFAULT_VALUE;
        } break;
        
        default: {
        } break;
    }
    
    *rsp_len = sizeof(open_meth_creat_result_t);
    
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: delete open method
*/
static uint32_t open_meth_delete_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len)
{
    open_meth_delete_t* cmd = cmd_dp_data;
    open_meth_delete_result_t* rsp = rsp_dp_data;
    uint8_t* rsp_len = rsp_dp_data_len;
    uint8_t ret = APP_PORT_SUCCESS;
    
    switch(cmd->meth)
    {
        case OPEN_METH_BASE: {
            ret += lock_hard_delete_all_by_memberid(cmd->memberid);
            if(ret == APP_PORT_SUCCESS) {
                APP_DEBUG_PRINTF("OPEN_METH_BASE delete start"); }
        } break;
        
        case OPEN_METH_PASSWORD: {
            ret += lock_hard_delete(cmd->hardid);
            if(ret == APP_PORT_SUCCESS) {
                APP_DEBUG_PRINTF("OPEN_METH_PASSWORD delete start"); }
        } break;
        
        case OPEN_METH_DOORCARD: {
            ret += lock_hard_delete(cmd->hardid);
            ret += lock_hard_doorcard_delete(cmd->hardid);
            if(ret == APP_PORT_SUCCESS) {
                APP_DEBUG_PRINTF("OPEN_METH_DOORCARD delete start"); }
        } break;
        
        case OPEN_METH_FINGER: {
            ret += lock_hard_delete(cmd->hardid);
            ret += lock_hard_finger_delete(cmd->hardid);
            if(ret == APP_PORT_SUCCESS) {
                APP_DEBUG_PRINTF("OPEN_METH_FINGER delete start"); }
        } break;
        
        case OPEN_METH_FACE: {
            ret += lock_hard_delete(cmd->hardid);
            ret += lock_hard_face_delete(cmd->hardid);
            if(ret == APP_PORT_SUCCESS) {
                APP_DEBUG_PRINTF("OPEN_METH_FACE delete start"); }
        } break;
        
        default: {
        } break;
    }
    
    if(ret == APP_PORT_SUCCESS) {
        rsp->result = 0xFF; //delete success
    } else {
        rsp->result = 0x00; //delete fail
    }
    *rsp_len = sizeof(open_meth_delete_result_t);
    
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: modify open method
*/
static uint32_t open_meth_modify_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len)
{
    open_meth_modify_t* cmd = cmd_dp_data;
    open_meth_modify_result_t* rsp = rsp_dp_data;
    uint8_t* rsp_len = rsp_dp_data_len;
    uint8_t ret = APP_PORT_SUCCESS;
    
    switch(cmd->meth)
    {
        case OPEN_METH_BASE: {
            ret = lock_hard_modify_all_by_memberid(cmd->memberid, cmd->time);
            if(ret == APP_PORT_SUCCESS) {
                APP_DEBUG_PRINTF("OPEN_METH_BASE modify start"); }
        } break;
        
        case OPEN_METH_PASSWORD: {
            ret = lock_hard_modify_in_local_flash(OPEN_METH_PASSWORD);
            if(ret == APP_PORT_SUCCESS) {
                APP_DEBUG_PRINTF("OPEN_METH_PASSWORD modify start"); }
        } break;
        
        case OPEN_METH_DOORCARD: {
            ret = lock_hard_modify_in_local_flash(OPEN_METH_PASSWORD);
            if(ret == APP_PORT_SUCCESS) {
                APP_DEBUG_PRINTF("OPEN_METH_DOORCARD modify start"); }
        } break;
        
        case OPEN_METH_FINGER: {
            ret = lock_hard_modify_in_local_flash(OPEN_METH_PASSWORD);
            if(ret == APP_PORT_SUCCESS) {
                APP_DEBUG_PRINTF("OPEN_METH_FINGER modify start"); }
        } break;
        
        default: {
        } break;
    }
    
    rsp->cycle = cmd->cycle;
    if(ret == APP_PORT_SUCCESS) {
        rsp->result = 0xFF; //modify success
    } else {
        rsp->result = 0x00; //modify fail
    }
    *rsp_len = sizeof(open_meth_modify_result_t);
    
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: 
*/
static uint32_t open_meth_freeze_or_unfreeze_handler(void* cmd_dp_data, uint8_t freeze_state)
{
    uint8_t* phardid = cmd_dp_data;
    uint32_t hardid = (phardid[0]<<24) + (phardid[1]<<16) + (phardid[2]<<8) + phardid[3];
    return lock_hard_freezeorunfreeze(hardid, freeze_state);
}

/*********************************************************
FN: 
*/
static uint32_t user_freeze_or_unfreeze_handler(void* cmd_dp_data, uint8_t freeze_state)
{
    uint8_t* pmemberid = cmd_dp_data;
    uint32_t memberid = (pmemberid[0]<<24) + (pmemberid[1]<<16) + (pmemberid[2]<<8) + pmemberid[3];
    return lock_hard_freezeorunfreeze_all_by_memberid(memberid, freeze_state);
}

/*********************************************************
FN: open with bt
*/
static uint32_t open_with_bt_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len)
{
    open_meth_with_bt_t* cmd = cmd_dp_data;
    open_meth_with_bt_result_t* rsp = rsp_dp_data;
    uint8_t* rsp_len = rsp_dp_data_len;
    
    if(cmd->open)
    {
        if(lock_open_with_bt() == APP_PORT_SUCCESS) {
            lock_open_record_report_with_delay(OR_LOG_OPEN_WITH_BT, cmd->memberid);
            rsp->result = 0x01; //open success
        } else {
            rsp->result = 0x00; //open fail
        }
    } else {
        //close, remain for future use
    }
    
    *rsp_len = sizeof(open_meth_with_bt_result_t);
    
    return 0;
}

/*********************************************************
FN: sync open method
*/
static uint32_t open_meth_sync_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len)
{
    uint8_t* pHard_num = cmd_dp_data;
    uint8_t hard_num = *pHard_num;
    open_meth_sync_node_t *pNode = (void*)(pHard_num + 1);
    //node
    uint8_t node_count = 0;
    static open_meth_sync_node_t node[HARDID_MAX_TOTAL];
    //rsp
    open_meth_sync_node_result_t rsp_node;
    uint8_t rsp_idx = 0;
    static uint8_t rsp[201];
    
    //delete invalid hard
    for(uint32_t idx=0; idx<hard_num; idx++)
    {
        if(pNode->hardid >= HARDID_MAX_TOTAL)
        {
            rsp_node.op_type = 0x01; //delete hard
            rsp_node.hardid = pNode->hardid;
            rsp_node.hard_type = pNode->hard_type;
            memset(&rsp_node.hard_attribute, 0, sizeof(open_meth_sync_hard_attribute_t));
            
            //rsp
            memcpy(rsp + 1 + rsp_idx*sizeof(open_meth_sync_node_result_t), &rsp_node, sizeof(open_meth_sync_node_result_t));
            rsp_idx++;
            if(rsp_idx >= 200/sizeof(open_meth_sync_node_result_t)) { break; }//防止数组访问越界
        }
        else
        {
            memcpy(&node[node_count], pNode, sizeof(open_meth_sync_node_t));
            node_count++;
            if(node_count >= HARDID_MAX_TOTAL) { break; }//防止数组访问越界
        }

        //1次加一个结构体
        pNode++;
    }
    
    //同步合法硬件
    for(uint32_t idx=0; idx<HARDID_MAX_TOTAL; idx++)
    {
        uint32_t idy;
        for(idy=0; idy<node_count; idy++)
        {
            if(node[idy].hardid == idx)
            {
                break;
            }
        }
        
        //APP有该硬件
        if(idy < node_count)
        {
            //本地有该硬件
            lock_hard_t hard;
            if(lock_hard_load(idx, &hard) == 0)
            {
                //比对crc8
                uint8_t raw_data[3];
                raw_data[0] = hard.hard_type;
                raw_data[1] = hard.member_id;
                raw_data[2] = hard.freeze_state;
                if(node[idy].hard_crc8 != (app_port_crc16_compute(raw_data, 3, NULL) & 0xFF))
                {
                    rsp_node.op_type = 0x00; //更新硬件
                    rsp_node.hardid = hard.hard_id; //等于idx
                    rsp_node.hard_type = hard.hard_type; //等于OPENMETH_FINGER
                    rsp_node.hard_attribute.memberid = hard.member_id;
                    rsp_node.hard_attribute.freeze_state = hard.freeze_state;
                    
                    //rsp
                    memcpy(rsp + 1 + rsp_idx*sizeof(open_meth_sync_node_result_t), &rsp_node, sizeof(open_meth_sync_node_result_t));
                    rsp_idx++;
                    if(rsp_idx >= 254/sizeof(open_meth_sync_node_result_t)) { break; }//防止数组访问越界
                }
            }
            else
            {
                rsp_node.op_type = 0x01; //删除硬件
                rsp_node.hardid = node[idy].hardid; //等于idx
                rsp_node.hard_type = node[idy].hard_type; //等于OPENMETH_FINGER
                memset(&rsp_node.hard_attribute, 0, sizeof(open_meth_sync_hard_attribute_t));
                
                //rsp
                memcpy(rsp + 1 + rsp_idx*sizeof(open_meth_sync_node_result_t), &rsp_node, sizeof(open_meth_sync_node_result_t));
                rsp_idx++;
                if(rsp_idx >= 254/sizeof(open_meth_sync_node_result_t)) { break; }//防止数组访问越界
            }
        }
        //APP无
        else
        {
            //本地有该硬件
            lock_hard_t hard;
            if(lock_hard_load(idx, &hard) == 0)
            {
                rsp_node.op_type = 0x00; //更新硬件
                rsp_node.hardid = hard.hard_id; //等于idx
                rsp_node.hard_type = hard.hard_type;
                rsp_node.hard_attribute.memberid = hard.member_id;
                rsp_node.hard_attribute.freeze_state = hard.freeze_state;
                
                //rsp
                memcpy(rsp + 1 + rsp_idx*sizeof(open_meth_sync_node_result_t), &rsp_node, sizeof(open_meth_sync_node_result_t));
                rsp_idx++;
                if(rsp_idx >= 254/sizeof(open_meth_sync_node_result_t)) { break; }//防止数组访问越界
            }
        }
    }
    
    //rsp
    rsp[0] = rsp_idx;
    *rsp_dp_data_len = 1 + rsp_idx*sizeof(open_meth_sync_node_result_t);
    memcpy(rsp_dp_data, rsp, *rsp_dp_data_len);
    
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: sync open method new
*/
static uint32_t open_meth_sync_new_handler(uint8_t cmd_dp_data_len, void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len)
{
    open_meth_sync_new_last_result_t* rsp = rsp_dp_data;
    uint8_t* rsp_len = rsp_dp_data_len;
    uint8_t ret = APP_PORT_SUCCESS;
    
    //检查硬件类型枚举的长度
    bool just_rsp_flag = false;
    if((cmd_dp_data_len == 0) || (cmd_dp_data_len >= OPEN_METH_MAX)) {
        just_rsp_flag = true;
    }
    
    //检查硬件类型枚举
    for(uint8_t idx=0; idx<cmd_dp_data_len; idx++) {
        uint8_t* hard_type = cmd_dp_data;
        if((hard_type[idx] >= OPEN_METH_MAX) || (hard_type[idx] == OPEN_METH_BASE)) {
            just_rsp_flag = true;
            break;
        }
    }
    
    if(!just_rsp_flag)
    {
        uint8_t* hard_type = cmd_dp_data;
        
        memset((void*)&g_sync_new, 0x00, sizeof(open_meth_sync_new_t));
        g_sync_new.flag = 1;
        g_sync_new.hard_type_len = cmd_dp_data_len;
        for(uint8_t idx=0; idx<cmd_dp_data_len; idx++) {
            g_sync_new.hard[idx].type = hard_type[idx];
            g_sync_new.hard[idx].idx = 0;
        }
        lock_open_meth_sync_new_report(0x00);
        
        *rsp_len = 0x00;
    } else {
        rsp->stage = 0x01; //sync finish
        rsp->pkgs = 0x00;
        *rsp_len = sizeof(open_meth_sync_new_last_result_t);
    }
    return ret;
}

/*********************************************************
FN: creat temp password
*/
static uint32_t temp_pw_creat_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len)
{
    temp_pw_creat_t* cmd = cmd_dp_data;
    temp_pw_creat_result_t* rsp = rsp_dp_data;
    uint8_t* rsp_len = rsp_dp_data_len;
    uint8_t ret = APP_PORT_SUCCESS;
    
    rsp->hardid = lock_get_hardid(OPEN_METH_TEMP_PW);
    if(lock_get_hardid(OPEN_METH_TEMP_PW) != HARD_ID_INVALID)
    {
        APP_DEBUG_PRINTF("OPEN_METH_TEMP_PW creat start");
        ret += lock_hard_save_in_local_flash(OPEN_METH_TEMP_PW);
        
        switch(cmd->type)
        {
            case 0: {
//                ret += lock_hard_delete_all_by_memberid(cmd->memberid);
            } break;
            
            case 1: {
//                ret += lock_hard_delete(cmd->hardid);
            } break;
            
            default: {
            } break;
        }
        
        if(ret == APP_PORT_SUCCESS) {
            rsp->result = 0x00;
        } else {
            rsp->result = 0x01;
        }
    }
    else
    {//hardid is used up
        APP_DEBUG_PRINTF("OPEN_METH_TEMP_PW hardid is used up");
        rsp->result = 0x02;
    }
    
    *rsp_len = sizeof(temp_pw_creat_result_t);
    
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: delete temp password
*/
static uint32_t temp_pw_delete_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len)
{
    temp_pw_delete_t* cmd = cmd_dp_data;
    temp_pw_delete_result_t* rsp = rsp_dp_data;
    uint8_t* rsp_len = rsp_dp_data_len;
    uint8_t ret = APP_PORT_SUCCESS;
    
    rsp->hardid = cmd->hardid;
    ret = lock_hard_delete(cmd->hardid);
    
    if(ret == APP_PORT_SUCCESS) {
        APP_DEBUG_PRINTF("OPEN_METH_TEMP_PW delete success");
        rsp->result = 0x00;
    } else {
        APP_DEBUG_PRINTF("OPEN_METH_TEMP_PW delete fail");
        rsp->result = 0x01;
    }
    *rsp_len = sizeof(temp_pw_delete_result_t);
    
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: modify temp password
*/
static uint32_t temp_pw_modify_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len)
{
    temp_pw_modify_t* cmd = cmd_dp_data;
    temp_pw_modify_result_t* rsp = rsp_dp_data;
    uint8_t* rsp_len = rsp_dp_data_len;
    uint8_t ret = APP_PORT_SUCCESS;
    
    rsp->hardid = cmd->hardid;
    ret = lock_hard_modify_in_local_flash(OPEN_METH_TEMP_PW);
    
    switch(cmd->type)
    {
        case 0: {
        } break;
        
        case 1: {
        } break;
        
        default: {
        } break;
    }

    if(ret == APP_PORT_SUCCESS) {
        APP_DEBUG_PRINTF("OPEN_METH_TEMP_PW modify success");
        rsp->result = 0x00;
    } else {
        APP_DEBUG_PRINTF("OPEN_METH_TEMP_PW modify fail");
        rsp->result = 0x01;
    }
    *rsp_len = sizeof(temp_pw_modify_result_t);

    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: 
*/
static uint32_t open_with_nopwd_remote_setkey_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len)
{
    open_with_nopwd_remote_setkey_t* cmd = cmd_dp_data;
    open_with_nopwd_remote_setkey_result_t* rsp = rsp_dp_data;
    uint8_t* rsp_len = rsp_dp_data_len;
    uint8_t ret = APP_PORT_SUCCESS;
    
    rsp->memberid = cmd->memberid;
    
    app_port_reverse_byte(&cmd->memberid, sizeof(uint16_t));
    app_port_reverse_byte(&cmd->time_begin, sizeof(uint32_t));
    app_port_reverse_byte(&cmd->time_end, sizeof(uint32_t));
    app_port_reverse_byte(&cmd->valid_num, sizeof(uint16_t));
    
    APP_DEBUG_PRINTF("WR_BSC_OPEN_WITH_NOPWD_REMOTE_SETKEY: time_begin-%d, time_end-%d", cmd->time_begin, cmd->time_end);
    ret = app_port_nv_set(SF_AREA_0, NV_ID_OPEN_WITH_NOPWD_REMOTE, cmd, sizeof(open_with_nopwd_remote_setkey_t));
    if(ret == APP_PORT_SUCCESS) {
        rsp->result = 0x00;
    } else {
        rsp->result = 0x01;
    }
    
    *rsp_len = sizeof(open_with_nopwd_remote_setkey_result_t);
    
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: 
*/
static uint32_t open_with_nopwd_remote_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len)
{
    open_with_nopwd_remote_t* cmd = cmd_dp_data;
    open_with_nopwd_remote_result_t* rsp = rsp_dp_data;
    uint8_t* rsp_len = rsp_dp_data_len;
    
    rsp->memberid = cmd->memberid;
    
    uint32_t timestamp = app_port_get_timestamp();
    
    open_with_nopwd_remote_setkey_t set_cmd;
    app_port_nv_get(SF_AREA_0, NV_ID_OPEN_WITH_NOPWD_REMOTE, &set_cmd, sizeof(open_with_nopwd_remote_setkey_t));
    
    if(!set_cmd.valid) {
        rsp->result = 0x02;
        APP_DEBUG_PRINTF("OPEN_WITH_NOPWD_REMOTE fail id: %d", rsp->result);
    } 
    else if(!(set_cmd.valid_num > 0)) {
        rsp->result = 0x03;
        APP_DEBUG_PRINTF("OPEN_WITH_NOPWD_REMOTE fail id: %d", rsp->result);
    }
    else if(!((timestamp >= set_cmd.time_begin) && (timestamp <= set_cmd.time_end))) {
        rsp->result = 0x04;
        APP_DEBUG_PRINTF("OPEN_WITH_NOPWD_REMOTE fail id: %d", rsp->result);
    }
    else if(!(0 == memcmp(cmd->password, set_cmd.password, 8))) {
        rsp->result = 0x05;
        APP_DEBUG_PRINTF("OPEN_WITH_NOPWD_REMOTE fail id: %d", rsp->result);
    }
    else {
        if(lock_open_with_nopwd_remote() == APP_PORT_SUCCESS) {
            if(cmd->type == 0x01) {
                lock_open_record_report_with_delay(OR_LOG_OPEN_WITH_REMOTE_PHONE, cmd->memberid);
            } else if(cmd->type == 0x02){
                lock_open_record_report_with_delay(OR_LOG_OPEN_WITH_REMOTE_VOICE, cmd->memberid);
            }
            
            APP_DEBUG_PRINTF("OPEN_WITH_NOPWD_REMOTE success");
            
            set_cmd.valid_num--;
            app_port_nv_set(SF_AREA_0, NV_ID_OPEN_WITH_NOPWD_REMOTE, cmd, sizeof(open_with_nopwd_remote_setkey_t));
            
            rsp->result = 0x00; //open success
        } else {
            rsp->result = 0x01; //open fail
            APP_DEBUG_PRINTF("OPEN_WITH_NOPWD_REMOTE fail id: %d", rsp->result);
        }
    }
    
    *rsp_len = sizeof(open_with_nopwd_remote_result_t);
    
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: 
*/
static uint32_t offline_pwd_set_T0_handler(void* cmd_dp_data, void* rsp_dp_data, uint8_t* rsp_dp_data_len)
{
    uint32_t T0_tmp = 0;
    app_port_string_op_intstr2int(cmd_dp_data, 10, (void*)&T0_tmp);
    lock_offline_pwd_set_T0(T0_tmp);
    
    return APP_PORT_SUCCESS;
}

















