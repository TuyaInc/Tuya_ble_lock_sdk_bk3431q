/**
****************************************************************************
* @file      lock_dynamic_pwd.h
* @brief     lock_dynamic_pwd
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __LOCK_DYNAMIC_PWD_H__
#define __LOCK_DYNAMIC_PWD_H__

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
#define DYNAMIC_PWD_TOKEN_SIZE			( 8 )		// 动态口令长度
#define DYNAMIC_PWD_TIME_STEP			( 5 * 60 )	// 时间步长 5分钟
#define DYNAMIC_PWD_TIME_WINDOW			( 5 * 60 )	// 容错时间窗口5分钟

/*********************************************************************
 * STRUCT
 */
typedef enum {
	DYNAMIC_PWD_VERIFY_SUCCESS		= ( 0 ), 	
	DYNAMIC_PWD_ERR_PARAS			= ( -1 ),   
	DYNAMIC_PWD_ERR_USED			= ( -2 ),
	DYNAMIC_PWD_ERR_TIMESEQ			= ( -3 ),
} dynamic_pwd_verify_err_code_t;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
int lock_dynamic_pwd_verify(uint8_t *pwd, uint8_t size);


#ifdef __cplusplus
}
#endif

#endif //__LOCK_DYNAMIC_PWD_H__
