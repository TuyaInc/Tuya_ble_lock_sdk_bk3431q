/**
****************************************************************************
* @file      lock_offline_pwd.h
* @brief     lock_offline_pwd
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __LOCK_OFFLINE_PWD_H__
#define __LOCK_OFFLINE_PWD_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "lock_common.h"

/*********************************************************************
 * CONSTANTS1
 */
#define OFFLINE_PWD_LEN			        (10)      //离线密码长度
#define OFFLINE_PWD_MAX_NUM		        (200)     //离线密码最大数量
#define OFFLINE_PWD_TIME_ACCURACY	    (3600u)   //离线密码时间精度-小时

//业务相关参数
#define PWD_MAX_SN_PER_PERIOD           (10)      //密码最大的数量/每周期
#define PWD_ACTIVE_PERIOD_SINGLE        (6*3600)  //单次密码激活窗口     6小时
#define PWD_ACTIVE_PERIOD_TIMELINESS    (24*3600) //时效密码激活窗口     24小时
#define PWD_ACTIVE_PERIOD_CLEAR_SINGLE  (24*3600) //清除单条密码激活窗口 24小时
#define PWD_ACTIVE_PERIOD_CLEAR_ALL     (24*3600) //清除所有密码激活窗口 24小时

typedef enum {
	PWD_TYPE_TIMELINESS     = 0,                  //时效密码
	PWD_TYPE_SINGLE         = 1,                  //单次密码
    PWD_TYPE_CLEAR_SINGLE   = 8,                  //清除单条密码
	PWD_TYPE_CLEAR_ALL      = 9,                  //清除所有密码
} enum_offline_pwd_type_t;

typedef enum {
	PWD_STATUS_UNUSED = 0,                        //未使用
	PWD_STATUS_INVALID,	                          //无效
	PWD_STATUS_VALID,		                      //有效
} enum_offline_pwd_status_t;

typedef enum {
	OFFLINE_PWD_VERIFY_SUCCESS       = (0),       //验证成功
	OFFLINE_PWD_CLEAR_SINGLE_SUCCESS = (1),       //清除成功
	OFFLINE_PWD_CLEAR_ALL_SUCCESS    = (2),       //清除成功
	OFFLINE_PWD_ERR_PARAM            = (-1),      //参数错误
	OFFLINE_PWD_ERR_DECRYPT          = (-2),      //解密错误
	OFFLINE_PWD_ERR_TYPE             = (-3),      //密码类型错误
	OFFLINE_PWD_ERR_INVALID          = (-4),      //密码状态失效
	OFFLINE_PWD_ERR_START_TIME       = (-5),      //当前时间 < 生效时间，还没开始生效
	OFFLINE_PWD_ERR_END_TIME         = (-6),      //当前时间 > 失效时间，已经失效
	OFFLINE_PWD_ERR_ACTIVE_TIME      = (-7),      //未在激活窗口内激活
    OFFLINE_PWD_ERR_BSS_SN           = (-8),      //错误的业务流水号
	OFFLINE_PWD_ERR_NO_SPACE         = (-9),      //无存储空间
	OFFLINE_PWD_ERR_NO_EXIST         = (-10),     //密码不存在
	OFFLINE_PWD_ERR_UNKNOW           = (-11),     //未知错误
} enum_offline_pwd_err_t;

/*********************************************************************
 * STRUCT
 */
#pragma pack(1)
typedef struct
{
	uint8_t type;			    // 离线密码类型
	uint8_t num_1_5_array[5];   // 起始时间 (T2-T0)/3600
	uint8_t num_6_9_array[4];   // 结束时间 (T3-T2)/3600
} lock_offline_pwd_t;

typedef struct
{
    uint8_t  status;
    uint8_t  type;
    uint32_t pwd;
} lock_offline_pwd_storage_t;

typedef struct
{
    bool found;
    uint32_t min_T;
    int pwdid;
} lock_offline_pwd_find_info_t;
#pragma pack()

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
uint32_t lock_offline_pwd_delete(int32_t pwdid);
uint32_t lock_offline_pwd_delete_all(void);
void     lock_offline_pwd_set_T0(uint32_t T0_tmp);
uint32_t lock_offline_pwd_get_T0(void);
int32_t  lock_offline_pwd_verify(uint8_t *key, uint8_t key_len,
                                    uint8_t *encrypt_pwd, uint8_t encrypt_pwd_len,
                                    uint32_t timestamp,
                                    uint8_t *plain_pwd, uint8_t *p_plain_pwd_len);

#ifdef __cplusplus
}
#endif

#endif //__LOCK_OFFLINE_PWD_H__
