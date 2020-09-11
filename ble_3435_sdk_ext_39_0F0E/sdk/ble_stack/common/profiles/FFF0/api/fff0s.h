/**
****************************************************************************
* @file      fff0s.h
* @brief     fff0s
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef _FFF0S_H_
#define _FFF0S_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "rwip_config.h"
#include "rwprf_config.h"
#if (BLE_FFF0_SERVER)
#include "fff0s_task.h"
#include "atts.h"
#include "prf_types.h"
#include "prf.h"

/*********************************************************************
 * CONSTANTS
 */
#define BK_SVC_CFG_FLAG       (0x3F)

enum
{		
    BK_SVC_UUID     = ATT_UUID_16(0x1910),
    BK_CHARW_UUID   = ATT_UUID_16(0x2b11),
    BK_CHARN_UUID   = ATT_UUID_16(0x2b10),
};

/// Battery Service Attributes Indexes
enum
{
	BK_SVC_IDX,

	BK_CHARW_IDX,
	BK_CHARW_VAL_IDX,

	BK_CHARN_IDX,
	BK_CHARN_VAL_IDX,
	BK_CHARN_CFG_IDX,

	BK_SVC_IDX_MAX,
};

/*********************************************************************
 * STRUCT
 */
/// FFF0 'Profile' Server environment variable
struct fff0s_env_tag
{
    /// profile environment
    prf_env_t prf_env;
   
    /// On-going operation
    struct ke_msg * operation;
    /// FFF0 Services Start Handle
    uint16_t start_hdl;
    /// Level of the FFF1
    uint8_t fff1_lvl[FFF0_FFF1_DATA_LEN];
	
	uint8_t fff2_value[FFF0_FFF2_DATA_LEN];
    /// BASS task state
    ke_state_t state[FFF0S_IDX_MAX];
    /// Notification configuration of peer devices.
    uint8_t ntf_cfg[BLE_CONNECTION_MAX];
    /// Database features
    uint8_t features;
};

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
const struct prf_task_cbs* fff0s_prf_itf_get(void);
uint16_t fff0s_get_att_handle(uint8_t att_idx);
uint8_t  fff0s_get_att_idx(uint16_t handle, uint8_t *att_idx);
void fff0s_notify_fff1_lvl(struct fff0s_env_tag* fff0s_env, struct fff0s_fff1_level_upd_req const *param);


#endif /* #if (BLE_FFF0_SERVER) */

#ifdef __cplusplus
}
#endif

#endif //__BK_TEST_H__


