#ifndef _MSG_PUB_H_
#define _MSG_PUB_H_

//#include "types.h"
#include "BK_System.h"


#define DELAY_MSG_PUT                (1)
#define OS_TICK_MSEC                 (10)

#define MSG_SUCCESS_RET              (0)
#define MSG_FAILURE_RET              (-1)

#define jtask_h                os_timer_h
#define jtask_init(a,b)        os_timer_init(a)
#define jtask_uninit           os_timer_uninit
#define jtask_schedule         os_timer_reset
#define jtask_stop             os_timer_stop
#define jtask_is_pending       os_timer_is_pending


typedef struct
{
    uint32 id;

    void *hdl;
    uint32 arg;
}MSG_T, *MSG_PTR;

/* public api*/
extern void msg_init(void);
extern void msg_dump(void);
extern void msg_uninit(void);
extern void msg_put(uint32 msg);
extern void msg_clear_all(void);
extern int msg_priority_put(MSG_T *msg);

#ifdef DELAY_MSG_PUT
extern int msg_delay_put(uint32 os_tick, MSG_T *msg);
#endif

//-------按键触发状态定义---------
#define KEY_SHORT_UP					0x00
#define KEY_HOLD						0x4000
#define KEY_LONG						0x8000
#define KEY_LONG_UP						0xc000
#define KEY_DOUBLE_CLICK 				0x2000
#define KEY_IRDA_LONG					0x1000
#define KEY_IRDA_SHORT					0x800

/* message table*/
#define KEY_MSG_GP                      (0x00000000)    /* Attention: special msg*/
#define BLUETOOTH_MSG_GP                (0x10000000)
#define SDMUSIC_MSG_GP                  (0x20000000)
#define UDISK_MSG_GP                    (0x30000000)
#define FM_MSG_GP                       (0x40000000)
#define LINEIN_MSG_GP                   (0x50000000)
#define OTHER_MSG_GP                    (0x60000000)

/* Name format: MSG_module_messageInformation
   assume: message number is less than 65535 at one module
*/
enum
{
    MSG_NULL = 0, 
    /* Attention: special msg for key press, from 0x00000000--0x00000fff*/
 //-------按键消息值定义---------
	MSG_KEY_0							 = MSG_NULL + 0x1,
	MSG_KEY_1,
	MSG_KEY_2,
	MSG_KEY_3,
	MSG_KEY_4,
	MSG_KEY_5,
	MSG_KEY_6,
	MSG_KEY_7,
	MSG_KEY_8,
	MSG_KEY_9,
	MSG_BT_MIX_KEY,
	MSG_KEY_PLAY,
	MSG_KEY_STOP,
	MSG_KEY_PLUS,
	MSG_KEY_MINUS,
	MSG_KEY_PREV,
	MSG_KEY_NEXT,
	MSG_KEY_VOL_DOWN,
	MSG_KEY_VOL_UP,
	MSG_KEY_MODE,
	MSG_KEY_POWER,
	MSG_KEY_CALL,
	MSG_NO_KEY = 0xff,
  
    /*BLK0: bluetooth msg*/
    MSG_BT_INIT                      = BLUETOOTH_MSG_GP + 0x0000,
    MSG_COEXIST_ENTER_BT,
    MSG_COEXIST_EXIT_BT,
    
    /*BLK1: sd music msg*/
    MSG_SD_INIT                      = SDMUSIC_MSG_GP   + 0x0000,
    MSG_SD_ATTACH,            /* sd attach or detach*/
	MSG_SD_DETACH,

	
    /*BLK2: usb disk msg*/
    MSG_UDISK_INIT                   = UDISK_MSG_GP     + 0x0000,
	MSG_USB_ATTACH,
	MSG_USB_DETATCH,

	
    /*BLK3: fm msg*/
    MSG_FM_INIT                      = FM_MSG_GP        + 0x0000,
    MSG_FM_SEEK_CONTIUE,
//    MSG_FM_MEMERIZE,
//    MSG_FM_INSTALL_CHANNEL,
//    MSG_FM_UNINSTALL,
	MSG_FM_AUTO_SCAN,
    MSG_FM_SEEK_PREV_CHANNEL,
    MSG_FM_SEEK_NEXT_CHANNEL,
    MSG_FM_AUTO_SEEK_START,
    MSG_FM_CHANNEL_SEEK_START,
    MSG_FM_CHANNEL_TUNE_CONTINUE,                  /* 0xc*/
    MSG_FM_CHANNEL_SEEK_CONFIG,
    MSG_FM_CHANNEL_TUNE_OVER,
    MSG_FM_TUNE_SUCCESS_CONTINUE,
    MSG_FM_TUNE_FAILURE_CONTINUE,
    MSG_FM_AUTO_SEEK_OVER,
	MSG_FM_SET_FRE,
    MSG_FM_DISABLE_MUTE,
    MSG_FM_ENABLE_MUTE,
    
    
    /*BLK4: linein msg*/
    MSG_LINEIN_INIT                  = LINEIN_MSG_GP  + 0x0000,
    
    /*BLK5: other msg*/   
    MSG_LED_INIT                     = OTHER_MSG_GP   + 0x0000,
    MSG_DEBUG_UART_RX,               /* debug uart gets datum*/
    MSG_SDADC,                       /* sdadc*/
    MSG_POWER_DOWN,
    MSG_POWER_UP,
    MSG_FLASH_WRITE,
    MSG_IRDA_RX,
    MSG_LOWPOWER_DETECT,
    MSG_MEDIA_READ_ERR,      /* mp3-mode,SD/Udisk read Err */
	
    MSG_LINEIN_ATTACH,
    MSG_LINEIN_DETACH,

    MSG_TIMER_PWM0_HDL,
    MSG_SAVE_VOLUME,
    MSG_CHANGE_MODE,
    MSG_CHANGE_HID_MODE,
    MSG_INPUT_TIMEOUT,
    MSG_HALF_SECOND,
    MSG_1S_TIMER
};

int msg_lush_put(MSG_T *msg_ptr);

#endif
// EOF
