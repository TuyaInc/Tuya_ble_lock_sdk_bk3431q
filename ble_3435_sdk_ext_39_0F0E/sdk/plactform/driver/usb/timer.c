/* (c) 2012 Jungo Ltd. All Rights Reserved. Jungo Confidential */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "driver_icu.h"
#include "timer.h"
#include "msg_pub.h"
#include "driver_pwm.h"
#include "driver_gpio.h"


#define UART_PRINTF	uart_printf
int uart_printf(const char *fmt,...);


extern void *ke_malloc(uint32_t size, uint8_t type);
void ke_free(void* mem_ptr);



static os_timer_t *timers_head = NULL;
static volatile uint64_t tick_counter = 0;
extern void MGC_AfsTimerExpired(void);


#define	UDISK_TIMER_TIMEOUT_VAL 1200//12s
#define	SD_TIMER_TIMEOUT_VAL 400//4s
static volatile uint16 timeout_timer;
static uint8 timeoutEnableflag;
static uint8 mediaflag;

void enable_timeout_timer(uint8 flag)
{
//	uint32 tmp;
//    UART_PRINTF("%s\r\n", __FUNCTION__);
	mediaflag = flag;
	timeoutEnableflag = 1;
//	VICMR_disable_interrupts(&tmp);
    REG_ICU_INT_GLOBAL_ENABLE = 0;
	timeout_timer = 0;
//	VICMR_restore_interrupts(tmp);
    REG_ICU_INT_GLOBAL_ENABLE = ICU_INT_GLOBAL_ENABLE_IRQ_SET | ICU_INT_GLOBAL_ENABLE_FIQ_SET;
}
void disable_timeout_timer(void)
{
//	uint32 tmp;
//    UART_PRINTF("%s\r\n", __FUNCTION__);
	timeoutEnableflag = 0;
//	VICMR_disable_interrupts(&tmp);
    REG_ICU_INT_GLOBAL_ENABLE = 0;
	timeout_timer = 0;
//	VICMR_restore_interrupts(tmp);
    REG_ICU_INT_GLOBAL_ENABLE = ICU_INT_GLOBAL_ENABLE_IRQ_SET | ICU_INT_GLOBAL_ENABLE_FIQ_SET;
}

uint8 timeout_handle(void)
{
//    UART_PRINTF("%s\r\n", __FUNCTION__);
	if(timeoutEnableflag)
	{
		if((timeout_timer > UDISK_TIMER_TIMEOUT_VAL)&&(1 == mediaflag))
			return 1;
		else if((timeout_timer > SD_TIMER_TIMEOUT_VAL)&&(0 == mediaflag))
			return 1;
		else
			return 0;
	}
	return 0;
}


/* Initialize new timer */
result_t os_timer_init(os_timer_h *timer_h)
{
//    DECLARE_FNAME("os_timer_init");

//    UART_PRINTF("%s\r\n", __FUNCTION__);
    *timer_h = (os_timer_h)ke_malloc(sizeof(os_timer_t),3);
    
    if (*timer_h == NULL)
    {
        UART_PRINTF("%s: malloc failed!\r\n", __FUNCTION__);
        return UWE_NOMEM;
    }
    memset(*timer_h, 0, sizeof(os_timer_t));

    return UWE_OK;
}

/* Un-Initialize timer */
void os_timer_uninit(os_timer_h timer_h)
{
    os_timer_t *node = (os_timer_t *)timer_h;
//    DECLARE_FNAME("os_timer_uninit");

//    UART_PRINTF("%s\r\n", __FUNCTION__);
    if (node->is_pending)
        os_timer_stop(timer_h);

    if (timer_h)
        ke_free(timer_h);
}

int os_timer_is_pending(os_timer_h timer_h)
{
    os_timer_t *node = (os_timer_t *)timer_h;

//    UART_PRINTF("%s\r\n", __FUNCTION__);
    return (node->is_pending);
}

