#include <stdint.h>        // standard integer definition
#include <string.h>        // string manipulation
#include <stddef.h>     // standard definition
#include "lld_evt.h"
#include "rc32k_cal.h"     // RW SW configuration
#include "pwm.h"
#include "uart.h"
#include "icu.h"
#include "pwm.h"


#define CAL_TIMER  1000000 //计算1s时间进行比较

uint32_t timer_625us_start,timer_1us_start;
uint32_t timer_625us_end,timer_1us_end;
uint8_t  timer_fast_slow_flag = 0;
uint32_t delta_time;
double rc32k_error_rate=1;


void rc32k_cal_init(PWM_DRV_DESC *pwm_drv_desc)
{
    if(pwm_drv_desc == NULL)
	{
		return;
	}
	if (pwm_drv_desc->channel > PWM_CHANNEL_NUMBER_MAX)
    {
        return;
    }
	if (pwm_drv_desc->duty_cycle > pwm_drv_desc->end_value)
    {
        return;
    }	
	
	//Config clk
	ICU_PWM_CLK_PWM_X_PWD_CLEAR(pwm_drv_desc->channel); 
    ICU_PWM_CLK_PWM_X_SEL_32KHZ(pwm_drv_desc->channel);
   //ICU_PWM_CLK_PWM_X_SEL_16MHZ(pwm_drv_desc->channel);
    
	//Config duty_cycle and end value
	REG_PWM_X_CNT(pwm_drv_desc->channel) = 
        ((((unsigned long)pwm_drv_desc->duty_cycle << PWM_CNT_DUTY_CYCLE_POSI) & PWM_CNT_DUTY_CYCLE_MASK)
       + (((unsigned long)pwm_drv_desc->end_value << PWM_CNT_END_VALUE_POSI) & PWM_CNT_END_VALUE_MASK)); 
    
    REG_PWM_CTRL = (REG_PWM_CTRL & (~(0x0F << (0x04 *  pwm_drv_desc->channel))))
             | ((pwm_drv_desc->mode & 0x0F) << (0x04 *  pwm_drv_desc->channel));

    lld_evt_time_get_us(&timer_625us_start,&timer_1us_start);
    
    REG_PWM_INTR=0x3f;
	// install interrupt handler
    p_PWM_Int_Handler[pwm_drv_desc->channel] = pwm_drv_desc->p_Int_Handler;

    REG_AHB0_ICU_INT_ENABLE |=  (ICU_INT_ENABLE_IRQ_PWM_X_MASK(pwm_drv_desc->channel)); 
	
}


void user_timer_cb(unsigned char ucChannel)
{
   
    lld_evt_time_get_us(&timer_625us_end,&timer_1us_end);
   
    delta_time=(timer_625us_end*625+timer_1us_end)-(timer_625us_start*625+timer_1us_start);
   

    if(delta_time<CAL_TIMER)
    {
        rc32k_error_rate=(double)(CAL_TIMER-delta_time);
        rc32k_error_rate /=(double)CAL_TIMER;
        rc32k_error_rate=1-rc32k_error_rate;
        timer_fast_slow_flag = 1;
    }
    else
    {
        rc32k_error_rate=(double)(delta_time-CAL_TIMER);
        rc32k_error_rate /=(double)CAL_TIMER;
        rc32k_error_rate=1+rc32k_error_rate;
        timer_fast_slow_flag = 2;
    }

    if((delta_time<(CAL_TIMER-10000))||(delta_time>(CAL_TIMER+10000)))
    {
        rc32k_error_rate=1;
        UART_PRINTF("rc32k_error_rate over!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
    }
    
    UART_PRINTF("delta_time=%d\r\n", delta_time); 

    pwm_disable(ucChannel);

    icu_set_sleep_mode(0);
    rwip_prevent_sleep_clear(BK_DRIVER_TIMER_ACTIVE);

    
}


void user_timer_init(void)
{	
	icu_set_sleep_mode(1);
	rwip_prevent_sleep_set(BK_DRIVER_TIMER_ACTIVE);
	PWM_DRV_DESC timer_desc;

	timer_desc.channel = 3;            				  
	timer_desc.mode    = 1<<0 | 1<<1 | 1<<2;// | 1<<4;   
	timer_desc.end_value  = 32000;                      
	timer_desc.duty_cycle = 0;                        
	timer_desc.p_Int_Handler = user_timer_cb;  
        
	rc32k_cal_init(&timer_desc);


}






