/**
****************************************************************************
* @file      bk_ble.h
* @brief     bk_ble
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __BK_BLE_H__
#define __BK_BLE_H__

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
#define BK_BT_ADDR_LEN                    BD_ADDR_LEN
#define BK_BT_ADDR_STR_LEN                (BK_BT_ADDR_LEN*2)

#define BK_CONN_INTERVAL_MIN               180 //最小可接受的连接间隔
#define BK_CONN_INTERVAL_MAX               200
#define BK_SLAVE_LATENCY                   0
#define BK_CONN_SUP_TIMEOUT                5000

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
void bk_conn_param_update(uint16_t cMin, uint16_t cMax, uint16_t latency, uint16_t timeout);
void bk_disconnect(void);
void bk_init_bt_mac_addr(void);
void bk_set_bt_mac_addr(uint8_t *addr);
void bk_get_bt_mac_addr(uint8_t *addr, uint8_t len);


#ifdef __cplusplus
}
#endif

#endif //__BK_BLE_H__
