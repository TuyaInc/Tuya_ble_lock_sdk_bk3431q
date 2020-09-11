#include "bk_common.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static uint32_t fiq_tmp;
static uint32_t irq_tmp;

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */
volatile bool g_system_sleep = false;




/*********************************************************
FN: 
*/
void bk_log_init(void)
{
    elog_init();
    /* set EasyLogger log format */
    elog_set_fmt(ELOG_LVL_ASSERT,  ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME|ELOG_FMT_FUNC|ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_ERROR,   ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME|ELOG_FMT_FUNC|ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_WARN,    ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME|ELOG_FMT_FUNC|ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_INFO,    ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG,   ELOG_FMT_LVL);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_LVL);
    /* start EasyLogger */
    elog_start();
}

/*********************************************************
FN: 
*/
void bk_log_hexdump(const char *name, uint8_t *buf, uint16_t size)
{
    elog_hexdump(name, 8, buf, size);
}

/*********************************************************
FN: 
*/
void bk_log_hexdump_for_tuya_ble_sdk(const char *name, uint8_t width, uint8_t *buf, uint16_t size)
{
    elog_hexdump(name, width, buf, size);
}

/*********************************************************
FN: 
*/
void bk_log_hexdump_empty(const char *name, uint8_t *buf, uint16_t size)
{
    //empty
}

/*********************************************************
FN: 
*/
void bk_system_reset(void)
{
    platform_reset(0);
}

/*********************************************************
FN: 
*/
void bk_wdt_feed(void)
{
    wdt_feed(WATCH_DOG_COUNT);
}

/*********************************************************
FN: 
*/
void bk_enter_critical(void)
{
    fiq_tmp = __disable_fiq();
    irq_tmp = __disable_irq();
}

/*********************************************************
FN: 
*/
void bk_exit_critical(void)
{
    if(!fiq_tmp) {
        __enable_fiq();
    }
    if(!irq_tmp) {
        __enable_irq();
    }
}




















