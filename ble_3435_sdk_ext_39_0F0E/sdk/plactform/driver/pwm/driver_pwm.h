/*************************************************************
 * @file        driver_pwm.h
 * @brief       Header file of driver_pwm.c
 * @author      GuWenFu
 * @version     V1.0
 * @date        2016-09-29
 * @par         
 * @attention   
 *
 * @history     2016-09-29 gwf    create this file
 */

#ifndef __DRIVER_PWM_H__

#define __DRIVER_PWM_H__


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

    
#include "BK_System.h"

    
#if (CHIP_ID == CHIP_7211)
#define REG_PWM_BASE_ADDR                   (0x00802A00UL)
#define PWM_CHANNEL_NUMBER_ALL              6
#define PWM_CHANNEL_NUMBER_MAX              (PWM_CHANNEL_NUMBER_ALL - 1)
#define PWM_CHANNEL_NUMBER_MIN              0
#elif (CHIP_ID == CHIP_3433)        /* #if (CHIP_ID == CHIP_7211) */
#define REG_PWM_BASE_ADDR                   (0x00806100UL)
#define PWM_CHANNEL_NUMBER_ALL              5
#define PWM_CHANNEL_NUMBER_MAX              (PWM_CHANNEL_NUMBER_ALL - 1)
#define PWM_CHANNEL_NUMBER_MIN              0
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))        /* #if (CHIP_ID == CHIP_7211) */
#define REG_PWM_BASE_ADDR                   (0x00806100UL)
#define PWM_CHANNEL_NUMBER_ALL              6
#define PWM_CHANNEL_NUMBER_MAX              (PWM_CHANNEL_NUMBER_ALL - 1)
#define PWM_CHANNEL_NUMBER_MIN              0
#endif      /* #if (CHIP_ID == CHIP_7211) */


#ifdef PWM_3231_STYLE
#define REG_PWM_CTRL_ADDR                   (REG_PWM_BASE_ADDR + 0x00 * 4)
#define REG_PWM_CTRL_MASK                   0xFFFFFFFFUL
#define REG_PWM_CTRL                        (*((volatile unsigned long *) REG_PWM_CTRL_ADDR))

#define PWM_CTRL_PWM_X_ENABLE_POSI(x)       (4 * (x))
#define PWM_CTRL_PWM_X_ENABLE_MASK(x)       (0x01UL << PWM_CTRL_PWM_X_ENABLE_POSI(x))
#define PWM_CTRL_PWM_X_ENABLE_SET(x)        (0x01UL << PWM_CTRL_PWM_X_ENABLE_POSI(x))

#define PWM_CTRL_PWM_X_INT_ENABLE_POSI(x)   (4 * (x) + 1)
#define PWM_CTRL_PWM_X_INT_ENABLE_MASK(x)   (0x01UL << PWM_CTRL_PWM_X_INT_ENABLE_POSI(x))
#define PWM_CTRL_PWM_X_INT_ENABLE_SET(x)    (0x01UL << PWM_CTRL_PWM_X_INT_ENABLE_POSI(x))

#define PWM_CTRL_PWM_X_MODE_POSI(x)         (4 * (x) + 2)
#define PWM_CTRL_PWM_X_MODE_MASK(x)         (0x03UL << PWM_CTRL_PWM_X_MODE_POSI(x))
#define PWM_CTRL_PWM_X_MODE_PWM_MODE(x)     (0x00UL << PWM_CTRL_PWM_X_MODE_POSI(x))
#define PWM_CTRL_PWM_X_MODE_TIMER(x)        (0x01UL << PWM_CTRL_PWM_X_MODE_POSI(x))
#define PWM_CTRL_PWM_X_MODE_CAP_POSEDGE(x)  (0x02UL << PWM_CTRL_PWM_X_MODE_POSI(x))
#define PWM_CTRL_PWM_X_MODE_CAP_NEGEDGE(x)  (0x03UL << PWM_CTRL_PWM_X_MODE_POSI(x))


