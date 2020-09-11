#include "rwip_config.h"          // SW configuration

#if (BLE_APP_PRESENT)
#include <string.h>
#include "app_task.h"              // Application Manager Task API
#include "app.h"                      // Application Manager Definition
#include "gapc_task.h"            // GAP Controller Task API
#include "gapm_task.h"          // GAP Manager Task API
#include "gattc_task.h"
#include "arch.h"                    // Platform Definitions

#include "ke_timer.h"             // Kernel timer
#include "app_fff0.h"              // fff0 Module Definition
#include "fff0s_task.h"
#include "app_dis.h"              // Device Information Module Definition
#include "diss_task.h"
#include "app_batt.h"             // Battery Module Definition
#include "bass_task.h"
#include "app_oads.h"             
#include "oads_task.h"              
#include "gpio.h"
#include "audio.h"
#include "uart.h"
#include "BK3435_reg.h"
#include "icu.h"
#include "reg_ble_em_cs.h"
#include "lld.h"
#include "wdt.h"
#include "user_config.h"
#include "lock_common.h"




/*********************************************************
FN: 获取handler
*/
static uint8_t appm_get_handler(const struct ke_state_handler *handler_list,
                                ke_msg_id_t msgid,
                                void *param,
                                ke_task_id_t src_id)
{
    uint8_t counter;
    // Get the message handler function by parsing the message table
    for (counter = handler_list->msg_cnt; 0 < counter; counter--)
    {
        struct ke_msg_handler handler = (*(handler_list->msg_table + counter - 1));
			
        if ((handler.id == msgid) ||
            (handler.id == KE_MSG_DEFAULT_HANDLER))
        {
            // If handler is NULL, message should not have been received in this state
            ASSERT_ERR(handler.func);

            return (uint8_t)(handler.func(msgid, param, TASK_APP, src_id));
        }
    }

    // If we are here no handler has been found, drop the message
    return (KE_MSG_CONSUMED);
}

