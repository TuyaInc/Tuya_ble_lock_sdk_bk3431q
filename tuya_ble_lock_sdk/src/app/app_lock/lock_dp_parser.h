/**
****************************************************************************
* @file      lock_dp_parser.h
* @brief     lock_dp_parser
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __LOCK_DP_PARSER_H__
#define __LOCK_DP_PARSER_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "lock_common.h"

/*********************************************************************
 * CONSTANTS
 */
//WR-write_report, OW-only_write, OR-only_report
//BSC-basic, STS-state_sync, SET-setting, RFU-remain_for_future_use
//PW-password, BT-blue_tooth
#define  WR_BSC_OPEN_METH_CREATE                  1   //基础功能-添加开锁方式
#define  WR_BSC_OPEN_METH_DELETE                  2   //基础功能-删除开锁方式
#define  WR_BSC_OPEN_METH_MODIFY                  3   //基础功能-修改开锁方式
#define  WR_BSC_OPEN_METH_FREEZE                  4   //基础功能-冻结开锁方式
#define  WR_BSC_OPEN_METH_UNFREEZE                5   //基础功能-解冻开锁方式
#define  OW_BSC_OPEN_WITH_BT                      6   //基础功能-蓝牙开锁
#define  OR_BSC_OPEN_WITH_BT_REPORT               7   //基础功能-蓝牙开锁反馈，弃用
#define  OR_STS_BATTERY_PERCENT                   8   //状态同步-电量百分比
#define  OR_STS_BATTERY_POSITION                  9   //状态同步-电量档位
#define  OR_STS_CHILD_LOCK                        10  //状态同步-童锁
#define  OR_STS_ANTI_LOCK                         11  //状态同步-上提反锁
#define  OR_LOG_OPEN_WITH_FINGER                  12  //开锁记录-指纹
#define  OR_LOG_OPEN_WITH_PW                      13  //开锁记录-普通密码
#define  OR_LOG_OPEN_WITH_DYNAMIC_PW              14  //开锁记录-动态密码
#define  OR_LOG_OPEN_WITH_CARD                    15  //开锁记录-门卡
#define  OR_LOG_OPEN_WITH_KEY                     16  //开锁记录-机械钥匙
#define  OR_LOG_DOOR_STATE                        17  //开锁记录-门的状态    //?
#define  OR_LOG_OPEN_INSIDE                       18  //开锁记录-门从内侧打开
#define  OR_LOG_OPEN_WITH_BT                      19  //开锁记录-蓝牙开锁
#define  OR_RFU_DOOR_OPENED                       20  //未被使用-门被打开
#define  OR_LOG_ALARM_REASON                      21  //警报记录-各种
#define  OR_LOG_ALARM_HIJACK                      22  //警报记录-劫持报警
#define  OR_RFU_ALARM_LOW_POWER                   23  //未被使用-低电报警
#define  OR_STS_DOORBELL_RING                     24  //状态同步-门铃提醒
#define  WR_SET_MESSAGE_SWITCH                    25  //锁的设置-短信通知
#define  WR_SET_DOOR_BELL                         26  //锁的设置-门铃音效
#define  WR_SET_LOCK_VOLUME                       27  //锁的设置-锁的音量
#define  WR_SET_LOCK_LANGUAGE                     28  //锁的设置-锁的语言切换
#define  WR_SET_WELCOME_WORDS                     29  //锁的设置-显示屏欢迎词
#define  WR_SET_KEY_TONE                          30  //锁的设置-按键音调
#define  WR_SET_NAVIGATE_VOLUME                   31  //锁的设置-锁的本地导航音量
#define  WR_STS_REVERSE_LOCK                      32  //状态同步-反锁状态
#define  WR_SET_AUTO_LOCK_SWITCH                  33  //锁的设置-自动落锁开关
#define  WR_SET_COMBINE_LOCK                      34  //锁的设置-单一开锁与组合开锁切换
#define  WR_BSC_OPEN_METH_SYNC                    35  //基础功能-同步开锁方式
#define  WR_SET_TIMER_LOCK                        36  //锁的设置-定时落锁
#define  WR_SET_TIMER_AUTO_LOCK                   37  //锁的设置-定时自动落锁
#define  WR_SET_FINGER_NUM                        38  //锁的设置-指纹录入次数
#define  OR_LOG_OPEN_WITH_FACE                    39  //开锁记录-人脸
#define  OR_STS_DOOR_STATE                        40  //状态同步-门开合状态
#define  OR_LOG_OPEN_WITH_EYE                     41  //开锁记录-虹膜
#define  OR_LOG_OPEN_WITH_PALM_PRINT              42  //开锁记录-掌纹
#define  OR_LOG_OPEN_WITH_FINGER_VEIN             43  //开锁记录-指静脉
#define  OR_STS_AUTH_LOCK_OUTTIME                 45  //状态同步-自动落锁倒计时
#define  WR_SET_HAND_LOCK                         46  //锁的设置-手动落锁
#define  OR_STS_LOCK_STATE                        47  //状态同步-锁开合状态
#define  WR_SET_MOTOR_DIRECTION                   48  //锁的设置-锁贴电机转动方向
#define  WR_SET_USER_FREEZE                       49  //锁的设置-冻结用户
#define  WR_SET_USER_UNFREEZE                     50  //锁的设置-解冻用户
#define  WR_BSC_TEMP_PW_CREAT                     51  //基础功能-添加临时密码
#define  WR_BSC_TEMP_PW_DELETE                    52  //基础功能-删除临时密码
#define  WR_BSC_TEMP_PW_MODIFY                    53  //基础功能-修改临时密码
#define  WR_BSC_OPEN_METH_SYNC_NEW                54  //基础功能-同步开锁方式（数据量大）
#define  OR_LOG_OPEN_WITH_TMP_PWD                 55  //开锁记录-临时密码
#define  WR_SET_MOTOR_TORQUE                      56  //锁的设置-电机扭力
#define  OR_LOG_OPEN_WITH_COMBINE                 57  //开锁记录-组合开锁
#define  WR_SET_AWAYHOME_ARMING_SWITCH            58  //锁的设置-离家布防
#define  WR_BSC_OPEN_WITH_NOPWD_REMOTE_SETKEY     60  //基础功能-免密远程开锁配置
#define  WR_BSC_OPEN_WITH_NOPWD_REMOTE            61  //基础功能-免密远程开锁
#define  OR_LOG_OPEN_WITH_REMOTE_PHONE            62  //开锁记录-远程手机开锁
#define  OR_LOG_OPEN_WITH_REMOTE_VOICE            63  //开锁记录-远程音箱开锁
#define  WR_BSC_SET_T0                            64  //基础功能-离线密码-T0时间下发
#define  OR_LOG_ALARM_OFFLINE_PWD_CLEAR_SINGLE    65  //警报记录-离线密码-清除单条密码
#define  OR_LOG_ALARM_OFFLINE_PWD_CLEAR_ALL       66  //警报记录-离线密码-清除所有密码
#define  OR_LOG_OPEN_WITH_OFFLINE_PWD             67  //开锁记录-离线密码


