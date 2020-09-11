/**
****************************************************************************
* @file      bk_svc.h
* @brief     bk_svc
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __BK_SVC_H__
#define __BK_SVC_H__

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

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
void bk_svc_init(void);
void bk_svc_receive_data(uint8_t* buf, uint16_t size);
void bk_svc_send_data(uint8_t* buf, uint16_t size);
void bk_svc_send_data_complete(void);
uint32_t bk_ble_notify(uint8_t* buf, uint8_t len);
void bk_ble_notify_handler(void);


#ifdef __cplusplus
}
#endif

#endif //__BK_SVC_H__
