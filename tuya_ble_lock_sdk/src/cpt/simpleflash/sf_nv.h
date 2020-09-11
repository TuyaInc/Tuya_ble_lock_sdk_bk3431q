/**
****************************************************************************
* @file      sf_nv.h
* @brief     sf_nv
* @author    suding
* @version   V1.0.0
* @date      2020-04
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2020 Tuya </center></h2>
*/


#ifndef __SF_NV_H__
#define __SF_NV_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "sf_port.h"

/*********************************************************************
 * CONSTANTS
 */
typedef enum {
    SF_SUCCESS = 0x00,
    SF_ERROR_COMMON,
    SF_ERROR_PARAM,
    SF_ERROR_FULL,
    SF_ERROR_NOT_FOUND,
} sf_status_t;

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
u32 sf_nv_init(u32 area_id);
u32 sf_nv_write(u32 area_id, u16 id, void *buf, u8 size);
u32 sf_nv_read(u32 area_id, u16 id, void *buf, u8 size);
u32 sf_nv_delete(u32 area_id, u16 id);

void sf_nv_test(u32 area_id);


#ifdef __cplusplus
}
#endif

#endif //__SF_NV_H__
