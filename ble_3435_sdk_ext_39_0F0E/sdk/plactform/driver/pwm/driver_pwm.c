/*************************************************************
 * @file        driver_pwm.c
 * @brief       code of PWM driver of BK3435
 * @author      GuWenFu
 * @version     V1.0
 * @date        2016-09-29
 * @par         
 * @attention   
 *
 * @history     2016-09-29 gwf    create this file
 */

#include <stdio.h>

#include "BK_System.h"
#include "driver_icu.h"
#include "driver_gpio.h"

#include "driver_pwm.h"


#if (CHIP_ID == CHIP_3231S)
#define REG_AHB0_ICU_PWMnCLKCON_ADDR(n)     (AHB_ICU_BASE + 0x2C + 0x04 * (n))
#define REG_AHB0_ICU_PWMnCLKCON(n)          (*( (volatile unsigned long *) REG_AHB0_ICU_PWMnCLKCON_ADDR(n)))

#define REG_APB_BK_PWMn_CNT_ADDR(n)         (APB_PWM_BASE + 0x08 + 2 * 0x04 * (n))
#define REG_APB_BK_PWMn_CNT(n)              (*( (volatile unsigned long *) REG_APB_BK_PWMn_CNT_ADDR(n)))

#define REG_APB_BK_PWMn_CAP_ADDR(n)         (APB_PWM_BASE + 0x0C + 2 * 0x04 * (n))
#define REG_APB_BK_PWMn_CAP(n)              (*( (volatile unsigned long *) REG_APB_BK_PWMn_CAP_ADDR(n)))
#endif      /* #if (CHIP_ID == CHIP_3231S) */


static void (*p_PWM_Int_Handler[PWM_CHANNEL_NUMBER_ALL])(unsigned char ucPWM_channel);


STATUS PWM_init(PWM_DRV_DESC *p_PWM_drv_desc)
{
    if (p_PWM_drv_desc == NULL)
    {
        return ERROR;
    }

    if (p_PWM_drv_desc->channel > PWM_CHANNEL_NUMBER_MAX)
    {
        return ERROR;
    }

    if (p_PWM_drv_desc->duty_cycle > p_PWM_drv_desc->end_value)
    {
        return ERROR;
    }

    // enable GPIO second function
    if ((p_PWM_drv_desc->mode & 0x0C) != 0x04)
    {
        GPIO_PWM_function_enable(p_PWM_drv_desc->channel);
    }

    // set PWM clock enable and clock source
    if (p_PWM_drv_desc->mode & 0x10)
    {
        // 16MHz
    }
    else
    {
        // 32KHz
        DelayMS(1);
    }
#if (CHIP_ID == CHIP_3231S)
    if (p_PWM_drv_desc->channel == PWM_CHANNEL_NUMBER_MAX)
    {
        REG_AHB0_ICU_PWM4CLKCON = (REG_AHB0_ICU_PWM4CLKCON & (~0x00F0))
                                | (0x0 << 4) | ((p_PWM_drv_desc->mode & 0x30) << 1);
    }
    else
    {
        REG_AHB0_ICU_PWMnCLKCON(p_PWM_drv_desc->channel) = 
                          (REG_AHB0_ICU_PWMnCLKCON(p_PWM_drv_desc->channel) & (~0x000F))
                          | 0x0 | ((p_PWM_drv_desc->mode & 0x30) >> 3);
    }
#elif ((CHIP_ID == CHIP_7211) || (CHIP_ID == CHIP_7231))        /* #if (CHIP_ID == CHIP_3231S) */
    ICU_PERI_CLK_PWD_CLEAR(ICU_PERI_CLK_PWD_PWM_X_MASK(p_PWM_drv_desc->channel));
//    ICU_PERI_CLK_GATE_DIS_CLEAR(ICU_PERI_CLK_GATE_DIS_PWM_MASK);

    if (p_PWM_drv_desc->mode & 0x10)
    {
        // select 26MHz
        REG_ICU_PWM_CLK_MUX &= ~ICU_PWM_X_CLK_MUX_SEL_MASK(p_PWM_drv_desc->channel);
    }
    else
    {
        // select 32KHz
        REG_ICU_PWM_CLK_MUX |=  ICU_PWM_X_CLK_MUX_SEL_MASK(p_PWM_drv_desc->channel);
    }
#elif (CHIP_ID == CHIP_3433)     /* #if (CHIP_ID == CHIP_3231S) */
    ICU_PWM_CLK_PWD_CLEAR();
    if (p_PWM_drv_desc->mode & 0x10)
    {
        // select 16MHz
        ICU_PWM_CLK_SEL_16MHZ();
    }
    else
    {
        // select 32KHz
        ICU_PWM_CLK_SEL_32KHZ();
    }
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))     /* #if (CHIP_ID == CHIP_3231S) */
    ICU_PWM_CLK_PWM_X_PWD_CLEAR(p_PWM_drv_desc->channel);
    if (p_PWM_drv_desc->mode & 0x10)
    {
        // select 16MHz
        ICU_PWM_CLK_PWM_X_SEL_16MHZ(p_PWM_drv_desc->channel);
    }
    else
    {
        // select 32KHz
        ICU_PWM_CLK_PWM_X_SEL_32KHZ(p_PWM_drv_desc->channel);
    }