#define REG_PWM_INTR_ADDR                   (REG_PWM_BASE_ADDR + 0x01 * 4)
#define REG_PWM_INTR_MASK                   0x3F
#define REG_PWM_INTR                        (*((volatile unsigned long *) REG_PWM_INTR_ADDR))

#define PWM_INTR_X_INT_STATUS_POSI(x)       (1 * (x))
#define PWM_INTR_X_INT_STATUS_MASK(x)       (0x01UL << PWM_INTR_X_INT_STATUS_POSI(x))
#define PWM_INTR_X_INT_STATUS_GET(x)        (REG_PWM_INTR & PWM_INTR_X_INT_STATUS_MASK(x))
#define PWM_INTR_X_INT_STATUS_CLEAR(x)      do {REG_PWM_INTR = PWM_INTR_X_INT_STATUS_MASK(x);} while(0)


#define REG_PWM0_CNT_ADDR                   (REG_PWM_BASE_ADDR + 0x02 * 4)
#define REG_PWM0_CNT_MASK                   0xFFFFFFFFUL
#define REG_PWM0_CNT                        (*((volatile unsigned long *) REG_PWM0_CNT_ADDR))

#define PWM0_CNT_END_VALUE_POSI             0
#define PWM0_CNT_END_VALUE_MASK             (0x0000FFFFUL << PWM0_CNT_END_VALUE_POSI)

#define PWM0_CNT_DUTY_CYCLE_POSI            16
#define PWM0_CNT_DUTY_CYCLE_MASK            (0x0000FFFFUL << PWM0_CNT_DUTY_CYCLE_POSI)


#define REG_PWM0_CAP_OUT_ADDR               (REG_PWM_BASE_ADDR + 0x03 * 4)
#define REG_PWM0_CAP_OUT_MASK               0x0000FFFFUL
#define REG_PWM0_CAP_OUT                    (*((volatile unsigned long *) REG_PWM0_CAP_OUT_ADDR))

#define PWM0_CAP_OUT_CAP_OUT_POSI           0
#define PWM0_CAP_OUT_CAP_OUT_MASK           (0x0000FFFFUL << PWM0_CAP_OUT_CAP_OUT_POSI)


#define REG_PWM1_CNT_ADDR                   (REG_PWM_BASE_ADDR + 0x04 * 4)
#define REG_PWM1_CNT_MASK                   0xFFFFFFFFUL
#define REG_PWM1_CNT                        (*((volatile unsigned long *) REG_PWM1_CNT_ADDR))

#define PWM1_CNT_END_VALUE_POSI             0
#define PWM1_CNT_END_VALUE_MASK             (0x0000FFFFUL << PWM1_CNT_END_VALUE_POSI)

#define PWM1_CNT_DUTY_CYCLE_POSI            16
#define PWM1_CNT_DUTY_CYCLE_MASK            (0x0000FFFFUL << PWM1_CNT_DUTY_CYCLE_POSI)


#define REG_PWM1_CAP_OUT_ADDR               (REG_PWM_BASE_ADDR + 0x05 * 4)
#define REG_PWM1_CAP_OUT_MASK               0x0000FFFFUL
#define REG_PWM1_CAP_OUT                    (*((volatile unsigned long *) REG_PWM1_CAP_OUT_ADDR))

#define PWM1_CAP_OUT_CAP_OUT_POSI           0
#define PWM1_CAP_OUT_CAP_OUT_MASK           (0x0000FFFFUL << PWM1_CAP_OUT_CAP_OUT_POSI)


#define REG_PWM2_CNT_ADDR                   (REG_PWM_BASE_ADDR + 0x06 * 4)
#define REG_PWM2_CNT_MASK                   0xFFFFFFFFUL
#define REG_PWM2_CNT                        (*((volatile unsigned long *) REG_PWM2_CNT_ADDR))

#define PWM2_CNT_END_VALUE_POSI             0
#define PWM2_CNT_END_VALUE_MASK             (0x0000FFFFUL << PWM2_CNT_END_VALUE_POSI)

#define PWM2_CNT_DUTY_CYCLE_POSI            16
#define PWM2_CNT_DUTY_CYCLE_MASK            (0x0000FFFFUL << PWM2_CNT_DUTY_CYCLE_POSI)


