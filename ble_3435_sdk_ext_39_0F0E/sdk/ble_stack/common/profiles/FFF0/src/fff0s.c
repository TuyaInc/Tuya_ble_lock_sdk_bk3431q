#include "rwip_config.h"
#if (BLE_FFF0_SERVER)
#include "attm.h"
#include "fff0s.h"
#include "fff0s_task.h"
#include "prf_utils.h"
#include "prf.h"
#include "ke_mem.h"
#include "uart.h"




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
/// FFF0 特征值列表
const struct attm_desc fff0_att_db[BK_SVC_IDX_MAX] =
{
    //服务定义 2800
    [BK_SVC_IDX]        = {ATT_DECL_PRIMARY_SERVICE, PERM(RD, ENABLE), 0, 0},

    //特征值1定义 write 2803
	[BK_CHARW_IDX]      = {ATT_DECL_CHARACTERISTIC,  PERM(RD, ENABLE), 0, 0},
    [BK_CHARW_VAL_IDX]  = {BK_CHARW_UUID,            PERM(WRITE_COMMAND, ENABLE)|PERM(WRITE_REQ, ENABLE), PERM(RI, ENABLE), FFF0_FFF2_DATA_LEN *sizeof(uint8_t)},

	//特征值2定义 notify 2803
	[BK_CHARN_IDX]      = {ATT_DECL_CHARACTERISTIC,  PERM(RD, ENABLE), 0, 0},
    [BK_CHARN_VAL_IDX]  = {BK_CHARN_UUID,            PERM(NTF, ENABLE),                                   PERM(RI, ENABLE), FFF0_FFF1_DATA_LEN * sizeof(uint8_t)},
	//notify cfg 2902
	[BK_CHARN_CFG_IDX]  = {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
};




/*********************************************************
FN: 
*/
static uint8_t fff0s_init (struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task, uint8_t sec_lvl,  struct fff0s_db_cfg* params)
{
    uint16_t shdl;
    struct fff0s_env_tag* fff0s_env = NULL;
    // Status
    uint8_t status = GAP_ERR_NO_ERROR;
    
    //-------------------- allocate memory required for the profile  ---------------------
    fff0s_env = (struct fff0s_env_tag* ) ke_malloc(sizeof(struct fff0s_env_tag), KE_MEM_ATT_DB);
    memset(fff0s_env, 0 , sizeof(struct fff0s_env_tag));

    // Save database configuration
    fff0s_env->features |= (params->features) ;
   
    shdl = *start_hdl;

    // Service content flag
    uint8_t cfg_flag = BK_SVC_CFG_FLAG;

    //Create FFF0 in the DB
    //------------------ create the attribute database for the profile -------------------
    status = attm_svc_create_db(&(shdl), BK_SVC_UUID, (uint8_t *)&cfg_flag,
            BK_SVC_IDX_MAX, NULL, env->task, &fff0_att_db[0],
            (sec_lvl & (PERM_MASK_SVC_DIS | PERM_MASK_SVC_AUTH | PERM_MASK_SVC_EKS)));


    //-------------------- Update profile task information  ---------------------
    if (status == ATT_ERR_NO_ERROR)
    {

        // allocate BASS required environment variable
        env->env = (prf_env_t*) fff0s_env;
        *start_hdl = shdl;
        fff0s_env->start_hdl = *start_hdl;
        fff0s_env->prf_env.app_task = app_task
                | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
        fff0s_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);

        // initialize environment variable
        env->id                     = TASK_ID_FFF0S;
        env->desc.idx_max           = FFF0S_IDX_MAX;
        env->desc.state             = fff0s_env->state;
        env->desc.default_handler   = &fff0s_default_handler;

        // service is ready, go into an Idle state
        ke_state_set(env->task, FFF0S_IDLE);
    }
    else if(fff0s_env != NULL)
    {
        ke_free(fff0s_env);
    }
     
    return (status);
}

/*********************************************************
FN: 
*/
static void fff0s_destroy(struct prf_task_env* env)
{
    struct fff0s_env_tag* fff0s_env = (struct fff0s_env_tag*) env->env;

    // clear on-going operation
    if(fff0s_env->operation != NULL)
    {
        ke_free(fff0s_env->operation);
    }

    // free profile environment variables
    env->env = NULL;
    ke_free(fff0s_env);
}

