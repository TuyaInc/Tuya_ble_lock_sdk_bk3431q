#include "bk_ble.h"
#include "app_port.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static uint8_t s_bk_bt_addr_default[BK_BT_ADDR_LEN] = {0x55, 0x44, 0x33, 0x22, 0x11, 0xFC};
static uint8_t s_bk_bt_addr_invalid[BK_BT_ADDR_LEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: 
*/
void bk_conn_param_update(uint16_t cMin, uint16_t cMax, uint16_t latency, uint16_t timeout)
{
	struct gapc_conn_param  up_param;
	up_param.intv_min   = (cMin*4)/5;
	up_param.intv_max   = (cMax*4)/5;
	up_param.latency    = latency;
	up_param.time_out   = timeout/10;
	appm_update_param(&up_param);
}

/*********************************************************
FN: ¶Ï¿ªÁ¬½Ó
*/
void bk_disconnect(void)
{
    appm_disconnect();
}

/*********************************************************
FN: 
*/
void bk_init_bt_mac_addr(void)
{
    uint8_t tmp_mac_str[BK_BT_ADDR_STR_LEN] = APP_PORT_DEFAULT_MAC_ADDR_STR;
    app_port_string_op_hexstr2hex(tmp_mac_str, BK_BT_ADDR_STR_LEN, s_bk_bt_addr_default);
    app_port_reverse_byte(s_bk_bt_addr_default, BK_BT_ADDR_LEN);
    memcpy(co_default_bdaddr.addr, s_bk_bt_addr_default, BK_BT_ADDR_LEN);
    
    uint8_t tmp_addr[BK_BT_ADDR_LEN];
    bk_flash_read(BK_FLASH_BLE_MAC_ADDR, tmp_addr, BK_BT_ADDR_LEN);
    if(0 != memcmp(tmp_addr, s_bk_bt_addr_invalid, BK_BT_ADDR_LEN))
    {
        memcpy(co_default_bdaddr.addr, tmp_addr, BK_BT_ADDR_LEN);
    }
    APP_DEBUG_HEXDUMP("tuya_mac", co_default_bdaddr.addr, BK_BT_ADDR_LEN);
}

/*********************************************************
FN: 
*/
void bk_set_bt_mac_addr(uint8_t *addr)
{
    bk_flash_erase(BK_FLASH_BLE_MAC_ADDR, 1);
    bk_flash_write(BK_FLASH_BLE_MAC_ADDR, addr, BK_BT_ADDR_LEN);
    memcpy(co_default_bdaddr.addr, addr, BK_BT_ADDR_LEN);
}

/*********************************************************
FN: 
*/
void bk_get_bt_mac_addr(uint8_t *addr, uint8_t len)
{
    if(len < BK_BT_ADDR_LEN) {
        return;
    }
    
    uint8_t tmp_addr[BK_BT_ADDR_LEN];
    bk_flash_read(BK_FLASH_BLE_MAC_ADDR, tmp_addr, BK_BT_ADDR_LEN);
    memcpy(addr, tmp_addr, BK_BT_ADDR_LEN);
}
