#define REG_PWM2_CAP_OUT_ADDR               (REG_PWM_BASE_ADDR + 0x07 * 4)
#define REG_PWM2_CAP_OUT_MASK               0x0000FFFFUL
#define REG_PWM2_CAP_OUT                    (*((volatile unsigned long *) REG_PWM2_CAP_OUT_ADDR))

#define PWM2_CAP_OUT_CAP_OUT_POSI           0
#define PWM2_CAP_OUT_CAP_OUT_MASK           (0x0000FFFFUL << PWM2_CAP_OUT_CAP_OUT_POSI)


#define REG_PWM3_CNT_ADDR                   (REG_PWM_BASE_ADDR + 0x08 * 4)
#define REG_PWM3_CNT_MASK                   0xFFFFFFFFUL
#define REG_PWM3_CNT                        (*((volatile unsigned long *) REG_PWM3_CNT_ADDR))

#define PWM3_CNT_END_VALUE_POSI             0
#define PWM3_CNT_END_VALUE_MASK             (0x0000FFFFUL << PWM3_CNT_END_VALUE_POSI)

#define PWM3_CNT_DUTY_CYCLE_POSI            16
#define PWM3_CNT_DUTY_CYCLE_MASK            (0x0000FFFFUL << PWM3_CNT_DUTY_CYCLE_POSI)


#define REG_PWM3_CAP_OUT_ADDR               (REG_PWM_BASE_ADDR + 0x09 * 4)
#define REG_PWM3_CAP_OUT_MASK               0x0000FFFFUL
#define REG_PWM3_CAP_OUT                    (*((volatile unsigned long *) REG_PWM3_CAP_OUT_ADDR))

#define PWM3_CAP_OUT_CAP_OUT_POSI           0
#define PWM3_CAP_OUT_CAP_OUT_MASK           (0x0000FFFFUL << PWM3_CAP_OUT_CAP_OUT_POSI)


#define REG_PWM4_CNT_ADDR                   (REG_PWM_BASE_ADDR + 0x0A * 4)
#define REG_PWM4_CNT_MASK                   0xFFFFFFFFUL
#define REG_PWM4_CNT                        (*((volatile unsigned long *) REG_PWM4_CNT_ADDR))

#define PWM4_CNT_END_VALUE_POSI             0
#define PWM4_CNT_END_VALUE_MASK             (0x0000FFFFUL << PWM4_CNT_END_VALUE_POSI)

#define PWM4_CNT_DUTY_CYCLE_POSI            16
#define PWM4_CNT_DUTY_CYCLE_MASK            (0x0000FFFFUL << PWM4_CNT_DUTY_CYCLE_POSI)


#define REG_PWM4_CAP_OUT_ADDR               (REG_PWM_BASE_ADDR + 0x0B * 4)
#define REG_PWM4_CAP_OUT_MASK               0x0000FFFFUL
#define REG_PWM4_CAP_OUT                    (*((volatile unsigned long *) REG_PWM4_CAP_OUT_ADDR))

#define PWM4_CAP_OUT_CAP_OUT_POSI           0
#define PWM4_CAP_OUT_CAP_OUT_MASK           (0x0000FFFFUL << PWM4_CAP_OUT_CAP_OUT_POSI)


#define REG_PWM5_CNT_ADDR                   (REG_PWM_BASE_ADDR + 0x0C * 4)
#define REG_PWM5_CNT_MASK                   0xFFFFFFFFUL
#define REG_PWM5_CNT                        (*((volatile unsigned long *) REG_PWM5_CNT_ADDR))

#define PWM5_CNT_END_VALUE_POSI             0
#define PWM5_CNT_END_VALUE_MASK             (0x0000FFFFUL << PWM5_CNT_END_VALUE_POSI)

#define PWM5_CNT_DUTY_CYCLE_POSI            16
#define PWM5_CNT_DUTY_CYCLE_MASK            (0x0000FFFFUL << PWM5_CNT_DUTY_CYCLE_POSI)


