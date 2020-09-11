/**
 ****************************************************************************************
 *
 * @file sdp_service.c
 *
 * @brief Sdp Service Client implementation.
 *
 * Copyright (C) Beken 2009-2015
 *
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"
#if (BLE_SDP_CLIENT)
#include "ke_mem.h"
#include "gap.h"
#include "ke_env.h"
#include "sdp_service.h"
#include "sdp_service_task.h"
#include "co_math.h"
#include "uart.h"
#include <stdint.h>             // standard definition
#include <stdbool.h>            // standard boolean
#include "arch.h"               // architecture
#include "co_math.h"            // computation utilities
#include "ke_config.h"          // kernel configuration
#include "ke_env.h"             // kernel environment
#include "ke_mem.h"             // kernel memory
#include "RomCallFlash.h"


uint8_t sdp_need_dis_flag = 0;
uint8_t prf_nb = 0;
struct sdp_env_tag Sdp_env[SDP_NB_SERVICE_INSTANCES_MAX];
struct sdp_cnx_env cnx_env[SDP_NB_SERVICE_INSTANCES_MAX];

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void sdp_service_init_env(void)
{
    for(int i = 0; i < SDP_NB_SERVICE_INSTANCES_MAX ; i++)
    {
        Sdp_env[i].use_status = INVALID_STATUS;
        Sdp_env[i].db_env[0].cnx_env = &cnx_env[i];
    }
	sdp_need_dis_flag = 0;
}

static uint8_t sdp_init (struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task, uint8_t sec_lvl,  void* params)
{
    UART_PRINTF("%s \r\n",__func__);
    uint8_t idx;
    struct sdp_db_env *db_env = (struct sdp_db_env *)params;
	
    UART_PRINTF("db_env->prf_nb = %x\r\n",db_env->prf_nb);
    UART_PRINTF("db_env->sdp.chars_nb = %x\r\n",db_env->cnx_env->sdp.chars_nb);
	
    for(int i =0; i < db_env->cnx_env->sdp.chars_nb; i++)
    {
        UART_PRINTF("uuid = 0x%02x,char_hdl = 0x%02x,val_hdl = 0x%02x,prop = 0x%02x\r\n",db_env->cnx_env->sdp.chars[i].uuid,db_env->cnx_env->sdp.chars[i].char_hdl,db_env->cnx_env->sdp.chars[i].val_hdl,db_env->cnx_env->sdp.chars[i].prop);
    }
    for(int i =0; i < db_env->cnx_env->sdp.descs_nb; i++)
    {
        UART_PRINTF("uuid = 0x%02x,desc_hdl = 0x%02x\r\n",db_env->cnx_env->sdp.descs[i].uuid,db_env->cnx_env->sdp.descs[i].desc_hdl);
    }
	
    //-------------------- allocate memory required for the profile  ---------------------
    struct sdp_env_tag* sdp_env = &Sdp_env[db_env->prf_nb];
	
    // allocate  required environment variable
    env->env = (prf_env_t*) sdp_env;
    sdp_env->prf_env.app_task = app_task
                                | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
    sdp_env->prf_env.prf_task = env->task | PERM(PRF_MI, ENABLE);
    // initialize environment variable
    env->role                   = PRF_CLIENT;
    env->id                     = TASK_ID_SDP;
    env->desc.idx_max           = SDP_IDX_MAX;
    env->desc.state             = sdp_env->state;
    env->desc.default_handler   = &sdp_default_handler;
    for(idx = 0; idx < SDP_IDX_MAX ; idx++)
    {
        sdp_env->db_env[0].cnx_env = db_env->cnx_env; //init env
        
        // service is ready, go into an Idle state
        ke_state_set(KE_BUILD_ID(env->task, idx), SDP_IDLE);
    }
    if(0 != db_env->cnx_env->sdp.descs_nb)
    {
        for(idx = 0; idx < SDP_IDX_MAX ; idx++)
        {
            UART_PRINTF("prf_register_atthdl2gatt start_hdl = 0x%x,end_hdl = 0x%x\r\n",sdp_env->db_env[0].cnx_env->sdp.svc.shdl,sdp_env->db_env[0].cnx_env->sdp.svc.ehdl);
            prf_register_atthdl2gatt(&(sdp_env->prf_env), 0, &sdp_env->db_env[0].cnx_env->sdp.svc);
        }
    }
    UART_PRINTF("%s end!!\r\n",__func__);
	
    return GAP_ERR_NO_ERROR;
}

/**
 ****************************************************************************************
 * @brief Destruction of the BASC module - due to a reset for instance.
 * This function clean-up allocated memory (attribute database is destroyed by another
 * procedure)
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 ****************************************************************************************
 */
