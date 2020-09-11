#include "lock_dp_report.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */
typedef enum
{
    DELAY_REPORT_TYPE_CREAT_SUB_REPORT = 0x00,
    DELAY_REPORT_TYPE_OPEN_RECORD_REPORT,
} delay_report_t;

/*********************************************************************
 * LOCAL STRUCT
 */
typedef struct
{
    open_meth_t meth;
    reg_stage_t stage;
    uint8_t hardid;
    uint8_t reg_idx;
    uint8_t result;
} creat_sub_report_t;

typedef struct
{
    uint8_t dp_id;
    uint32_t hardid;
} open_record_report_t;

static struct 
{
    uint8_t type;
    union
    {
        creat_sub_report_t creat_sub_report;
        open_record_report_t open_record_report;
    };
} delay_report_param;

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: 
*/
void delay_report_outtime_handler(void)
{
    switch(delay_report_param.type)
    {
        case DELAY_REPORT_TYPE_CREAT_SUB_REPORT: {
            lock_hard_creat_sub_report(delay_report_param.creat_sub_report.meth,
                                        delay_report_param.creat_sub_report.stage,
                                        delay_report_param.creat_sub_report.hardid,
                                        delay_report_param.creat_sub_report.reg_idx,
                                        delay_report_param.creat_sub_report.result);
        } break;
        
        case DELAY_REPORT_TYPE_OPEN_RECORD_REPORT: {
            lock_open_record_report(delay_report_param.open_record_report.dp_id,
                                     delay_report_param.open_record_report.hardid);
        } break;
        
        default: {
        } break;
    }
}

/*********************************************************  creat open meth  *********************************************************/

/*********************************************************
FN: reg hard process report
PM: reg_idx - Please refer to the documentation
    result - Please refer to the documentation
*/
void lock_hard_creat_sub_report(uint8_t meth, uint8_t stage, uint8_t hardid, uint8_t reg_idx, uint8_t result)
{
    open_meth_creat_result_t* rsp = (void*)g_rsp.dp_data;
    rsp->stage = (reg_stage_t)stage;
    rsp->hardid = hardid;
    rsp->reg_num = reg_idx;
    rsp->result = result;
    g_rsp.dp_data_len = sizeof(open_meth_creat_result_t);
    
    app_port_dp_data_report((void*)&g_rsp, (3 + g_rsp.dp_data_len));
}

/*********************************************************
FN: 
*/
uint32_t lock_hard_creat_sub_report_with_delay(uint8_t meth, uint8_t stage, uint8_t hardid, uint8_t reg_idx, uint8_t result)
{
    delay_report_param.type = DELAY_REPORT_TYPE_CREAT_SUB_REPORT;
    delay_report_param.creat_sub_report.meth = (open_meth_t)meth;
    delay_report_param.creat_sub_report.stage = (reg_stage_t)stage;
    delay_report_param.creat_sub_report.hardid = hardid;
    delay_report_param.creat_sub_report.reg_idx = reg_idx;
    delay_report_param.creat_sub_report.result = result;
    
    lock_timer_start(LOCK_TIMER_DELAY_REPORT);
    return APP_PORT_SUCCESS;
}




/*********************************************************  record report  *********************************************************/

