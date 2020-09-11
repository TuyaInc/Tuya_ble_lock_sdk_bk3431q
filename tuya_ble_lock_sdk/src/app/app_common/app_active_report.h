/**
****************************************************************************
* @file      app_active_report.h
* @brief     app_active_report
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __APP_ACTIVE_REPORT_H__
#define __APP_ACTIVE_REPORT_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "app_common.h"

/*********************************************************************
 * CONSTANTS
 */
#define APP_ACTIVE_REPORT_ADV_INTERVAL 100 //ms

typedef enum
{
    ACTICE_REPORT_STOP_STATE_OUTTIME = 0x00,
    ACTICE_REPORT_STOP_STATE_BONDING,
} app_active_report_stop_state_t;

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
void app_active_report_start(void);
void app_active_report_stop(uint8_t state);
void app_active_report_finished_and_disconnect(void);
void app_active_report_finished_and_disconnect_handler(void);


#ifdef __cplusplus
}
#endif

#endif //__APP_ACTIVE_REPORT_H__
