/**
****************************************************************************
* @file      lock_test.h
* @brief     This file mainly implements the product test function.
             Because this product is lock, "product test" is called "lock_test".
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __LOCK_TEST_H__
#define __LOCK_TEST_H__

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
//RFU-remain_for_future_use, PID-product_id, FW-firmware, FPM-finger_print_module
#define LOCK_TEST_CMD_ENTER                  0xFF02
#define LOCK_TEST_CMD_EXIT                   0xFF03
#define LOCK_TEST_CMD_LED                    0x0001
#define LOCK_TEST_CMD_RELAY                  0x0002
#define LOCK_TEST_CMD_KEY                    0x0003
#define LOCK_TEST_CMD_SWITCH_SENSOR          0x0004
#define LOCK_TEST_CMD_RFU                    0x0005
#define LOCK_TEST_CMD_ANALOG_SENSOR          0x0006
#define LOCK_TEST_CMD_MOTOR                  0x0007
#define LOCK_TEST_CMD_BATTERY_PARAM          0x0008
#define LOCK_TEST_CMD_BATTERY_CALIBRATION    0x0009
#define LOCK_TEST_CMD_LED_RGBCW              0x000A
#define LOCK_TEST_CMD_BURNIN                 0x000B
#define LOCK_TEST_CMD_INFRARED_TX            0x000C
#define LOCK_TEST_CMD_INFRARED_RX_ENTER      0x000D
#define LOCK_TEST_CMD_INFRARED_RX_EXIT       0x000E
#define LOCK_TEST_CMD_WRITE_SN               0x000F
#define LOCK_TEST_CMD_READ_SN                0x0010
#define LOCK_TEST_CMD_BLE_RSSI               0x0011
#define LOCK_TEST_CMD_WIFI_RSSI              0x0012
#define LOCK_TEST_CMD_GSENSOR                0x0013
#define LOCK_TEST_CMD_READ_MAC               0x0014
#define LOCK_TEST_CMD_READ_PID               0x0015
#define LOCK_TEST_CMD_READ_FW_INFO           0x0016
#define LOCK_TEST_CMD_FPM                    0x0017

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
void lock_test_process(uint16_t cmd, uint8_t* data, uint16_t len);


#ifdef __cplusplus
}
#endif

#endif //__LOCK_TEST_H__