#endif      /* #if (CHIP_ID == CHIP_3231S) */

    // set end_value and duty_cycle
#if (CHIP_ID == CHIP_3231S)
    REG_APB_BK_PWMn_CNT(p_PWM_drv_desc->channel) = 
                          (((unsigned long)p_PWM_drv_desc->duty_cycle & 0x0000FFFF) << 16) 
                        + (( unsigned long)p_PWM_drv_desc->end_value & 0x0000FFFF);
#elif ((CHIP_ID == CHIP_7211) || (CHIP_ID == CHIP_7231))        /* #if (CHIP_ID == CHIP_3231S) */
    REG_PWM_X_CNT(p_PWM_drv_desc->channel) = 
        ((((unsigned long)p_PWM_drv_desc->duty_cycle << PWM_CNT_DUTY_CYCLE_POSI) & PWM_CNT_DUTY_CYCLE_MASK)
       + (((unsigned long)p_PWM_drv_desc->end_value << PWM_CNT_END_VALUE_POSI) & PWM_CNT_END_VALUE_MASK));
#elif (CHIP_ID == CHIP_3433)     /* #if (CHIP_ID == CHIP_3231S) */
    REG_PWM_X_CNT(p_PWM_drv_desc->channel) = 
        ((((unsigned long)p_PWM_drv_desc->duty_cycle << PWM_X_CNT_DUTY_CYCLE_POSI) & PWM_X_CNT_DUTY_CYCLE_MASK)
       + (((unsigned long)p_PWM_drv_desc->end_value << PWM_X_CNT_END_VALUE_POSI) & PWM_X_CNT_END_VALUE_MASK));
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))     /* #if (CHIP_ID == CHIP_3231S) */
    REG_PWM_X_CNT(p_PWM_drv_desc->channel) = 
        ((((unsigned long)p_PWM_drv_desc->duty_cycle << PWM_CNT_DUTY_CYCLE_POSI) & PWM_CNT_DUTY_CYCLE_MASK)
       + (((unsigned long)p_PWM_drv_desc->end_value << PWM_CNT_END_VALUE_POSI) & PWM_CNT_END_VALUE_MASK));
#endif      /* #if (CHIP_ID == CHIP_3231S) */

    // set PWM enable, PWM int enable, PWM mode selection
#if (CHIP_ID == CHIP_3231S)
    REG_APB_BK_PWM_CTRL = (REG_APB_BK_PWM_CTRL & (~(0x0F << (0x04 *  p_PWM_drv_desc->channel))))
                        | ((p_PWM_drv_desc->mode & 0x0F) << (0x04 *  p_PWM_drv_desc->channel));
#elif ((CHIP_ID == CHIP_7211) || (CHIP_ID == CHIP_7231))        /* #if (CHIP_ID == CHIP_3231S) */
    REG_PWM_CTRL = (REG_PWM_CTRL & (~(0x0F << (0x04 *  p_PWM_drv_desc->channel))))
                 | ((p_PWM_drv_desc->mode & 0x0F) << (0x04 *  p_PWM_drv_desc->channel));
