/**
 ****************************************************************************************
 *
 * @file fee0s.c
 *
 * @brief fee0 Server Implementation.
 *
 * Copyright (C) beken 2009-2015
 *
 *
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_FEE0_SERVER)
#include "attm.h"
#include "fee0s.h"
#include "fee0s_task.h"
#include "prf_utils.h"
#include "prf.h"
#include "ke_mem.h"

#include "uart.h"



/*
 * FEE0 ATTRIBUTES DEFINITION
 ****************************************************************************************
 */
 
/// Full FEE0 Database Description - Used to add attributes into the database
const struct attm_desc fee0_att_db[FEE0S_IDX_NB] =
{
    // FEE0 Service Declaration
    [FEE0S_IDX_SVC]            =   {ATT_DECL_PRIMARY_SERVICE, PERM(RD, ENABLE), 0, 0},

	[FEE0S_IDX_FEE2_LVL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    //  Characteristic Value
    [FEE0S_IDX_FEE2_LVL_VAL]   =   {ATT_USER_SERVER_CHAR_FEE2,PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE), FEE0_FEE2_DATA_LEN *sizeof(uint8_t)},

	// fee1 Level Characteristic Declaration
	[FEE0S_IDX_FEE1_LVL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // fee1 Level Characteristic Value
    [FEE0S_IDX_FEE1_LVL_VAL]   =   {ATT_USER_SERVER_CHAR_FEE1, PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE), FEE0_FEE1_DATA_LEN * sizeof(uint8_t)},

	// fee1 Level Characteristic - Client Characteristic Configuration Descriptor
	[FEE0S_IDX_FEE1_LVL_NTF_CFG] = {ATT_DESC_CLIENT_CHAR_CFG,  PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},

	// fee1 Level Characteristic Declaration
	[FEE0S_IDX_FEE3_LVL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // fee1 Level Characteristic Value
    [FEE0S_IDX_FEE3_LVL_VAL]   =   {ATT_USER_SERVER_CHAR_FEE3, PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE), FEE0_FEE3_DATA_LEN * sizeof(uint8_t)},

	// fee1 Level Characteristic - Client Characteristic Configuration Descriptor
	[FEE0S_IDX_FEE3_LVL_NTF_CFG] = {ATT_DESC_CLIENT_CHAR_CFG,  PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
	
};/// Macro used to retrieve permission value from access and rights on attribute.


static uint8_t fee0s_init (struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task, uint8_t sec_lvl,  struct fee0s_db_cfg* params)
{
    uint16_t shdl;
    struct fee0s_env_tag* fee0s_env = NULL;
    // Status
    uint8_t status = GAP_ERR_NO_ERROR;
    
    //-------------------- allocate memory required for the profile  ---------------------
    fee0s_env = (struct fee0s_env_tag* ) ke_malloc(sizeof(struct fee0s_env_tag), KE_MEM_ATT_DB);
    memset(fee0s_env, 0 , sizeof(struct fee0s_env_tag));

    // Service content flag
    uint16_t cfg_flag = 0x1ff;

    // Save database configuration
    fee0s_env->features |= (params->features) ;

    shdl = *start_hdl;

    //Create FEE0 in the DB
    //------------------ create the attribute database for the profile -------------------
    status = attm_svc_create_db(&(shdl), ATT_USER_SERVER_FEE0, (uint8_t *)&cfg_flag,
            FEE0S_IDX_NB, NULL, env->task, &fee0_att_db[0],
            (sec_lvl & (PERM_MASK_SVC_DIS | PERM_MASK_SVC_AUTH | PERM_MASK_SVC_EKS)));

	//Set optional permissions
    if (status == GAP_ERR_NO_ERROR)
    {
        //Set optional permissions
        if((params->features & 0x01) == FEE0_FEE1_LVL_NTF_SUP)
        {
            // Battery Level characteristic value permissions
            uint16_t perm = PERM(RD, ENABLE) | PERM(NTF, ENABLE);

            attm_att_set_permission(shdl + FEE0S_IDX_FEE1_LVL_VAL, perm, 0);
        }
		if((params->features & 0x02) == FEE0_FEE3_LVL_NTF_SUP)
		{
			uint16_t perm = PERM(RD, ENABLE) | PERM(NTF, ENABLE);

            attm_att_set_permission(shdl + FEE0S_IDX_FEE3_LVL_VAL, perm, 0);
		}
    }
	
    //-------------------- Update profile task information  ---------------------
    if (status == ATT_ERR_NO_ERROR)
    {

        // allocate BASS required environment variable
        env->env = (prf_env_t*) fee0s_env;
        *start_hdl = shdl;
        fee0s_env->start_hdl = *start_hdl;
        fee0s_env->prf_env.app_task = app_task
                | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
        fee0s_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);

        // initialize environment variable
        env->id                     = TASK_ID_FEE0S;
        env->desc.idx_max           = FEE0S_IDX_MAX;
        env->desc.state             = fee0s_env->state;
        env->desc.default_handler   = &fee0s_default_handler;

        // service is ready, go into an Idle state
        ke_state_set(env->task, FEE0S_IDLE);
    }
    else if(fee0s_env != NULL)
    {
        ke_free(fee0s_env);
    }
     
    return (status);
}


