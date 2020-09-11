/*************************************************************
 * @file        BK_System.h
 * @brief       Header file of BK_System.c
 * @author      GuWenFu
 * @version     V1.0
 * @date        2016-09-29
 * @par         
 * @attention   
 *
 * @history     2016-09-29 gwf    create this file
 */

#ifndef  __BK_SYSTEM_H__

#define  __BK_SYSTEM_H__


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "config.h"


#ifndef NULL
#define NULL 0
#endif


#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))


#define NUMBER_ROUND_UP(a,b)        ((a) / (b) + (((a) % (b)) ? 1 : 0))
#define NUMBER_ROUND_DOWN(a,b)      ((a) / (b))


//#define RAM_CODE       __attribute__((section("ram_code")))
#define RAM_CODE



#if 1

typedef unsigned char       BYTE;
typedef signed   char       int8;       // 有符号8位整型变量
typedef signed   short      int16;      // 有符号16位整型变量
typedef signed   long       int32;      // 有符号32位整型变量
typedef signed   long long  int64;      // 有符号64位整型变量
typedef unsigned char       uint32; 
typedef signed   long long  int64;      // 有符号64位整型变量
typedef unsigned char       uint8;      // 无符号8位整型变量
typedef unsigned short      uint16; 

     // 无符号32位整型变量
typedef unsigned long long  uint64;     // 无符号64位整型变量
typedef float               fp32;       // 单精度浮点数(32位长度)
typedef double              fp64;       // 双精度浮点数(64位长度)

typedef signed   char       int8_t;     // 有符号8位整型变量
typedef signed   short      int16_t;    // 有符号16位整型变量
typedef signed   long       int32_t;    // 有符号32位整型变量
typedef signed   long long  int64_t;    // 有符号64位整型变量
typedef unsigned char       uint8_t;    // 无符号8位整型变量
typedef unsigned short      uint16_t;   // 无符号16位整型变量
typedef unsigned long       uint32_t;   // 无符号32位整型变量
typedef unsigned long long  uint64_t;   // 无符号64位整型变量
typedef float               fp32_t;     // 单精度浮点数(32位长度)
typedef double              fp64_t;     // 双精度浮点数(64位长度)

typedef signed   char       s8;
typedef signed   short      s16;
typedef signed   long       s32;
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned long       u32;
typedef unsigned long       u_int32;

typedef unsigned char  		UINT8;      /* Unsigned  8 bit quantity        */
typedef signed   char  		INT8;       /* Signed    8 bit quantity        */
typedef unsigned short 		UINT16;     /* Unsigned 16 bit quantity        */
typedef signed   short 		INT16;      /* Signed   16 bit quantity        */
typedef unsigned int   		UINT32;     /* Unsigned 32 bit quantity        */
typedef signed   int   		INT32;      /* Signed   32 bit quantity        */
typedef unsigned long long  UINT64;		/* Unsigned 32 bit quantity        */
typedef signed   long long  INT64;		/* Signed   32 bit quantity        */
typedef float         		FP32;		/* Single precision floating point */
typedef double         		FP64;		/* Double precision floating point */
typedef unsigned int        size_t;
typedef unsigned char       BOOLEAN;

typedef int				    INT;
typedef unsigned int	    UINT;
typedef unsigned char	    BYTE;
typedef short			    SHORT;
typedef unsigned short	    WORD;
typedef unsigned short	    WCHAR;
typedef long			    LONG;
typedef unsigned long	    DWORD;
typedef unsigned long long  QWORD;


#if 1
typedef enum
{
    FALSE=0,
    TRUE=!FALSE
}bool, BOOL;
#endif

typedef enum        // by gwf
{
    OK = 0,
    ERROR = -1
} STATUS;

typedef enum        // by gwf
{
    NO = 0,
    YES = 1
} ASK;


#endif
extern void DelayNops(volatile unsigned long nops);

extern void DelayUS(volatile unsigned long timesUS);
extern void DelayMS(volatile unsigned long timesMS);


#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif      /* __BK_SYSTEM_H__ */
