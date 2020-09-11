/**
 ****************************************************************************************
 *
 * @file sdp_service_task.c
 *
 * @brief SDP Service Client Task implementation.
 *
 * Copyright (C) Beken 2016-2017
 *
 *
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @addtogroup SDPTASK
 * @{
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#include "user_config.h"

#if (BLE_SDP_CLIENT)
#include "gap.h"
#include "attm.h"
#include "sdp_service.h"
#include "sdp_service_task.h"
#include "gattc_task.h"
#include "co_math.h"
#include "uart.h"
/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/*
 * GLOBAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Handles reception of the @ref BASC_ENABLE_REQ message.
 * The handler enables the NTF Service Client Role.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int sdp_enable_req_handler(ke_msg_id_t const msgid,
                                  struct sdp_enable_req const *param,
                                  ke_task_id_t const dest_id,
                                  ke_task_id_t const src_id)
{
	// Status
	int msg_status = KE_MSG_CONSUMED;
	return (msg_status);
}
/**
 ****************************************************************************************
 * @brief Handles reception of the @ref SDP_READ_INFO_REQ message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int sdp_read_info_req_handler(ke_msg_id_t const msgid,
                                     struct sdp_read_info_req const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
	UART_PRINTF("%s\r\n",__func__);

	int msg_status = KE_MSG_CONSUMED;
	uint8_t conidx = KE_IDX_GET(dest_id);
	uint8_t char_idx;
	uint16_t handle;
	uint8_t desc_nb;
	struct sdp_env_tag *sdp_env =(struct sdp_env_tag *) prf_env_get_from_uuid(param->uuid,0);
	if(sdp_env != NULL)
	{
		char_idx = sdp_env->db_env[conidx].cnx_env->sdp.char_idx;
		desc_nb = sdp_env->db_env[conidx].cnx_env->sdp.descs_nb;
		handle = sdp_env->db_env[conidx].cnx_env->sdp.chars[char_idx].val_hdl;
		uint16_t val_prop =  sdp_env->db_env[conidx].cnx_env->sdp.chars[char_idx].prop;
		UART_PRINTF("desc_nb = 0x%x,char_idx = 0x%x,value_handle = 0x%x\r\n",desc_nb,char_idx,handle);
		if(SDPC_CHAR_NTF_CFG == param->info)
		{
			for(int i = 0; i < desc_nb; i++)
			{
				if((ATT_DESC_CLIENT_CHAR_CFG == sdp_env->db_env[conidx].cnx_env->sdp.descs[i].uuid)
				        && (sdp_env->db_env[conidx].cnx_env->sdp.descs[i].char_code == char_idx))
				{
					handle = sdp_env->db_env[conidx].cnx_env->sdp.descs[i].desc_hdl;
					val_prop |= ATT_CHAR_PROP_RD;
					break;
				}
			}
		}
		else if(SDPC_CHAR_USER_DESC_VAL == param->info)
		{
			for(int i = 0; i < desc_nb; i++)
			{
				if((ATT_DESC_CHAR_USER_DESCRIPTION == sdp_env->db_env[conidx].cnx_env->sdp.descs[i].uuid)
				        && (sdp_env->db_env[conidx].cnx_env->sdp.descs[i].char_code == char_idx))
				{
					handle = sdp_env->db_env[conidx].cnx_env->sdp.descs[i].desc_hdl;
					val_prop |= ATT_CHAR_PROP_RD;
					break;
				}
			}
		}
		UART_PRINTF("prop = 0x%x,char_idx = 0x%x\r\n",val_prop,char_idx);
		UART_PRINTF("handle = 0x%x\r\n",handle);
		if((val_prop & ATT_CHAR_PROP_RD) == ATT_CHAR_PROP_RD )
		{
			UART_PRINTF("prf_read_char_send \r\n");
			prf_read_char_send(&(sdp_env->prf_env), conidx,handle, handle, handle);
		}
		else
		{
			UART_PRINTF("val_prop not read prop!!!! \r\n");
		}
	}
	else
	{
		UART_PRINTF("param unvalid uuid =  0x%02x\r\n",param->uuid);
	}

	return (msg_status);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref BASC_BATT_LEVEL_NTF_CFG_REQ message.
 * It allows configuration of the peer ntf/stop characteristic for Battery Level Characteristic.
 * Will return an error code if that cfg char does not exist.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int sdp_write_value_info_req_handler(ke_msg_id_t const msgid,
        struct sdp_write_info_req const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)

{
	int msg_status = KE_MSG_CONSUMED;

	uint8_t conidx = KE_IDX_GET(dest_id);
	uint8_t char_idx;
	struct sdp_env_tag *sdp_env =(struct sdp_env_tag *) prf_env_get_from_uuid(param->uuid,0);
	if(sdp_env != NULL)
	{
		char_idx = sdp_env->db_env[conidx].cnx_env->sdp.char_idx;
		uint16_t handle = sdp_env->db_env[conidx].cnx_env->sdp.chars[char_idx].val_hdl;
		uint16_t length = param->length;
		uint16_t val_prop =  sdp_env->db_env[conidx].cnx_env->sdp.chars[char_idx].prop;
		uint8_t *buf = (uint8_t *)(&param->data[0]);
		uint8_t operation = GATTC_WRITE_NO_RESPONSE;
		if((val_prop & ATT_CHAR_PROP_WR) == ATT_CHAR_PROP_WR)
		{
			operation = GATTC_WRITE;
			UART_PRINTF("operation = %x\r\n",operation);
		}
		prf_gatt_write(&(sdp_env->prf_env), conidx,handle, buf,length,operation,0);
	}
	else
	{
		UART_PRINTF("param unvalid uuid =  0x%02x\r\n",param->uuid);
	}
	return (msg_status);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref BASC_BATT_LEVEL_NTF_CFG_REQ message.
 * It allows configuration of the peer ntf/stop characteristic for Battery Level Characteristic.
 * Will return an error code if that cfg char does not exist.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int sdp_write_ntf_cfg_req_handler(ke_msg_id_t const msgid,
        struct sdp_w_ntf_cfg_req const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
	UART_PRINTF("%s\r\n",__func__);

	int msg_status = KE_MSG_CONSUMED;
	uint8_t conidx = KE_IDX_GET(dest_id);
	uint8_t char_idx;
	// put value in air format
	UART_PRINTF("param->uuid = %x\r\n",param->uuid);
	struct sdp_env_tag *sdp_env =(struct sdp_env_tag *) prf_env_get_from_uuid(param->uuid,param->char_num);
	if(sdp_env != NULL)
	{
		char_idx = sdp_env->db_env[conidx].cnx_env->sdp.char_idx;
		uint16_t handle = sdp_env->db_env[conidx].cnx_env->sdp.chars[char_idx].val_hdl;
		UART_PRINTF("handle = 0x%x\r\n",handle + 1);
		prf_gatt_write_ntf_ind(&(sdp_env->prf_env), conidx,  handle + 1, param->ntf_cfg, 0xa5);
	}
	else
	{
		UART_PRINTF("param unvalid uuid =  0x%02x\r\n",param->uuid);
	}
	return (msg_status);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_CMP_EVT message.
 * This generic event is received for different requests, so need to keep track.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_cmp_evt_handler(ke_msg_id_t const msgid,
                                 struct gattc_cmp_evt const *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id)
{
	uint8_t state = ke_state_get(dest_id);

	UART_PRINTF("sdp %s dest_id = %x\r\n",__func__,dest_id);
	UART_PRINTF("operation = 0x%x,status = 0x%x,seq_num = 0x%x\r\n",param->operation,param->status,param->seq_num);
	struct gattc_cmp_evt *cmp_evt  = KE_MSG_ALLOC(GATTC_CMP_EVT, TASK_APP, dest_id, gattc_cmp_evt);
	cmp_evt->operation  = param->operation;
	cmp_evt->status = param->status;
	cmp_evt->seq_num = param->seq_num ;

	ke_msg_send(cmp_evt);

	return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_READ_IND message.
 * Generic event received after every simple read command sent to peer server.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_read_ind_handler(ke_msg_id_t const msgid,
                                  struct gattc_read_ind const *param,
                                  ke_task_id_t const dest_id,
                                  ke_task_id_t const src_id)
{
	uint8_t state = ke_state_get(dest_id);

	UART_PRINTF("%s \r\n",__func__);
	UART_PRINTF("handle = 0x%x\r\n",param->handle);
	UART_PRINTF("length = 0x%x\r\n",param->length);
	UART_PRINTF("offset = 0x%x\r\n",param->offset);
	UART_PRINTF("value = 0x");

	for(int i =0 ; i < param->length; i++)
	{
		UART_PRINTF("%x ",param->value[i]);
	}
	UART_PRINTF("\r\n");

	return (KE_MSG_CONSUMED);
}



//主机端接收到从设备发过来的Notify的回调函数
/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_EVENT_IND message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
#if USB_DRIVER
struct rev_ntf_data notify_data;
extern void test_usb_device(void);
#include "app.h"
#include "sbc_decoder.h"
#endif
static int gattc_event_ind_handler(ke_msg_id_t const msgid,
                                   struct gattc_event_ind const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
	uint8_t state = ke_state_get(dest_id);

#if 1 ///190726
	UART_PRINTF("RECEIVE Value 1 = \r\n");
	for(int i = 0; i< param->length; i++)
	{
		UART_PRINTF("%02x ",param->value[i]);
	}
	UART_PRINTF("\r\n");
#endif


#if USB_DRIVER   
if(param->length==0x08)
    {
        UART_PRINTF("sk 1\r\n");
        notify_data.notify_standard_key_status=1;
        memcpy(&notify_data.notify_standard_key[1],param->value,param->length);

    }
    if(param->length==0x02)
    {
        notify_data.notify_media_key_status=1;
	 if((param->value[0] != 0) && (param->value[0] != 0))
	 {
        	memcpy(&notify_data.notify_media_key[1],param->value,param->length);
	 }
	  UART_PRINTF("%x  %x\r\n",notify_data.notify_media_key[1],notify_data.notify_media_key[2]);

    }
    if(param->length==0x01)
    {
        notify_data.notify_power_key_status=1;
        memcpy(&notify_data.notify_power_key[1],param->value,param->length);

    }
    if(param->length==0x04)
    {
        notify_data.notify_mouse_status=1;
        memcpy(&notify_data.notify_mouse[1],param->value,param->length);

    }
    if(param->length==0x13 )
    {
        notify_data.notify_voice_status=1;
	  decode_vocie_data(param->value,param->length);
      	  //  memcpy(&notify_data.notify_voice[notify_data.notify_voice_write*param->length],param->value,param->length);
        notify_data.notify_voice_write++;
        notify_data.notify_voice_write%=15;
			
    }
    if(param->length==0x12)
    {
        notify_data.notify_sensor_status=1;
        memcpy(&notify_data.notify_sensor[1],param->value,param->length);

    }

    UART_PRINTF("sk 11\r\n");
    test_usb_device();
    
#endif
	return (KE_MSG_CONSUMED);
}

//主机端接收到从设备发过来的Indication的回调函数
static int gattc_event_req_ind_handler(ke_msg_id_t const msgid,
                                       struct gattc_event_ind const *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id)
{
	uint8_t state = ke_state_get(dest_id);
#if 1
	UART_PRINTF("RECIVE value 2 =  \r\n");
	for(int i = 0; i< param->length; i++)
	{
		UART_PRINTF("%02x ",param->value[i]);
	}
	UART_PRINTF("\r\n");
#endif
	struct gattc_event_cfm *cfm  = KE_MSG_ALLOC(GATTC_EVENT_CFM, src_id, dest_id, gattc_event_cfm);
	cfm->handle = param->handle;
	ke_msg_send(cfm);

	return (KE_MSG_CONSUMED);
}
/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
/// Default State handlers definition
const struct ke_msg_handler sdp_default_state[] =
{
	{SDP_ENABLE_REQ,                (ke_msg_func_t)sdp_enable_req_handler},
	{SDP_READ_INFO_REQ,             (ke_msg_func_t)sdp_read_info_req_handler},
	{SDP_WRITE_VALUE_INFO_REQ,      (ke_msg_func_t)sdp_write_value_info_req_handler},
	{SDP_WRITE_NTF_CFG_REQ,         (ke_msg_func_t)sdp_write_ntf_cfg_req_handler},
	{GATTC_READ_IND,                (ke_msg_func_t)gattc_read_ind_handler},
	{GATTC_EVENT_IND,               (ke_msg_func_t)gattc_event_ind_handler},
	{GATTC_EVENT_REQ_IND,           (ke_msg_func_t)gattc_event_req_ind_handler},
	{GATTC_CMP_EVT,                 (ke_msg_func_t)gattc_cmp_evt_handler},
};
// Specifies the message handlers that are common to all states.
const struct ke_state_handler sdp_default_handler = KE_STATE_HANDLER(sdp_default_state);
#endif /* (BLE_BATT_CLIENT) */
/// @} BASCTASK