#define REG_PWM5_CAP_OUT_ADDR               (REG_PWM_BASE_ADDR + 0x0D * 4)
#define REG_PWM5_CAP_OUT_MASK               0x0000FFFFUL
#define REG_PWM5_CAP_OUT                    (*((volatile unsigned long *) REG_PWM5_CAP_OUT_ADDR))

#define PWM5_CAP_OUT_CAP_OUT_POSI           0
#define PWM5_CAP_OUT_CAP_OUT_MASK           (0x0000FFFFUL << PWM5_CAP_OUT_CAP_OUT_POSI)


#define REG_PWM_X_CNT_ADDR(x)               (REG_PWM_BASE_ADDR + (x) * 2 * 4 + 0x08)
#define REG_PWM_X_CNT_MASK                  0xFFFFFFFFUL
#define REG_PWM_X_CNT(x)                    (*((volatile unsigned long *) REG_PWM_X_CNT_ADDR(x)))

#define PWM_CNT_END_VALUE_POSI              0
#define PWM_CNT_END_VALUE_MASK              (0x0000FFFFUL << PWM_CNT_END_VALUE_POSI)

#define PWM_CNT_DUTY_CYCLE_POSI             16
#define PWM_CNT_DUTY_CYCLE_MASK             (0x0000FFFFUL << PWM_CNT_DUTY_CYCLE_POSI)


#define REG_PWM_X_CAP_OUT_ADDR(x)           (REG_PWM_BASE_ADDR + (x) * 2 * 4 + 0x0C)
#define REG_PWM_X_CAP_OUT_MASK              0x0000FFFFUL
#define REG_PWM_X_CAP_OUT(x)                (*((volatile unsigned long *) REG_PWM_X_CAP_OUT_ADDR(x)))

#define PWM_CAP_OUT_CAP_OUT_POSI            0
#define PWM_CAP_OUT_CAP_OUT_MASK            (0x0000FFFFUL << PWM_CAP_OUT_CAP_OUT_POSI)


#elif defined PWM_3433_STYLE        /* #ifdef PWM_3231_STYLE */


#define REG_PWM0_CNT_ADDR                   (REG_PWM_BASE_ADDR + 0x00 * 4)
#define REG_PWM0_CNT_MASK                   0xFFFFFFFFUL
#define REG_PWM0_CNT                        (*((volatile unsigned long *) REG_PWM0_CNT_ADDR))

#define PWM0_CNT_DUTY_CYCLE_POSI            0
#define PWM0_CNT_DUTY_CYCLE_MASK            (0x0000FFFFUL << PWM0_CNT_DUTY_CYCLE_POSI)

#define PWM0_CNT_END_VALUE_POSI             16
#define PWM0_CNT_END_VALUE_MASK             (0x0000FFFFUL << PWM0_CNT_END_VALUE_POSI)


#define REG_PWM1_CNT_ADDR                   (REG_PWM_BASE_ADDR + 0x01 * 4)
#define REG_PWM1_CNT_MASK                   0xFFFFFFFFUL
#define REG_PWM1_CNT                        (*((volatile unsigned long *) REG_PWM1_CNT_ADDR))

#define PWM1_CNT_DUTY_CYCLE_POSI            0
#define PWM1_CNT_DUTY_CYCLE_MASK            (0x0000FFFFUL << PWM1_CNT_DUTY_CYCLE_POSI)

#define PWM1_CNT_END_VALUE_POSI             16
#define PWM1_CNT_END_VALUE_MASK             (0x0000FFFFUL << PWM1_CNT_END_VALUE_POSI)


#define REG_PWM2_CNT_ADDR                   (REG_PWM_BASE_ADDR + 0x02 * 4)
#define REG_PWM2_CNT_MASK                   0xFFFFFFFFUL
#define REG_PWM2_CNT                        (*((volatile unsigned long *) REG_PWM2_CNT_ADDR))

#define PWM2_CNT_DUTY_CYCLE_POSI            0
#define PWM2_CNT_DUTY_CYCLE_MASK            (0x0000FFFFUL << PWM2_CNT_DUTY_CYCLE_POSI)

