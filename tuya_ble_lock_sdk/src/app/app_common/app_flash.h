/**
****************************************************************************
* @file      app_flash.h
* @brief     app_flash
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __APP_FLASH_H__
#define __APP_FLASH_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "app_common.h"

/*********************************************************************
 * CONSTANTS
 */
//define different hard's max num, default value is 10, user can change it when needed
#define HARDID_MAX_PASSWORD    10
#define HARDID_MAX_DOORCARD    10
#define HARDID_MAX_FINGER      10
#define HARDID_MAX_FACE        10
#define HARDID_MAX_TEMP_PW     10
#define HARDID_MAX_TOTAL       (HARDID_MAX_PASSWORD+HARDID_MAX_DOORCARD+HARDID_MAX_FINGER+HARDID_MAX_FACE+HARDID_MAX_TEMP_PW) //this value can not bigger then 255

#define HARD_ID_INVALID        0xFFFFFFFF

//if user need more event storage, can change this value
#define EVTID_MAX                    64

#define HARD_TIME_MAX_LEN            17
#define HARD_PASSWORD_MAX_LEN        10
#define HARD_HARD_SN_MAX_LEN         20
#define HARD_WELCOME_WORDS_MAX_LEN   50

enum {
    NV_ID_LOCK_SETTING = 0,
    NV_ID_EVT_ID,
    NV_ID_T0_STORAGE,
    NV_ID_OPEN_WITH_NOPWD_REMOTE,
    NV_ID_APP_TEST_HID_STR,
    NV_ID_APP_TEST_MAC_STR,
    NV_ID_APP_TEST_NV_IF_AUTH,
    NV_ID_OTA_FILE_MD5,
    NV_ID_OTA_DATA_LEN,
    NV_ID_OTA_DATA_CRC,
};

/*********************************************************************
 * STRUCT
 */
//hard
typedef struct
{
	uint8_t hard_type;    //1-password, 2-doorcard, 3-finger
	uint8_t admin_flag;   //1-admin, 0-member
	uint8_t member_id;
	uint8_t hard_id;
	uint8_t time[HARD_TIME_MAX_LEN];
	uint8_t valid_num;    //0x00-valid forever, 0xFF-invalid
	uint8_t password_len; //max = HARD_PASSWORD_MAX_LEN
	uint8_t password[HARD_PASSWORD_MAX_LEN];
	uint8_t freeze_state; //refer to lock_freeze_t
	uint8_t temp_pw_type;
} lock_hard_t;

typedef struct
{
    uint8_t  message_switch;
	uint8_t  door_bell;
	uint8_t  lock_volume;
	uint8_t  lock_language;
	uint8_t  welcome_words[HARD_WELCOME_WORDS_MAX_LEN];
	uint8_t  key_tone;
    uint8_t  navigation_volume;
    uint8_t  auto_lock_switch;
	uint8_t  combine_lock_switch;
	uint32_t timer_lock;
	uint32_t timer_auto_lock;
	uint8_t  finger_number;
    uint8_t  hand_lock;
    uint8_t  motor_direction;
    uint8_t  motor_torque;
    uint8_t  awayhome_arming;
} lock_settings_t;


/*********************************************************************
 * EXTERNAL VARIABLES
 */
extern uint8_t hardid_start[];
extern uint8_t hardid_max[];
extern lock_settings_t lock_settings;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
/*********************************************************  hard  *********************************************************/
uint32_t lock_get_hardid(uint8_t hard_type);
uint32_t lock_hardid_is_valid(uint32_t hardid);
uint32_t lock_get_vaild_hardid_num(uint8_t hard_type);
uint32_t lock_hard_save(lock_hard_t* hard);
uint32_t lock_hard_load(uint8_t hardid, lock_hard_t* hard);
uint32_t lock_hard_load_by_password(uint8_t password_len, uint8_t* password, lock_hard_t* hard);
uint32_t lock_hardid_load_by_memberid(uint8_t memberid, uint8_t* hardtype_array, uint8_t* hradid_array, uint8_t *hradid_num);
uint32_t lock_hard_delete(uint8_t hardid);
uint32_t lock_hard_delete_all(void);
uint32_t lock_hard_delete_all_by_memberid(uint8_t memberid);
uint32_t lock_hard_modify_all_by_memberid(uint8_t memberid, uint8_t* time);
uint32_t lock_hard_freezeorunfreeze(uint8_t hardid, uint8_t freeze_state);
uint32_t lock_hard_freezeorunfreeze_all_by_memberid(uint8_t memberid, uint8_t freeze_state);
uint32_t lock_hard_save_in_local_flash(uint8_t meth);
uint32_t lock_hard_modify_in_local_flash(uint8_t meth);

/*********************************************************  event  *********************************************************/
uint32_t lock_next_evtid(uint32_t index);
uint32_t lock_last_evtid(uint32_t index);
uint32_t lock_evtid_save(uint32_t evt_id);
uint32_t lock_evtid_load(void);
uint32_t lock_evt_save(uint32_t timestamp, uint8_t *data, uint32_t len);
uint32_t lock_evt_load(uint32_t evt_id, uint8_t *data, uint32_t len);
uint32_t lock_evt_delete(uint32_t evt_id);
uint32_t lock_evt_delete_all(void);

/*********************************************************  setting  *********************************************************/
uint32_t lock_settings_save(void);
uint32_t lock_settings_load(void);
uint32_t lock_settings_default(void);
uint32_t lock_settings_delete_and_default(void);

/*********************************************************  init  *********************************************************/
uint32_t lock_flash_erease_all(bool is_all);
uint32_t lock_flash_init(void);

#ifdef __cplusplus
}
#endif

#endif //__APP_FLASH_H__
