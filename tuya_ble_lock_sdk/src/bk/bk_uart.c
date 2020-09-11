#include "bk_uart.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: 
*/
static void uart1_rx_handler(uint8_t *buf, uint8_t len)
{
//    BK_HEXDUMP("uart1", buf, len);
    tuya_ble_uart_receive_data(buf, len); //产测打开这个，和烧录用同一个串口
}

/*********************************************************
FN: 
*/
static void uart2_rx_handler(uint8_t *buf, uint8_t len)
{
//    BK_HEXDUMP("uart2", buf, len);
    tuya_ble_uart_receive_data(buf, len);
}

/*********************************************************
FN: 
*/
void bk_uart1_init(void)
{
	uart_init(9600);
	uart_cb_register(uart1_rx_handler);
}

/*********************************************************
FN: 
*/
void bk_uart2_init(void)
{
	uart2_init(460800);
	uart2_cb_register(uart2_rx_handler);
}

/*********************************************************
FN: 
*/
void bk_uart1_send(const uint8_t* buf, uint32_t size)
{
    uart_send((void*)buf, size);
}

/*********************************************************
FN: 
*/
void bk_uart2_send(const uint8_t* buf, uint32_t size)
{
    uart2_send((void*)buf, size);
}

















