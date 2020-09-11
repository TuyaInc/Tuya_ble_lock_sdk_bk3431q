#include "bk_svc.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */
#define NOTIFY_MAX_NUM  64

/*********************************************************************
 * LOCAL STRUCT
 */
typedef struct
{
    uint32_t len;
    uint8_t  data[20];
} notify_data_t;

/*********************************************************************
 * LOCAL VARIABLES
 */
static notify_data_t notify_data[NOTIFY_MAX_NUM];
static volatile uint32_t s_notify_start = 0;
static volatile uint32_t s_notify_end = 0;
static volatile uint32_t s_notify_flag = 0;

/*********************************************************************
 * LOCAL FUNCTION
 */
static uint8_t bk_next_index(uint8_t index);
static uint8_t bk_queue_full(void);
static uint8_t bk_queue_empty(void);

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: 
*/
void bk_svc_init(void)
{
    
}

/*********************************************************
FN: 
*/
void bk_svc_receive_data(uint8_t* buf, uint16_t size)
{
    tuya_ble_gatt_receive_data(buf, size);
//    bk_svc_send_data(buf, size);
//    BK_HEXDUMP("Svc rx", buf, size);
}

/*********************************************************
FN: 作为从机向主机发送数据
*/
void bk_svc_send_data(uint8_t* buf, uint16_t size)
{
    app_fff1_send_lvl(buf, size);
}

/*********************************************************
FN: 发送完成
*/
void bk_svc_send_data_complete(void)
{
    s_notify_start = bk_next_index(s_notify_start);
}




/*********************************************************
FN: 
*/
static uint8_t bk_next_index(uint8_t index)
{
    return (index < NOTIFY_MAX_NUM-1) ? (index + 1) : 0;
}

static uint8_t bk_queue_full(void)
{
    uint8_t tmp = s_notify_start;
    return bk_next_index(s_notify_end) == tmp;
}

static uint8_t bk_queue_empty(void)
{
    uint8_t tmp = s_notify_start;
    return s_notify_end == tmp;
}

/*********************************************************
FN: 
*/
uint32_t bk_ble_notify(uint8_t* buf, uint8_t len)
{
    if(!bk_queue_full()) {
        notify_data[s_notify_end].len = len;
        memcpy(notify_data[s_notify_end].data, buf, len);

        s_notify_end = bk_next_index(s_notify_end);
        return 0;
    }
    return 1;
}

/*********************************************************
FN: 
*/
void bk_ble_notify_handler(void)
{
    if(!bk_queue_empty()) {
        if(s_notify_flag == s_notify_start) {
            s_notify_flag = bk_next_index(s_notify_flag);
            bk_svc_send_data(notify_data[s_notify_start].data, notify_data[s_notify_start].len);
        }
    }
}










