/**
****************************************************************************
* @file      app_common.h
* @brief     app_common
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
//c_lib
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"

//app common
#include "app_port.h"
#include "app_flash.h"
#include "app_ota.h"
#include "app_active_report.h"
#include "app_test.h"
//app lock
#include "lock_dp_parser.h"
#include "lock_dp_report.h"
#include "lock_timer.h"
#include "lock_hard.h"
#include "lock_dynamic_pwd.h"
#include "lock_offline_pwd.h"
#include "lock_test.h"

/*********************************************************************
 * CONSTANTS
 */
typedef enum
{
    APP_EVT_START_REG_CARD = 0,
    APP_EVT_CANCEL_REG_CARD,
    APP_EVT_START_REG_FINGER,
    APP_EVT_CANCEL_REG_FINGER,
    APP_EVT_START_REG_FACE,
    APP_EVT_CANCEL_REG_FACE,
    APP_EVT_BLE_GAP_EVT_CONNECTED,
    APP_EVT_BLE_GAP_EVT_DISCONNECTED,
    APP_EVT_TIMER_0,
    APP_EVT_TIMER_1,
    APP_EVT_TIMER_2,
    APP_EVT_TIMER_3,
    APP_EVT_TIMER_4,
    APP_EVT_TIMER_5,
    APP_EVT_TIMER_6,
    APP_EVT_TIMER_7,
    APP_EVT_TIMER_8,
    APP_EVT_TIMER_9,
    APP_EVT_TIMER_10,
    APP_EVT_TIMER_11,
    APP_EVT_TIMER_12,
    APP_EVT_TIMER_13,
    APP_EVT_TIMER_14,
    APP_EVT_TIMER_15,
    APP_EVT_TIMER_16,
    APP_EVT_TIMER_17,
    APP_EVT_TIMER_18,
    APP_EVT_TIMER_19,
} app_evt_t;

/*********************************************************************
 * STRUCT
 */
typedef struct
{
    uint32_t len;
    uint8_t  value[];
} app_evt_data_t;

typedef struct 
{
    app_evt_t evt;
    union 
    {
        app_evt_data_t data;
    };
} app_evt_param_t;


/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
void app_common_init(void);
void app_common_evt_send(app_evt_param_t* param);
void app_common_evt_send_only_evt(app_evt_t evt);
void app_common_evt_send_with_data(app_evt_t evt, void* buf, uint32_t len);


#ifdef __cplusplus
}
#endif

#endif //__APP_COMMON_H__
