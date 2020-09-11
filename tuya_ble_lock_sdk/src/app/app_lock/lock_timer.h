/**
****************************************************************************
* @file      lock_timer.h
* @brief     lock_timer
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __LOCK_TIMER_H__
#define __LOCK_TIMER_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "lock_common.h"

/*********************************************************************
 * CONSTANTS
 */
typedef enum
{
    LOCK_TIMER_CONN_PARAM_UPDATE = 0x00,
    LOCK_TIMER_DELAY_REPORT,
    LOCK_TIMER_BONDING_CONN,
    LOCK_TIMER_RESET_WITH_DISCONN,
    LOCK_TIMER_CONN_MONITOR,
    LOCK_TIMER_APP_TEST_OUTTIME,
    LOCK_TIMER_APP_TEST_RESET_OUTTIME,
    LOCK_TIMER_ACTIVE_REPORT,
    LOCK_TIMER_RESET_WITH_DISCONN2,
    LOCK_TUMER_MAX,
} lock_timer_t;

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
uint32_t lock_timer_creat(void);
uint32_t lock_timer_delete(lock_timer_t p_timer);
uint32_t lock_timer_start(lock_timer_t p_timer);
uint32_t lock_timer_stop(lock_timer_t p_timer);

void conn_param_update_outtime_cb_handler(void);
void delay_report_outtime_cb_handler(void);
void bonding_conn_outtime_cb_handler(void);
void reset_with_disconn_outtime_cb_handler(void);
void conn_monitor_outtime_cb_handler(void);
void app_test_outtime_cb_handler(void);
void app_test_reset_outtime_cb_handler(void);
void app_active_report_outtime_cb_handler(void);
void reset_with_disconn2_outtime_cb_handler(void);


#ifdef __cplusplus
}
#endif

#endif //__LOCK_TIMER_H__
