/**
****************************************************************************
* @file      bk_timer.h
* @brief     bk_timer
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __BK_TIMER_H__
#define __BK_TIMER_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "bk_common.h"

/*********************************************************************
 * CONSTANTS
 */
#define BK_TIMER_COUNT_ENDLESS      0xFFFFFFFF

#define BK_TIMER_MAX_NUM            20

typedef enum {
    BK_TIMER_SINGLE_SHOT,
    BK_TIMER_REPEATED,
} bk_timer_mode_t;

/*********************************************************************
 * STRUCT
 */
typedef struct
{
    uint32_t count;
    uint32_t delay;
} bk_timer_t;

typedef void (*bk_timer_handler_t)(void*);


/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
void bk_timer_handler(ke_msg_id_t timer_id);
void bk_timer_start_0(ke_msg_id_t const timer_id, uint32_t ms, uint32_t count);
void bk_timer_stop_0(ke_msg_id_t const timer_id);
uint8_t bk_timer_is_running(ke_msg_id_t const timer_id);

uint32_t bk_timer_create(void** p_timer_id, uint32_t timeout_value_ms, bk_timer_mode_t mode, bk_timer_handler_t timeout_handler);
uint32_t bk_timer_delete(void* timer_id);
uint32_t bk_timer_start(void* timer_id);
uint32_t bk_timer_stop(void* timer_id);

/*********************************************************  RTC 2  *********************************************************/
void bk_local_timer_start(void);
void bk_update_timestamp(uint32_t app_timestamp);
uint32_t bk_get_app_timestamp_when_update(void);
uint32_t bk_get_local_timestamp(void);
uint32_t bk_get_timestamp(void);
uint32_t bk_get_old_timestamp(uint32_t old_local_timestamp);

/*********************************************************  delay  *********************************************************/
void bk_delay_ms(uint32_t ms);
void bk_delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif //__BK_TIMER_H__
