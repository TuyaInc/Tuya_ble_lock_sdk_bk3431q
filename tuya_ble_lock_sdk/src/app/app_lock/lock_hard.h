/**
****************************************************************************
* @file      lock_hard.h
* @brief     lock_hard
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __LOCK_HARD_H__
#define __LOCK_HARD_H__

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
#define FINGER_REG_TOTAL_TIMES 6

typedef enum
{
    UART_SIMULATE_REG_PASSWORD = 0x00,
    UART_SIMULATE_REG_DOORCARD,
    UART_SIMULATE_REG_FINGER,
    UART_SIMULATE_REG_FACE,
    UART_SIMULATE_DELETE_PASSWORD,//0x04
    UART_SIMULATE_DELETE_DOORCARD,
    UART_SIMULATE_DELETE_FINGER,
    UART_SIMULATE_DELETE_FACE,
    UART_SIMULATE_MODIFY_PASSWORD,//0x08
    UART_SIMULATE_MODIFY_DOORCARD,
    UART_SIMULATE_MODIFY_FINGER,
    UART_SIMULATE_MODIFY_FACE,
    UART_SIMULATE_REPORT_OPEN_RECORD,//0x0c
    UART_SIMULATE_REPORT_ALARM,
    UART_SIMULATE_STATE_SYNC,
    UART_SIMULATE_DYNAMIC_PWD,
    UART_SIMULATE_OFFLINE_PWD,//0x10
    UART_SIMULATE_FACTORY_RESET = 0x20,
    UART_SIMULATE_COMMON_DP,
    UART_SIMULATE_COMMON_DP_WITH_TIMESTAMP,
    UART_SIMULATE_SET_FLAG,
    UART_SIMULATE_ACTIVE_REPORT = 0x30,
    UART_SIMULATE_DELETE_FLASH = 0x40,
} uart_simulate_cmd_t;

/*********************************************************************
 * STRUCT
 */
typedef struct
{
    uint8_t open_with_bt_flag;
    uint8_t creat_pw_flag;
} lock_hard_uart_simulate_auto_switch_t;

/*********************************************************************
 * EXTERNAL VARIABLES
 */
extern volatile lock_hard_uart_simulate_auto_switch_t g_auto_switch;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
/*****************************************************   -doorcard-   ******************************************************/
void lock_hard_doorcard_start_reg(void* buf, uint32_t size);
void lock_hard_doorcard_cancel_reg(void);
uint32_t lock_hard_doorcard_delete(uint8_t hardid);

/*****************************************************   -finger-   ******************************************************/
void lock_hard_finger_start_reg(void* buf, uint32_t size);
void lock_hard_finger_cancel_reg(void);
uint32_t lock_hard_finger_delete(uint8_t hardid);

/*****************************************************   -face-   ******************************************************/
void lock_hard_face_start_reg(void* buf, uint32_t size);
void lock_hard_face_cancel_reg(void);
uint32_t lock_hard_face_delete(uint8_t hardid);

/*****************************************************   -simulate-   ******************************************************/
void lock_hard_uart_simulate(uint8_t cmd, uint8_t* data, uint16_t len);


#ifdef __cplusplus
}
#endif

#endif //__LOCK_HARD_H__