/*********************************************************
FN: 
*/
static void fff0s_create(struct prf_task_env* env, uint8_t conidx)
{
    struct fff0s_env_tag* fff0s_env = (struct fff0s_env_tag*) env->env;
    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);

    // force notification config to zero when peer device is connected
    fff0s_env->ntf_cfg[conidx] = 0;
}

/*********************************************************
FN: 
*/
static void fff0s_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{
    struct fff0s_env_tag* fff0s_env = (struct fff0s_env_tag*) env->env;

    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);
    // force notification config to zero when peer device is disconnected
    fff0s_env->ntf_cfg[conidx] = 0;
}

/*********************************************************
FN: 
*/
void fff0s_notify_fff1_lvl(struct fff0s_env_tag* fff0s_env, struct fff0s_fff1_level_upd_req const *param)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *fff1_lvl = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
            KE_BUILD_ID(TASK_GATTC, 0), prf_src_task_get(&(fff0s_env->prf_env),0),
            gattc_send_evt_cmd, sizeof(uint8_t)* (param->length));

    // Fill in the parameter structure
    fff1_lvl->operation = GATTC_NOTIFY;
    fff1_lvl->handle = fff0s_get_att_handle(BK_CHARN_VAL_IDX);
    // pack measured value in database
    fff1_lvl->length = param->length;
  	//fff1_lvl->value[0] = fff0s_env->fff1_lvl[0];
	memcpy(&fff1_lvl->value[0],&param->fff1_level[0],param->length);
    // send notification to peer device
    ke_msg_send(fff1_lvl);
}

/// BASS Task interface required by profile manager
const struct prf_task_cbs fff0s_itf =
{
        (prf_init_fnct) fff0s_init,
        fff0s_destroy,
        fff0s_create,
        fff0s_cleanup,
};

const struct prf_task_cbs* fff0s_prf_itf_get(void)
{
   return &fff0s_itf;
}

/*********************************************************
FN: 
*/
uint16_t fff0s_get_att_handle( uint8_t att_idx)
{
		
    struct fff0s_env_tag *fff0s_env = PRF_ENV_GET(FFF0S, fff0s);
    uint16_t handle = ATT_INVALID_HDL;
   
    handle = fff0s_env->start_hdl;

    // increment index according to expected index
    if(att_idx < BK_CHARN_CFG_IDX)
    {
        handle += att_idx;
    }
    // FFF1 notification
    else if((att_idx == BK_CHARN_CFG_IDX) && (((fff0s_env->features ) & 0x01) == FFF0_FFF1_LVL_NTF_SUP))
    {
        handle += BK_CHARN_CFG_IDX;			
    }		      
    else
    {
        handle = ATT_INVALID_HDL;
    }
    return handle;
}

/*********************************************************
FN: 
*/
uint8_t fff0s_get_att_idx(uint16_t handle, uint8_t *att_idx)
{
    struct fff0s_env_tag* fff0s_env = PRF_ENV_GET(FFF0S, fff0s);
    uint16_t hdl_cursor = fff0s_env->start_hdl;
    uint8_t status = PRF_APP_ERROR;

    // Browse list of services
    // handle must be greater than current index 
    // check if it's a mandatory index
    if(handle <= (hdl_cursor + BK_CHARN_VAL_IDX))
    {
        *att_idx = handle -hdl_cursor;
        status = GAP_ERR_NO_ERROR;
        
    }
    hdl_cursor += BK_CHARN_VAL_IDX;

    // check if it's a notify index
    if(((fff0s_env->features ) & 0x01) == FFF0_FFF1_LVL_NTF_SUP)
    {
        hdl_cursor++;
        if(handle == hdl_cursor)
        {
            *att_idx = BK_CHARN_CFG_IDX;
            status = GAP_ERR_NO_ERROR;
        }
    }
    hdl_cursor++;
    
    return (status);
}


#endif // (BLE_fff0_SERVER)


 
