#include "lock_offline_pwd.h"
#include "fpe_decrypt.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static uint32_t T0 = 1589251799;
static volatile bool is_T0_updated = true;

/*********************************************************************
 * LOCAL FUNCTION
 */
static uint32_t lock_offline_pwd_save(int32_t pwdid, lock_offline_pwd_storage_t* pwd_storage);
static uint32_t lock_offline_pwd_load(int32_t pwdid, lock_offline_pwd_storage_t* pwd_storage);
static void lock_offline_pwd_calculate_T2_T3(lock_offline_pwd_t *pwd, uint32_t* pT2, uint32_t* pT3);
static void lock_offline_pwd_storage_calculate_T2_T3(lock_offline_pwd_storage_t* pwd_storage, uint32_t* pT2, uint32_t* pT3);
static int32_t lock_offline_pwd_find(uint32_t T_now);
static int32_t is_offline_pwd_exist(enum_offline_pwd_type_t type, uint32_t pwd_integer, lock_offline_pwd_storage_t *data);
static int32_t lock_offline_pwd_clear(enum_offline_pwd_type_t type, uint32_t pwd);

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: 
*/
static uint32_t lock_offline_pwd_save(int32_t pwdid, lock_offline_pwd_storage_t* pwd_storage)
{
    if(pwdid >= OFFLINE_PWD_MAX_NUM) {
        return APP_PORT_ERROR_COMMON;
    }
    
	uint32_t err_code = app_port_nv_set(SF_AREA_3, pwdid, pwd_storage, sizeof(lock_offline_pwd_storage_t));
	if(err_code == APP_PORT_SUCCESS) {
        return APP_PORT_SUCCESS;
	}
    return APP_PORT_ERROR_COMMON;
}

/*********************************************************
FN: 
*/
static uint32_t lock_offline_pwd_load(int32_t pwdid, lock_offline_pwd_storage_t* pwd_storage)
{
    if(pwdid >= OFFLINE_PWD_MAX_NUM) {
        return APP_PORT_ERROR_COMMON;
    }
    
	uint32_t err_code = app_port_nv_get(SF_AREA_3, pwdid, pwd_storage, sizeof(lock_offline_pwd_storage_t));
	if(err_code == APP_PORT_SUCCESS) {
        return APP_PORT_SUCCESS;
    }
	return APP_PORT_ERROR_COMMON;
}

/*********************************************************
FN: 
*/
uint32_t lock_offline_pwd_delete(int32_t pwdid)
{
    if(pwdid >= OFFLINE_PWD_MAX_NUM) {
        return APP_PORT_ERROR_COMMON;
    }
    
	uint32_t err_code = app_port_nv_del(SF_AREA_3, pwdid);
	if(err_code == APP_PORT_SUCCESS) {
        return APP_PORT_SUCCESS;
	}
    return APP_PORT_ERROR_COMMON;
}