/* Reset/Start timer with specific timeout (in msec) */
result_t os_timer_reset(os_timer_h timer_h, uint32_t msec, jthread_func func,
                        void *arg)
{
    os_timer_t *node = (os_timer_t *)timer_h;
    os_timer_t **iter = &timers_head;
//    DECLARE_FNAME("os_timer_reset");

//    UART_PRINTF("%s\r\n", __FUNCTION__);
    if (node->is_pending)
        os_timer_stop(timer_h);

    node->is_pending = TRUE;
    node->ticks = (msec + (MSEC_IN_TICK - 1))/MSEC_IN_TICK;
    node->ticks += tick_counter;
    node->f = func;
    node->arg = arg;

    while (*iter && (*iter)->ticks < node->ticks)
        iter = &(*iter)->next;

    node->next = *iter;
    *iter = node;

    return UWE_OK;
}

/* Stop timer */
void os_timer_stop(os_timer_h timer_h)
{
    os_timer_t *node = (os_timer_t *)timer_h;
    os_timer_t **iter = &timers_head;
//    DECLARE_FNAME("os_timer_stop");

//    UART_PRINTF("%s\r\n", __FUNCTION__);
    if (!node->is_pending)
        return;

    node->is_pending = FALSE;

    while (*iter && *iter != node)
        iter = &(*iter)->next;

    if (*iter)
        *iter = (*iter)->next;
}

/* Check if timer is waiting to run */
bool os_timer_pending(os_timer_h timer_h)
{
//    DECLARE_FNAME("os_timer_pending");
//    UART_PRINTF("%s\r\n", __FUNCTION__);
    return ((os_timer_t *)timer_h)->is_pending;
}

//#pragma push
//#pragma O0
static float loops_per_usec;

/* Gauge execution rate per clock tick */
void compute_cpu_speed(void)
{
#define LOOPS_FOR_COMPUTE 1000000
    volatile uint64_t a;
    volatile uint64_t b;
    volatile uint32_t count = LOOPS_FOR_COMPUTE;

//    UART_PRINTF("%s\r\n", __FUNCTION__);
    a = tick_counter;
    while (count)
        count--;

    b = tick_counter;

    loops_per_usec = LOOPS_FOR_COMPUTE /((b - a) * MSEC_IN_TICK);
    loops_per_usec /= 1000;
}

/* Delay execution for N micro-seconds (don't lose context) */
__inline void os_delay_us(uint32_t usec) {
    volatile uint32_t loops;
    /* loops = usec * loops_per_usec; */
    /* while (loops) */
    /*     loops--; */
    while (usec--) {
        loops=10;
        while (loops--);
    }
}
__inline void os_delay_us_2(uint32_t usec) {
    volatile uint32_t loops;
    /* loops = usec * loops_per_usec; */
    /* while (loops) */
    /*     loops--; */
    while (usec--) {
        loops=2;
        while (loops--);
    }
}
//#pragma pop

void os_delay_ms (uint32_t msec) {        /**< 78mHz */
    volatile uint32_t loops;
    while (msec--) {
        loops=9700;
        while (loops--);
    }
}

/* Get current time (real or from system boot) */
void os_get_time(os_time_t *time)
{
    uint64_t timer = tick_counter;
    uint32_t tick_in_sec = 1000 / MSEC_IN_TICK;

    time->tv_sec = timer / tick_in_sec;
    time->tv_usec = (timer % tick_in_sec) * MSEC_IN_TICK * 1000;
}

/* Get ticks-in-second */
uint32_t os_get_hz(void)
{
    return 1000 / MSEC_IN_TICK;
}

void disable_pwm0(void) {
//    BK3000_PWM0_CONF = 0;
    PWM_disable(0);
}

void enable_pwm0(void) {
//    BK3000_PWM0_CONF = (PWM0_COUNT << sft_PWM_PT2_CNT )
//        | (1 << sft_PWM_PT2_ENABLE);
    PWM_enable(0);
}

void timer_pwm0_isr(unsigned char ucChannel);
void timer_pwm0_init(void)
{
    PWM_DRV_DESC pwm_drv_desc;
//    UART_PRINTF("%s\r\n", __FUNCTION__);
    pwm_drv_desc.channel = 0;                           // PWM0
    pwm_drv_desc.mode    = 1<<0 | 1<<1 | 1<<2 | 0<<4;   // enable PWM, enable int, Timer mode, 32KHz
    pwm_drv_desc.end_value  = 64000;                    // PWM end_value
    pwm_drv_desc.duty_cycle = 0;                        // PWM duty_cycle
    pwm_drv_desc.p_Int_Handler = timer_pwm0_isr;  // PWM int handler
    PWM_init(&pwm_drv_desc);
//    GPIO_Output(24, TRUE);

//    GPIO_Set_Mode(26, 1, FALSE, FALSE);
//    enable_pwm0();
//    BK3000_PMU_PERI_PWDS &= ~(bit_PMU_PWM1_PWD | PMU_PWM1_CLK_SEL_ROSC);
}