#define PWM2_CNT_END_VALUE_POSI             16
#define PWM2_CNT_END_VALUE_MASK             (0x0000FFFFUL << PWM2_CNT_END_VALUE_POSI)


#define REG_PWM3_CNT_ADDR                   (REG_PWM_BASE_ADDR + 0x03 * 4)
#define REG_PWM3_CNT_MASK                   0xFFFFFFFFUL
#define REG_PWM3_CNT                        (*((volatile unsigned long *) REG_PWM3_CNT_ADDR))

#define PWM3_CNT_DUTY_CYCLE_POSI            0
#define PWM3_CNT_DUTY_CYCLE_MASK            (0x0000FFFFUL << PWM3_CNT_DUTY_CYCLE_POSI)

#define PWM3_CNT_END_VALUE_POSI             16
#define PWM3_CNT_END_VALUE_MASK             (0x0000FFFFUL << PWM3_CNT_END_VALUE_POSI)


#define REG_PWM4_CNT_ADDR                   (REG_PWM_BASE_ADDR + 0x04 * 4)
#define REG_PWM4_CNT_MASK                   0xFFFFFFFFUL
#define REG_PWM4_CNT                        (*((volatile unsigned long *) REG_PWM4_CNT_ADDR))

#define PWM4_CNT_DUTY_CYCLE_POSI            0
#define PWM4_CNT_DUTY_CYCLE_MASK            (0x0000FFFFUL << PWM4_CNT_DUTY_CYCLE_POSI)

#define PWM4_CNT_END_VALUE_POSI             16
#define PWM4_CNT_END_VALUE_MASK             (0x0000FFFFUL << PWM4_CNT_END_VALUE_POSI)


#define REG_PWM_X_CNT_ADDR(x)               (REG_PWM_BASE_ADDR + (0x00 + (x)) * 4)
#define REG_PWM_X_CNT_MASK                  0xFFFFFFFFUL
#define REG_PWM_X_CNT(x)                    (*((volatile unsigned long *) REG_PWM_X_CNT_ADDR(x)))

#define PWM_X_CNT_DUTY_CYCLE_POSI           0
#define PWM_X_CNT_DUTY_CYCLE_MASK           (0x0000FFFFUL << PWM_X_CNT_DUTY_CYCLE_POSI)

#define PWM_X_CNT_END_VALUE_POSI            16
#define PWM_X_CNT_END_VALUE_MASK            (0x0000FFFFUL << PWM_X_CNT_END_VALUE_POSI)


#define REG_PWM_CONTROL_ADDR                (REG_PWM_BASE_ADDR + 0x05 * 4)
#define REG_PWM_CONTROL_MASK                0x001F0F9FUL
#define REG_PWM_CONTROL                     (*((volatile unsigned long *) REG_PWM_CONTROL_ADDR))

#define PWM_CONTROL_PWM0_MODE_POSI          0
#define PWM_CONTROL_PWM0_MODE_MASK          (0x01UL << PWM_CONTROL_PWM0_MODE_POSI)

#define PWM_CONTROL_PWM1_MODE_POSI          1
#define PWM_CONTROL_PWM1_MODE_MASK          (0x01UL << PWM_CONTROL_PWM1_MODE_POSI)

#define PWM_CONTROL_PWM2_MODE_POSI          2
#define PWM_CONTROL_PWM2_MODE_MASK          (0x01UL << PWM_CONTROL_PWM2_MODE_POSI)

#define PWM_CONTROL_PWM3_MODE_POSI          3
#define PWM_CONTROL_PWM3_MODE_MASK          (0x01UL << PWM_CONTROL_PWM3_MODE_POSI)

#define PWM_CONTROL_PWM4_MODE_POSI          4
#define PWM_CONTROL_PWM4_MODE_MASK          (0x01UL << PWM_CONTROL_PWM4_MODE_POSI)

#define PWM_CONTROL_PWM_X_MODE_POSI(x)      (x)
#define PWM_CONTROL_PWM_X_MODE_MASK(x)      (0x01UL << PWM_CONTROL_PWM_X_MODE_POSI(x))

