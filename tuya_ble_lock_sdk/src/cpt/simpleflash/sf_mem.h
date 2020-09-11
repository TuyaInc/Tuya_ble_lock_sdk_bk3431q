/**
****************************************************************************
* @file      sf_mem.h
* @brief     sf_mem
* @author    suding
* @version   V1.0.0
* @date      2020-04
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2020 Tuya </center></h2>
*/


#ifndef __SF_MEM_H__
#define __SF_MEM_H__

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
#define MEM_SIZE ((1024*2)/sizeof(uint32_t))

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
bool  sd_mem_init( void );
void* sd_malloc( u32 size );
bool  sd_free( void* mem );



#ifdef __cplusplus
}
#endif

#endif //__SF_MEM_H__
