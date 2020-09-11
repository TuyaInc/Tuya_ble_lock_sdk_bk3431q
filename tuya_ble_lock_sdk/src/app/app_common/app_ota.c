#include "app_ota.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static volatile uint8_t  s_ota_state = TUYA_BLE_OTA_REQ;
static volatile int32_t  s_pkg_id;
static uint32_t s_data_len;
static uint32_t s_data_crc;
static volatile bool s_ota_success = false;
//file info
static app_ota_file_info_storage_t s_file;
static app_ota_file_info_storage_t s_old_file;

/*********************************************************************
 * LOCAL FUNCTION
 */
static uint32_t app_ota_enter(void);
static uint32_t app_ota_exit(void);
static uint32_t app_ota_get_crc32_in_flash(uint32_t len);
//static void app_ota_setting_write_complete_cb(nrf_fstorage_evt_t* p_evt);
static uint32_t app_ota_req_handler(uint8_t* cmd, uint16_t cmd_size, tuya_ble_ota_data_response_t* rsp);
static uint32_t app_ota_file_info_handler(uint8_t* cmd, uint16_t cmd_size, tuya_ble_ota_data_response_t* rsp);
static uint32_t app_ota_file_offset_handler(uint8_t* cmd, uint16_t cmd_size, tuya_ble_ota_data_response_t* rsp);
static uint32_t app_ota_data_handler(uint8_t* cmd, uint16_t cmd_size, tuya_ble_ota_data_response_t* rsp);
static uint32_t app_ota_end_handler(uint8_t* cmd, uint16_t cmd_size, tuya_ble_ota_data_response_t* rsp);

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: 
*/
uint32_t app_ota_init(void)
{
    return 0;
}

/*********************************************************
FN: 
*/
void app_ota_handler(tuya_ble_ota_data_t* ota)
{
    tuya_ble_ota_data_response_t rsp;
    rsp.type = ota->type;
    
    if(ota->type != TUYA_BLE_OTA_DATA)
    {
        APP_DEBUG_PRINTF("ota_cmd_type: %d", ota->type);
        APP_DEBUG_HEXDUMP("ota_cmd_data", ota->p_data, ota->data_len);
    }
    
    switch(ota->type)
    {
        case TUYA_BLE_OTA_REQ: {
            app_ota_req_handler(ota->p_data, ota->data_len, &rsp);
        } break;
        
        case TUYA_BLE_OTA_FILE_INFO: {
            app_ota_file_info_handler(ota->p_data, ota->data_len, &rsp);
        } break;
        
        case TUYA_BLE_OTA_FILE_OFFSET_REQ: {
            app_ota_file_offset_handler(ota->p_data, ota->data_len, &rsp);
        } break;
        
        case TUYA_BLE_OTA_DATA: {
            app_ota_data_handler(ota->p_data, ota->data_len, &rsp);
        } break;
        
        case TUYA_BLE_OTA_END: {
            app_ota_end_handler(ota->p_data, ota->data_len, &rsp);
        } break;
        
        case TUYA_BLE_OTA_UNKONWN: {
        } break;
        
        default: {
        } break;
    }
}

