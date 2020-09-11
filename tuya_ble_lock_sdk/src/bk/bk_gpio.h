/**
****************************************************************************
* @file      bk_gpio.h
* @brief     bk_gpio
* @author    suding
* @version   V1.0.0
* @date      2019-10-12
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __BK_GPIO_H__
#define __BK_GPIO_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "bk_common.h"

/*********************************************************************
 * CONSTANTS
 */
#define BK_OUTPUT_PIN_0 0x12
#define BK_OUTPUT_PIN_1 0x13
#define BK_INPUT_PIN_0  0x10
#define BK_INPUT_PIN_1  0x11

enum
{
    LEVEL_INVALID = 0xFF,
    LEVEL_LOW  = 0,
    LEVEL_HIGH = 1,
};

/*********************************************************************
 * STRUCT
 */
typedef struct
{
    uint8_t pin;
    uint8_t level;
} bk_out_pin_t;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
void bk_gpio_init_output(uint8_t pin);
void bk_gpio_init_input(uint8_t pin, uint8_t pull_type);
uint8_t bk_gpio_get_input(uint8_t pin);
uint8_t bk_gpio_get_output(uint8_t pin);
void tuya_gpio_set(uint8_t pin, uint8_t level);
void tuya_gpio_reverse(uint8_t pin);
void bk_gpio_int_init(void);

void bk_gpio_init(void);

#ifdef __cplusplus
}
#endif

#endif //__BK_GPIO_H__
