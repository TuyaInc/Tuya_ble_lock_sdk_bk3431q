#include "lock_test.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static volatile bool s_lock_test_mode = false;
//record last cmd
static int16_t s_lock_test_last_cmd = -1;

/*********************************************************************
 * LOCAL FUNCTION
 */
static bool is_lock_test_mode(void);
static void lock_test_mode_enter(void);
static void lock_test_mode_exit(void);
static void lock_test_rsp(uint16_t cmd, uint8_t* buf, uint16_t size);

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: 
*/
void lock_test_process(uint16_t cmd, uint8_t* data, uint16_t len)
{
    bool is_param_error = false;
    
    //if not in lock_test_mode, only handle cmd "LOCK_TEST_CMD_ENTER"
    if((!is_lock_test_mode()) && (cmd != LOCK_TEST_CMD_ENTER)) {
        return;
    }
    
    if(cmd == s_lock_test_last_cmd) {
        return;
    }
    s_lock_test_last_cmd = cmd;
    
	switch(cmd)
	{
		case LOCK_TEST_CMD_ENTER: {
            if(len == 0)
            {
                if(app_port_get_connect_status() != BONDING_CONN) {
                    lock_test_mode_enter();
                    
                    uint8_t tmp_buf[] = "{\"ret\":true}";
                    lock_test_rsp(cmd, tmp_buf, strlen((void*)tmp_buf));
                }else {
                    APP_DEBUG_PRINTF("lock test mode enter fail!", cmd);
                    APP_DEBUG_PRINTF("The device is bonded.", cmd);
                    
                    uint8_t tmp_buf[] = "{\"ret\":false}";
                    lock_test_rsp(cmd, tmp_buf, strlen((void*)tmp_buf));
                }
            }else {
                is_param_error = true;
                goto LOCK_TEST_PARAM_ERROR;
            }
        } break;
        
		case LOCK_TEST_CMD_LED: {
            if(len == 1)
            {
                if(*data > 0x02) {
                    is_param_error = true;
                    goto LOCK_TEST_PARAM_ERROR;
                }
                
                if(*data == 0x00) {
                    //turn on all lights
                }
                else if(*data == 0x01) {
                    //turn off all lights
                }
                else if(*data == 0x02) {
                    //toggle lights
                }
                
                uint8_t tmp_buf[] = "{\"ret\":true}";
                lock_test_rsp(cmd, tmp_buf, strlen((void*)tmp_buf));
            }else {
                is_param_error = true;
                goto LOCK_TEST_PARAM_ERROR;
            }
        } break;
        
		case LOCK_TEST_CMD_MOTOR: {
        } break;
        
		case LOCK_TEST_CMD_READ_MAC: {
            if(len == 0)
            {
                uint8_t addr[APP_PORT_BLE_ADDR_LEN];
                app_port_get_bt_mac_addr(addr);
                app_port_reverse_byte(addr, APP_PORT_BLE_ADDR_LEN);
                
                uint8_t addr_str[APP_PORT_BLE_ADDR_STR_LEN+1] = {0};
                app_port_string_op_hex2hexstr(addr, APP_PORT_BLE_ADDR_LEN, addr_str);
                
                uint8_t tmp_buf[50],tmp_len;
                tmp_len = sprintf((void*)tmp_buf, "{\"ret\":true,\"mac\":\"%s\"}", addr_str);
                lock_test_rsp(cmd, tmp_buf, tmp_len);
            }else {
                is_param_error = true;
                goto LOCK_TEST_PARAM_ERROR;
            }
        } break;
        
		case LOCK_TEST_CMD_READ_PID: {
            if(len == 0)
            {
                uint8_t pid[8+1] = {0};
                memcpy(pid, TUYA_DEVICE_PID, APP_PORT_PID_LEN);
                
                uint8_t tmp_buf[50],tmp_len;
                tmp_len = sprintf((void*)tmp_buf, "{\"ret\":true,\"pid\":\"%s\"}", pid);
                lock_test_rsp(cmd, tmp_buf, tmp_len);
            }else {
                is_param_error = true;
                goto LOCK_TEST_PARAM_ERROR;
            }
        } break;
        
		case LOCK_TEST_CMD_READ_FW_INFO: {
            if(len == 0)
            {
                uint8_t firmName[40] = {0};
                memcpy(firmName, TUYA_DEVICE_FIR_NAME, strlen(TUYA_DEVICE_FIR_NAME));
                uint8_t firmVer[10] = {0};
                memcpy(firmVer, TUYA_DEVICE_FVER_STR, strlen(TUYA_DEVICE_FVER_STR));
                
                uint8_t tmp_buf[90],tmp_len;
                tmp_len = sprintf((void*)tmp_buf, "{\"ret\":true,\"firmName\":\"%s\",\"firmVer\":\"%s\"}", firmName, firmVer);
                lock_test_rsp(cmd, tmp_buf, tmp_len);
            }else {
                is_param_error = true;
                goto LOCK_TEST_PARAM_ERROR;
            }
        } break;
        
		case LOCK_TEST_CMD_EXIT: {
            if(len == 0)
            {
                lock_test_mode_exit();
                
                uint8_t tmp_buf[] = "{\"ret\":true}";
                lock_test_rsp(cmd, tmp_buf, strlen((void*)tmp_buf));
            } else {
                is_param_error = true;
                goto LOCK_TEST_PARAM_ERROR;
            }
        } break;
        
		default: {
        } break;
    }
    
LOCK_TEST_PARAM_ERROR:
    if(is_param_error)
    {
        APP_DEBUG_PRINTF("LOCK_TEST_PARAM_ERROR! Please check your param.");
        APP_DEBUG_PRINTF("Error cmd: %d", cmd);
        lock_test_mode_exit();
    }
}

/*********************************************************
FN: 
*/
static bool is_lock_test_mode(void)
{
    return s_lock_test_mode;
}

/*********************************************************
FN: 
*/
static void lock_test_mode_enter(void)
{
    APP_DEBUG_PRINTF("lock test mode enter success!");
    s_lock_test_mode = true;
}

/*********************************************************
FN: 
*/
static void lock_test_mode_exit(void)
{
    APP_DEBUG_PRINTF("lock test mode exit success!");
    s_lock_test_mode = false;
}

/*********************************************************
FN: 
*/
static void lock_test_rsp(uint16_t sub_cmd, uint8_t* buf, uint16_t size)
{
    static uint8_t rsp[256];
    uint16_t rsp_len;
    
    uint16_t data_len = size + 3;
    
    rsp[0] = 0x66;
    rsp[1] = 0xAA;
    rsp[2] = 0x00;
    rsp[3] = 0xF0;
    rsp[4] = data_len >> 8;
    rsp[5] = data_len;
    rsp[6] = 0x03;
    rsp[7] = sub_cmd >> 8;
    rsp[8] = sub_cmd;
    memcpy(&rsp[9], buf, size);
    rsp_len = data_len + 6;

    rsp[rsp_len] = app_port_check_sum(rsp, rsp_len);
    rsp_len += 1;
    
    extern void tuya_ble_lock_test_rsp(uint8_t* buf, uint16_t size);
    tuya_ble_lock_test_rsp(rsp, rsp_len);
}






















