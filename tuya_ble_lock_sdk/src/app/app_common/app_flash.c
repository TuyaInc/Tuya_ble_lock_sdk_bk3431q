#include "app_flash.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */
/* Attention: hard type in this file = open meth in "lock_dp_parser.h" */

//define different hard's start id
//if you need to add a new hard, you must define its start id here,
//and you must add its start id to "hardid_start" array.
#define HARDID_START_PASSWORD  0
#define HARDID_START_DOORCARD  (HARDID_START_PASSWORD+HARDID_MAX_PASSWORD)
#define HARDID_START_FINGER    (HARDID_START_DOORCARD+HARDID_MAX_DOORCARD)
#define HARDID_START_FACE      (HARDID_START_FINGER+HARDID_MAX_FINGER)
#define HARDID_START_TEMP_PW   (HARDID_START_FACE+HARDID_MAX_FACE)
#define HARDID_START_MAX       (HARDID_START_TEMP_PW+HARDID_MAX_TEMP_PW)

//hardid to hardid bitmap
#define SELECTBIT(hardid)      (hardid_bitmap[hardid/8] &   (1<<hardid%8))
#define SETBIT(hardid)         (hardid_bitmap[hardid/8] |=  (1<<hardid%8))
#define CLEARBIT(hardid)       (hardid_bitmap[hardid/8] &= ~(1<<hardid%8))

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
//max hard number = 8*32 = 256
static volatile uint8_t hardid_bitmap[32];

static uint32_t s_evt_id = 0;

static uint8_t hardid_array[HARDID_MAX_TOTAL];
static uint8_t hardtype_array[HARDID_MAX_TOTAL];

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */
//open method(hard type) to hard start id
uint8_t hardid_start[] = 
{
    0,
    HARDID_START_PASSWORD,
    HARDID_START_DOORCARD,
    HARDID_START_FINGER,
    HARDID_START_FACE,
    HARDID_START_TEMP_PW,
    HARDID_START_MAX,
};
uint8_t hardid_max[] = 
{
    0,
    HARDID_MAX_PASSWORD,
    HARDID_MAX_DOORCARD,
    HARDID_MAX_FINGER,
    HARDID_MAX_FACE,
    HARDID_MAX_TEMP_PW,
};

lock_settings_t lock_settings;




/*********************************************************  hard  *********************************************************/

/*********************************************************
FN: get a new hardid by hard_type
PM: hard_type - open_meth_t
*/
uint32_t lock_get_hardid(uint8_t hard_type)
{
    if(hard_type >= OPEN_METH_MAX)
    {
        return HARD_ID_INVALID;
    }
    
    for(uint32_t hardid=hardid_start[hard_type]; hardid<hardid_start[hard_type+1]; hardid++)
    {
        //not valid
		if(!SELECTBIT(hardid))
		{
            return hardid;
        }
    }
    return HARD_ID_INVALID;
}

/*********************************************************
FN: 
*/
uint32_t lock_hardid_is_valid(uint32_t hardid)
{
    if(hardid >= HARDID_MAX_TOTAL)
    {
        return 0; //invalid
    }
	return SELECTBIT(hardid);
}

/*********************************************************
FN: lookup valid hardid num by hard_type
PM: hard_type - open_meth_t
*/
uint32_t lock_get_vaild_hardid_num(uint8_t hard_type)
{
    if(hard_type >= OPEN_METH_MAX)
    {
        return APP_PORT_SUCCESS;
    }
    
    uint32_t num = 0;
    for(uint32_t hardid=hardid_start[hard_type]; hardid<hardid_start[hard_type+1]; hardid++)
    {
        //valid
		if(SELECTBIT(hardid))
		{
            num++;
        }
    }
    return num;
}


/*********************************************************
FN: 
*/
uint32_t lock_hard_save(lock_hard_t* hard)
{
    uint8_t hardid = hard->hard_id;
    if(hardid >= HARDID_MAX_TOTAL) {
        return 1; //error
    }
    
	uint32_t err_code = app_port_nv_set(SF_AREA_1, hardid, hard, sizeof(lock_hard_t));
	if(err_code == APP_PORT_SUCCESS) {
        SETBIT(hardid);
        return APP_PORT_SUCCESS;
	}
    return APP_PORT_ERROR_COMMON;
}

/*********************************************************
FN: 
*/
uint32_t lock_hard_load(uint8_t hardid, lock_hard_t* hard)
{
    if(hardid >= HARDID_MAX_TOTAL) {
        return APP_PORT_ERROR_COMMON;
    }
	
	return app_port_nv_get(SF_AREA_1, hardid, hard, sizeof(lock_hard_t));
}