//open method
typedef enum
{
    OPEN_METH_BASE = 0,
    OPEN_METH_PASSWORD,
    OPEN_METH_DOORCARD,
    OPEN_METH_FINGER,
    OPEN_METH_FACE,
    OPEN_METH_TEMP_PW, //实际的临时密码值为0xF0，此处为方便存储演示，取值0x05
    OPEN_METH_MAX,
} open_meth_t;

//reg state
typedef enum
{
    REG_STAGE_STSRT    = 0x00,
    REG_STAGE_RUNNING  = 0xFC,
    REG_STAGE_FAILED   = 0xFD,
    REG_STAGE_CANCEL   = 0xFE,
    REG_STAGE_COMPLETE = 0xFF,
} reg_stage_t;

//reg failed reason
typedef enum
{
    REG_NOUSE_DEFAULT_VALUE  = 0x00,
    REG_FAILD_OUTTIME  = 0x00,
    REG_FAILD_FAILED   = 0x01,
    REG_FAILD_REPEAT   = 0x02,
    REG_FAILD_NO_HARDID  = 0x03,
    REG_FAILD_INVALID_PW  = 0x04,
    REG_FAILD_INVALID_PW_LEN  = 0x05,
    REG_FAILD_INVALID_OPEN_METH  = 0x06,
    REG_FAILD_FINGER_RUNNING  = 0x07,
    REG_FAILD_CARD_RUNNING  = 0x08,
    REG_FAILD_FACE_RUNNING  = 0x08,
    REG_FAILD_WONG_NUMBER  = 0xFE,
} reg_failed_reason_t;