static void sdp_destroy(struct prf_task_env* env)
{
    UART_PRINTF("sdp_destroy START env->id = 0x%x,role = %x\r\n",env->id,env->role);
		
	if(sdp_need_dis_flag == 1)
	{
		UART_PRINTF("no need sdp_destroy \r\n");
		return;
	}
    uint8_t idx;
    struct sdp_env_tag* sdp_env = (struct sdp_env_tag*) env->env;
    UART_PRINTF("use_status = 0x%x\r\n",sdp_env->use_status);
    sdp_env->use_status = INVALID_STATUS;
    // cleanup environment variable for each task instances
    for(idx = 0; idx < SDP_IDX_MAX ; idx++)
    {
        if(sdp_env->db_env[idx].cnx_env != NULL)
        {
            if(sdp_env->operation != NULL)
            {
                sdp_env->operation = NULL;
                UART_PRINTF("sdp_destroy sdp_env->operation = NULL\r\n");
            }
        }
    }
    // free profile environment variables
    UART_PRINTF("sdp_destroy end\r\n");
}
/**
 ****************************************************************************************
 * @brief Handles Connection creation
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 * @param[in]        conidx     Connection index
 ****************************************************************************************
 */
static void sdp_create(struct prf_task_env* env, uint8_t conidx)
{
    /* Put BAS Client in Idle state */
    ke_state_set(KE_BUILD_ID(env->task, conidx), SDP_IDLE);
}
/**
 ****************************************************************************************
 * @brief Handles Disconnection
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 * @param[in]        conidx     Connection index
 * @param[in]        reason     Detach reason
 ****************************************************************************************
 */
static void sdp_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{

    UART_PRINTF("sdp_cleanup env->id = 0x%x,role = %x,conidx = %x reason = 0x%x\r\n",env->id,env->role,conidx,reason);
	if(sdp_need_dis_flag == 1)
	{
		UART_PRINTF("no need sdp_cleanup \r\n");
		return;
	}
    struct sdp_env_tag* sdp_env = (struct sdp_env_tag*) env->env;
    UART_PRINTF("use_status = 0x%x\r\n",sdp_env->use_status);
    sdp_env->use_status = INVALID_STATUS;
    uint8_t idx;
    for(idx = 0; idx < SDP_IDX_MAX ; idx++)
    {
        if(0 != sdp_env->db_env[idx].cnx_env->sdp.descs_nb)
        {
            prf_unregister_atthdl2gatt(&(sdp_env->prf_env), idx, &sdp_env->db_env[idx].cnx_env->sdp.svc);
            sdp_env->db_env[idx].cnx_env->sdp.descs_nb = 0;
        }
    }
    // clean-up environment variable allocated for task instance
    if(sdp_env->db_env[conidx].cnx_env != NULL)
    {
        if(sdp_env->operation != NULL)
        {
            struct ke_msg *msg = ke_msg2param(sdp_env->operation);
            UART_PRINTF("operation = 0x%08x\r\n",(uint32_t)sdp_env->operation);
            UART_PRINTF("msgid = 0x%02x,dest_id = 0x%02x,src_id = 0x%02x\r\n",msg->id,msg->dest_id,msg->src_id);
					
            ke_free(sdp_env->operation);
					
        }
        if(sdp_env->db_env[conidx].cnx_env->sdp.chars_nb != 0)
        {
            sdp_env->db_env[conidx].cnx_env->sdp.chars_nb = 0;
            UART_PRINTF("000042\r\n");
        }
        if(sdp_env->db_env[conidx].cnx_env->sdp.descs_nb != 0)
        {
            sdp_env->db_env[conidx].cnx_env->sdp.descs_nb = 0;
            UART_PRINTF("000044\r\n");
        }
        UART_PRINTF("00006\r\n");
    }
    prf_nb = 0;
    UART_PRINTF("sdp_cleanup end\r\n");
    /* Put BAS Client in Free state */
    ke_state_set(KE_BUILD_ID(env->task, conidx), SDP_FREE);
}

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
/// BASC Task interface required by profile manager
const struct prf_task_cbs sdp_itf =
{
    sdp_init,
    sdp_destroy,
    sdp_create,
    sdp_cleanup,
};
/*
 * GLOBAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */
