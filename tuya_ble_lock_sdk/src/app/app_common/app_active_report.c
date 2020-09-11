#include "app_active_report.h"

/*
文件说明：
    为方便移植，该文件的内容不符合app_port对上层的封装标准。
使用场景：
    在设备处于离线状态并需要主动上报数据时，调用app_active_report_start()触发网关连接设备，设备上线后即可进行数据上报；
    设备上报完成后调用app_active_report_finished_and_disconnect()断开连接。
移植流程：
    1. 在lock_timer.c/h文件中创建“LOCK_TIMER_ACTIVE_REPORT”定时器，用于超时后恢复广播间隔；
    2. 在case“TUYA_BLE_CB_EVT_CONNECTE_STATUS”中调用app_active_report_stop(ACTICE_REPORT_STOP_STATE_BONDING)，用于绑定后关闭定时器;
    3. 在case“APP_EVT_BLE_GAP_EVT_DISCONNECTED”中调用app_active_report_finished_and_disconnect_handler()，用于数据上报完成后恢复现场；
*/


/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static volatile bool s_active_report_running_flag = false;
static volatile bool s_active_report_bonding_flag = false;

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: 
*/
void app_active_report_start(void)
{
    if (ke_state_get(TASK_APP) == APPM_ADVERTISING) {
        //改变广播间隔
        g_adv_param.adv_interval_min = APP_ACTIVE_REPORT_ADV_INTERVAL;
        g_adv_param.adv_interval_max = APP_ACTIVE_REPORT_ADV_INTERVAL;
        bk_adv_param_set();
        
        //改变标志位
        g_scan_rsp[8] |= 0x01;
        bk_adv_update_advDataAndScanRsp();
        
        //启动定时器
        lock_timer_start(LOCK_TIMER_ACTIVE_REPORT);
        
        s_active_report_running_flag = true;
        APP_DEBUG_PRINTF("app_active_report_start");
    }
    else {
        APP_DEBUG_PRINTF("device is not advertising");
    }
}

/*********************************************************
FN: 
*/
void app_active_report_stop(uint8_t state)
{
    if(s_active_report_running_flag) {
        if(state == ACTICE_REPORT_STOP_STATE_OUTTIME) {
            if (ke_state_get(TASK_APP) == APPM_ADVERTISING)
            {
                //恢复广播间隔
                g_adv_param.adv_interval_min = APP_PORT_ADV_INTERVAL;
                g_adv_param.adv_interval_max = APP_PORT_ADV_INTERVAL;
                bk_adv_param_set();
                
                APP_DEBUG_PRINTF("app_active_report_stop: 30s is timeout");
            }
            else {
                APP_DEBUG_PRINTF("device is not advertising");
            }
        }
        else if(state == ACTICE_REPORT_STOP_STATE_BONDING) {
            //关闭定时器
            lock_timer_stop(LOCK_TIMER_ACTIVE_REPORT);
            
            s_active_report_bonding_flag = true;
            APP_DEBUG_PRINTF("app_active_report_stop: device is bonding");
        }
    }
    else {
        APP_DEBUG_PRINTF("app_active_report is not running");
    }
}

/*********************************************************
FN: 
*/
void app_active_report_finished_and_disconnect(void)
{
    app_port_ble_gap_disconnect();
}

/*********************************************************
FN: 
*/
void app_active_report_finished_and_disconnect_handler(void)
{
    if(s_active_report_running_flag && s_active_report_bonding_flag) {
        //恢复广播间隔
        g_adv_param.adv_interval_min = APP_PORT_ADV_INTERVAL;
        g_adv_param.adv_interval_max = APP_PORT_ADV_INTERVAL;
        bk_adv_param_set();
        
        //恢复标志位
        g_scan_rsp[8] &= 0xFE;
        bk_adv_update_advDataAndScanRsp();
        
        s_active_report_running_flag = false;
        s_active_report_bonding_flag = false;
        APP_DEBUG_PRINTF("app_active_report_finished_and_disconnect_handler");
    }
    else {
        APP_DEBUG_PRINTF("app_active_report is not running or device is not bonding");
    }
}


















