#include "bk_gpio.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static uint8_t s_bk_gpio_int_array[] = 
{
    BK_INPUT_PIN_0,
    BK_INPUT_PIN_1,
};

/*********************************************************************
 * LOCAL FUNCTION
 */
static void bk_gpio_int_handler(uint32_t pin);
static uint32_t bk_gpio_int_pin_change_format(uint32_t pin);

/*********************************************************************
 * VARIABLES
 */
bk_out_pin_t bk_output_pin_array[] = 
{
    {BK_OUTPUT_PIN_0, LEVEL_LOW},
    {BK_OUTPUT_PIN_1, LEVEL_LOW},
};
uint8_t bk_out_pins_num = sizeof(bk_output_pin_array)/sizeof(bk_output_pin_array[0]);




/*********************************************************
FN: 
*/
void bk_gpio_init_output(uint8_t pin)
{
    gpio_config(pin, OUTPUT, PULL_NONE);
    
    gpio_set(pin, LEVEL_LOW);
    for(uint8_t idx=0; idx<bk_out_pins_num; idx++) {
        if(bk_output_pin_array[idx].pin == pin) {
            bk_output_pin_array[idx].level = LEVEL_LOW;
        }
    }
}

/*********************************************************
FN: 
*/
void bk_gpio_init_input(uint8_t pin, uint8_t pull_type)
{
    gpio_config(pin, INPUT, (Pull_Type)pull_type);
}

/*********************************************************
FN: 
*/
uint8_t bk_gpio_get_input(uint8_t pin)
{
    return gpio_get_input(pin);
}

/*********************************************************
FN: 
*/
uint8_t bk_gpio_get_output(uint8_t pin)
{
    for(uint8_t idx=0; idx<bk_out_pins_num; idx++) {
        if(bk_output_pin_array[idx].pin == pin) {
            return bk_output_pin_array[idx].level;
        }
    }
    return LEVEL_INVALID;
}

/*********************************************************
FN: 输出引脚设置电平
*/
void tuya_gpio_set(uint8_t pin, uint8_t level)
{
    gpio_set(pin, level);
    
    for(uint8_t idx=0; idx<bk_out_pins_num; idx++) {
        if(bk_output_pin_array[idx].pin == pin) {
            bk_output_pin_array[idx].level = level;
        }
    }
}

/*********************************************************
FN: 输出引脚电平翻转
*/
void tuya_gpio_reverse(uint8_t pin)
{   
    uint8_t level = bk_gpio_get_output(pin);
    tuya_gpio_set(pin, !level);
}




/*********************************************************
FN: 外部中断
*/
static void bk_gpio_int_config(uint8_t* gpio_array, uint8_t len)
{
    for(uint8_t i=0; i<len; i++)
    {
        if(gpio_array[i] == BK_INPUT_PIN_0) { //上升沿触发
            bk_gpio_init_input(gpio_array[i], PULL_LOW);
            REG_APB5_GPIO_WUATOD_TYPE |= 0<<(8*(gpio_array[i]>>4)+(gpio_array[i]&0x0f)); //0<<2 = 0
        } else {                              //下降沿触发
            bk_gpio_init_input(gpio_array[i], PULL_HIGH);
            REG_APB5_GPIO_WUATOD_TYPE |= 1<<(8*(gpio_array[i]>>4)+(gpio_array[i]&0x0f)); //1<<2 = 4
        }
        REG_APB5_GPIO_WUATOD_STAT |= 1<<(8*(gpio_array[i]>>4)+(gpio_array[i]&0x0f));
        bk_delay_ms(2);
        REG_APB5_GPIO_WUATOD_ENABLE |= 1<<(8*(gpio_array[i]>>4)+(gpio_array[i]&0x0f));
        REG_AHB0_ICU_DEEP_SLEEP0 |= 1<<(8*(gpio_array[i]>>4)+(gpio_array[i]&0x0f));
    }
    REG_AHB0_ICU_INT_ENABLE |= (0x01 << 9);
}

/*********************************************************
FN: 外部中断初始化
*/
void bk_gpio_int_init(void)
{
    bk_gpio_int_config(s_bk_gpio_int_array, sizeof(s_bk_gpio_int_array));
    gpio_cb_register(bk_gpio_int_handler);
}

/*********************************************************
FN: 外部中断处理
*/
static void bk_gpio_int_handler(uint32_t pin)
{
    //引脚序号是8进制
    pin = bk_gpio_int_pin_change_format(pin);
    
    switch(pin)
    {
        case BK_INPUT_PIN_0: {
//            if(bk_gpio_get_input(pin) == LEVEL_HIGH)
            {
                BK_PRINTF("BK_INPUT_PIN_0");
            }
        } break;
        
        case BK_INPUT_PIN_1: {
//            if(bk_gpio_get_input(pin) == LEVEL_LOW)
            {
                BK_PRINTF("BK_INPUT_PIN_1");
            }
        } break;
        
        default: {
        } break;
    }
    bk_gpio_int_config(s_bk_gpio_int_array, sizeof(s_bk_gpio_int_array));
}

/*********************************************************
FN: 外部中断引脚转换
*/
static uint32_t bk_gpio_int_pin_change_format(uint32_t pin)
{
    uint32_t zero_count = 0;
    for(int idx=0; idx<32; idx++) {
        pin = pin>>1;
        if(pin == 0) {
            break;
        }
        zero_count++;
    }
    return (((zero_count/8)*0x10) + (zero_count%8));
}




/*********************************************************
FN: 初始化引脚
*/
void bk_gpio_init(void)
{
    //output
    for(uint32_t idx=0; idx<bk_out_pins_num; idx++)
    {
        bk_gpio_init_output(bk_output_pin_array[idx].pin);
    }
    
    //input
    bk_gpio_init_input(BK_INPUT_PIN_0, PULL_NONE);
    bk_gpio_init_input(BK_INPUT_PIN_1, PULL_NONE);
    
    //external interrupt
    bk_gpio_int_init();
}



