const struct prf_task_cbs* sdp_prf_itf_get(void)
{
    return &sdp_itf;
}
void sdp_discover_all_service(void)
{
    UART_PRINTF("sdp_discover_all_service\r\n");
    sdp_service_init_env();
    struct gattc_sdp_svc_disc_cmd * svc_req = KE_MSG_ALLOC_DYN(GATTC_SDP_SVC_DISC_CMD,
					            TASK_GATTC, TASK_APP,
					            gattc_sdp_svc_disc_cmd, ATT_UUID_16_LEN);
    //gatt request type: by UUID
    svc_req->operation         = GATTC_SDP_DISC_SVC_ALL;
    //start handle;
    svc_req->start_hdl        = ATT_1ST_REQ_START_HDL;
    //end handle
    svc_req->end_hdl          = ATT_1ST_REQ_END_HDL;
    // UUID search
    svc_req->uuid_len = ATT_UUID_16_LEN;
	
    //set the first two bytes to the value array, LSB to MSB:Health Thermometer Service UUID first
    co_write16p(&(svc_req->uuid[0]), 0x0000);
    //send the message to GATT, which will send back the response when it gets it
    ke_msg_send(svc_req);
}

extern struct prf_env_tag prf_env;
void sdp_extract_svc_info(struct gattc_sdp_svc_ind const *ind)
{
    UART_PRINTF("*************************************************************************************************\r\n");
    uint8_t sdp_env_idx;
    uint8_t add_svc_flag = false;
    UART_PRINTF("prf_env.prf_used  = %d\r\n",prf_env.prf_used);
    if(prf_env.prf_used == SDP_NB_SERVICE_INSTANCES_MAX )
    {
        UART_PRINTF("not have prf_used = %d\r\n",BLE_NB_PROFILES);
        return;
    }
    for(int i = 0; i < SDP_NB_SERVICE_INSTANCES_MAX; i ++)
    {
        if(Sdp_env[i].use_status == INVALID_STATUS)
        {
            sdp_env_idx = i;
            add_svc_flag = true;
            Sdp_env[sdp_env_idx].use_status = VALID_STATUS;
            UART_PRINTF("found sdp_env_idx = %d,use_status = 0x%x\r\n",sdp_env_idx,Sdp_env[sdp_env_idx].use_status);
            UART_PRINTF("p_use_status = 0x%x\r\n",&Sdp_env[sdp_env_idx].use_status);
            break;
        }
    }
    UART_PRINTF("%s\r\n",__func__);
    UART_PRINTF("service uuid len = 0x%x,uuid = 0x",ind->uuid_len);
    uint16_t uuid ;
    uint16_t length;
    if(ind->uuid_len == 0x10)
    {
        uuid = (ind->uuid[12] | (ind->uuid[13] << 8));
    }
    else
    {
        uuid = (ind->uuid[0] | (ind->uuid[1] << 8));
    }
    for(int i = 0; i < ind->uuid_len; i++)
    {
        UART_PRINTF("%02x",ind->uuid[i]);
    }
    UART_PRINTF("\r\n");
    UART_PRINTF("uuid16 = %02x\r\n",uuid);
    length =  ind->end_hdl - ind->start_hdl;
    UART_PRINTF("start_hdl  = 0x%02x\r\n",ind->start_hdl);
    UART_PRINTF("end_hdl  = 0x%02x\r\n",ind->end_hdl);
    UART_PRINTF("length = %d\r\n",length);
    //Counters
    uint8_t svc_char,svc_char_cnt;
    uint8_t svc_desc,svc_desc_cnt;
    uint8_t fnd_att;
    /**  search svc_char_cnt and desc_cnt for alloc memory **/
    svc_char_cnt = 0;
    svc_desc_cnt = 0;
    for (fnd_att=0; fnd_att< (ind->end_hdl - ind->start_hdl); fnd_att++)
    {
        if(ind->info[fnd_att].att_type == GATTC_SDP_ATT_CHAR)
        {
            uint16_t val_hdl  = ind->info[fnd_att].att_char.handle;
            // check that value handle is in a valid range
            if((val_hdl <= ind->end_hdl) && (val_hdl > (ind->start_hdl + fnd_att)))
            {
                //Look over requested characteristics
                for (svc_char=0; svc_char < ind->end_hdl -ind->start_hdl ; svc_char++)
                {
                    svc_char_cnt = (svc_char_cnt + 1) % SDP_NB_CHAR_INSTANCES_MAX;
                    // find end of characteristic handle and discover descriptors
                    do
                    {
                        fnd_att++;
                        // found a descriptor
                        if(ind->info[fnd_att].att_type == GATTC_SDP_ATT_DESC)
                        {
                            //Retrieve characteristic descriptor handle using UUID
                            for(svc_desc = 0; svc_desc<ind->end_hdl - ind->start_hdl; svc_desc++)
                            {
                                // check if it's expected descriptor
                                if ( svc_desc == svc_char)
                                {
                                    svc_desc_cnt = (svc_desc_cnt + 1 ) % SDP_NB_DESC_INSTANCES_MAX;
                                }
                            }
                        }
                    }
                    while(((ind->start_hdl+ 1 + fnd_att) <= ind->end_hdl)
                            && (ind->info[fnd_att].att_type != GATTC_SDP_ATT_CHAR)
                            && (ind->info[fnd_att].att_type != GATTC_SDP_INC_SVC));
                    // return to previous valid value
                    fnd_att--;
                    // search next characteristic
                    break;
                }
            }
        }
    }
    /**************************end search ************************************/
    struct sdp_db_env *env = NULL ;
    struct prf_char_inf *chars = NULL;
    struct prf_char_desc_inf *descs = NULL;
    if(true == add_svc_flag)
    {
        env = Sdp_env[sdp_env_idx].db_env;
        env->prf_nb = sdp_env_idx;
        chars = env[0].cnx_env->sdp.chars;
        descs = env[0].cnx_env->sdp.descs;
        env[0].cnx_env->sdp.svc.shdl = ind->start_hdl;
        env[0].cnx_env->sdp.svc.ehdl = ind->end_hdl;
        svc_char_cnt = 0;
        svc_desc_cnt = 0;
        env[0].cnx_env->sdp.chars_nb = 0;
        env[0].cnx_env->sdp.descs_nb = 0;
        for (fnd_att=0; fnd_att< (ind->end_hdl - ind->start_hdl); fnd_att++)
        {
            if(ind->info[fnd_att].att_type == GATTC_SDP_ATT_CHAR)
            {
                uint16_t char_hdl = ind->start_hdl+ 1 + fnd_att;
                uint16_t val_hdl  = ind->info[fnd_att].att_char.handle;
                uint8_t  val_prop = ind->info[fnd_att].att_char.prop;
                uint8_t  char_idx = fnd_att;
                // check that value handle is in a valid range
                if((val_hdl <= ind->end_hdl) && (val_hdl > (ind->start_hdl + fnd_att)))
                {
                    // retrieve value index
                    uint8_t val_idx = (val_hdl - ind->start_hdl - 1);
                    //Look over requested characteristics
                    for (svc_char=0; svc_char < ind->end_hdl -ind->start_hdl ; svc_char++)
                    {
                        UART_PRINTF("char uuid len = 0x%x ,",ind->info[val_idx].att.uuid_len);
                        UART_PRINTF("char uuid = 0x%04x \r\n",attm_convert_to16((uint8_t *)ind->info[val_idx].att.uuid,ind->info[val_idx].att.uuid_len));
                        for(int i = 0; i < ind->info[val_idx].att.uuid_len; i++)
                        {
                            UART_PRINTF("%02x",ind->info[val_idx].att.uuid[i]);
                        }
                        UART_PRINTF("\r\n");
                        UART_PRINTF("char_hdl = 0x%x,val_hdl = 0x%x,val_prop = 0x%x,svc_char = 0x%x ,val_idx = 0x%x\r\n",char_hdl,val_hdl,val_prop,svc_char,val_idx);
                        //Save properties and handles
                        chars[svc_char_cnt].uuid           = attm_convert_to16((uint8_t *)ind->info[val_idx].att.uuid,ind->info[val_idx].att.uuid_len);
                        chars[svc_char_cnt].char_hdl       = char_hdl;
                        chars[svc_char_cnt].val_hdl        = val_hdl;
                        chars[svc_char_cnt].prop           = val_prop;
                        svc_char_cnt++;
                        env[0].cnx_env->sdp.chars_nb = svc_char_cnt;
                        // find end of characteristic handle and discover descriptors
                        do
                        {
                            fnd_att++;
                            // found a descriptor
                            if(ind->info[fnd_att].att_type == GATTC_SDP_ATT_DESC)
                            {
                                //Retrieve characteristic descriptor handle using UUID
                                for(svc_desc = 0; svc_desc<ind->end_hdl - ind->start_hdl; svc_desc++)
                                {
                                    // check if it's expected descriptor
                                    if ( svc_desc == svc_char)
                                    {
                                        UART_PRINTF("svc_desc = 0x%x,desc uuid = 0x%02x ",svc_desc,attm_convert_to16((uint8_t*)ind->info[fnd_att].att.uuid,ind->info[fnd_att].att.uuid_len));
                                        descs[svc_desc_cnt].uuid = attm_convert_to16((uint8_t*)&(ind->info[fnd_att].att.uuid[0]),ind->info[fnd_att].att.uuid_len);
                                        descs[svc_desc_cnt].desc_hdl = ind->start_hdl + 1 + fnd_att;
                                        descs[svc_desc_cnt].char_code =  svc_char_cnt - 1;
                                        UART_PRINTF("desc_hdl = 0x%x desc_char_code = 0x%x,val_idx = 0x%x\r\n",ind->start_hdl + 1 + fnd_att,svc_char_cnt - 1,val_idx);
                                        // search for next descriptor
                                        svc_desc_cnt++;
                                        env[0].cnx_env->sdp.descs_nb++;
                                        //break;
                                    }
                                }
                            }
                        } while(((ind->start_hdl+ 1 + fnd_att) <= ind->end_hdl)
                                && (ind->info[fnd_att].att_type != GATTC_SDP_ATT_CHAR)
                                && (ind->info[fnd_att].att_type != GATTC_SDP_INC_SVC));
                        // return to previous valid value
                        fnd_att--;
                        // previous handle was end of the characteristic
                        chars[svc_char_cnt].char_ehdl_off    = fnd_att - char_idx;
                        // search next characteristic
                        break;
                    }
                }
            }
        }

        UART_PRINTF("%s  end\r\n",__func__);
        sdp_add_profiles(Sdp_env[sdp_env_idx].db_env);
    }
}

