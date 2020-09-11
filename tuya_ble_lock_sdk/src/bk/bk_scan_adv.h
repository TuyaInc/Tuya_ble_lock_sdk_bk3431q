/**
****************************************************************************
* @file      bk_scan_adv.h
* @brief     bk_scan_adv
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __BK_SCAN_ADV_H__
#define __BK_SCAN_ADV_H__

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
#define  NRFS_ADV_MAX_LEN        31
#define  DEFAULT_ADV_DATA_LEN    3
#define  DEFAULT_SCAN_RSP_LEN    5

#define  BK_ADV_INTERVAL_MIN     APP_PORT_ADV_INTERVAL
#define  BK_ADV_INTERVAL_MAX     APP_PORT_ADV_INTERVAL

/*********************************************************************
 * STRUCT
 */
typedef struct
{
    double  adv_interval_min; //ms
    double  adv_interval_max; //ms
    uint8_t adv_type;
    uint8_t adv_power;
    uint8_t adv_channal_map;
} adv_param_t;

/*********************************************************************
 * EXTERNAL VARIABLES
 */
extern adv_param_t             g_adv_param;
extern uint32_t                g_adv_data_len;
extern uint8_t                 g_adv_data[];
extern uint32_t                g_scan_rsp_len;
extern uint8_t                 g_scan_rsp[];

//更新广播参数
extern volatile bool g_adv_restart_glag;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
void bk_adv_start(void);
void bk_adv_stop(void);
void bk_adv_update_advDataAndScanRsp(void);
void bk_adv_param_set(void);


#ifdef __cplusplus
}
#endif

#endif //__BK_SCAN_ADV_H__
