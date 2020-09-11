/**
****************************************************************************
* @file      sf_port.h
* @brief     sf_port
* @author    suding
* @version   V1.0.0
* @date      2020-04
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2020 Tuya </center></h2>
*/


#ifndef __SF_PORT_H__
#define __SF_PORT_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#include "bk_common.h"

/*********************************************************************
 * CONSTANTS
 */
#define SF_AREA_NUM         (5)

#define SF_WRITE_MIN_SIZE   (0x04)
#define SF_ERASE_MIN_SIZE   (0x1000)

#define SF_AREA0_BASE       (0x68000)
#define SF_AREA1_BASE       (0x6A000) //lock_hard
#define SF_AREA2_BASE       (0x6C000) //lock_event
#define SF_AREA3_BASE       (0x6E000) //offline_password
#define SF_AREA4_BASE       (0x70000)
#define SF_AREA_SIZE        (2*SF_ERASE_MIN_SIZE)  //min = 2*SF_ERASE_MIN_SIZE

enum
{
    SF_AREA_0 = 0,
    SF_AREA_1,
    SF_AREA_2,
    SF_AREA_3,
    SF_AREA_4,
};

#define SF_MEM_EN           1

#define SF_DEBUG_EN         1

#if (SF_DEBUG_EN)
    #define SF_PRINTF(...)   log_d(__VA_ARGS__)
    #define SF_HEXDUMP       sf_log_hexdump
#else
    #define SF_PRINTF(...)
    #define SF_HEXDUMP       sf_log_hexdump_empty
#endif


typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

/*********************************************************************
 * STRUCT
 */
#include "sf_mem.h"
#include "sf_nv.h"

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
u32 sf_port_flash_read(u32 addr, void* buf, u32 size);
u32 sf_port_flash_write(u32 addr, void* buf, u32 size);
u32 sf_port_flash_erase(u32 addr, u32 num);

void  sf_mem_init(void);
void* sf_malloc(u32 size);
u32   sf_free(void* buf);


#ifdef __cplusplus
}
#endif

#endif //__SF_PORT_H__