/*********************************************************
FN: open lock event record and report
PM: dp_id - defined in "lock_dp_parser.h"
    hardid - if dp_id=OR_LOG_OPEN_WITH_BT/OR_LOG_OPEN_WITH_REMOTE_PHONE/OR_LOG_OPEN_WITH_REMOTE_VOICE, then hardid=memberid
           - if dp_id=OR_LOG_OPEN_INSIDE,  then hardid=0x00
           - if dp_id=OR_LOG_DOOR_STATE,   then hardid=0x00-close,0x01-open
*/
#define OFFLINE_RECORD_LEN 19
uint32_t lock_open_record_report(uint8_t dp_id, uint32_t hardid)
{
	uint32_t timestamp = app_port_get_timestamp();
    
    g_rsp.dp_id = dp_id;
    
    switch(dp_id)
    {
        case OR_LOG_OPEN_INSIDE: {
            g_rsp.dp_type = APP_PORT_DT_BOOL;
            g_rsp.dp_data_len = APP_PORT_DT_BOOL_LEN;
            g_rsp.dp_data[0] = hardid;
        } break;
        
        default: {
            g_rsp.dp_type = APP_PORT_DT_VALUE;
            g_rsp.dp_data_len = APP_PORT_DT_VALUE_LEN;
            g_rsp.dp_data[0] = hardid>>24;
            g_rsp.dp_data[1] = hardid>>16;
            g_rsp.dp_data[2] = hardid>>8;
            g_rsp.dp_data[3] = hardid;
        } break;
    }
    
//    lock_evt_save(timestamp, (void*)&g_rsp, (3 + g_rsp.dp_data_len));
    lock_evt_save(timestamp, (void*)&g_rsp, (OFFLINE_RECORD_LEN));
    
    return app_port_dp_data_with_time_report(timestamp, (void*)&g_rsp, (3 + g_rsp.dp_data_len));
}