#elif (CHIP_ID == CHIP_3433)     /* #if (CHIP_ID == CHIP_3231S) */
    if (p_PWM_drv_desc->mode & 0x04)    // TIMER mode
    {
        REG_PWM_CONTROL = (REG_PWM_CONTROL
                        & (~PWM_CONTROL_PWM_X_MODE_MASK(p_PWM_drv_desc->channel))
                        & (~PWM_CONTROL_PWM_CLK_DIV_MASK))
                        | PWM_CONTROL_PWM_ENABLE_MASK;
    }
    else                                // PWM mode
    {
        // enable GPIO second function
        GPIO_PWM_function_enable(p_PWM_drv_desc->channel);

        REG_PWM_CONTROL = (REG_PWM_CONTROL
                        & (~PWM_CONTROL_PWM_CLK_DIV_MASK))
                        | PWM_CONTROL_PWM_X_MODE_MASK(p_PWM_drv_desc->channel)
                        | PWM_CONTROL_PWM_ENABLE_MASK;
    }
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))     /* #if (CHIP_ID == CHIP_3231S) */
    REG_PWM_CTRL = (REG_PWM_CTRL & (~(0x0F << (0x04 *  p_PWM_drv_desc->channel))))
                 | ((p_PWM_drv_desc->mode & 0x0F) << (0x04 *  p_PWM_drv_desc->channel));
#endif      /* #if (CHIP_ID == CHIP_3231S) */

    if (p_PWM_drv_desc->mode & 0x02)    // int enable
    {
        // install interrupt handler
        p_PWM_Int_Handler[p_PWM_drv_desc->channel] = p_PWM_drv_desc->p_Int_Handler;

#if (CHIP_ID == CHIP_3231S)
        ICU_INT_BIT_ENABLE(INT_STATUS_PWM_bit);
#elif ((CHIP_ID == CHIP_7211) || (CHIP_ID == CHIP_7231))        /* #if (CHIP_ID == CHIP_3231S) */
        ICU_INT_ENABLE_SET(ICU_INT_ENABLE_IRQ_PWM_MASK);
#elif (CHIP_ID == CHIP_3433)     /* #if (CHIP_ID == CHIP_3231S) */
        ICU_INT_ENABLE_SET(ICU_INT_ENABLE_IRQ_PWM_X_MASK(p_PWM_drv_desc->channel));
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))     /* #if (CHIP_ID == CHIP_3231S) */
        ICU_INT_ENABLE_SET(ICU_INT_ENABLE_IRQ_PWM_X_MASK(p_PWM_drv_desc->channel));
#endif      /* #if (CHIP_ID == CHIP_3231S) */
    }
    else                                // int disable
    {
        p_PWM_Int_Handler[p_PWM_drv_desc->channel] = NULL;

#if (CHIP_ID == CHIP_3231S)
        ICU_INT_BIT_DISABLE(INT_STATUS_PWM_bit);
#elif ((CHIP_ID == CHIP_7211) || (CHIP_ID == CHIP_7231))        /* #if (CHIP_ID == CHIP_3231S) */
//        ICU_INT_ENABLE_CLEAR(ICU_INT_ENABLE_IRQ_PWM_MASK);
#elif (CHIP_ID == CHIP_3433)     /* #if (CHIP_ID == CHIP_3231S) */
        ICU_INT_ENABLE_CLEAR(ICU_INT_ENABLE_IRQ_PWM_X_MASK(p_PWM_drv_desc->channel));
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))     /* #if (CHIP_ID == CHIP_3231S) */
        ICU_INT_ENABLE_CLEAR(ICU_INT_ENABLE_IRQ_PWM_X_MASK(p_PWM_drv_desc->channel));
#endif      /* #if (CHIP_ID == CHIP_3231S) */
    }

//    for (ulBuffer1=0; ulBuffer1<1000; ulBuffer1++)
//    {
//        ulBuffer[ulBuffer1] = ulBuffer1;
//    }
//    ulBuffer1 = 0x55AAAA55;
    return OK;
}

void PWM_enable(unsigned char ucChannel)
{
    if (ucChannel > PWM_CHANNEL_NUMBER_MAX)
    {
        return;
    }
#if (CHIP_ID == CHIP_3231S)
    REG_APB_BK_PWM_CTRL |= (1 << (ucChannel*4));
#elif ((CHIP_ID == CHIP_7211) || (CHIP_ID == CHIP_7231))        /* #if (CHIP_ID == CHIP_3231S) */
    REG_PWM_CTRL |= PWM_CTRL_PWM_X_ENABLE_SET(ucChannel);
#elif (CHIP_ID == CHIP_3433)     /* #if (CHIP_ID == CHIP_3231S) */
    REG_PWM_CONTROL |= PWM_CONTROL_PWM_ENABLE_MASK;
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))     /* #if (CHIP_ID == CHIP_3231S) */
    REG_PWM_CTRL |= PWM_CTRL_PWM_X_ENABLE_SET(ucChannel);