static void fee0s_destroy(struct prf_task_env* env)
{
    struct fee0s_env_tag* fee0s_env = (struct fee0s_env_tag*) env->env;

    // clear on-going operation
    if(fee0s_env->operation != NULL)
    {
        ke_free(fee0s_env->operation);
    }

    // free profile environment variables
    env->env = NULL;
    ke_free(fee0s_env);
}

static void fee0s_create(struct prf_task_env* env, uint8_t conidx)
{
    struct fee0s_env_tag* fee0s_env = (struct fee0s_env_tag*) env->env;
    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);

    // force notification config to zero when peer device is connected
    fee0s_env->ntf_cfg[conidx] = 0;
}


static void fee0s_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{
    struct fee0s_env_tag* fee0s_env = (struct fee0s_env_tag*) env->env;

    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);
    // force notification config to zero when peer device is disconnected
    fee0s_env->ntf_cfg[conidx] = 0;
}

uint16_t g_hande = 0xff;
void fee0s_notify_fee1_lvl(struct fee0s_env_tag* fee0s_env, struct fee0s_fee1_level_upd_req const *param)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *fee1_lvl = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
            KE_BUILD_ID(TASK_GATTC, 0), prf_src_task_get(&(fee0s_env->prf_env),0),
            gattc_send_evt_cmd, sizeof(uint8_t)* (param->length));

    // Fill in the parameter structure
    fee1_lvl->operation = GATTC_NOTIFY;
    fee1_lvl->handle = fee0s_get_att_handle(FEE0S_IDX_FEE1_LVL_VAL);
	// save current to g_handle
	g_hande = fee1_lvl->handle;
    // pack measured value in database
    fee1_lvl->length = param->length;
	memcpy(&fee1_lvl->value[0],&param->fee1_level[0],param->length);
    // send notification to peer device
    ke_msg_send(fee1_lvl);
}


void fee0s_notify_fee3_lvl(struct fee0s_env_tag* fee0s_env, struct fee0s_fee3_level_upd_req const *param)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *fee3_lvl = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
            KE_BUILD_ID(TASK_GATTC, 0), prf_src_task_get(&(fee0s_env->prf_env),0),
            gattc_send_evt_cmd, sizeof(uint8_t)* (param->length));

    // Fill in the parameter structure
    fee3_lvl->operation = GATTC_NOTIFY;
    fee3_lvl->handle = fee0s_get_att_handle(FEE0S_IDX_FEE3_LVL_VAL);
	// save current to g_handle
	g_hande = fee3_lvl->handle;
    // pack measured value in database
    fee3_lvl->length = param->length;
	memcpy(&fee3_lvl->value[0],&param->fee3_level[0],param->length);
    // send notification to peer device
    ke_msg_send(fee3_lvl);
}



