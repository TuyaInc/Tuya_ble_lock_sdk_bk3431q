/**
****************************************************************************
* @file      bk_common.h
* @brief     bk_common
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __BK_COMMON_H__
#define __BK_COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
//
#include "gapc_task.h"          // GAP Controller Task API
#include "gapm_task.h"          // GAP Manager Task API
//
#include "ke_timer.h"
#include "co_utils.h"
//
#include "app.h"
#include "app_task.h"
#include "app_fff0.h"
#include "uart.h"
#include "flash.h"
#include "gpio.h"
#include "pwm.h"
#include "rtc.h"
#include "rf.h"
//

//nrfs
#include "bk_scan_adv.h"
#include "bk_ble.h"
#include "bk_svc.h"
#include "bk_uart.h"
#include "bk_timer.h"
#include "bk_flash.h"
#include "bk_gpio.h"
#include "bk_test.h"
//cpt
#include "cpt_math.h"
#include "cpt_string_op.h"
#include "easyflash.h"
#include "elog.h"

//tuya_ble_sdk
#include "tuya_ble_api.h"
#include "app_port.h"

/*********************************************************************
 * CONSTANTS
 */
#define BK_DEBUG_EN 1

#if (BK_DEBUG_EN)

    #define BK_PRINTF       log_d
    #define BK_HEXDUMP      bk_log_hexdump
    
#else

    #define BK_PRINTF(...)
    #define BK_HEXDUMP      bk_log_hexdump_empty
    
#endif

//return value
typedef enum {
    BK_SUCCESS  = 0x00,
    BK_ERROR_COMMON  = 0x01,
} bk_status_t;

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */
extern volatile bool g_system_sleep;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
void bk_log_init(void);
void bk_log_hexdump(const char *name, uint8_t *buf, uint16_t size);
void bk_log_hexdump_for_tuya_ble_sdk(const char *name, uint8_t width, uint8_t *buf, uint16_t size);
void bk_log_hexdump_empty(const char *name, uint8_t *buf, uint16_t size);

void bk_system_reset(void);
void bk_wdt_feed(void);
void bk_enter_critical(void);
void bk_exit_critical(void);

#ifdef __cplusplus
}
#endif

#endif //__BK_COMMON_H__