#endif      /* #if (CHIP_ID == CHIP_3231S) */
}

void PWM_disable(unsigned char ucChannel)
{
    if (ucChannel > PWM_CHANNEL_NUMBER_MAX)
    {
        return;
    }
#if (CHIP_ID == CHIP_3231S)
    // 芯片内部设计要求, 这里必须要同时关闭中断使能, 否则照样会触发中断
    REG_APB_BK_PWM_CTRL &= ~(3 << (ucChannel*4));
#elif ((CHIP_ID == CHIP_7211) || (CHIP_ID == CHIP_7231))        /* #if (CHIP_ID == CHIP_3231S) */
    // 芯片内部设计要求, 这里必须要同时关闭中断使能, 否则照样会触发中断
    REG_PWM_CTRL &= (~(PWM_CTRL_PWM_X_ENABLE_SET(ucChannel) | PWM_CTRL_PWM_X_INT_ENABLE_SET(ucChannel)));
#elif (CHIP_ID == CHIP_3433)     /* #if (CHIP_ID == CHIP_3231S) */
    // 芯片内部设计要求, 只能清零计数值来禁用
//    REG_PWM_CONTROL &= (~PWM_CONTROL_PWM_ENABLE_MASK);
    REG_PWM_X_CNT(ucChannel) = 0;
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))     /* #if (CHIP_ID == CHIP_3231S) */
    // 芯片内部设计要求, 这里必须要同时关闭中断使能, 否则照样会触发中断
    REG_PWM_CTRL &= (~(PWM_CTRL_PWM_X_ENABLE_SET(ucChannel) | PWM_CTRL_PWM_X_INT_ENABLE_SET(ucChannel)));
#endif      /* #if (CHIP_ID == CHIP_3231S) */
}

void PWM_int_enable(unsigned char ucChannel)
{
    if (ucChannel > PWM_CHANNEL_NUMBER_MAX)
    {
        return;
    }
#if (CHIP_ID == CHIP_3231S)
    REG_APB_BK_PWM_CTRL |= (1 << (ucChannel*4 + 1));
#elif ((CHIP_ID == CHIP_7211) || (CHIP_ID == CHIP_7231))        /* #if (CHIP_ID == CHIP_3231S) */
    REG_PWM_CTRL |= PWM_CTRL_PWM_X_INT_ENABLE_SET(ucChannel);
#elif (CHIP_ID == CHIP_3433)     /* #if (CHIP_ID == CHIP_3231S) */
    //
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))     /* #if (CHIP_ID == CHIP_3231S) */
    REG_PWM_CTRL |= PWM_CTRL_PWM_X_INT_ENABLE_SET(ucChannel);
#endif      /* #if (CHIP_ID == CHIP_3231S) */
}

void PWM_int_disable(unsigned char ucChannel)
{
    if (ucChannel > PWM_CHANNEL_NUMBER_MAX)
    {
        return;
    }
#if (CHIP_ID == CHIP_3231S)
    REG_APB_BK_PWM_CTRL &= ~(1 << (ucChannel*4 + 1));
#elif ((CHIP_ID == CHIP_7211) || (CHIP_ID == CHIP_7231))        /* #if (CHIP_ID == CHIP_3231S) */
    REG_PWM_CTRL &= (~PWM_CTRL_PWM_X_INT_ENABLE_SET(ucChannel));
#elif (CHIP_ID == CHIP_3433)     /* #if (CHIP_ID == CHIP_3231S) */
    //
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))     /* #if (CHIP_ID == CHIP_3231S) */
    REG_PWM_CTRL &= (~PWM_CTRL_PWM_X_INT_ENABLE_SET(ucChannel));
#endif      /* #if (CHIP_ID == CHIP_3231S) */
}