/// BASS Task interface required by profile manager
const struct prf_task_cbs fee0s_itf =
{
        (prf_init_fnct) fee0s_init,
        fee0s_destroy,
        fee0s_create,
        fee0s_cleanup,
};


const struct prf_task_cbs* fee0s_prf_itf_get(void)
{
   return &fee0s_itf;
}


uint16_t fee0s_get_att_handle( uint8_t att_idx)
{
		
    struct fee0s_env_tag *fee0s_env = PRF_ENV_GET(FEE0S, fee0s);
    uint16_t handle = ATT_INVALID_HDL;
   
    handle = fee0s_env->start_hdl;

    // increment index according to expected index
    if(att_idx < FEE0S_IDX_FEE1_LVL_NTF_CFG)
    {
        handle += att_idx;
    }
    // FEE1 notification
    else if((att_idx == FEE0S_IDX_FEE1_LVL_NTF_CFG) && (((fee0s_env->features ) & 0x01) == FEE0_FEE1_LVL_NTF_SUP))
    {
        handle += FEE0S_IDX_FEE1_LVL_NTF_CFG;			
    }		      
    else if(att_idx < FEE0S_IDX_FEE3_LVL_NTF_CFG)
   	{
		handle += att_idx;
	}
	// FEE3 notification
	else if((att_idx == FEE0S_IDX_FEE3_LVL_NTF_CFG) && (((fee0s_env->features ) & 0x02) == FEE0_FEE3_LVL_NTF_SUP))
	{
		handle += FEE0S_IDX_FEE3_LVL_NTF_CFG;
	}
	else
    {
        handle = ATT_INVALID_HDL;
    }
    

    return handle;
}

uint8_t fee0s_get_att_idx(uint16_t handle, uint8_t *att_idx)
{
    struct fee0s_env_tag* fee0s_env = PRF_ENV_GET(FEE0S, fee0s);
    uint16_t hdl_cursor1 = fee0s_env->start_hdl;
	uint16_t hdl_cursor2 = fee0s_env->start_hdl;
    uint8_t status = PRF_APP_ERROR;

    // Browse list of services
    // handle must be greater than current index 
    // check if it's a mandatory index
    if(handle <= (hdl_cursor1 + FEE0S_IDX_FEE1_LVL_VAL))
    {
        *att_idx = handle -hdl_cursor1;
        status = GAP_ERR_NO_ERROR;
        
    }
	else if(handle <= (hdl_cursor2 + FEE0S_IDX_FEE3_LVL_VAL))
	{
		*att_idx = handle -hdl_cursor2;
        status = GAP_ERR_NO_ERROR;
	}
	hdl_cursor1 += FEE0S_IDX_FEE1_LVL_VAL;
	hdl_cursor2 += FEE0S_IDX_FEE3_LVL_VAL;

    // check if it's a notify index
    if(((fee0s_env->features ) & 0x01) == FEE0_FEE1_LVL_NTF_SUP)
    {
        hdl_cursor1++;
        if(handle == hdl_cursor1)
        {
            *att_idx = FEE0S_IDX_FEE1_LVL_NTF_CFG;
            status = GAP_ERR_NO_ERROR;
        }
    }
    hdl_cursor1++;
	if(((fee0s_env->features ) & 0x02) == FEE0_FEE3_LVL_NTF_SUP)
    {
		hdl_cursor2++;
		if(handle == hdl_cursor2)
        {
            *att_idx = FEE0S_IDX_FEE3_LVL_NTF_CFG;
            status = GAP_ERR_NO_ERROR;
        }
    }
	hdl_cursor2++;
    
    return (status);
}


#endif // (BLE_fff0_SERVER)


 