/*********************************************************
FN: 
*/
// Timer 0
static int bk_timer0_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER0);
    return KE_MSG_CONSUMED;
}
// Timer 1
static int bk_timer1_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER1);
    return KE_MSG_CONSUMED;
}
// Timer 2
static int bk_timer2_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER2);
    return KE_MSG_CONSUMED;
}
// Timer 3
static int bk_timer3_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER3);
    return KE_MSG_CONSUMED;
}
// Timer 4
static int bk_timer4_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER4);
    return KE_MSG_CONSUMED;
}
// Timer 5
static int bk_timer5_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER5);
    return KE_MSG_CONSUMED;
}
// Timer 6
static int bk_timer6_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER6);
    return KE_MSG_CONSUMED;
}
// Timer 7
static int bk_timer7_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER7);
    return KE_MSG_CONSUMED;
}
// Timer 8
static int bk_timer8_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER8);
    return KE_MSG_CONSUMED;
}
// Timer 9
static int bk_timer9_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER9);
    return KE_MSG_CONSUMED;
}
// Timer 10
static int bk_timer10_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER10);
    return KE_MSG_CONSUMED;
}
// Timer 11
static int bk_timer11_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER11);
    return KE_MSG_CONSUMED;
}
// Timer 12
static int bk_timer12_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER12);
    return KE_MSG_CONSUMED;
}
// Timer 13
static int bk_timer13_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER13);
    return KE_MSG_CONSUMED;
}
// Timer 14
static int bk_timer14_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER14);
    return KE_MSG_CONSUMED;
}
// Timer 15
static int bk_timer15_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER15);
    return KE_MSG_CONSUMED;
}
// Timer 16
static int bk_timer16_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER16);
    return KE_MSG_CONSUMED;
}
// Timer 17
static int bk_timer17_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER17);
    return KE_MSG_CONSUMED;
}
// Timer 18
static int bk_timer18_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER18);
    return KE_MSG_CONSUMED;
}
// Timer 19
static int bk_timer19_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) {
    bk_timer_handler(BK_TIMER19);
    return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief Handles ready indication from the GAP. - Reset the stack
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapm_device_ready_ind_handler(ke_msg_id_t const msgid,
                                         void const *param,
                                         ke_task_id_t const dest_id,
                                         ke_task_id_t const src_id)
{
    // Application has not been initialized
    ASSERT_ERR(ke_state_get(dest_id) == APPM_INIT);

    // Reset the stack
    struct gapm_reset_cmd* cmd = KE_MSG_ALLOC(GAPM_RESET_CMD,
                                              TASK_GAPM, TASK_APP,
                                              gapm_reset_cmd);
    cmd->operation = GAPM_RESET; // main_road 1
    ke_msg_send(cmd);
    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief Handles GAP manager command complete events.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapm_cmp_evt_handler(ke_msg_id_t const msgid,
                                struct gapm_cmp_evt const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
//	BK_PRINTF("param->operation = 0x%x, param->status = 0x%x", param->operation, param->status);
    switch(param->operation)
    {
        // Reset completed
        case (GAPM_RESET): { // main_road 2
            if(param->status == GAP_ERR_NO_ERROR) {
                // Set Device configuration
                struct gapm_set_dev_config_cmd* cmd = KE_MSG_ALLOC(GAPM_SET_DEV_CONFIG_CMD,
	                                                                   TASK_GAPM, TASK_APP,
                                                                   gapm_set_dev_config_cmd);
                // Set the operation
                cmd->operation = GAPM_SET_DEV_CONFIG;
                // Set the device role - Peripheral
                cmd->role      = GAP_ROLE_PERIPHERAL;
                // Set Data length parameters
                cmd->sugg_max_tx_octets = BLE_MIN_OCTETS;
                cmd->sugg_max_tx_time   = BLE_MAX_TIME_4_2;

		 		cmd->max_mtu = BLE_MIN_OCTETS;//BLE_MIN_OCTETS;
                //Do not support secure connections
                cmd->pairing_mode = GAPM_PAIRING_LEGACY;
                
 				//cmd->addr_type   = GAPM_CFG_ADDR_HOST_PRIVACY; //2017-10-24 by alen
                // load IRK
                memcpy(cmd->irk.key, app_env.loc_irk, KEY_LEN);

                app_env.next_svc = 0;

                // Send message
                ke_msg_send(cmd);
            } else {
                ASSERT_ERR(0);
            }
        } break;

        case (GAPM_PROFILE_TASK_ADD): {
            //添加服务
            if (!appm_add_svc()) {
                //进入就绪状态
                ke_state_set(TASK_APP, APPM_READY); // main_road 6

                lock_common_init();
                bk_test();
				bk_adv_start();
            }
        } break;
        
        // Device Configuration updated
        case (GAPM_SET_DEV_CONFIG): {
            ASSERT_INFO(param->status == GAP_ERR_NO_ERROR, param->operation, param->status);
            //进入创建服务状态
            ke_state_set(TASK_APP, APPM_CREATE_DB); // main_road 3
            // Add the first required service in the database
            // and wait for the PROFILE_ADDED_IND
            appm_add_svc();
        } break;

        case (GAPM_ADV_NON_CONN):
        case (GAPM_ADV_UNDIRECT):
        case (GAPM_ADV_DIRECT):
		case (GAPM_UPDATE_ADVERTISE_DATA):
        case (GAPM_ADV_DIRECT_LDC): {
			if (param->status == GAP_ERR_TIMEOUT)
			{
                ke_state_set(TASK_APP, APPM_READY);
				
				//device not bonded, start general adv
				bk_adv_start();
            }
			else if (param->status == GAP_ERR_CANCELED)
			{
//                BK_PRINTF("%s, GAP_ERR_CANCELED", __func__);
                ke_state_set(TASK_APP, APPM_READY);

                if(g_adv_restart_glag)
				{
                    BK_PRINTF("restart adv");
                    g_adv_restart_glag = false;
                    bk_adv_start();
                }
            }
		} break;

        default: { // Drop the message
        } break;
    }
    return (KE_MSG_CONSUMED);
}

/*********************************************************
FN: 
*/
static int gapc_get_dev_info_req_ind_handler(ke_msg_id_t const msgid,
        struct gapc_get_dev_info_req_ind const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    switch(param->req)
    {
        case GAPC_DEV_NAME: {
            struct gapc_get_dev_info_cfm * cfm = KE_MSG_ALLOC_DYN(GAPC_GET_DEV_INFO_CFM,
                                                    src_id, dest_id,
                                                    gapc_get_dev_info_cfm, APP_DEVICE_NAME_MAX_LEN);
            cfm->req = param->req;
            cfm->info.name.length = appm_get_dev_name(cfm->info.name.value);

            // Send message
            ke_msg_send(cfm);
        } break;

        case GAPC_DEV_APPEARANCE: {
            // Allocate message
            struct gapc_get_dev_info_cfm *cfm = KE_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM,
                                                    src_id, dest_id,
                                                    gapc_get_dev_info_cfm);
            cfm->req = param->req;
            
            // No appearance
            cfm->info.appearance = 0;

            // Send message
            ke_msg_send(cfm);
        } break;

        case GAPC_DEV_SLV_PREF_PARAMS: {
            // Allocate message
            struct gapc_get_dev_info_cfm *cfm = KE_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM,
                    								src_id, dest_id,
                                                    gapc_get_dev_info_cfm);
            cfm->req = param->req;
            // Slave preferred Connection interval Min
            cfm->info.slv_params.con_intv_min = 8;
            // Slave preferred Connection interval Max
            cfm->info.slv_params.con_intv_max = 10;
            // Slave preferred Connection latency
            cfm->info.slv_params.slave_latency = 180;
            // Slave preferred Link supervision timeout
            cfm->info.slv_params.conn_timeout  = 600;  // 6s (600*10ms)

            // Send message
            ke_msg_send(cfm);
        } break;

        default: { /* Do Nothing */
        } break;
    }
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles GAPC_SET_DEV_INFO_REQ_IND message.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_set_dev_info_req_ind_handler(ke_msg_id_t const msgid,
        struct gapc_set_dev_info_req_ind const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
	// Set Device configuration
	struct gapc_set_dev_info_cfm* cfm = KE_MSG_ALLOC(GAPC_SET_DEV_INFO_CFM, src_id, dest_id,
                                                 gapc_set_dev_info_cfm);
	// Reject to change parameters
	cfm->status = GAP_ERR_REJECTED;
	cfm->req = param->req;
	// Send message
	ke_msg_send(cfm);

	return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles connection complete event from the GAP. Enable all required profiles
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_connection_req_ind_handler(ke_msg_id_t const msgid,
                                           struct gapc_connection_req_ind const *param,
                                           ke_task_id_t const dest_id,
                                           ke_task_id_t const src_id)
{
    app_env.conidx = KE_IDX_GET(src_id);
    
    //检查连接句柄是否有效
    if (app_env.conidx != GAP_INVALID_CONIDX)
    {
        BK_PRINTF("bk connected");
        {
            tuya_ble_connected_handler();
            app_common_evt_send_only_evt(APP_EVT_BLE_GAP_EVT_CONNECTED);
        }
        
        //记录连接句柄
        app_env.conhdl = param->conhdl;

        // Send connection confirmation
        struct gapc_connection_cfm *cfm = KE_MSG_ALLOC(GAPC_CONNECTION_CFM,
                KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
                gapc_connection_cfm);

        cfm->auth = GAP_AUTH_REQ_NO_MITM_NO_BOND;
        // Send the message
        ke_msg_send(cfm);

        //进入连接状态
        ke_state_set(dest_id, APPM_CONNECTED);
    } else {
        // No connection has been establish, restart advertising
		bk_adv_start();
    }
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles GAP controller command complete events.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_cmp_evt_handler(ke_msg_id_t const msgid,
                                struct gapc_cmp_evt const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
	switch(param->operation)
	{
    	case (GAPC_UPDATE_PARAMS): { //0x09
			if (param->status != GAP_ERR_NO_ERROR) {
            	BK_PRINTF("gapc update params fail !");
			} else {
				BK_PRINTF("gapc update params ok !");
			}
    	} break;

		case (GAPC_SECURITY_REQ): { //0x0c
			if (param->status != GAP_ERR_NO_ERROR) {
	            BK_PRINTF("gapc security req fail !");
	        } else {
	            BK_PRINTF("gapc security req ok !");
	        }
		} break;
        
		case (GAPC_BOND): { // 0xa
	        if (param->status != GAP_ERR_NO_ERROR) {
	            BK_PRINTF("gapc bond fail !");
	        } else {
	            BK_PRINTF("gapc bond ok !");
	        }
    	} break;
		
		case (GAPC_ENCRYPT): { // 0xb
			if (param->status != GAP_ERR_NO_ERROR) {
				BK_PRINTF("gapc encrypt start fail !");
			} else {
				BK_PRINTF("gapc encrypt start ok !");
			}
		} break;

    	default: {
        } break;
    }
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles disconnection complete event from the GAP.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_disconnect_ind_handler(ke_msg_id_t const msgid,
                                      struct gapc_disconnect_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
	BK_PRINTF("bk Disconnected: 0x%02x", param->reason);

    {
        g_sync_new.flag = 0;
        tuya_ble_disconnected_handler();
        app_common_evt_send_only_evt(APP_EVT_BLE_GAP_EVT_DISCONNECTED);
    }
    
	//进入就绪状态
	ke_state_set(TASK_APP, APPM_READY);
    
	//重启广播
	bk_adv_start();

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles profile add indication from the GAP.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapm_profile_added_ind_handler(ke_msg_id_t const msgid,
                                          struct gapm_profile_added_ind *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
    // Current State
    uint8_t state = ke_state_get(dest_id);

    if (state == APPM_CREATE_DB) {
//        BK_PRINTF("APPM_CREATE_DB: %d", param->prf_task_id);
        switch (param->prf_task_id)
        {
            default: {
            } break;
        }
    } else {
        ASSERT_INFO(0, state, src_id);
    }
    return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief Handles reception of all messages sent from the lower layers to the application
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int appm_msg_handler(ke_msg_id_t const msgid,
                            void *param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
    // Retrieve identifier of the task from received message
    ke_task_id_t src_task_id = MSG_T(msgid);
    // Message policy
    uint8_t msg_pol          = KE_MSG_CONSUMED;

    switch (src_task_id)
    {
        case (TASK_ID_GAPC): {
            // else drop the message
        } break;

        case (TASK_ID_GATTC): {
            // Service Changed - Drop
        } break;

        case (TASK_ID_FFF0S): {
            // Call the Health Thermometer Module
            msg_pol = appm_get_handler(&app_fff0_table_handler, msgid, param, src_id);
        } break;
				
//        case (TASK_ID_DISS): {
//            // Call the Device Information Module
//            msg_pol = appm_get_handler(&app_dis_table_handler, msgid, param, src_id);
//        } break;

        default: {
        } break;
    }
    return (msg_pol);
}

/*******************************************************************************
 * Function: gapc_le_pkt_size_ind_handler 会在MTU更新回调接口 gattc_mtu_changed_ind_handler 触发后触发该handler
 * Description: GAPC_LE_PKT_SIZE_IND
 * Input: msgid   -Id of the message received.
 *		  param   -Pointer to the parameters of the message.
 *		  dest_id -ID of the receiving task instance
 *		  src_id  -ID of the sending task instance.
 * Return: If the message was consumed or not.
 * Others: void
*******************************************************************************/
static int gapc_le_pkt_size_ind_handler (ke_msg_id_t const msgid, 
									const struct gapc_le_pkt_size_ind  *param,
                 					ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
   	BK_PRINTF("%s", __func__);
	BK_PRINTF("1max_rx_octets = %d",param->max_rx_octets);
	BK_PRINTF("1max_rx_time = %d",param->max_rx_time);
	BK_PRINTF("1max_tx_octets = %d",param->max_tx_octets);
	BK_PRINTF("1max_tx_time = %d",param->max_tx_time);
	
	return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief  GAPC_PARAM_UPDATED_IND
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_param_updated_ind_handler (ke_msg_id_t const msgid, 
									const struct gapc_param_updated_ind  *param,
                 					ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    BK_PRINTF("conn_param_update: min-%dms, max-%dms, latency-%d, timeout-%dms", \
        (uint16_t)(param->con_interval*1.25), \
        (uint16_t)(param->con_interval*1.25), \
        (uint16_t)(param->con_latency), \
        (uint16_t)(param->sup_to*10) );
	return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief  GATTC_MTU_CHANGED_IND
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_mtu_changed_ind_handler(ke_msg_id_t const msgid,
                                     struct gattc_mtu_changed_ind const *ind,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
	BK_PRINTF("%s",__func__);
	BK_PRINTF("ind->mtu = %d,seq = %d",ind->mtu,ind->seq_num);
	
 	return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief   GAPC_PARAM_UPDATE_REQ_IND
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_param_update_req_ind_handler(ke_msg_id_t const msgid,
                                struct gapc_param_update_req_ind const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
	BK_PRINTF("%s", __func__);
	// Prepare the GAPC_PARAM_UPDATE_CFM message
    struct gapc_param_update_cfm *cfm = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CFM,
                                             src_id, dest_id,
                                             gapc_param_update_cfm);
	 
	cfm->ce_len_max = 0xffff;
	cfm->ce_len_min = 0xffff;
	cfm->accept = true; 

	// Send message
    ke_msg_send(cfm);
	 
	return (KE_MSG_CONSUMED);
}




/*
 * GLOBAL VARIABLES DEFINITION
 ****************************************************************************************
 */
/* Default State handlers definition. */
const struct ke_msg_handler appm_default_state[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,    	(ke_msg_func_t)appm_msg_handler},
    {GAPM_DEVICE_READY_IND,     	(ke_msg_func_t)gapm_device_ready_ind_handler},
    {GAPM_CMP_EVT,             		(ke_msg_func_t)gapm_cmp_evt_handler},
    {GAPC_GET_DEV_INFO_REQ_IND, 	(ke_msg_func_t)gapc_get_dev_info_req_ind_handler},
    {GAPC_SET_DEV_INFO_REQ_IND, 	(ke_msg_func_t)gapc_set_dev_info_req_ind_handler},
    {GAPC_CONNECTION_REQ_IND,   	(ke_msg_func_t)gapc_connection_req_ind_handler},
    {GAPC_CMP_EVT,             		(ke_msg_func_t)gapc_cmp_evt_handler},
    {GAPC_DISCONNECT_IND,       	(ke_msg_func_t)gapc_disconnect_ind_handler},
    {GAPM_PROFILE_ADDED_IND,    	(ke_msg_func_t)gapm_profile_added_ind_handler},
    {GAPC_LE_PKT_SIZE_IND,			(ke_msg_func_t)gapc_le_pkt_size_ind_handler},
    {GAPC_PARAM_UPDATED_IND,		(ke_msg_func_t)gapc_param_updated_ind_handler},
    {GATTC_MTU_CHANGED_IND,			(ke_msg_func_t)gattc_mtu_changed_ind_handler},
    {GAPC_PARAM_UPDATE_REQ_IND, 	(ke_msg_func_t)gapc_param_update_req_ind_handler},
    {BK_TIMER0,				        (ke_msg_func_t)bk_timer0_handler},
    {BK_TIMER1,				        (ke_msg_func_t)bk_timer1_handler},
    {BK_TIMER2,				        (ke_msg_func_t)bk_timer2_handler},
    {BK_TIMER3,				        (ke_msg_func_t)bk_timer3_handler},
    {BK_TIMER4,				        (ke_msg_func_t)bk_timer4_handler},
    {BK_TIMER5,				        (ke_msg_func_t)bk_timer5_handler},
    {BK_TIMER6,				        (ke_msg_func_t)bk_timer6_handler},
    {BK_TIMER7,				        (ke_msg_func_t)bk_timer7_handler},
    {BK_TIMER8,				        (ke_msg_func_t)bk_timer8_handler},
    {BK_TIMER9,				        (ke_msg_func_t)bk_timer9_handler},
    {BK_TIMER10,                    (ke_msg_func_t)bk_timer10_handler},
    {BK_TIMER11,                    (ke_msg_func_t)bk_timer11_handler},
    {BK_TIMER12,                    (ke_msg_func_t)bk_timer12_handler},
    {BK_TIMER13,                    (ke_msg_func_t)bk_timer13_handler},
    {BK_TIMER14,                    (ke_msg_func_t)bk_timer14_handler},
    {BK_TIMER15,                    (ke_msg_func_t)bk_timer15_handler},
    {BK_TIMER16,                    (ke_msg_func_t)bk_timer16_handler},
    {BK_TIMER17,                    (ke_msg_func_t)bk_timer17_handler},
    {BK_TIMER18,                    (ke_msg_func_t)bk_timer18_handler},
    {BK_TIMER19,                    (ke_msg_func_t)bk_timer19_handler},
};

/* Specifies the message handlers that are common to all states. */
const struct ke_state_handler appm_default_handler = KE_STATE_HANDLER(appm_default_state);

/* Defines the place holder for the states of all the task instances. */
ke_state_t appm_state[APP_IDX_MAX];

#endif //(BLE_APP_PRESENT)

/// @} APPTASK
