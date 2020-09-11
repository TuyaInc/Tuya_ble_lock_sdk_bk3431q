/**
 ****************************************************************************************
 *
 * @file fee0s_task.h
 *
 * @brief Header file - Battery Service Server Role Task.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */


#ifndef _FEE0S_TASK_H_
#define _FEE0S_TASK_H_


#include "rwprf_config.h"
#if (BLE_FEE0_SERVER)
#include <stdint.h>
#include "rwip_task.h" // Task definitions
/*
 * DEFINES
 ****************************************************************************************
 */

///Maximum number of FFF0 Server task instances
#define FEE0S_IDX_MAX     0x01
///Maximal number of FFF0 that can be added in the DB

#define  FEE0_FEE1_DATA_LEN  128
#define  FEE0_FEE2_DATA_LEN  128
#define  FEE0_FEE3_DATA_LEN  128
/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Possible states of the FFF0S task
enum fee0s_state
{
    /// Idle state
    FEE0S_IDLE,
    /// busy state
    FEE0S_BUSY,
    /// Number of defined states.
    FEE0S_STATE_MAX
};

/// Messages for FFF0 Server
enum fee0s_msg_id
{
    /// Start the FFF0 Server - at connection used to restore bond data
	FEE0S_CREATE_DB_REQ   = TASK_FIRST_MSG(TASK_ID_FEE0S),
	
    /// FFF1 Level Value Update Request
    FEE0S_FEE1_LEVEL_UPD_REQ,
    /// Inform APP if FFF1 Level value has been notified or not
    FEE0S_FEE1_LEVEL_UPD_RSP,
    /// Inform APP that FFF1 Level Notification Configuration has been changed - use to update bond data
    FEE0S_FEE1_LEVEL_NTF_CFG_IND,

	/// FFF3 Level Value Update Request
    FEE0S_FEE3_LEVEL_UPD_REQ,
    /// Inform APP if FFF1 Level value has been notified or not
    FEE0S_FEE3_LEVEL_UPD_RSP,

	FEE0S_FEE3_LEVEL_NTF_CFG_IND,
	
	FEE0S_FEE2_WRITER_REQ_IND,

	FEE0S_FEE1_LEVEL_PERIOD_NTF,

	FEE0S_FEE3_LEVEL_PERIOD_NTF
};

/// Features Flag Masks
enum fee0s_features
{
    /// FFF1 Level Characteristic doesn't support notifications
    FEE0_FEE1_LVL_NTF_NOT_SUP,
    /// FFF1 Level Characteristic support notifications
    FEE0_FEE1_LVL_NTF_SUP,
    /// FFF3 Level Characteristic support notifications
    FEE0_FEE3_LVL_NTF_SUP,
};


/*
 * APIs Structures
 ****************************************************************************************
 */

/// Parameters for the database creation
struct fee0s_db_cfg
{
    /// Number of FFF0 to add
    uint8_t fee0_nb;
    /// Features of each FFF0 instance
    uint8_t features;
   };

/// Parameters of the @ref FFF0S_ENABLE_REQ message
struct fee0s_enable_req
{
    /// connection index
    uint8_t  conidx;
    /// Notification Configuration
    uint8_t  ntf_cfg;
    /// Old FFF1 Level used to decide if notification should be triggered
    uint8_t  old_fee1_lvl;
};


///Parameters of the @ref FFF0S_BATT_LEVEL_UPD_REQ message
struct fee0s_fee1_level_upd_req
{
    /// BAS instance
    uint8_t conidx;
	
	uint8_t length;
    /// fff1 Level
    uint8_t fee1_level[FEE0_FEE1_DATA_LEN];
};

///Parameters of the @ref FFF0S_BATT_LEVEL_UPD_REQ message
struct fee0s_fee3_level_upd_req
{
    /// BAS instance
    uint8_t conidx;
	
	uint8_t length;
    /// fff1 Level
    uint8_t fee3_level[FEE0_FEE3_DATA_LEN];
};

///Parameters of the @ref FFF0S_FFF1_LEVEL_UPD_RSP message
struct fee0s_fee1_level_upd_rsp
{
    ///status
    uint8_t status;
};

///Parameters of the @ref BASS_BATT_LEVEL_NTF_CFG_IND message
struct fee0s_fee1_level_ntf_cfg_ind
{
    /// connection index
    uint8_t  conidx;
    ///Notification Configuration
    uint8_t  ntf_cfg;
};

///Parameters of the @ref FFF0S_FFF1_LEVEL_UPD_RSP message
struct fee0s_fee3_level_upd_rsp
{
    ///status
    uint8_t status;
};

///Parameters of the @ref BASS_BATT_LEVEL_NTF_CFG_IND message
struct fee0s_fee3_level_ntf_cfg_ind
{
    /// connection index
    uint8_t  conidx;
    ///Notification Configuration
    uint8_t  ntf_cfg;
};

/// Parameters of the @ref FFF0S_FFF2_WRITER_REQ_IND message
struct fee0s_fee2_writer_ind
{
    /// Alert level
    uint8_t fee2_value[FEE0_FEE2_DATA_LEN];
	
	uint8_t length;
    /// Connection index
    uint8_t conidx;
};


/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler fee0s_default_handler;
#endif // BLE_FFF0_SERVER


#endif /* _FFF0S_TASK_H_ */

