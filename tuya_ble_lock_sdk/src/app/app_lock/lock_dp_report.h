/**
****************************************************************************
* @file      lock_dp_report.h
* @brief     lock_dp_report
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __LOCK_DP_REPORT_H__
#define __LOCK_DP_REPORT_H__

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

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
/*********************************************************  creat open meth  *********************************************************/
void lock_hard_creat_sub_report(uint8_t meth, uint8_t stage, uint8_t hardid, uint8_t reg_idx, uint8_t result);
void delay_report_outtime_handler(void);
uint32_t lock_hard_creat_sub_report_with_delay(uint8_t meth, uint8_t stage, uint8_t hardid, uint8_t reg_idx, uint8_t result);

/*********************************************************  record report  *********************************************************/
uint32_t lock_open_record_report(uint8_t dp_id, uint32_t hardid);
uint32_t lock_open_record_report_with_delay(uint8_t dp_id, uint32_t hardid);
uint32_t lock_open_record_combine_report(uint8_t combine_enum, uint8_t size, uint8_t* hardid);
uint32_t lock_open_record_report_offline_pwd(uint8_t dp_id, uint8_t* pwd);
uint32_t lock_alarm_record_report(uint8_t alarm_reason);
void lock_offline_evt_report(uint8_t status);
void lock_open_meth_sync_new_report(uint8_t status);

/*********************************************************  state sync  *********************************************************/
uint32_t lock_state_sync_report(uint8_t dp_id, uint32_t data);


#ifdef __cplusplus
}
#endif

#endif //__LOCK_DP_REPORT_H__