//reg abnormal reason
typedef enum
{
    REG_ABNORMAL_NONE = 0x00,
    REG_ABNORMAL_FP_INCOMPLETE = 0x01,
} reg_abnormal_reason_t;

//alarm reason
typedef enum
{
    ALARM_WRONG_FINGER = 0,
    ALARM_WRONG_PASSWORD,
    ALARM_WRONG_CARD,
    ALARM_WRONG_FACE,
    ALARM_TONGUE_BAD,
    ALARM_TOO_HOT,          //0x05
    ALARM_UNLOCKED_OUTTIME,
    ALARM_TONGUE_NOT_OUT,
    ALARM_PRY,
    ALARM_KEY_IN,
    ALARM_LOW_BATTERY,      //0x0A
    ALARM_POWER_OFF,
    ALARM_SHOCK,
} alarm_reason_t;

//
typedef enum
{
    FREEZE_ON = 0,
    FREEZE_OFF = 1,
} lock_freeze_t;

#define OPEN_WITH_NOPWD_REMOTE_KEY "nopwd_remote"

/*********************************************************************
 * STRUCT
 */
#pragma pack(1)
//create open method
typedef struct
{
    open_meth_t meth;
    reg_stage_t stage;
    uint8_t admin_falg;
    uint8_t memberid;
    uint8_t hardid;
    uint8_t time[17];
    uint8_t valid_num;
    uint8_t password_len;
    uint8_t password[10];
} open_meth_creat_t;
typedef struct
{
    open_meth_t meth;
    reg_stage_t stage;
    uint8_t admin_falg;
    uint8_t memberid;
    uint8_t hardid;
    uint8_t reg_num;
    uint8_t result;
} open_meth_creat_result_t;

//delete open method
typedef struct
{
    open_meth_t meth;   
    reg_stage_t stage;  
    uint8_t admin_falg; 
    uint8_t memberid;   
    uint8_t hardid;     
    uint8_t delete_style;
} open_meth_delete_t;
typedef struct
{
    open_meth_t meth;   
    reg_stage_t stage;  
    uint8_t admin_falg; 
    uint8_t memberid;   
    uint8_t hardid;     
    uint8_t delete_style;
    uint8_t result;     
} open_meth_delete_result_t;

//modify open method
typedef struct
{
    open_meth_t meth;    
    reg_stage_t stage;   
    uint8_t admin_falg;  
    uint8_t memberid;    
    uint8_t hardid;      
    uint8_t time[17];    
    uint8_t cycle;       
    uint8_t password_len;
    uint8_t password[10];
} open_meth_modify_t;
typedef struct
{
    open_meth_t meth;
    reg_stage_t stage;
    uint8_t amin_falg;
    uint8_t memberid; 
    uint8_t hardid;   
    uint8_t cycle;    
    uint8_t result;   
} open_meth_modify_result_t;

//create temp pw
typedef struct
{
    uint8_t type;
    uint8_t time[17];
    uint8_t valid_num;
    uint8_t password_len;
    uint8_t password[10];
} temp_pw_creat_t;
typedef struct
{
    uint8_t hardid;
    uint8_t result;
} temp_pw_creat_result_t;

//delete temp pw
typedef struct
{
    uint8_t hardid;
} temp_pw_delete_t;
typedef struct
{
    uint8_t hardid;
    uint8_t result;
} temp_pw_delete_result_t;

