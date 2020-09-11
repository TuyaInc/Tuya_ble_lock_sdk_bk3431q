/**
****************************************************************************
* @file      bk_test.h
* @brief     bk_test
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __WDT_H__
#define __WDT_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "BK3435_reg.h"
#include "stdint.h"

/*********************************************************************
 * CONSTANTS
 */
#define WATCH_DOG_COUNT 0x3FFF

#define ICU_WDT_CLK_PWD_CLEAR()             do {REG_AHB0_ICU_WDTCLKCON = 0x00;} while (0)
#define ICU_WDT_CLK_PWD_SET()               do {REG_AHB0_ICU_WDTCLKCON = 0x01;} while (0)

#define WDKEY_ENABLE1           0x005A
#define WDKEY_ENABLE2           0x00A5

#define WDKEY_ENABLE_FOREVER    0x00FF

#define WDKEY_DISABLE1          0x00DE
#define WDKEY_DISABLE2          0x00DA

#define WDT_CONFIG_PERIOD_POSI          0
#define WDT_CONFIG_PERIOD_MASK          (0x0000FFFFUL << WDT_CONFIG_PERIOD_POSI)

#define WDT_CONFIG_WDKEY_POSI           16
#define WDT_CONFIG_WDKEY_MASK           (0x00FFUL << WDT_CONFIG_WDKEY_POSI)

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
void wdt_enable(uint16_t wdt_cnt);
void wdt_disable(void);
void wdt_feed(uint16_t wdt_cnt);


#ifdef __cplusplus
}
#endif

#endif //__BK_TEST_H__
