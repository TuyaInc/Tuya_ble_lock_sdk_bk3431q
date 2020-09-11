#include "rwip_config.h"             // SW configuration
#if (BLE_APP_PRESENT)
#include <string.h>
#include "app_task.h"                // Application task Definition
#include "app.h"                     // Application Definition
#include "gap.h"                     // GAP Definition
#include "gapm_task.h"               // GAP Manager Task API
#include "gapc_task.h"               // GAP Controller Task API
#include "co_bt.h"                   // Common BT Definition
#include "co_math.h"                 // Common Maths Definition
#include "ke_timer.h"
#include "app_fff0.h"                // Application security Definition
#include "app_dis.h"                 // Device Information Service Application Definitions
#include "app_batt.h"                // Battery Application Definitions
#include "app_oads.h"                // Application oads Definition
#include "nvds.h"                    // NVDS Definitions
#include "rf.h"
#include "uart.h"
#include "adc.h"
#include "gpio.h"
#include "wdt.h"
#include "user_config.h"
#include "gattc_task.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */
#define APP_DEVICE_NAME_LENGTH_MAX      (18)

/// List of service to add in the database
enum appm_svc_list
{
    APPM_SVC_FFF0,
//    APPM_SVC_DIS,
    APPM_SVC_LIST_STOP ,
};

/*********************************************************************
 * LOCAL STRUCT
 */
typedef void (*appm_add_svc_func_t)(void);

/*********************************************************************
 * LOCAL VARIABLES
 */
/// Application Task Descriptor
static const struct ke_task_desc TASK_DESC_APP = {NULL, &appm_default_handler,
                                                  appm_state, APPM_STATE_MAX, APP_IDX_MAX};

/// List of functions used to create the database
static const appm_add_svc_func_t appm_add_svc_func_list[APPM_SVC_LIST_STOP] =
{
    (appm_add_svc_func_t)app_fff0_add_fff0s,
//    (appm_add_svc_func_t)app_dis_add_dis,
};

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */
/// Application Environment Structure
struct app_env_tag app_env;




/*********************************************************
FN: Initialize the BLE demo application.
*/
void appm_init()
{
    uint8_t key_len = KEY_LEN;

    // Reset the application manager environment
    memset(&app_env, 0, sizeof(app_env));

    // Create APP task
    ke_task_create(TASK_APP, &TASK_DESC_APP); // main_road 0

    // Initialize Task state
    ke_state_set(TASK_APP, APPM_INIT);

    if (nvds_get(NVDS_TAG_LOC_IRK, &key_len, app_env.loc_irk) != NVDS_OK)
    {
        uint8_t counter;

        // generate a new IRK
        for (counter = 0; counter < KEY_LEN; counter++)
        {
            app_env.loc_irk[counter]    = (uint8_t)co_rand_word();
        }

        // Store the generated value in NVDS
        if (nvds_put(NVDS_TAG_LOC_IRK, KEY_LEN, (uint8_t *)&app_env.loc_irk) != NVDS_OK)
        {
            ASSERT_INFO(0, NVDS_TAG_LOC_IRK, 0);
        }
    }

    //设备信息服务
//    app_dis_init();
}

/*********************************************************
FN: Add a required service in the database
*/
bool appm_add_svc(void)
{
    // Indicate if more services need to be added in the database
    bool more_svc = false;

    // Check if another should be added in the database
    if (app_env.next_svc != APPM_SVC_LIST_STOP)
    {
        ASSERT_INFO(appm_add_svc_func_list[app_env.next_svc] != NULL, app_env.next_svc, 1);

        // Call the function used to add the required service
        appm_add_svc_func_list[app_env.next_svc](); // main_road 4

        // Select following service to add
        app_env.next_svc++;
        more_svc = true;
    }

    return more_svc;
}

/*********************************************************
FN: 设备主动断开连接函数
*/
void appm_disconnect(void)
{
    struct gapc_disconnect_cmd *cmd = KE_MSG_ALLOC(GAPC_DISCONNECT_CMD,
                                                   KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
                                                   gapc_disconnect_cmd);

    cmd->operation = GAPC_DISCONNECT;
    cmd->reason    = CO_ERROR_REMOTE_USER_TERM_CON;

    // Send the message
    ke_msg_send(cmd);
}

/*********************************************************
FN: Put the device in general discoverable and connectable mode
*/
void appm_start_advertising(void)
{
    //处于就绪状态
    if (ke_state_get(TASK_APP) == APPM_READY)
    {
        //准备消息队列，消息类型：GAPM_START_ADVERTISE_CMD
        struct gapm_start_advertise_cmd *cmd = KE_MSG_ALLOC(GAPM_START_ADVERTISE_CMD,
                                                            TASK_GAPM, TASK_APP,
                                                            gapm_start_advertise_cmd);
        //广播参数
        cmd->op.addr_src    = GAPM_STATIC_ADDR;             //源地址为Public或者Static
        cmd->channel_map    = g_adv_param.adv_channal_map;  //广播通道
        cmd->intv_min       = ((g_adv_param.adv_interval_min*8)/5); //广播间隔 - min
        cmd->intv_max       = ((g_adv_param.adv_interval_max*8)/5); //广播间隔 - max
        cmd->op.code        = g_adv_param.adv_type;         //广播类型 - 不定向可连接
        cmd->info.host.mode = GAP_GEN_DISCOVERABLE; //广播模式 - 通用可发现模式

 		// 初始化广播数据
        cmd->info.host.adv_data_len      = g_adv_data_len;
        memcpy(&cmd->info.host.adv_data, g_adv_data, g_adv_data_len);
        // 初始化扫描响应数据
        cmd->info.host.scan_rsp_data_len = g_scan_rsp_len;
        memcpy(&cmd->info.host.scan_rsp_data, g_scan_rsp, g_scan_rsp_len);

        //发送message
        ke_msg_send(cmd);
	 	BK_PRINTF("start advertising");

        //设置任务状态
        ke_state_set(TASK_APP, APPM_ADVERTISING);	
    }
}