void sdp_add_profiles(struct sdp_db_env *db_env)
{
    UART_PRINTF("%s \r\n",__func__);
    // return;
    struct sdp_db_env *env;
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                            TASK_GAPM, TASK_APP,
                                            gapm_profile_task_add_cmd, sizeof(struct sdp_db_env));
    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = 0;//PERM(SVC_AUTH, ENABLE);
    req->prf_task_id = TASK_ID_SDP + prf_nb++;
    req->app_task = TASK_APP;
    req->start_hdl = 0; 
    env = (struct sdp_db_env *)req->param ;
    env->cnx_env = db_env->cnx_env;
    env->prf_nb = db_env->prf_nb;
    UART_PRINTF("env->cnx_env->sdp.chars_nb = %d\r\n",env->cnx_env->sdp.chars_nb);
    ke_msg_send(req);
}

void sdp_enable_rsp_send(struct sdp_env_tag *sdp_env, uint8_t conidx, uint8_t status)
{
    // Send APP the details of the discovered attributes on
    struct sdp_enable_rsp * rsp = KE_MSG_ALLOC(SDP_ENABLE_RSP,
                                  prf_dst_task_get(&(sdp_env->prf_env) ,conidx),
                                  prf_src_task_get(&(sdp_env->prf_env) ,conidx),
                                  sdp_enable_rsp);
    rsp->status = status;
    ke_msg_send(rsp);
}

#endif /* (BLE_CLIENT) */
/// @} SDP