/*********************************************************
FN: 
*/
uint32_t lock_offline_pwd_delete_all(void)
{
    for (int32_t idx=0; idx<OFFLINE_PWD_MAX_NUM; idx++)
    {
        lock_offline_pwd_delete(idx);
    }
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: 
*/
void lock_offline_pwd_set_T0(uint32_t T0_tmp)
{
    if (T0_tmp == lock_offline_pwd_get_T0()) {
        APP_DEBUG_PRINTF("set T0, but same->[%d]", T0);
        return;
    }
    
	T0 = T0_tmp;
    app_port_nv_set(SF_AREA_0, NV_ID_T0_STORAGE, &T0, sizeof(uint32_t));
    APP_DEBUG_PRINTF("set T0->[%d]", T0);
    
    is_T0_updated = true;
}

/*********************************************************
FN: 
*/
uint32_t lock_offline_pwd_get_T0(void)
{
    if (is_T0_updated) {
        app_port_nv_get(SF_AREA_0, NV_ID_T0_STORAGE, &T0, sizeof(uint32_t));
        APP_DEBUG_PRINTF("get T0->[%d]", T0);
        is_T0_updated = false;
    }
    return T0;
}

/*********************************************************
FN: 
*/
static void lock_offline_pwd_calculate_T2_T3(lock_offline_pwd_t *pwd, uint32_t* pT2, uint32_t* pT3)
{
	uint32_t num_1_5 = 0;
    uint32_t num_6_9 = 0;
	uint32_t T0_round;
    uint32_t T2 = 0;
    uint32_t T3 = 0;

    num_1_5 = app_port_num_array_2_int(pwd->num_1_5_array, 0, 5);
    num_6_9 = app_port_num_array_2_int(pwd->num_6_9_array, 0, 4);

    // T0 = T0 - (T0 % 3600)
    // num_1_5 = (T2 - T0)/3600      T2 = num_1_5*3600 + T0
	// num_6_9 = (T3 - T2)/3600      T3 = num_6_9*3600 + T2
    T0_round = T0 - (T0 % 3600);
    T2 = num_1_5*OFFLINE_PWD_TIME_ACCURACY + T0_round;
    if(pwd->type == PWD_TYPE_TIMELINESS || pwd->type == PWD_TYPE_CLEAR_SINGLE) {
        T3 = (num_6_9*OFFLINE_PWD_TIME_ACCURACY + T2);
    } else if (pwd->type == PWD_TYPE_SINGLE || pwd->type == PWD_TYPE_CLEAR_ALL) {
        T3 = num_6_9;
    }

	*pT2 = T2;
	*pT3 = T3;
    
    
    //打印 T2、T3，当前时间
    tuya_ble_time_struct_data_t datatime;
    
    tuya_ble_utc_sec_2_mytime(T2, &datatime, 0);
	APP_DEBUG_PRINTF("T2->[%02d-%02d-%02d %02d:xx:xx]", datatime.nYear, datatime.nMonth, datatime.nDay, datatime.nHour);
    
    if (pwd->type == PWD_TYPE_TIMELINESS || pwd->type == PWD_TYPE_CLEAR_SINGLE) {
        tuya_ble_utc_sec_2_mytime(T3, &datatime, 0);
        APP_DEBUG_PRINTF("T3->[%02d-%02d-%02d %02d:xx:xx]", datatime.nYear, datatime.nMonth, datatime.nDay, datatime.nHour);
    } else {
        APP_DEBUG_PRINTF("C3->[%d]", T3);
    }
    
    tuya_ble_utc_sec_2_mytime(app_port_get_timestamp(), &datatime, 0);
	APP_DEBUG_PRINTF("NOW->[%02d-%02d-%02d %02d:xx:xx]", datatime.nYear, datatime.nMonth, datatime.nDay, datatime.nHour);
}

/*********************************************************
FN: 
*/
static void lock_offline_pwd_storage_calculate_T2_T3(lock_offline_pwd_storage_t* pwd_storage, uint32_t* pT2, uint32_t* pT3)
{
	uint32_t num_1_5 = 0;
    uint32_t num_6_9 = 0;
	uint32_t T0_round;
    uint32_t T2 = 0;
    uint32_t T3 = 0;
   
    num_1_5 = (pwd_storage->pwd / 10000);
    num_6_9 = (pwd_storage->pwd % 10000);
    
    // T0 = T0 - (T0 % 3600)
    // num_1_5 = (T2 - T0)/3600      T2 = num_1_5*3600 + T0
	// num_6_9 = (T3 - T2)/3600      T3 = num_6_9*3600 + T2
    T0_round = T0 - (T0 % 3600);
    T2 = num_1_5*OFFLINE_PWD_TIME_ACCURACY + T0_round;
    if (pwd_storage->type == PWD_TYPE_TIMELINESS) {
        T3 = (num_6_9*OFFLINE_PWD_TIME_ACCURACY + T2);
    } else if (pwd_storage->type == PWD_TYPE_SINGLE) {
        T3 = T2 + PWD_ACTIVE_PERIOD_SINGLE;
    } else if (pwd_storage->type == PWD_TYPE_CLEAR_ALL) {
        T3 = T2 + PWD_ACTIVE_PERIOD_CLEAR_ALL;
    }
    
	*pT2 = T2;
	*pT3 = T3;
}

/*********************************************************
FN: 
*/
static int32_t lock_offline_pwd_find(uint32_t T_now)
{
    uint32_t T2, T3;
	int32_t  find_pwdid = -1;
    bool find_flag = false;
    lock_offline_pwd_find_info_t info_invalid_pwd;
    lock_offline_pwd_find_info_t info_valid_pwd;
    lock_offline_pwd_find_info_t info_single_pwd;
    lock_offline_pwd_find_info_t info_clear_all;
    
    // 离线密码存储优先级
    // 1.未使用的
    // 2.过期的（T3 < T_now），直接覆盖
    // 3.时效密码中密码状态为无效的，找失效时间最早的
    // 4.时效密码中密码状态为有效的，找生效时间最早的
    // 5.如都是单次密码，找失效时间最早的
    // 6.如都是清空所有密码，找失效时间最早的
    info_invalid_pwd.found = false;
    info_valid_pwd.found   = false;
    info_single_pwd.found  = false;
    info_clear_all.found = false;
    
    for (int32_t idx=0; idx<OFFLINE_PWD_MAX_NUM; idx++)
    {
        lock_offline_pwd_storage_t pwd_storage = {0};
        lock_offline_pwd_load(idx, &pwd_storage);
        
        //未使用的
		if (pwd_storage.status == PWD_STATUS_UNUSED)
        {
			find_pwdid = idx;
			find_flag = true;
            APP_DEBUG_PRINTF("find unused pwdid->[%d]", idx);
            break;
		}
        else
        {
            lock_offline_pwd_storage_calculate_T2_T3(&pwd_storage, &T2, &T3);
            
            //过期的
            if (T3 < T_now) {
                find_pwdid = idx;
			    find_flag = true;
                APP_DEBUG_PRINTF("find outtime(T3<T_now) pwdid->[%d]", idx);
                break;
            }
            //时效密码
            else if (pwd_storage.type == PWD_TYPE_TIMELINESS) {
                //密码状态为无效的，找失效时间最早的
                if (pwd_storage.status == PWD_STATUS_INVALID) {
                    if (!info_invalid_pwd.found) {
                        info_invalid_pwd.found = true;
                        info_invalid_pwd.min_T = T3;
                        info_invalid_pwd.pwdid = idx;
                    } else if (T3 < info_invalid_pwd.min_T) {
                        info_invalid_pwd.min_T = T3;
                        info_invalid_pwd.pwdid = idx;
                    }
                }
                //密码状态为有效的，找生效时间最早的
                else {
                    if (!info_valid_pwd.found) {
                        info_valid_pwd.found = true;
                        info_valid_pwd.min_T = T2;
                        info_valid_pwd.pwdid = idx;
                    } else if (T2 < info_valid_pwd.min_T) {
                        info_valid_pwd.min_T = T2;
                        info_valid_pwd.pwdid = idx;
                    }
                }
            }
            //如都是单次密码，找失效时间最早的
            else if (pwd_storage.type == PWD_TYPE_SINGLE) {
                if (!info_single_pwd.found) {
                    info_single_pwd.found = true;
                    info_single_pwd.min_T = T3;
                    info_single_pwd.pwdid = idx;
                } else if (T3 < info_single_pwd.min_T) {
                    info_single_pwd.min_T = T3;
                    info_single_pwd.pwdid = idx;
                }
            }
            //如都是清空所有密码，找失效时间最早的
            else if (pwd_storage.type == PWD_TYPE_CLEAR_ALL) {
                if (!info_clear_all.found) {
                    info_clear_all.found = true;
                    info_clear_all.min_T = T3;
                    info_clear_all.pwdid = idx;
                } else if (T3 < info_clear_all.min_T) {
                    info_clear_all.min_T = T3;
                    info_clear_all.pwdid = idx;
                }
            }
        }
	}

    if (!find_flag) {
        if (info_invalid_pwd.found) {
            find_pwdid = info_invalid_pwd.pwdid;
        }
        else if (info_valid_pwd.found) {
            find_pwdid = info_valid_pwd.pwdid;
        }
        else if (info_single_pwd.found) {
            find_pwdid = info_single_pwd.pwdid;
        }
        else if (info_clear_all.found) {
            find_pwdid = info_clear_all.pwdid;
        }
        APP_DEBUG_PRINTF("all pwdid full, cover id->[%d]", find_pwdid);
    }
	return find_pwdid;
}

/*********************************************************
FN: 
RT: >= 0 exist
    -1
    -2
*/
static int32_t is_offline_pwd_exist(enum_offline_pwd_type_t type, uint32_t pwd_int, lock_offline_pwd_storage_t *data)
{
	for (int32_t idx=0; idx<OFFLINE_PWD_MAX_NUM; idx++)
    {
        lock_offline_pwd_storage_t pwd_storage = {0};
        lock_offline_pwd_load(idx, &pwd_storage);
        
		if ((pwd_storage.status != PWD_STATUS_UNUSED)
            && (pwd_storage.type == type)
            && (pwd_storage.pwd == pwd_int)) {
            
			memcpy(data, &pwd_storage, sizeof(lock_offline_pwd_storage_t));
            return idx;
		}
	}
	return -1;
}

/*********************************************************
FN: 
*/
static int32_t lock_offline_pwd_clear(enum_offline_pwd_type_t type, uint32_t pwd)
{
    if ((type != PWD_TYPE_CLEAR_SINGLE) && (type != PWD_TYPE_CLEAR_ALL)) {
        APP_DEBUG_PRINTF("Warning! clear offline pwd, but type err");
        return OFFLINE_PWD_ERR_PARAM;
    }
    
    bool clear_all_flag = false;
    
    for (int32_t idx=0; idx<OFFLINE_PWD_MAX_NUM; idx++)
    {
        lock_offline_pwd_storage_t pwd_storage = {0};
        lock_offline_pwd_load(idx, &pwd_storage);
        
        //清除单条密码
        if(type == PWD_TYPE_CLEAR_SINGLE && pwd_storage.pwd == pwd)
        {
            if(pwd_storage.status == PWD_STATUS_VALID) {
                pwd_storage.status = PWD_STATUS_INVALID;
                lock_offline_pwd_save(idx, &pwd_storage);
                
                APP_DEBUG_PRINTF("OFFLINE_PWD_CLEAR_SINGLE_SUCCESS");
                return OFFLINE_PWD_CLEAR_SINGLE_SUCCESS;
            }
            else if (pwd_storage.status == PWD_STATUS_INVALID) {
                APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_INVALID");
                return OFFLINE_PWD_ERR_INVALID;
            }
        }
        //清除所有密码
        else if (type == PWD_TYPE_CLEAR_ALL)
        {
            if(pwd_storage.status == PWD_STATUS_VALID) {
                clear_all_flag = true;
                pwd_storage.status = PWD_STATUS_INVALID;
                lock_offline_pwd_save(idx, &pwd_storage);
            }
        }
	}
    
    if(type == PWD_TYPE_CLEAR_SINGLE) {
        APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_NO_EXIST");
        return OFFLINE_PWD_ERR_NO_EXIST;
    }
    else {
        if(clear_all_flag) {
            APP_DEBUG_PRINTF("OFFLINE_PWD_CLEAR_ALL_SUCCESS");
            return OFFLINE_PWD_CLEAR_ALL_SUCCESS;
        }
        else {
            APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_NO_EXIST");
            return OFFLINE_PWD_ERR_NO_EXIST;
        }
    }
}

/*********************************************************
FN: 
PM: key - 16字节无符号整数，默认填充字符‘0’，在结尾填充“tuya_ble_current_para.sys_settings.login_key”（6字节），例如login_key为‘8f7d54’，则key为‘00000000008f7d54’
    key_len - 16字节
    encrypt_pwd - 离线密码（用户看到的密码）
    encrypt_pwd_len - 10字节
    timestamp - 当前时间戳
    plain_pwd - 明文离线密码
    p_plain_pwd_len - 10字节
*/
int32_t lock_offline_pwd_verify(uint8_t *key, uint8_t key_len,
                                    uint8_t *encrypt_pwd, uint8_t encrypt_pwd_len,
                                    uint32_t timestamp,
                                    uint8_t *plain_pwd, uint8_t *p_plain_pwd_len)
{
	uint8_t decrypt_pwd[OFFLINE_PWD_LEN] = {0};
    uint8_t decrypt_pwd_len;
    
	uint32_t T2, T3, T_now = timestamp;
	uint32_t active_period;
    
    //输入参数检查
	if ((key == NULL)
            || (key_len != 16 && key_len != 24 && key_len != 32)
            || (encrypt_pwd == NULL)
            || (encrypt_pwd_len != OFFLINE_PWD_LEN)) {
		APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_PARAM");
		return OFFLINE_PWD_ERR_PARAM;
	}

    //fpe解密
	if (fpe_decrypt(key, key_len, encrypt_pwd, encrypt_pwd_len, decrypt_pwd, &decrypt_pwd_len)) {
		APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_DECRYPT");
		return OFFLINE_PWD_ERR_DECRYPT;
	}
    //参数检查
    if (decrypt_pwd_len > OFFLINE_PWD_LEN) {
		APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_UNKNOW");
        return OFFLINE_PWD_ERR_UNKNOW;
    }
    
    //提取明文密码
    if (plain_pwd != NULL) {
        memcpy(plain_pwd, decrypt_pwd, decrypt_pwd_len);
        *p_plain_pwd_len = decrypt_pwd_len;
    }
    
    //离线密码类型检查
	lock_offline_pwd_t *pwd = (void*)decrypt_pwd;
    APP_DEBUG_HEXDUMP("decrypt_pwd", (void*)pwd, sizeof(lock_offline_pwd_t));
	if (pwd->type != PWD_TYPE_TIMELINESS
            && pwd->type != PWD_TYPE_SINGLE
            && pwd->type != PWD_TYPE_CLEAR_SINGLE
            && pwd->type != PWD_TYPE_CLEAR_ALL) {
		APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_TYPE");
		return OFFLINE_PWD_ERR_TYPE;
	}
	
    //读取T0
    lock_offline_pwd_get_T0();
    //计算T2、T3
	lock_offline_pwd_calculate_T2_T3(pwd, &T2, &T3);
    
    
	uint32_t num_1_9 = app_port_num_array_2_int(decrypt_pwd, 1, 9);
	APP_DEBUG_PRINTF("offline pwd info, type->[%d] num_1_9->[%09d]", pwd->type, num_1_9);
    //时效/单次密码
    if (pwd->type == PWD_TYPE_TIMELINESS || pwd->type == PWD_TYPE_SINGLE)
    {
        //时效密码
        if(pwd->type == PWD_TYPE_TIMELINESS) {
            if (T2 > T_now) {
                APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_START_TIME");
        	    return OFFLINE_PWD_ERR_START_TIME;
            }
        	else if (T3 < T_now) {
                APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_END_TIME");
        		return OFFLINE_PWD_ERR_END_TIME;
            }
            active_period = PWD_ACTIVE_PERIOD_TIMELINESS;
        }
        //单次密码
        else if(pwd->type == PWD_TYPE_SINGLE) {
            if (T2 > T_now) {
                APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_START_TIME");
        		return OFFLINE_PWD_ERR_START_TIME;
            }
            else if (T3 > PWD_MAX_SN_PER_PERIOD) {
                APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_BSS_SN");
                return OFFLINE_PWD_ERR_BSS_SN;
            }
            active_period = PWD_ACTIVE_PERIOD_SINGLE;
        }

        int32_t exist_pwdid = 0;
        lock_offline_pwd_storage_t pwd_storage = {0};
        exist_pwdid = is_offline_pwd_exist((enum_offline_pwd_type_t)pwd->type, num_1_9, &pwd_storage);
        APP_DEBUG_PRINTF("is_offline_pwd_exist, pwdid-->[%d]", exist_pwdid);
        //离线密码已经存在
        if (exist_pwdid >= 0) {
            if(pwd_storage.status == PWD_STATUS_INVALID) {
                APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_INVALID");
                return OFFLINE_PWD_ERR_INVALID;
            } else{
                APP_DEBUG_PRINTF("OFFLINE_PWD_VERIFY_SUCCESS");
                return OFFLINE_PWD_VERIFY_SUCCESS;
            }
        }
        //离线密码超过激活周期
		else if ((T_now - T2) > active_period) {
            APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_ACTIVE_TIME");
			return OFFLINE_PWD_ERR_ACTIVE_TIME;
        }
        //存储离线密码
		else {
            APP_DEBUG_PRINTF("lock_offline_pwd_find");
            int32_t find_pwdid = lock_offline_pwd_find(T_now);
			if (find_pwdid < 0) {
				APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_NO_SPACE");
				return OFFLINE_PWD_ERR_NO_SPACE;
			}
			
			pwd_storage.status = (pwd->type == PWD_TYPE_TIMELINESS) ? (PWD_STATUS_VALID) : (PWD_STATUS_INVALID); //单次密码使用一次后就失效
			pwd_storage.type   = pwd->type;
			pwd_storage.pwd    = num_1_9;
            lock_offline_pwd_save(find_pwdid, &pwd_storage);
            
            APP_DEBUG_PRINTF("OFFLINE_PWD_VERIFY_SUCCESS");
			return OFFLINE_PWD_VERIFY_SUCCESS;
        }
    }
    //清除单条密码
    else if (pwd->type == PWD_TYPE_CLEAR_SINGLE)
    {
        //检查时效性
        if (T2 > T_now) {
            APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_NO_EXIST");
            return OFFLINE_PWD_ERR_NO_EXIST;
        }
        else if (T3 < T_now) {
            APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_END_TIME");
            return OFFLINE_PWD_ERR_END_TIME;
        }
        
        return lock_offline_pwd_clear(PWD_TYPE_CLEAR_SINGLE, num_1_9);
    }
    //清除所有密码
    else if (pwd->type == PWD_TYPE_CLEAR_ALL)
    {
        if (T2 > T_now) {
            APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_START_TIME");
			return OFFLINE_PWD_ERR_START_TIME;
        }
        //离线密码超过激活周期
		else if ((T_now - T2) > PWD_ACTIVE_PERIOD_CLEAR_ALL) {
            APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_ACTIVE_TIME");
			return OFFLINE_PWD_ERR_ACTIVE_TIME;
        }
		else if (T3 > PWD_MAX_SN_PER_PERIOD) {
            APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_BSS_SN");
            return OFFLINE_PWD_ERR_BSS_SN;
        }
        
        int32_t exist_pwdid = 0;
        lock_offline_pwd_storage_t pwd_storage = {0};
        exist_pwdid = is_offline_pwd_exist((enum_offline_pwd_type_t)pwd->type, num_1_9, &pwd_storage);
        APP_DEBUG_PRINTF("is_offline_pwd_exist, pwdid-->[%d]", exist_pwdid);
        //离线密码已经存在
        if (exist_pwdid >= 0) {
            APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_INVALID");
            return OFFLINE_PWD_ERR_INVALID;
        }
        //存储离线密码
		else {
            int32_t find_pwdid = lock_offline_pwd_find(T_now);
			if (find_pwdid < 0) {
				APP_DEBUG_PRINTF("OFFLINE_PWD_ERR_NO_SPACE");
				return OFFLINE_PWD_ERR_NO_SPACE;
			}
			
			pwd_storage.status = PWD_STATUS_INVALID; //清空所有密码使用一次后就失效
			pwd_storage.type   = pwd->type;
			pwd_storage.pwd    = num_1_9;
            lock_offline_pwd_save(find_pwdid, &pwd_storage);
            
            return lock_offline_pwd_clear(PWD_TYPE_CLEAR_ALL, 0);
        }
    }
    
    //Can't reach here
	return OFFLINE_PWD_ERR_UNKNOW;
}