/*********************************************************
FN: 设备主动停止广播函数
*/
void appm_stop_advertising(void)
{
    if (ke_state_get(TASK_APP) == APPM_ADVERTISING)
    {
        //准备消息队列，消息类型：GAPM_CANCEL_CMD
        struct gapm_cancel_cmd *cmd = KE_MSG_ALLOC(GAPM_CANCEL_CMD,
                                                   TASK_GAPM, TASK_APP,
                                                   gapm_cancel_cmd);
        cmd->operation = GAPM_CANCEL;

        //发送message
        ke_msg_send(cmd);
	 	BK_PRINTF("appm stop advertising");

        //设置任务状态
        ke_state_set(TASK_APP, APPM_READY);
    }
}

/*********************************************************
FN: 更新广播数据
*/
void appm_update_adv_data( uint8_t* adv_buff, uint8_t adv_len, uint8_t* scan_buff, uint8_t scan_len)
{
	if (ke_state_get(TASK_APP) == APPM_ADVERTISING 
                 && (adv_len <= ADV_DATA_LEN) && (scan_len <= ADV_DATA_LEN))
	{
		struct gapm_update_advertise_data_cmd *cmd =  KE_MSG_ALLOC(
            		            GAPM_UPDATE_ADVERTISE_DATA_CMD,
            		            TASK_GAPM,
            		            TASK_APP,
            		            gapm_update_advertise_data_cmd);

		cmd->operation = GAPM_UPDATE_ADVERTISE_DATA;
		cmd->adv_data_len = adv_len;
		cmd->scan_rsp_data_len = scan_len;

		//memcpy
		memcpy(&cmd->adv_data[0], adv_buff, adv_len);
		memcpy(&cmd->scan_rsp_data[0], scan_buff, scan_len);
        
		// Send the message
		ke_msg_send(cmd);
	}
}

/*********************************************************
FN: 更新mtu
*/
void appc_gatt_mtu_change(uint8_t conidx)
{
    if(ke_state_get(TASK_APP) == APPM_CONNECTED)
    {
        struct gattc_exc_mtu_cmd *cmd = KE_MSG_ALLOC(GATTC_EXC_MTU_CMD,
                                    KE_BUILD_ID(TASK_GATTC, conidx), TASK_APP,  
                                    gattc_exc_mtu_cmd);
        cmd->operation = GATTC_MTU_EXCH;
        cmd->seq_num = 0;
        ke_msg_send(cmd);
    } else {
        BK_PRINTF("%s can't pro. cur state :%d",ke_state_get(TASK_APP));
    }
}

/*********************************************************
FN: 数据长度扩展
*/
void appc_le_data_length_update_req(uint8_t conidx,uint16_t max_tx_octets,uint16_t max_tx_time)
{
    if(ke_state_get(TASK_APP) == APPM_CONNECTED)
    {
        // Prepare the GAPC_SET_LE_PKT_SIZE_CMD message
        struct gapc_set_le_pkt_size_cmd *cmd = KE_MSG_ALLOC(GAPC_SET_LE_PKT_SIZE_CMD,
                                                     KE_BUILD_ID(TASK_GAPC, conidx),TASK_APP , //KE_BUILD_ID(TASK_APPM,conidx)
                                                     gapc_set_le_pkt_size_cmd);
        cmd->operation = GAPC_SET_LE_PKT_SIZE;
        cmd->tx_octets = max_tx_octets;
        cmd->tx_time = max_tx_time;
        BK_PRINTF("appc_le_data_length_update_req tx_octets = %d,tx_time = %d", max_tx_octets,max_tx_time);
        // Send the message
        ke_msg_send(cmd);
    } else {
        BK_PRINTF("%s can't pro. cur state :%d",ke_state_get(TASK_APP));
    }
}

/*********************************************************
FN: 更新连接参数
*/
void appm_update_param(struct gapc_conn_param *conn_param)
{
    // Prepare the GAPC_PARAM_UPDATE_CMD message
    struct gapc_param_update_cmd *cmd = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CMD,
                                                     KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
                                                     gapc_param_update_cmd);
    cmd->operation  = GAPC_UPDATE_PARAMS;
    cmd->intv_min   = conn_param->intv_min;
    cmd->intv_max   = conn_param->intv_max;
    cmd->latency    = conn_param->latency;
    cmd->time_out   = conn_param->time_out;
    // not used by a slave device
    cmd->ce_len_min = 0xFFFF;
    cmd->ce_len_max = 0xFFFF;
//    BK_PRINTF("appm_update_param: intv_min = %d, intv_max = %d, latency = %d, time_out = %d",
//		cmd->intv_min,cmd->intv_max,cmd->latency,cmd->time_out);
    // Send the message
    ke_msg_send(cmd);
}

/*********************************************************
FN: 获取设备名称
*/
uint8_t appm_get_dev_name(uint8_t* name)
{
    // copy name to provided pointer
    memcpy(name, app_env.dev_name, app_env.dev_name_len);
    // return name length
    return app_env.dev_name_len;
}

#endif //(BLE_APP_PRESENT)