/*********************************************************
FN: 
*/
uint32_t lock_hard_load_by_password(uint8_t password_len, uint8_t* password, lock_hard_t* hard)
{
    if(password_len == 0)
    {
        return APP_PORT_ERROR_COMMON;
    }
    if(password == NULL)
    {
        return APP_PORT_ERROR_COMMON;
    }
    
    for(uint32_t hardid=hardid_start[OPEN_METH_PASSWORD]; hardid<hardid_start[OPEN_METH_PASSWORD+1]; hardid++)
    {
        //valid
		if(SELECTBIT(hardid))
		{
            lock_hard_t data;
            //load hard member
            if(lock_hard_load(hardid, &data) == 0)
            {
                if((data.password_len == password_len) && (memcmp(data.password, password, password_len) == 0))
                {
                    if(hard != NULL)
                    {
                        memcpy(hard, &data, sizeof(lock_hard_t));
                    }
                    return APP_PORT_SUCCESS;
                }
            }
        }
    }
	return APP_PORT_ERROR_COMMON; //not found
}

/*********************************************************
FN: 
*/
uint32_t lock_hardid_load_by_memberid(uint8_t memberid, uint8_t* hardtype_array, uint8_t* hardid_array, uint8_t *hardid_num)
{
    lock_hard_t hard;
	*hardid_num = 0;
    
    for(uint32_t hardid=hardid_start[OPEN_METH_PASSWORD]; hardid<hardid_start[OPEN_METH_MAX]; hardid++)
    {
        //valid
		if(SELECTBIT(hardid))
		{
            //load hard member
            if(lock_hard_load(hardid, &hard) == 0)
            {
                if(hard.member_id == memberid)
                {
                    hardid_array[*hardid_num] = hardid;
                    hardtype_array[*hardid_num] = hard.hard_type;
                    *hardid_num += 1;
                }
            }
        }
    }
	return APP_PORT_SUCCESS;
}

/*********************************************************
FN: delete lock hard in local flash
*/
uint32_t lock_hard_delete(uint8_t hardid)
{
    if(hardid >= HARDID_MAX_TOTAL) {
        return APP_PORT_ERROR_COMMON;
    }
    
	uint32_t err_code = app_port_nv_del(SF_AREA_1, hardid);
	if(err_code == APP_PORT_SUCCESS) {
        CLEARBIT(hardid);
        return APP_PORT_SUCCESS;
	}
    
    return APP_PORT_ERROR_COMMON;
}