//modify temp pw
typedef struct
{
    uint8_t hardid;
    uint8_t type;
    uint8_t time[17];
    uint8_t valid_num;
    uint8_t password_len;
    uint8_t password[10];
} temp_pw_modify_t;
typedef struct
{
    uint8_t hardid;
    uint8_t result;
} temp_pw_modify_result_t;

//open with nopwd remote setkey
typedef struct
{
    uint8_t  valid;
    uint16_t memberid;
    uint32_t time_begin;
    uint32_t time_end;
    uint16_t valid_num;
    uint8_t  password[8];
} open_with_nopwd_remote_setkey_t;
typedef struct
{
    uint8_t  result;
    uint16_t memberid;
} open_with_nopwd_remote_setkey_result_t;

//open with nopwd remote
typedef struct
{
    uint8_t  open;
    uint16_t memberid;
    uint8_t  password[8];
    uint8_t  type;
} open_with_nopwd_remote_t;
typedef struct
{
    uint8_t  result;
    uint16_t memberid;
} open_with_nopwd_remote_result_t;

//sync open method
// - hard_attribute
typedef struct
{
    uint8_t memberid;
    uint8_t freeze_state;
} open_meth_sync_hard_attribute_t;
typedef struct
{
    uint8_t hardid;
    uint8_t hard_type;
    uint8_t hard_crc8;
} open_meth_sync_node_t;
typedef struct
{
    uint8_t op_type;
    uint8_t hardid;
    uint8_t hard_type;
    open_meth_sync_hard_attribute_t hard_attribute;
} open_meth_sync_node_result_t;

//sync open method new
typedef struct
{
    uint8_t hardid;
    uint8_t hard_type;
    open_meth_sync_hard_attribute_t hard_attribute;
} open_meth_sync_node_new_t;
typedef struct
{
    uint8_t stage;
    uint8_t pkgs;
} open_meth_sync_new_last_result_t;
typedef struct
{
    uint8_t type;
    uint8_t idx;
    uint8_t count;
} open_meth_sync_new_hard_t;
typedef struct
{
    uint8_t flag;
    uint8_t hard_type_len;
    uint8_t idx;
    uint8_t pkg_count;
    open_meth_sync_new_hard_t hard[OPEN_METH_MAX];
} open_meth_sync_new_t;

//open with bt
typedef struct
{
    uint8_t open;
    uint8_t memberid;
} open_meth_with_bt_t;
typedef struct
{
    uint8_t result;
    uint8_t memberid;
} open_meth_with_bt_result_t;


//dp point
typedef struct
{
    uint8_t dp_id;
    uint8_t dp_type;
    uint8_t dp_data_len;
    union
    {
        uint8_t dp_data[256];
        open_meth_creat_t open_meth_creat;
        open_meth_creat_result_t open_meth_creat_result;
        open_meth_modify_t open_meth_modify;
        open_meth_modify_result_t open_meth_modify_result;
        open_meth_delete_t open_meth_delete;
        open_meth_delete_result_t open_meth_delete_result;
        temp_pw_creat_t temp_pw_creat;
        temp_pw_creat_result_t temp_pw_creat_result;
        temp_pw_delete_t temp_pw_delete;
        temp_pw_delete_result_t temp_pw_delete_result;
        temp_pw_modify_t temp_pw_modify;
        temp_pw_modify_result_t temp_pw_modify_result;
        open_meth_with_bt_t open_meth_with_bt;
        open_meth_with_bt_result_t open_meth_with_bt_result;
    };
} lock_dp_t;
#pragma pack()

/*********************************************************************
 * EXTERNAL VARIABLES
 */
extern lock_dp_t g_cmd;
extern lock_dp_t g_rsp;
extern volatile open_meth_sync_new_t g_sync_new;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
uint32_t lock_dp_parser_handler(void* dp_data);


#ifdef __cplusplus
}
#endif

#endif //__LOCK_DP_PARSER_H__