/*********************************************************
FN: 
*/
static uint32_t app_ota_enter(void)
{
    s_pkg_id = -1;
    s_data_len = 0;
    s_data_crc = 0;
    s_ota_success = false;
    memset(&s_file, 0x00, sizeof(app_ota_file_info_storage_t));
    memset(&s_old_file, 0x00, sizeof(app_ota_file_info_storage_t));
    
    app_port_conn_param_update(20, 24, 0, 5000);
    app_port_ble_conn_evt_ext();
    
    tuya_ble_device_enter_critical();
    app_port_nv_erase(APP_OTA_START_ADDR, APP_OTA_FILE_MAX_LEN);
    tuya_ble_device_exit_critical();
    
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: 
*/
static uint32_t app_ota_exit(void)
{
    if(!s_ota_success)
    {
//        memset(&s_dfu_settings, 0, sizeof(nrf_dfu_settings_t));
    }
    
    s_ota_state = TUYA_BLE_OTA_REQ;
    lock_timer_start(LOCK_TIMER_RESET_WITH_DISCONN);
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: 
*/
uint32_t app_ota_get_ota_state(void)
{
    return s_ota_state;
}

/*********************************************************
FN: 
*/
uint32_t app_ota_disconn_handler(void)
{
    if(s_ota_state > TUYA_BLE_OTA_REQ) {
        return app_ota_exit();
    } else {
        return 0;
    }
}

/*********************************************************
FN: 
*/
static uint32_t app_ota_get_crc32_in_flash(uint32_t len)
{
    static uint8_t buf[APP_OTA_PKG_LEN];
    
    if(len == 0)
    {
        return 0;
    }
    
    uint32_t crc_temp = 0;
    uint32_t read_addr = APP_OTA_START_ADDR;
    uint32_t cnt = len/APP_OTA_PKG_LEN;
    uint32_t remainder = len%APP_OTA_PKG_LEN;
    
    for(uint32_t idx=0; idx<cnt; idx++)
    {
        tuya_ble_nv_read(read_addr, buf, APP_OTA_PKG_LEN);
        crc_temp = app_port_crc32_compute(buf, APP_OTA_PKG_LEN, &crc_temp);
        read_addr += APP_OTA_PKG_LEN;
    }

    if(remainder > 0)
    {
        tuya_ble_nv_read(read_addr, buf, APP_OTA_PKG_LEN);
        crc_temp = app_port_crc32_compute(buf, remainder, &crc_temp);
        read_addr += remainder;
    }
    
    return crc_temp;
}

/*********************************************************
FN: 
*/
static uint32_t app_ota_rsp(tuya_ble_ota_data_response_t* rsp, void* rsp_data, uint16_t data_size)
{
    rsp->p_data = rsp_data;
    rsp->data_len = data_size;
    return app_port_ota_rsp(rsp);
}

/*********************************************************
FN: 
*/
static uint32_t app_ota_req_handler(uint8_t* cmd, uint16_t cmd_size, tuya_ble_ota_data_response_t* rsp)
{
    //param check
    if(s_ota_state != TUYA_BLE_OTA_REQ)
    {
        APP_DEBUG_PRINTF("Error: TUYA_BLE_OTA_REQ- s_ota_state error");
        //rsp
        app_ota_req_rsp_t req_rsp;
        memset(&req_rsp, 0x00, sizeof(app_ota_req_rsp_t));
        req_rsp.flag = 0x01; //refuse ota
        
        app_ota_rsp(rsp, &req_rsp, sizeof(app_ota_req_rsp_t));
        app_ota_exit();
        return APP_PORT_ERROR_COMMON;
    }
    
    //param check
    if((cmd_size != 0x0001) || (*cmd != 0x00))
    {
        APP_DEBUG_PRINTF("Error: TUYA_BLE_OTA_REQ- param error");
        //rsp
        app_ota_req_rsp_t req_rsp;
        memset(&req_rsp, 0x00, sizeof(app_ota_req_rsp_t));
        req_rsp.flag = 0x01; //refuse ota
        
        app_ota_rsp(rsp, &req_rsp, sizeof(app_ota_req_rsp_t));
        app_ota_exit();
        return APP_PORT_ERROR_COMMON;
    }
    
    {
        app_ota_enter();

        //rsp
        app_ota_req_rsp_t req_rsp;
        memset(&req_rsp, 0x00, sizeof(app_ota_req_rsp_t));
        req_rsp.flag = 0x00; //accept ota
        req_rsp.ota_version = APP_OTA_VERSION;
        req_rsp.type = 0x00; //firmware info
        req_rsp.version = TUYA_DEVICE_FVER_NUM;
        app_port_reverse_byte(&req_rsp.version, sizeof(uint32_t));
        req_rsp.package_maxlen = APP_OTA_PKG_LEN;
        app_port_reverse_byte(&req_rsp.package_maxlen, sizeof(uint16_t));
        
        app_ota_rsp(rsp, &req_rsp, sizeof(app_ota_req_rsp_t));
        s_ota_state = TUYA_BLE_OTA_FILE_INFO;
    }
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: 
*/
static uint32_t app_ota_file_info_handler(uint8_t* cmd, uint16_t cmd_size, tuya_ble_ota_data_response_t* rsp)
{
    //param check
    if(s_ota_state != TUYA_BLE_OTA_FILE_INFO)
    {
        APP_DEBUG_PRINTF("Error: TUYA_BLE_OTA_FILE_INFO- s_ota_state error");
        //rsp none
        app_ota_exit();
        return APP_PORT_ERROR_COMMON;
    }

    //param check
    app_ota_file_info_t* file_info = (void*)cmd;
    if(file_info->type != 0x00)
    {
        APP_DEBUG_PRINTF("Error: TUYA_BLE_OTA_FILE_INFO- file_info->type error");
        //rsp none
        app_ota_exit();
        return APP_PORT_ERROR_COMMON;
    }
    
    {
        //file info
        app_port_reverse_byte(&file_info->version, sizeof(uint32_t));
        app_port_reverse_byte(&file_info->file_len, sizeof(uint32_t));
        app_port_reverse_byte(&file_info->crc32, sizeof(uint32_t));
        s_file.len = file_info->file_len;
        s_file.crc32 = file_info->crc32;
        memcpy(s_file.md5, file_info->md5, APP_OTA_FILE_MD5_LEN);
        
        //rsp
        app_ota_file_info_rsp_t file_info_rsp;
        memset(&file_info_rsp, 0x00, sizeof(app_ota_file_info_rsp_t));
        file_info_rsp.type = 0x00; //firmware info
        if(memcmp(file_info->pid, TUYA_DEVICE_PID, APP_PORT_PID_LEN)) {
            file_info_rsp.state = 0x01; //pid error
        }
        else if(file_info->version <= TUYA_DEVICE_FVER_NUM) {
            file_info_rsp.state = 0x02; //version error
        }
        else if(file_info->file_len > APP_OTA_FILE_MAX_LEN) {
            file_info_rsp.state = 0x03; //size error
        } else {
            file_info_rsp.state = 0x00;
            s_ota_state = TUYA_BLE_OTA_FILE_OFFSET_REQ;
        }
        
        app_port_nv_get(SF_AREA_0, NV_ID_OTA_DATA_LEN, &s_old_file.len, sizeof(uint32_t));
        app_port_nv_get(SF_AREA_0, NV_ID_OTA_DATA_CRC, &s_old_file.crc32, sizeof(uint32_t));
        file_info_rsp.old_file_len = s_old_file.len;
        app_port_reverse_byte(&file_info_rsp.old_file_len, sizeof(uint32_t));
        file_info_rsp.old_crc32 = s_old_file.crc32;
        app_port_reverse_byte(&file_info_rsp.old_crc32, sizeof(uint32_t));
        memset(file_info_rsp.old_md5, 0x00, APP_OTA_FILE_MD5_LEN);
        app_ota_rsp(rsp, &file_info_rsp, sizeof(app_ota_file_info_rsp_t));
        
        if(file_info_rsp.state != 0x00) {
            APP_DEBUG_PRINTF("Error: TUYA_BLE_OTA_FILE_INFO- errorid: %d", file_info_rsp.state);
            app_ota_exit();
        }
    }
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: 
*/
static uint32_t app_ota_file_offset_handler(uint8_t* cmd, uint16_t cmd_size, tuya_ble_ota_data_response_t* rsp)
{
    //param check
    if(s_ota_state != TUYA_BLE_OTA_FILE_OFFSET_REQ)
    {
        APP_DEBUG_PRINTF("Error: TUYA_BLE_OTA_FILE_OFFSET_REQ- s_ota_state error");
        //rsp none
        app_ota_exit();
        return APP_PORT_ERROR_COMMON;
    }

    //param check
    app_ota_file_offset_t* file_offset = (void*)cmd;
    if(file_offset->type != 0x00)
    {
        APP_DEBUG_PRINTF("Error: TUYA_BLE_OTA_FILE_OFFSET_REQ- file_offset->type error");
        //rsp none
        app_ota_exit();
        return APP_PORT_ERROR_COMMON;
    }
    
    {
        app_port_reverse_byte(&file_offset->offset, sizeof(uint32_t));
        
        //rsp
        app_ota_file_offset_rsp_t file_offset_rsp;
        memset(&file_offset_rsp, 0x00, sizeof(app_ota_file_offset_rsp_t));
        file_offset_rsp.type = 0x00;
        {
            if(file_offset->offset > 0)
            {
                uint8_t md5[APP_OTA_FILE_MD5_LEN];
                app_port_nv_get(SF_AREA_0, NV_ID_OTA_DATA_LEN, &s_old_file.len, sizeof(uint32_t));
                app_port_nv_get(SF_AREA_0, NV_ID_OTA_DATA_CRC, &s_old_file.crc32, sizeof(uint32_t));
                app_port_nv_get(SF_AREA_0, NV_ID_OTA_FILE_MD5, md5, APP_OTA_FILE_MD5_LEN);
                if((memcmp(md5, s_file.md5, APP_OTA_FILE_MD5_LEN) == 0) && (app_ota_get_crc32_in_flash(s_data_len) == s_data_crc) && (file_offset->offset >= s_data_len)) {
                    file_offset_rsp.offset = s_data_len;
//                    s_pkg_id = s_data_len/APP_OTA_PKG_LEN;//every time from zero
                } else {
                    file_offset_rsp.offset = 0;
                    s_data_len = 0;
                    s_data_crc = 0;
                }
            }
            app_port_nv_set(SF_AREA_0, NV_ID_OTA_FILE_MD5, &s_file.md5, APP_OTA_FILE_MD5_LEN);
        }
        app_port_reverse_byte(&file_offset_rsp.offset, sizeof(uint32_t));
        
        app_ota_rsp(rsp, &file_offset_rsp, sizeof(app_ota_file_offset_rsp_t));
        s_ota_state = TUYA_BLE_OTA_DATA;
    }
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: 
*/
static uint32_t app_ota_data_handler(uint8_t* cmd, uint16_t cmd_size, tuya_ble_ota_data_response_t* rsp)
{
    //param check
    if(s_ota_state != TUYA_BLE_OTA_DATA)
    {
        APP_DEBUG_PRINTF("Error: TUYA_BLE_OTA_DATA- s_ota_state error");
        //rsp
        app_ota_data_rsp_t ota_data_rsp;
        memset(&ota_data_rsp, 0x00, sizeof(app_ota_data_rsp_t));
        ota_data_rsp.state = 0x04; //unknow error
        
        app_ota_rsp(rsp, &ota_data_rsp, sizeof(app_ota_data_rsp_t));
        app_ota_exit();
        return APP_PORT_ERROR_COMMON;
    }

    //param check
    app_ota_data_t* ota_data = (void*)cmd;
    if(ota_data->type != 0x00)
    {
        APP_DEBUG_PRINTF("Error: TUYA_BLE_OTA_DATA- ota_data->type error");
        //rsp
        app_ota_data_rsp_t ota_data_rsp;
        memset(&ota_data_rsp, 0x00, sizeof(app_ota_data_rsp_t));
        ota_data_rsp.state = 0x04; //unknow error
        
        app_ota_rsp(rsp, &ota_data_rsp, sizeof(app_ota_data_rsp_t));
        app_ota_exit();
        return APP_PORT_ERROR_COMMON;
    }
    
    {
        app_port_reverse_byte(&ota_data->pkg_id, sizeof(uint16_t));
        app_port_reverse_byte(&ota_data->len, sizeof(uint16_t));
        app_port_reverse_byte(&ota_data->crc16, sizeof(uint16_t));
        
        //rsp
        app_ota_data_rsp_t ota_data_rsp;
        memset(&ota_data_rsp, 0x00, sizeof(app_ota_data_rsp_t));
        ota_data_rsp.type = 0x00;
        if(s_pkg_id+1 != ota_data->pkg_id) {
            ota_data_rsp.state = 0x01; //package id error
        }
        else if(cmd_size-7 != ota_data->len) {
            ota_data_rsp.state = 0x02; //size error
        }
        else if(app_port_crc16_compute(ota_data->data, ota_data->len, NULL) != ota_data->crc16) {
            ota_data_rsp.state = 0x03; //crc error
        } else {
            ota_data_rsp.state = 0x00;
            
            //erase
//            bool flag_4k = false;
//            if((s_data_len == 0) || ((s_data_len + ota_data->len) >= (((s_data_len/TUYA_NV_ERASE_MIN_SIZE) + 1)*TUYA_NV_ERASE_MIN_SIZE)))
//            {
//                if(s_data_len == 0)
//                {
//                    app_port_nv_erase(APP_OTA_START_ADDR, TUYA_NV_ERASE_MIN_SIZE);
//                }
//                uint32_t erase_addr = APP_OTA_START_ADDR + (((s_data_len/TUYA_NV_ERASE_MIN_SIZE) + 1)*TUYA_NV_ERASE_MIN_SIZE);
//                app_port_nv_erase(erase_addr, TUYA_NV_ERASE_MIN_SIZE);
//                if(s_data_len + ota_data->len == s_file.len)
//                {
//                    for(uint32_t idx=0; idx<(APP_OTA_END_ADDR - erase_addr)/TUYA_NV_ERASE_MIN_SIZE; idx++)
//                    {
//                        app_port_nv_erase(erase_addr + idx*TUYA_NV_ERASE_MIN_SIZE, TUYA_NV_ERASE_MIN_SIZE);
//                    }
//                }
//                
//                flag_4k = true;
//            }
            
            if(APP_PORT_SUCCESS != app_port_nv_write(APP_OTA_START_ADDR + s_data_len, ota_data->data, ota_data->len))
            {
                ota_data_rsp.state = 0x04; //write error
            }
            else
            {
                s_data_len += ota_data->len;
                if(s_data_len < s_file.len)
                {
                    APP_DEBUG_PRINTF("s_pkg_id: %d", s_pkg_id);
                    s_ota_state = TUYA_BLE_OTA_DATA;
                }
                else if(s_data_len == s_file.len)
                {
                    s_ota_state = TUYA_BLE_OTA_END;
                }
                else
                {
                    ota_data_rsp.state = 0x04;
                }
                s_pkg_id++;
                
                s_data_crc = app_port_crc32_compute(ota_data->data, ota_data->len, &s_data_crc);
//                if(flag_4k)
//                {
//                    app_port_nv_set(SF_AREA_0, NV_ID_OTA_DATA_LEN, &s_data_len, sizeof(uint32_t));
//                    app_port_nv_set(SF_AREA_0, NV_ID_OTA_DATA_CRC, &s_data_crc, sizeof(uint32_t));
//                }
            }
        }
        app_ota_rsp(rsp, &ota_data_rsp, sizeof(app_ota_data_rsp_t));
        
        if(ota_data_rsp.state != 0x00) {
            APP_DEBUG_PRINTF("Error: TUYA_BLE_OTA_DATA- errorid: %d", ota_data_rsp.state);
            app_ota_exit();
        }
    }
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: 
*/
static uint32_t app_ota_end_handler(uint8_t* cmd, uint16_t cmd_size, tuya_ble_ota_data_response_t* rsp)
{
    //param check
    if(s_ota_state != TUYA_BLE_OTA_END)
    {
        APP_DEBUG_PRINTF("Error: TUYA_BLE_OTA_END- s_ota_state error");
        //rsp
        app_ota_end_rsp_t end_rsp;
        memset(&end_rsp, 0x00, sizeof(app_ota_end_rsp_t));
        end_rsp.state = 0x03; //unknow error
        
        app_ota_rsp(rsp, &end_rsp, sizeof(app_ota_end_rsp_t));
        app_ota_exit();
        return APP_PORT_ERROR_COMMON;
    }

    //param check
    if((cmd_size != 0x0001) || (*cmd != 0x00))
    {
        APP_DEBUG_PRINTF("Error: TUYA_BLE_OTA_END- type error");
        //rsp
        app_ota_end_rsp_t end_rsp;
        memset(&end_rsp, 0x00, sizeof(app_ota_end_rsp_t));
        end_rsp.state = 0x03; //unknow error
        
        app_ota_rsp(rsp, &end_rsp, sizeof(app_ota_end_rsp_t));
        app_ota_exit();
        return APP_PORT_ERROR_COMMON;
    }
    
    {
        app_port_nv_del(SF_AREA_0, NV_ID_OTA_DATA_LEN);
        app_port_nv_del(SF_AREA_0, NV_ID_OTA_DATA_CRC);
        app_port_nv_del(SF_AREA_0, NV_ID_OTA_FILE_MD5);
        
        //rsp
        app_ota_end_rsp_t end_rsp;
        memset(&end_rsp, 0x00, sizeof(app_ota_end_rsp_t));
        end_rsp.type = 0x00;
        if(s_data_len != s_file.len)
        {
            end_rsp.state = 0x01; //total size error
        }
        else if(s_file.crc32 != app_ota_get_crc32_in_flash(s_data_len))
        {
            end_rsp.state = 0x02; //crc error
        }
        else
        {
            {
                uint16_t image_len; //该值在bk的固件中标识固件长度（单位：4字节），在这里做检查，防止越界
                app_port_nv_read(APP_OTA_START_ADDR+6, (void*)&image_len, sizeof(uint16_t));
                APP_DEBUG_PRINTF("image_len: %dbyte", image_len*4);
                if(image_len > (APP_OTA_FILE_MAX_LEN/4))
                {
                    tuya_ble_device_enter_critical();
                    app_port_nv_erase(APP_OTA_START_ADDR, APP_OTA_FILE_MAX_LEN);
                    tuya_ble_device_exit_critical();
                    end_rsp.state = 0x01;
                }
                else
                {
                    end_rsp.state = 0x00;
                    s_ota_success = true;
                    APP_DEBUG_PRINTF("ota success");
                }
            }
            
            end_rsp.state = 0x00;
            s_ota_success = true;
            APP_DEBUG_PRINTF("ota success");
        }
        app_ota_rsp(rsp, &end_rsp, sizeof(app_ota_end_rsp_t));
        
        app_ota_exit();
        
        if(end_rsp.state != 0x00) {
            APP_DEBUG_PRINTF("Error: TUYA_BLE_OTA_END- errorid: %d", end_rsp.state);
            app_ota_exit();
        }
    }
    return APP_PORT_SUCCESS;
}






















