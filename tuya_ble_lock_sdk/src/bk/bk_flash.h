/**
****************************************************************************
* @file      bk_flash.h
* @brief     bk_flash
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __BK_FLASH_H__
#define __BK_FLASH_H__

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
#define BK_FLASH_START_ADDR        0x44000
#define BK_FLASH_END_ADDR          0x64000

#define BK_FLASH_OTA_START_ADDR    BK_FLASH_START_ADDR
#define BK_FLASH_OTA_END_ADDR      0x64000

#define BK_FLASH_BLE_MAC_ADDR      0x7F000

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
void bk_flash_init(void);
void bk_flash_read(uint32_t addr, uint8_t *buf, uint32_t size);
void bk_flash_write(uint32_t addr, uint8_t *buf, uint32_t size);
void bk_flash_erase(uint32_t addr, uint32_t num);


#ifdef __cplusplus
}
#endif

#endif //__BK_FLASH_H__
