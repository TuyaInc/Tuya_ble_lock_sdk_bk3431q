/**
****************************************************************************
* @file      app_ota.h
* @brief     app_ota
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __APP_OTA_H__
#define __APP_OTA_H__

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
#define APP_OTA_VERSION         0x03
#define APP_OTA_PKG_LEN         (128+64)
#define APP_OTA_FILE_MD5_LEN    16
#define APP_OTA_START_ADDR      BK_FLASH_OTA_START_ADDR
#define APP_OTA_END_ADDR        BK_FLASH_OTA_END_ADDR
#define APP_OTA_FILE_MAX_LEN    (APP_OTA_END_ADDR-APP_OTA_START_ADDR)

/*********************************************************************
 * STRUCT
 */
#pragma pack(1)
typedef struct{
	uint8_t  flag;
	uint8_t  ota_version;
    uint8_t  type;
    uint32_t version;
    uint16_t package_maxlen;
} app_ota_req_rsp_t;

typedef struct{
	uint8_t  type;
	uint8_t  pid[8];
    uint32_t version;
    uint8_t  md5[APP_OTA_FILE_MD5_LEN];
    uint32_t file_len;
    uint32_t crc32;
} app_ota_file_info_t;

typedef struct{
	uint8_t  type;
	uint8_t  state;
    uint32_t old_file_len;
    uint32_t old_crc32;
    uint8_t  old_md5[APP_OTA_FILE_MD5_LEN];
} app_ota_file_info_rsp_t;

typedef struct{
	uint8_t  type;
    uint32_t offset;
} app_ota_file_offset_t;

typedef struct{
	uint8_t  type;
    uint32_t offset;
} app_ota_file_offset_rsp_t;

typedef struct{
	uint8_t  type;
    uint16_t pkg_id;
    uint16_t len;
    uint16_t crc16;
    uint8_t  data[];
} app_ota_data_t;

typedef struct{
	uint8_t type;
    uint8_t state;
} app_ota_data_rsp_t;

typedef struct{
	uint8_t  type;
    uint8_t state;
} app_ota_end_rsp_t;

typedef struct{
	uint32_t len;
    uint32_t crc32;
    uint8_t  md5[APP_OTA_FILE_MD5_LEN];
} app_ota_file_info_storage_t;
#pragma pack()

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
uint32_t app_ota_init(void);
void app_ota_handler(tuya_ble_ota_data_t* ota);
uint32_t app_ota_get_ota_state(void);
uint32_t app_ota_disconn_handler(void);


#ifdef __cplusplus
}
#endif

#endif //__APP_OTA_H__