#define PWM_CONTROL_PWM_ENABLE_POSI         7
#define PWM_CONTROL_PWM_ENABLE_MASK         (0x01UL << PWM_CONTROL_PWM_ENABLE_POSI)

#define PWM_CONTROL_PWM_CLK_DIV_POSI        8
#define PWM_CONTROL_PWM_CLK_DIV_MASK        (0x0FUL << PWM_CONTROL_PWM_CLK_DIV_POSI)

#define PWM_CONTROL_PWM0_INT_FLAG_POSI      16
#define PWM_CONTROL_PWM0_INT_FLAG_MASK      (0x01UL << PWM_CONTROL_PWM0_INT_FLAG_POSI)

#define PWM_CONTROL_PWM1_INT_FLAG_POSI      17
#define PWM_CONTROL_PWM1_INT_FLAG_MASK      (0x01UL << PWM_CONTROL_PWM1_INT_FLAG_POSI)

#define PWM_CONTROL_PWM2_INT_FLAG_POSI      18
#define PWM_CONTROL_PWM2_INT_FLAG_MASK      (0x01UL << PWM_CONTROL_PWM2_INT_FLAG_POSI)

#define PWM_CONTROL_PWM3_INT_FLAG_POSI      19
#define PWM_CONTROL_PWM3_INT_FLAG_MASK      (0x01UL << PWM_CONTROL_PWM3_INT_FLAG_POSI)

#define PWM_CONTROL_PWM4_INT_FLAG_POSI      20
#define PWM_CONTROL_PWM4_INT_FLAG_MASK      (0x01UL << PWM_CONTROL_PWM4_INT_FLAG_POSI)

#define PWM_CONTROL_PWM_X_INT_FLAG_POSI(x)  ((x) + 16)
#define PWM_CONTROL_PWM_X_INT_FLAG_MASK(x)  (0x01UL << PWM_CONTROL_PWM_X_INT_FLAG_POSI(x))
#endif      /* #ifdef PWM_3231_STYLE */


//----------------------------------------------
// PWM driver description
//----------------------------------------------
typedef struct
{
    unsigned char       channel;        // PWM 0~5, GPIOB P1.0~P1.5

/* mode:     PWM mode
 * bit[0]:   PWM enable
 *          0:  PWM disable
 *          1:  PWM enable
 * bit[1]:   PWM interrupt enable
 *          0:  PWM interrupt disable
 *          1:  PWM interrupt enable
 * bit[3:2]: PWM mode selection
 *          00: PWM mode
 *          01: TIMER
 *          10: Capture Posedge
 *          11: Capture Negedge
 * bit[5:4]: PWM clock select
 // 注意：对于部分型号的芯片，PWM的时钟是不能单独设置的，所以修改一个PWM的时钟会导致其他PWM的时钟也跟着改变
 *          00: PWM clock 32KHz
 *          01: PWM clock 26MHz
 *          10/11: PWM clock DPLL
 * bit[7:6]: reserved
 */
    unsigned char       mode;
    unsigned char       pre_divid;		// PWM pre-divide clk
    unsigned short      end_value;      // PWM counter end value
    unsigned short      duty_cycle;     // PWM counter duty cycle, 
                                        // this value must smaller or equal to end value
    void (*p_Int_Handler)(unsigned char ucChannel);     // PWM channel Interrupt Handler
} PWM_DRV_DESC;


extern STATUS PWM_init(PWM_DRV_DESC *p_PWM_drv_desc);
extern void PWM_enable(unsigned char ucChannel);
extern void PWM_disable(unsigned char ucChannel);
extern void PWM_int_enable(unsigned char ucChannel);
extern void PWM_int_disable(unsigned char ucChannel);
extern void PWM_int_handler_clear(unsigned char ucChannel);
extern unsigned short PWM_capture_value_get(unsigned char ucChannel);

extern void PWM_InterruptHandler(unsigned char ucChannel);


#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif      /* __DRIVER_PWM_H__ */