/*********************************************************
FN: 
*/
uint32_t lock_open_record_report_with_delay(uint8_t dp_id, uint32_t hardid)
{
    delay_report_param.type = DELAY_REPORT_TYPE_OPEN_RECORD_REPORT;
    delay_report_param.open_record_report.dp_id = dp_id;
    delay_report_param.open_record_report.hardid = hardid;
    
    lock_timer_start(LOCK_TIMER_DELAY_REPORT);
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: open lock event record and report
PM: dp_id - defined in "lock_dp_parser.h"
    combine_enum - 
    hardid - hard id
*/
uint32_t lock_open_record_combine_report(uint8_t combine_enum, uint8_t size, uint8_t* hardid)
{
//    if(size != 2) {
//        return APP_PORT_ERROR_COMMON;
//    }
//    
//	uint32_t timestamp = app_port_get_timestamp();
//    
//    g_rsp.dp_id = OR_LOG_OPEN_WITH_COMBINE;
//    g_rsp.dp_type = APP_PORT_DT_VALUE;
//    g_rsp.dp_data_len = APP_PORT_DT_ENUM_LEN + size;
//    g_rsp.dp_data[0] = combine_enum;
//    for(uint8_t idx=0; idx<size; idx++) {
//        g_rsp.dp_data[1+idx] = hardid[idx];
//    }
    
	uint32_t timestamp = app_port_get_timestamp();
    
    g_rsp.dp_id = OR_LOG_OPEN_WITH_COMBINE;
    g_rsp.dp_type = APP_PORT_DT_ENUM;
    g_rsp.dp_data_len = APP_PORT_DT_ENUM_LEN;
    g_rsp.dp_data[0] = combine_enum;
    
//    lock_evt_save(timestamp, (void*)&g_rsp, (3 + g_rsp.dp_data_len));
    lock_evt_save(timestamp, (void*)&g_rsp, (OFFLINE_RECORD_LEN));
    
    return app_port_dp_data_with_time_report(timestamp, (void*)&g_rsp, (3 + g_rsp.dp_data_len));
}

/*********************************************************
FN: 
*/
uint32_t lock_open_record_report_offline_pwd(uint8_t dp_id, uint8_t* pwd)
{
	uint32_t timestamp = app_port_get_timestamp();
    
    g_rsp.dp_id = dp_id;
    g_rsp.dp_type = APP_PORT_DT_RAW;
    g_rsp.dp_data_len = OFFLINE_PWD_LEN+6;
    memcpy(&g_rsp.dp_data[0], pwd, OFFLINE_PWD_MAX_NUM);
    
//    lock_evt_save(timestamp, (void*)&g_rsp, (3 + g_rsp.dp_data_len));
    lock_evt_save(timestamp, (void*)&g_rsp, (OFFLINE_RECORD_LEN));
    
    return app_port_dp_data_with_time_report(timestamp, (void*)&g_rsp, (3 + g_rsp.dp_data_len));
}

/*********************************************************
FN: alarm event record and report
*/
uint32_t lock_alarm_record_report(uint8_t alarm_reason)
{
	uint32_t timestamp = app_port_get_timestamp();
    
    g_rsp.dp_id = OR_LOG_ALARM_REASON;
    g_rsp.dp_type = APP_PORT_DT_ENUM;
    g_rsp.dp_data_len = APP_PORT_DT_ENUM_LEN;
    g_rsp.dp_data[0] = alarm_reason;
    
//    lock_evt_save(timestamp, (void*)&g_rsp, (3 + g_rsp.dp_data_len));
    lock_evt_save(timestamp, (void*)&g_rsp, (OFFLINE_RECORD_LEN));
    
    return app_port_dp_data_with_time_report(timestamp, (void*)&g_rsp, (3 + g_rsp.dp_data_len));
}

/*********************************************************
FN: offline event report
*/
static uint32_t last_evt_id = 0;
void lock_offline_evt_report(uint8_t status)
{
    if(status != 0 && status != 0xFF)
    {
        return;
    }
    
    if(status == 0)
    {
        if(last_evt_id != 63) {
            APP_DEBUG_PRINTF("last_evt_id: %d", last_evt_id);
        }
        
        if(lock_evt_delete(last_evt_id) == 0)
        {
            lock_evtid_save(last_evt_id);
        }
    }
    
    uint32_t evt_id = lock_evtid_load();
    static uint8_t buf[256];
    if(lock_evt_load(lock_last_evtid(evt_id), buf, OFFLINE_RECORD_LEN+5) == 0)
    {
        uint32_t timestamp;
        
        memcpy(&timestamp, buf+1, sizeof(uint32_t));
        if(timestamp < 30*365*24*3600) //less then 30 years
        {
            timestamp = app_port_get_old_timestamp(timestamp);
        }
        memcpy(&g_rsp, buf+5, OFFLINE_RECORD_LEN);
        app_port_dp_data_with_time_report(timestamp, (void*)&g_rsp, (3 + g_rsp.dp_data_len));
    }
    last_evt_id = lock_last_evtid(evt_id);
}

/*********************************************************
FN: 
*/
volatile open_meth_sync_new_t g_sync_new;

void lock_open_meth_sync_new_report(uint8_t status)
{
    if(status != 0)
    {
        return;
    }
    
    volatile open_meth_sync_new_hard_t* hard_type = g_sync_new.hard;
    uint8_t* pIdx = (void*)&g_sync_new.idx; //hard_type_idx
    
    g_rsp.dp_id = WR_BSC_OPEN_METH_SYNC_NEW;
    g_rsp.dp_type = APP_PORT_DT_RAW;
    g_rsp.dp_data[0] = 0x00;
    g_rsp.dp_data[1] = g_sync_new.pkg_count;
    g_rsp.dp_data_len = 2;
    
    //轮询每个硬件类型枚举
    for(uint8_t idx=hard_type[*pIdx].idx; idx<hardid_max[hard_type[*pIdx].type]; idx++)
    {
        lock_hard_t hard;
        if(lock_hard_load(hardid_start[hard_type[*pIdx].type]+idx, &hard) == APP_PORT_SUCCESS)
        {
            open_meth_sync_node_new_t node_rsp;
            node_rsp.hardid = hard.hard_id;
            node_rsp.hard_type = hard.hard_type;
            node_rsp.hard_attribute.memberid = hard.member_id;
            node_rsp.hard_attribute.freeze_state = hard.freeze_state;
            
            memcpy(&g_rsp.dp_data[g_rsp.dp_data_len], &node_rsp, sizeof(open_meth_sync_node_new_t));
            g_rsp.dp_data_len += sizeof(open_meth_sync_node_new_t);
            
            hard_type[*pIdx].count++;
        }
        
        //每种硬件类型的硬件个数计数
        hard_type[*pIdx].idx++;
        //某种硬件类型遍历结束或者达到最大包
        if((hard_type[*pIdx].idx == hardid_max[hard_type[*pIdx].type])
            || ((hard_type[*pIdx].count > 0) && ((hard_type[*pIdx].count % 20) == 0)))
        {
            bool is_break = false;
            
            if(hard_type[*pIdx].count > 0) {
                g_sync_new.pkg_count++;
                app_port_dp_data_report((void*)&g_rsp, (3 + g_rsp.dp_data_len));
                is_break = true;
            }
            
            //某种硬件类型遍历结束
            if(hard_type[*pIdx].idx == hardid_max[hard_type[*pIdx].type]) {
                //硬件类型个数计数
                (*pIdx)++;
                idx=hard_type[*pIdx].idx - 1;
//                APP_DEBUG_PRINTF("hard_type_idx: %d", *pIdx);
                if((*pIdx) == g_sync_new.hard_type_len)
                {
                    g_rsp.dp_id = WR_BSC_OPEN_METH_SYNC_NEW;
                    g_rsp.dp_type = APP_PORT_DT_RAW;
                    g_rsp.dp_data_len = sizeof(open_meth_sync_new_last_result_t);
                    
                    open_meth_sync_new_last_result_t* rsp = (void*)g_rsp.dp_data;
                    rsp->stage = 0x01;
                    rsp->pkgs = g_sync_new.pkg_count;
                    
                    app_port_dp_data_report((void*)&g_rsp, (3 + g_rsp.dp_data_len));
                    g_sync_new.flag = 0;
                    is_break = true;
                }
            }
            
            if(is_break) {
                break;
            }
        }
    }
}




/*********************************************************  state sync  *********************************************************/

/*********************************************************
FN: open lock event record and report
PM: dp_id - defined in "lock_dp_parser.h"
    data  - refer to dp_type
*/
uint32_t lock_state_sync_report(uint8_t dp_id, uint32_t data)
{
    g_rsp.dp_id = dp_id;
    
    switch(dp_id)
    {
        //dp_type = value
        case OR_STS_AUTH_LOCK_OUTTIME:
        case OR_STS_BATTERY_PERCENT: {
            g_rsp.dp_type = APP_PORT_DT_VALUE;
            g_rsp.dp_data_len = APP_PORT_DT_VALUE_LEN;
            g_rsp.dp_data[0] = data>>24;
            g_rsp.dp_data[1] = data>>16;
            g_rsp.dp_data[2] = data>>8;
            g_rsp.dp_data[3] = data;
        } break;
        
        //dp_type = enum, but data is value, compatible with OR_BATTERY_PERCENT
        case OR_STS_BATTERY_POSITION: {
            g_rsp.dp_type = APP_PORT_DT_ENUM;
            g_rsp.dp_data_len = APP_PORT_DT_ENUM_LEN;
            g_rsp.dp_data[0] = (data > 66) ? 0x00 : ((data > 33) ? 0x01 : 0x02);
        } break;
        
        //dp_type = enum
        case OR_STS_DOOR_STATE: {
            g_rsp.dp_type = APP_PORT_DT_ENUM;
            g_rsp.dp_data_len = APP_PORT_DT_ENUM_LEN;
            g_rsp.dp_data[0] = data;
        } break;
        
        //dp_type = bool
        case OR_STS_CHILD_LOCK:
        case OR_STS_ANTI_LOCK:
        case WR_STS_REVERSE_LOCK:
        case OR_STS_DOORBELL_RING:
        case OR_STS_LOCK_STATE: {
            g_rsp.dp_type = APP_PORT_DT_BOOL;
            g_rsp.dp_data_len = APP_PORT_DT_BOOL_LEN;
            g_rsp.dp_data[0] = data;
        } break;
        
        default: {
        } break;
    }
  
    return app_port_dp_data_report((void*)&g_rsp, (3 + g_rsp.dp_data_len));
}




