/*********************************************************
FN: 
*/
uint32_t lock_hard_delete_all(void)
{
    for(uint32_t hardid=hardid_start[OPEN_METH_PASSWORD]; hardid<hardid_start[OPEN_METH_MAX]; hardid++)
	{
        //valid
		if(SELECTBIT(hardid))
		{
			lock_hard_delete(hardid);
		}
	}
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: 
*/
uint32_t lock_hard_delete_all_by_memberid(uint8_t memberid)
{
    uint8_t hardid_num = 0;
    uint8_t ret = 0;
    
    lock_hardid_load_by_memberid(memberid, hardtype_array, hardid_array, &hardid_num);
    for(uint32_t idx = 0; idx<hardid_num; idx++)
    {
        //delete hard by hard id
        if(hardtype_array[idx] == OPEN_METH_PASSWORD)
        {
            //don't need to delete
        }
        else if(hardtype_array[idx] == OPEN_METH_DOORCARD)
        {
            ret += lock_hard_doorcard_delete(hardid_array[idx]);
        }
        else if(hardtype_array[idx] == OPEN_METH_FINGER)
        {
            ret += lock_hard_finger_delete(hardid_array[idx]);
        }
        else if(hardtype_array[idx] == OPEN_METH_FACE)
        {
            ret += lock_hard_face_delete(hardid_array[idx]);
        }
        
        //delete hard in local flash by hard id
        ret += lock_hard_delete(hardid_array[idx]);
    }
    return ret;
}

/*********************************************************
FN: 
*/
uint32_t lock_hard_modify_all_by_memberid(uint8_t memberid, uint8_t* time)
{
    uint8_t ret = APP_PORT_SUCCESS;
    uint8_t hardid_num = 0;
    
    //load all hard id by memberid
    lock_hardid_load_by_memberid(memberid, hardtype_array, hardid_array, &hardid_num);
    for(uint32_t idx = 0; idx<hardid_num; idx++)
    {
        lock_hard_t hard;
        ret += lock_hard_load(hardid_array[idx], &hard);
        
        //update
        memcpy(hard.time, time, 17);
        
        //save
        ret += lock_hard_save(&hard);
    }
    return ret;
}

/*********************************************************
FN: 
*/
uint32_t lock_hard_freezeorunfreeze(uint8_t hardid, uint8_t freeze_state)
{
    uint8_t ret = APP_PORT_SUCCESS;
    
    //load hard
    lock_hard_t hard;
    lock_hard_load(hardid, &hard);
    
    //update
    hard.freeze_state = freeze_state;
    
    //save
    ret = lock_hard_save(&hard);
    
    return ret;
}

/*********************************************************
FN: 
*/
uint32_t lock_hard_freezeorunfreeze_all_by_memberid(uint8_t memberid, uint8_t freeze_state)
{
    uint8_t ret = APP_PORT_SUCCESS;
    uint8_t hardid_num = 0;
    
    //load all hard id by memberid
    lock_hardid_load_by_memberid(memberid, hardtype_array, hardid_array, &hardid_num);
    for(uint32_t idx = 0; idx<hardid_num; idx++)
    {
        lock_hard_t hard;
        ret += lock_hard_load(hardid_array[idx], &hard);
        
        //update
        hard.freeze_state = freeze_state;
        
        //save
        ret += lock_hard_save(&hard);
    }
    return ret;
}

/*********************************************************
FN: 
PM: metn - open meth is the same as hard type
*/
uint32_t lock_hard_save_in_local_flash(uint8_t meth)
{
    lock_hard_t hard;
    hard.hard_type = meth;
    hard.admin_flag = g_cmd.open_meth_creat.admin_falg;
    hard.member_id = g_cmd.open_meth_creat.memberid;
    hard.hard_id = lock_get_hardid(meth);
    memcpy(hard.time, g_cmd.open_meth_creat.time, HARD_TIME_MAX_LEN);
    hard.valid_num = g_cmd.open_meth_creat.valid_num;
    hard.password_len = 0;
    memset(hard.password, 0, HARD_PASSWORD_MAX_LEN);
    if(meth == OPEN_METH_PASSWORD)
    {
        hard.password_len = g_cmd.open_meth_creat.password_len;
        memcpy(hard.password, g_cmd.open_meth_creat.password, g_cmd.open_meth_creat.password_len);
    }
    if(meth == OPEN_METH_TEMP_PW)
    {
        hard.admin_flag = 0x00;
        hard.member_id = 0x00;
        hard.hard_id = lock_get_hardid(meth);
        hard.temp_pw_type = g_cmd.temp_pw_creat.type;
        memcpy(hard.time, g_cmd.temp_pw_creat.time, HARD_TIME_MAX_LEN);
        hard.valid_num = g_cmd.temp_pw_creat.valid_num;
        hard.password_len = g_cmd.temp_pw_creat.password_len;
        memcpy(hard.password, g_cmd.temp_pw_creat.password, g_cmd.temp_pw_creat.password_len);
    }
    hard.freeze_state = FREEZE_OFF; //default unfreeze
    uint32_t ret = lock_hard_save(&hard);
    return ret;
}

/*********************************************************
FN: 
PM: metn - open meth is the same as hard type
*/
uint32_t lock_hard_modify_in_local_flash(uint8_t meth)
{
    uint8_t ret = APP_PORT_SUCCESS;
    if(meth == OPEN_METH_TEMP_PW) {
        //load
        lock_hard_t hard;
        ret += lock_hard_load(g_cmd.temp_pw_modify.hardid, &hard);
        //update
        hard.temp_pw_type = g_cmd.temp_pw_modify.type;
        memcpy(hard.time, g_cmd.temp_pw_modify.time, HARD_TIME_MAX_LEN);
        hard.valid_num = g_cmd.temp_pw_modify.valid_num;
        hard.password_len = g_cmd.temp_pw_modify.password_len;
        memcpy(hard.password, g_cmd.temp_pw_modify.password, g_cmd.temp_pw_modify.password_len);
        //save
        ret += lock_hard_save(&hard);
    } else {
        //load
        lock_hard_t hard;
        ret += lock_hard_load(g_cmd.open_meth_modify.hardid, &hard);
        //update
        hard.admin_flag = g_cmd.open_meth_modify.admin_falg;
        hard.member_id = g_cmd.open_meth_modify.memberid;
        memcpy(hard.time, g_cmd.open_meth_modify.time, HARD_TIME_MAX_LEN);
        hard.valid_num = g_cmd.open_meth_modify.cycle;
        if(meth == OPEN_METH_PASSWORD)
        {
            hard.password_len = g_cmd.open_meth_modify.password_len;
            memcpy(hard.password, g_cmd.open_meth_modify.password, g_cmd.open_meth_modify.password_len);
        }
        //save
        ret += lock_hard_save(&hard);
    }
    return ret;
}




/*********************************************************  event  *********************************************************/

/*********************************************************
FN: next/last event id
*/
uint32_t lock_next_evtid(uint32_t index)
{
    return (index < (EVTID_MAX-1)) ? (index + 1) : 0;
}
uint32_t lock_last_evtid(uint32_t index)
{
    return (index > 0) ? (index - 1) : (EVTID_MAX-1);
}

/*********************************************************
FN: save/load event id
*/
uint32_t lock_evtid_save(uint32_t evt_id)
{
    return app_port_nv_set(SF_AREA_0, NV_ID_EVT_ID, &evt_id, sizeof(evt_id));
}
uint32_t lock_evtid_load(void)
{
    app_port_nv_get(SF_AREA_0, NV_ID_EVT_ID, &s_evt_id, sizeof(s_evt_id));
    return s_evt_id;
}

/*********************************************************
FN: evt = event = open lock + alarm
*/
uint32_t lock_evt_save(uint32_t timestamp, uint8_t *data, uint32_t len)
{
    if(app_port_get_connect_status() != BONDING_CONN)
    {
        uint8_t buf[256];
        
        buf[0] = 0; //time type
        memcpy(buf+1, &timestamp, sizeof(uint32_t));
        memcpy(buf+5, data, len);
        
        uint32_t err_code = app_port_nv_set(SF_AREA_2, s_evt_id, buf, len + 5);
        if(err_code == APP_PORT_SUCCESS)
        {
            s_evt_id = lock_next_evtid(s_evt_id);
            APP_DEBUG_PRINTF("s_evt_id: %d", s_evt_id);
            lock_evtid_save(s_evt_id);
            return 0;
        }
        return 1;//save fail
    }
	return 1;//not connect/bond
}

/*********************************************************
FN: 
*/
uint32_t lock_evt_load(uint32_t evt_id, uint8_t *data, uint32_t len)
{
	return app_port_nv_get(SF_AREA_2, evt_id, data, len);
}

/*********************************************************
FN: 
*/
uint32_t lock_evt_delete(uint32_t evt_id)
{
	uint32_t err_code = app_port_nv_del(SF_AREA_2, evt_id);
	if(err_code == APP_PORT_SUCCESS)
    {
        return 0;
	}
    return 1;
}

/*********************************************************
FN: 
*/
uint32_t lock_evt_delete_all(void)
{
    for(uint32_t idx=0; idx<EVTID_MAX; idx++)
    {
        lock_evt_delete(idx);
    }
    
    s_evt_id = 0;
    lock_evtid_save(s_evt_id);
    
	return 0;
}




/*********************************************************  setting  *********************************************************/

/*********************************************************
FN: 
*/
uint32_t lock_settings_save(void)
{
	return app_port_nv_set(SF_AREA_0, NV_ID_LOCK_SETTING, &lock_settings, sizeof(lock_settings_t));
}

/*********************************************************
FN: 
*/
uint32_t lock_settings_load(void)
{
    return app_port_nv_get(SF_AREA_0, NV_ID_LOCK_SETTING, &lock_settings, sizeof(lock_settings_t));
}

/*********************************************************
FN: 
*/
uint32_t lock_settings_default(void)
{
	memset(&lock_settings, 0, sizeof(lock_settings_t));
    lock_settings.message_switch = 0x00;
    lock_settings.door_bell = 0x00;
    lock_settings.lock_volume = 0x00;
    lock_settings.lock_language = 0;
    memset(lock_settings.welcome_words, 0x00, 50);
    lock_settings.key_tone = 0x00;
    lock_settings.navigation_volume = 0x00;
    lock_settings.auto_lock_switch = 0x00;
    lock_settings.combine_lock_switch = 0x00;
    lock_settings.timer_lock = 0x00;
    lock_settings.timer_auto_lock = 0x00;
    lock_settings.finger_number = 0x00;
    lock_settings.hand_lock = 0x01;
    lock_settings.motor_direction = 0x00;
	lock_settings_save();
    return 0;
}

/*********************************************************
FN: 
*/
uint32_t lock_settings_delete_and_default(void)
{
	uint32_t err_code = app_port_nv_del(SF_AREA_0, NV_ID_LOCK_SETTING);
	if(err_code == APP_PORT_SUCCESS)
    {
        //set default settings
        lock_settings_default();
        return 0;
	}
    return 1;
}




/*********************************************************  init  *********************************************************/

/*********************************************************
FN: 
*/
uint32_t lock_flash_erease_all(bool is_delete_app_test_data)
{
    app_port_nv_set_default();
    return 0;
}


/*********************************************************
FN: 
*/
uint32_t lock_flash_init(void)
{
    //init hardid_bitmap
	for(uint32_t hardid=0; hardid<HARDID_MAX_TOTAL; hardid++)
	{
        lock_hard_t hard;
		if(lock_hard_load(hardid, &hard) == APP_PORT_SUCCESS)
		{
            SETBIT(hardid);
		}
		else
		{
            CLEARBIT(hardid);
		}
	}
    
    //init s_evt_id
    lock_evtid_load();
    
    //if no lock_settings, set default settings
	if(lock_settings_load() != 0)
	{
		lock_settings_default();
	}
    
    return 0;
}




















