/**
****************************************************************************
* @file      sd_math.h
* @brief     sd_math
* @author    suding
* @version   V1.0.0
* @date      2019-07-09
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __SD_MATH_H__
#define __SD_MATH_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "stdint.h"

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
uint8_t cpt_check_sum(uint8_t *buf, uint32_t size);
uint16_t cpt_check_sum_16(uint8_t *buf, uint32_t size);
uint16_t cpt_crc16_compute(uint8_t* buf, uint16_t size, uint16_t* p_crc);
uint32_t cpt_crc32_compute(uint8_t* buf, uint32_t size, uint32_t* p_crc);
void cpt_reverse_byte(void* buf, uint32_t size);
uint32_t cpt_num_array_2_int(uint8_t *num_array, uint32_t start_idx, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif //__SD_MATH_H__
