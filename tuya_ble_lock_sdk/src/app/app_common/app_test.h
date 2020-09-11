/**
****************************************************************************
* @file      app_test.h
* @brief     This file mainly implements the authorization test function.
             "authorization test" is called "app_test".
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __APP_TEST_H__
#define __APP_TEST_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "app_common.h"

/*********************************************************************
 * CONSTANTS
 */
#define APP_TEST_CMD_ENTER                0x00
#define APP_TEST_CMD_QUERY_HID            0x01
#define APP_TEST_CMD_GPIO_TEST            0x02
#define APP_TEST_CMD_WRITE_AUTH_INFO      0x03
#define APP_TEST_CMD_QUERY_INFO           0x04
#define APP_TEST_CMD_RESET                0x05
#define APP_TEST_CMD_QUERY_FINGERPRINT    0x06
#define APP_TEST_CMD_WRITE_HID            0x07
#define APP_TEST_CMD_RSSI_TEST            0x08
#define APP_TEST_CMD_WRITE_OEM_INFO       0x09

#define APP_TEST_MODE_ENTER_OUTTIME_MS    500

#define APP_TEST_H_ID_LEN                 19

/*********************************************************************
 * STRUCT
 */
#pragma pack(1)
typedef struct 
{
    uint16_t sof;
    uint8_t  ver;
    uint8_t  id;
    uint16_t len;
    uint8_t  data[];
} app_test_cmd_t;

typedef struct 
{
    uint16_t sof;
    uint8_t  ver;
    uint8_t  id;
    uint16_t len;
    uint8_t  data[];
} app_test_rsp_t;

typedef struct 
{
    uint8_t  protocol;
    uint16_t sub_id;
    uint8_t  data[];
} lock_test_cmd_t;

typedef struct 
{
    uint8_t  protocol;
    uint16_t sub_id;
    uint8_t  data[];
} lock_test_rsp_t;
#pragma pack()

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
void app_test_init(void);
void app_test_pre_process(uint8_t cmd_id, uint8_t* buf, uint16_t size);
void app_test_outtime_handler(void);

/*********************************************************  factory_test cmd distribution  *********************************************************/
void factory_test_process(uint8_t* p_in_data, uint16_t in_len, uint8_t* p_out_data, uint16_t* out_len);

#ifdef __cplusplus
}
#endif

#endif //__APP_TEST_H__