void RAM_CODE timer_pwm0_swi(void)
{
    MSG_T msg;
    os_timer_t *tmp;

    while (timers_head && timers_head->ticks <= tick_counter)
    {
        tmp = timers_head;
        timers_head = timers_head->next;
        tmp->is_pending = FALSE;

        msg.id = (uint32)MSG_TIMER_PWM0_HDL;
        msg.hdl = (void *)tmp->f;
        //msg.arg = (uint32)tmp->arg;  ///frank 190729 ???

        msg_lush_put(&msg);
    }
}

void timer_pwm0_isr(unsigned char ucChannel)
{
//    BK3000_PWM0_CONF |= bit_PWM_PT2_INT_FLAG;           /* clear the interrupt*/
    
//    UART_PRINTF("t\r\n");
//    GPIO_Output_Reverse(24);
    if ((REG_PWM0_CNT & PWM0_CNT_END_VALUE_MASK) == 64000)
    {
        PWM_disable(0);
        DelayMS(1);
        REG_PWM0_CNT = PWM0_COUNT;
        DelayMS(1);
        PWM_enable(0);
        PWM_int_enable(0);
    }
    tick_counter ++;
//    GPIO_Output_Reverse(26);
    timer_polling_handler();
}

uint64_t os_get_tick_counter(void)
{
    return tick_counter;
}

void os_tick_delay(uint32 count)
{
    uint64_t t0 = os_get_tick_counter();

    while((t0 + (count)) > os_get_tick_counter())
    {
        UART_PRINTF("-");
    }
}

void timer_polling_handler(void)
{
	static uint16 timer_1s_cnt = 0;
	timer_1s_cnt++;
	if(timer_1s_cnt == 50)
	{
		msg_put((uint32)MSG_HALF_SECOND);
	}
	else
	{
		 if(timer_1s_cnt >= 100)
		 {
		 	timer_1s_cnt = 0;
			msg_put((uint32)MSG_HALF_SECOND);
			msg_put((uint32)MSG_1S_TIMER);
		 }
	}
	
//    app_button_led_action();

//    INC_SLEEP_TICK;
    
	if(timeoutEnableflag)
		timeout_timer++;
//	app_wave_file_play();
//	app_low_power_scanning();
//    app_sd_scanning();
//	app_linein_scanning();
//    key_scanning();
    timer_pwm0_swi();
	MGC_AfsTimerExpired();
}

#define	PWM0_PT2_COUNT 2000//750//1MHz (0.75ms)
void enable_pwm0_pt2(void) 
{
//    BK3000_PWM0_PT2_CONF = (PWM0_PT2_COUNT << sft_PWM_PT2_CNT )
//        | (1 << sft_PWM_PT2_ENABLE);
    PWM_enable(3);
}

void timer_pwm0_pt2_init(void)
{
//    UART_PRINTF("%s\r\n", __FUNCTION__);
    
//    enable_pwm0_pt2();
//    BK3000_PMU_PERI_PWDS &= ~(bit_PMU_PWM0_PWD);
//	BK3000_PMU_PERI_PWDS &= ~(0x3 << 14);//wd timer select
}

RAM_CODE void timer_pwm0_pt2_isr(void)
{
//	uint8 led_num;
////	 gpio_output(20,1);
//	uint32 reg = BK3000_PWM0_PT2_CONF;
////	uint16 timer_config_val;
////	BK3000_PWM0_PT2_CONF |= bit_PWM_PT2_INT_FLAG; 
//	reg|= bit_PWM_PT2_INT_FLAG;
//	led_num = led_scan();
//	BK3000_PWM0_PT2_CONF = reg;
////	gpio_output(20,0);
}


void timer_pwm0_pt0_isr(void)
{
}
void timer_pwm0_pt1_isr(void)
{
}
void timer_pwm1_pt0_isr(void)
{
}
void timer_pwm1_pt1_isr(void)
{
}


/* end of file*/