unsigned short PWM_capture_value_get(unsigned char ucChannel)
{
    if (ucChannel > PWM_CHANNEL_NUMBER_MAX)
    {
        return 0;
    }
#if (CHIP_ID == CHIP_3231S)
    return REG_APB_BK_PWMn_CAP(ucChannel);
#elif ((CHIP_ID == CHIP_7211) || (CHIP_ID == CHIP_7231))        /* #if (CHIP_ID == CHIP_3231S) */
    return REG_PWM_X_CAP_OUT(ucChannel);
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))     /* #if (CHIP_ID == CHIP_3231S) */
    return REG_PWM_X_CAP_OUT(ucChannel);
#else
    return 0;
#endif      /* #if (CHIP_ID == CHIP_3231S) */
}

void PWM_int_handler_clear(unsigned char ucChannel)
{
    if (ucChannel > PWM_CHANNEL_NUMBER_MAX)
    {
        return;
    }
    p_PWM_Int_Handler[ucChannel] = NULL;
}

//unsigned short test123(void)
//{
//    return 1234;
//}
//extern unsigned short test312(void);
//unsigned short test234(void)
//{
//    return test312();
//}
//extern unsigned short test345(void);
//unsigned short test456(void)
//{
//    return test345();
//}
//unsigned short test789(void)
//{
//    UART_PRINTF("PWM cap int handler: channel[%d] cap_out = %d\r\n",
//           0, test456());
//    return 0;
//}
//extern unsigned short test987(void);

void PWM_InterruptHandler(unsigned char ucChannel)
{
#if (CHIP_ID == CHIP_3433)
    if (p_PWM_Int_Handler[ucChannel] != NULL)
    {
        (void)p_PWM_Int_Handler[ucChannel](ucChannel);
    }
    REG_PWM_CONTROL = (REG_PWM_CONTROL & (~0x001F0000UL)) 
                    | (0x01UL << (ucChannel + PWM_CONTROL_PWM0_INT_FLAG_POSI));

#else       /* #if (CHIP_ID == CHIP_3433) */
    int i;
    unsigned long ulIntStatus;

#if (CHIP_ID == CHIP_3231S)
    ulIntStatus = REG_APB_BK_PWM_INTR;
#elif ((CHIP_ID == CHIP_7211) || (CHIP_ID == CHIP_7231))        /* #if (CHIP_ID == CHIP_3231S) */
    ulIntStatus = REG_PWM_INTR;
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))     /* #if (CHIP_ID == CHIP_3231S) */
    ulIntStatus = REG_PWM_INTR;
#endif      /* #if (CHIP_ID == CHIP_3231S) */

//    UART_PRINTF("ulIntStatus = 0x%x\r\n", ulIntStatus);
    for (i=0; i<PWM_CHANNEL_NUMBER_ALL; i++)
    {
        if (ulIntStatus & (0x01<<i))
        {
//            REG_APB5_GPIOA_DATA ^= 0xFF;
            if (p_PWM_Int_Handler[i] != NULL)
            {
                (void)p_PWM_Int_Handler[i]((unsigned char)i);
//                test987();
            }
//          UART_PRINTF("PWM cap int handler: channel[%d] cap_out = %d\r\n",
//                 i, test789());
        }
    }
//    DelayUS(200);       // delay

#if (CHIP_ID == CHIP_3231S)
    do
    {
        REG_APB_BK_PWM_INTR = ulIntStatus;
    } while (REG_APB_BK_PWM_INTR & ulIntStatus & 0x3F);         // delay
#elif (CHIP_ID == CHIP_7211)     /* #if (CHIP_ID == CHIP_3231S) */
    do
    {
        REG_PWM_INTR = ulIntStatus;
    } while (REG_PWM_INTR & ulIntStatus & REG_PWM_INTR_MASK);   // delay
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))     /* #if (CHIP_ID == CHIP_3231S) */
    do
    {
        REG_PWM_INTR = ulIntStatus;
    } while (REG_PWM_INTR & ulIntStatus & REG_PWM_INTR_MASK);   // delay
#elif (CHIP_ID == CHIP_7231)
    REG_PWM_INTR = ulIntStatus;
#endif      /* #if (CHIP_ID == CHIP_3231S) */

//    ulIntStatus = REG_PWM_INTR;
//    UART_PRINTF("ulIntStatus = 0x%x\r\n", ulIntStatus);

//    UART_PRINTF("\r\n");
#endif      /* #if (CHIP_ID == CHIP_3433) */
}
