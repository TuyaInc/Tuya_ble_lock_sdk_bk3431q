/**
****************************************************************************
* @file      bk_uart.h
* @brief     bk_uart
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __BK_UART_H__
#define __BK_UART_H__

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
void bk_uart1_init(void);
void bk_uart2_init(void);
void bk_uart1_send(const uint8_t* buf, uint32_t size);
void bk_uart2_send(const uint8_t* buf, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif //__BK_UART_H__
