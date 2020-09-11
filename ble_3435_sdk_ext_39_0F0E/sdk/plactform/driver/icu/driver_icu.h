/*************************************************************
 * @file        driver_icu.h
 * @brief       Header file of driver_icu.c
 * @author      GuWenFu
 * @version     V1.0
 * @date        2016-09-29
 * @par         
 * @attention   
 *
 * @history     2016-09-29 gwf    create this file
 */

#ifndef __DRIVER_ICU_H__

#define __DRIVER_ICU_H__


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#include "config.h"


#if ((CHIP_ID == CHIP_3433) || (CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))
#define REG_ICU_BASE_ADDR                   (0x00800000UL)


#if (CHIP_ID == CHIP_3433)
#define REG_ICU_POWER_CONTROL_ADDR          (REG_ICU_BASE_ADDR + 0x00 * 4)
#define REG_ICU_POWER_CONTROL_MASK          0x00FFUL
#define REG_ICU_POWER_CONTROL               (*((volatile unsigned long *) REG_ICU_POWER_CONTROL_ADDR))

#define ICU_POWER_CONTROL_CLK_SRC_SEL_POSI  0
#define ICU_POWER_CONTROL_CLK_SRC_SEL_MASK  (0x03UL << ICU_POWER_CONTROL_CLK_SRC_SEL_POSI)

#define ICU_POWER_CONTROL_SLEEP_SEL_POSI    2
#define ICU_POWER_CONTROL_SLEEP_SEL_MASK    (0x01UL << ICU_POWER_CONTROL_SLEEP_SEL_POSI)

#define ICU_POWER_CONTROL_VOLTAGE_POSI      3
#define ICU_POWER_CONTROL_VOLTAGE_MASK      (0x0FUL << ICU_POWER_CONTROL_VOLTAGE_POSI)

#define ICU_POWER_CONTROL_POWER_ON_FLAG_POSI     7
#define ICU_POWER_CONTROL_POWER_ON_FLAG_MASK     (0x01UL << ICU_POWER_CONTROL_POWER_ON_FLAG_POSI)

#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))        /* #if (CHIP_ID == CHIP_3433) */

#define REG_ICU_POWER_CONTROL_ADDR          (REG_ICU_BASE_ADDR + 0x00 * 4)
#define REG_ICU_POWER_CONTROL_MASK          0x03FFUL
#define REG_ICU_POWER_CONTROL               (*((volatile unsigned long *) REG_ICU_POWER_CONTROL_ADDR))

#define ICU_POWER_CONTROL_CLK_SRC_SEL_POSI  0
#define ICU_POWER_CONTROL_CLK_SRC_SEL_MASK  (0x03UL << ICU_POWER_CONTROL_CLK_SRC_SEL_POSI)

#define ICU_POWER_CONTROL_SLEEP_SEL_POSI    2
#define ICU_POWER_CONTROL_SLEEP_SEL_MASK    (0x01UL << ICU_POWER_CONTROL_SLEEP_SEL_POSI)

#define ICU_POWER_CONTROL_VOLTAGE_SLEEP_POSI    3
#define ICU_POWER_CONTROL_VOLTAGE_SLEEP_MASK    (0x07UL << ICU_POWER_CONTROL_VOLTAGE_SLEEP_POSI)

#define ICU_POWER_CONTROL_VOLTAGE_NORMAL_POSI   6
#define ICU_POWER_CONTROL_VOLTAGE_NORMAL_MASK   (0x07UL << ICU_POWER_CONTROL_VOLTAGE_NORMAL_POSI)

#define ICU_POWER_CONTROL_JTAG_MODE_POSI    9
#define ICU_POWER_CONTROL_JTAG_MODE_MASK    (0x01UL << ICU_POWER_CONTROL_JTAG_MODE_POSI)

#define CLOSE_JTAG_MODE()                   do {REG_ICU_POWER_CONTROL &= (~ICU_POWER_CONTROL_JTAG_MODE_MASK);} while(0)
#define OPEN_JTAG_MODE()                    do {REG_ICU_POWER_CONTROL |= ( ICU_POWER_CONTROL_JTAG_MODE_MASK);} while(0)
#endif      /* #if (CHIP_ID == CHIP_3433) */


#define REG_ICU_CORE_CLK_ADDR               (REG_ICU_BASE_ADDR + 0x01 * 4)
#define REG_ICU_CORE_CLK_MASK               0x00FFUL
#define REG_ICU_CORE_CLK                    (*((volatile unsigned long *) REG_ICU_CORE_CLK_ADDR))

#define ICU_CORE_CLK_PWD_POSI               0
#define ICU_CORE_CLK_PWD_MASK               (0x01UL << ICU_CORE_CLK_PWD_POSI)

#define ICU_CORE_CLK_DIV_POSI               1
#define ICU_CORE_CLK_DIV_MASK               (0x7FUL << ICU_CORE_CLK_DIV_POSI)


#define REG_ICU_ADC_CLK_ADDR                (REG_ICU_BASE_ADDR + 0x02 * 4)
#define REG_ICU_ADC_CLK_MASK                0x01FFUL
#define REG_ICU_ADC_CLK                     (*((volatile unsigned long *) REG_ICU_ADC_CLK_ADDR))

#define ICU_ADC_CLK_PWD_POSI                0
#define ICU_ADC_CLK_PWD_MASK                (0x01UL << ICU_ADC_CLK_PWD_POSI)

#define ICU_ADC_CLK_DIV_POSI                1
#define ICU_ADC_CLK_DIV_MASK                (0x7FUL << ICU_ADC_CLK_DIV_POSI)

#define ICU_ADC_CLK_SEL_POSI                8
#define ICU_ADC_CLK_SEL_MASK                (0x01UL << ICU_ADC_CLK_SEL_POSI)

#define ICU_ADC_CLK_PWD_CLEAR()             do {REG_ICU_ADC_CLK &= (~0x0001);} while (0)
#define ICU_ADC_CLK_PWD_SET()               do {REG_ICU_ADC_CLK |=   0x0001;} while (0)


#define REG_ICU_UART_CLK_ADDR               (REG_ICU_BASE_ADDR + 0x03 * 4)
#define REG_ICU_UART_CLK_MASK               0x01UL
#define REG_ICU_UART_CLK                    (*((volatile unsigned long *) REG_ICU_UART_CLK_ADDR))

#define ICU_UART_CLK_PWD_POSI               0
#define ICU_UART_CLK_PWD_MASK               (0x01UL << ICU_UART_CLK_PWD_POSI)

#define ICU_UART_CLK_PWD_CLEAR()            do {REG_ICU_UART_CLK = 0x00;} while (0)
#define ICU_UART_CLK_PWD_SET()              do {REG_ICU_UART_CLK = 0x01;} while (0)

#define ICU_UART1_CLK_PWD_POSI              0
#define ICU_UART1_CLK_PWD_MASK              (0x01UL << ICU_UART_CLK_PWD_POSI)

#define ICU_UART1_CLK_PWD_CLEAR()           do {REG_ICU_UART1_CLK = 0x00;} while (0)
#define ICU_UART1_CLK_PWD_SET()             do {REG_ICU_UART1_CLK = 0x01;} while (0)


#define REG_ICU_I2C_CLK_ADDR                (REG_ICU_BASE_ADDR + 0x04 * 4)
#define REG_ICU_I2C_CLK_MASK                0x01UL
#define REG_ICU_I2C_CLK                     (*((volatile unsigned long *) REG_ICU_I2C_CLK_ADDR))

#define ICU_I2C_CLK_PWD_POSI                0
#define ICU_I2C_CLK_PWD_MASK                (0x01UL << ICU_I2C_CLK_PWD_POSI)

#define ICU_I2C_CLK_PWD_CLEAR()             do {REG_ICU_I2C_CLK = 0x00;} while (0)
#define ICU_I2C_CLK_PWD_SET()               do {REG_ICU_I2C_CLK = 0x01;} while (0)


#define REG_ICU_SPI_CLK_ADDR                (REG_ICU_BASE_ADDR + 0x05 * 4)
#define REG_ICU_SPI_CLK_MASK                0x01UL
#define REG_ICU_SPI_CLK                     (*((volatile unsigned long *) REG_ICU_SPI_CLK_ADDR))

#define ICU_SPI_CLK_PWD_POSI                0
#define ICU_SPI_CLK_PWD_MASK                (0x01UL << ICU_SPI_CLK_PWD_POSI)

#define ICU_SPI_CLK_PWD_CLEAR()             do {REG_ICU_SPI_CLK = 0x00;} while (0)
#define ICU_SPI_CLK_PWD_SET()               do {REG_ICU_SPI_CLK = 0x01;} while (0)


#define REG_ICU_BLE_CLK_ADDR                (REG_ICU_BASE_ADDR + 0x06 * 4)
#define REG_ICU_BLE_CLK_MASK                0x07UL
#define REG_ICU_BLE_CLK                     (*((volatile unsigned long *) REG_ICU_BLE_CLK_ADDR))

#define ICU_BLE_CLK_PWD_POSI                0
#define ICU_BLE_CLK_PWD_MASK                (0x01UL << ICU_BLE_CLK_PWD_POSI)

#define ICU_BLE_CLK_RW_CLK_SEL_POSI         1
#define ICU_BLE_CLK_RW_CLK_SEL_MASK         (0x03UL << ICU_BLE_CLK_RW_CLK_SEL_POSI)


#define REG_ICU_WDT_CLK_ADDR                (REG_ICU_BASE_ADDR + 0x07 * 4)
#define REG_ICU_WDT_CLK_MASK                0x01UL
#define REG_ICU_WDT_CLK                     (*((volatile unsigned long *) REG_ICU_WDT_CLK_ADDR))

#define ICU_WDT_CLK_PWD_POSI                0
#define ICU_WDT_CLK_PWD_MASK                (0x01UL << ICU_WDT_CLK_PWD_POSI)

#define ICU_WDT_CLK_PWD_CLEAR()             do {REG_ICU_WDT_CLK = 0x00;} while (0)
#define ICU_WDT_CLK_PWD_SET()               do {REG_ICU_WDT_CLK = 0x01;} while (0)


#if (CHIP_ID == CHIP_3433)
#define REG_ICU_PWM_CLK_ADDR                (REG_ICU_BASE_ADDR + 0x09 * 4)
#define REG_ICU_PWM_CLK_MASK                0x01UL
#define REG_ICU_PWM_CLK                     (*((volatile unsigned long *) REG_ICU_PWM_CLK_ADDR))

#define ICU_PWM_CLK_PWD_POSI                0
#define ICU_PWM_CLK_PWD_MASK                (0x01UL << ICU_PWM_CLK_PWD_POSI)

#define ICU_PWM_CLK_SEL_POSI                1
#define ICU_PWM_CLK_SEL_MASK                (0x01UL << ICU_PWM_CLK_SEL_POSI)

#define ICU_PWM_CLK_PWD_CLEAR()             do {REG_ICU_PWM_CLK &= ~0x01;} while (0)
#define ICU_PWM_CLK_PWD_SET()               do {REG_ICU_PWM_CLK |=  0x01;} while (0)

#define ICU_PWM_CLK_SEL_32KHZ()             do {REG_ICU_PWM_CLK &= ~0x02;} while (0)
#define ICU_PWM_CLK_SEL_16MHZ()             do {REG_ICU_PWM_CLK |=  0x02;} while (0)

#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))        /* #if (CHIP_ID == CHIP_3433) */

#if (CHIP_ID == CHIP_3435_v2)
#define REG_ICU_I2S_SDM_USB_ADDR            (REG_ICU_BASE_ADDR + 0x08 * 4)
#define REG_ICU_I2S_SDM_USB_MASK            0x07UL
#define REG_ICU_I2S_SDM_USB                 (*((volatile unsigned long *) REG_ICU_I2S_SDM_USB_ADDR))

#define ICU_USB_PWD_CLEAR()                 do {REG_ICU_I2S_SDM_USB &= ~0x01;} while (0)
#define ICU_USB_PWD_SET()                   do {REG_ICU_I2S_SDM_USB |=  0x01;} while (0)

#define ICU_PDM_PWD_CLEAR()                 do {REG_ICU_I2S_SDM_USB &= ~0x02;} while (0)
#define ICU_PDM_PWD_SET()                   do {REG_ICU_I2S_SDM_USB |=  0x02;} while (0)

#define ICU_I2S_PWD_CLEAR()                 do {REG_ICU_I2S_SDM_USB &= ~0x04;} while (0)
#define ICU_I2S_PWD_SET()                   do {REG_ICU_I2S_SDM_USB |=  0x04;} while (0)
#endif


#define REG_ICU_PWM_CLK_ADDR                (REG_ICU_BASE_ADDR + 0x09 * 4)
#define REG_ICU_PWM_CLK_MASK                0x1FFFUL
#define REG_ICU_PWM_CLK                     (*((volatile unsigned long *) REG_ICU_PWM_CLK_ADDR))

#define ICU_PWM_CLK_PWM0_PWD_POSI           0
#define ICU_PWM_CLK_PWM0_PWD_MASK           (0x01UL << ICU_PWM_CLK_PWM0_PWD_POSI)

#define ICU_PWM_CLK_PWM0_SEL_POSI           1
#define ICU_PWM_CLK_PWM0_SEL_MASK           (0x01UL << ICU_PWM_CLK_PWM0_SEL_POSI)

#define ICU_PWM_CLK_PWM1_PWD_POSI           2
#define ICU_PWM_CLK_PWM1_PWD_MASK           (0x01UL << ICU_PWM_CLK_PWM1_PWD_POSI)

#define ICU_PWM_CLK_PWM1_SEL_POSI           3
#define ICU_PWM_CLK_PWM1_SEL_MASK           (0x01UL << ICU_PWM_CLK_PWM1_SEL_POSI)

#define ICU_PWM_CLK_PWM2_PWD_POSI           4
#define ICU_PWM_CLK_PWM2_PWD_MASK           (0x01UL << ICU_PWM_CLK_PWM2_PWD_POSI)

#define ICU_PWM_CLK_PWM2_SEL_POSI           5
#define ICU_PWM_CLK_PWM2_SEL_MASK           (0x01UL << ICU_PWM_CLK_PWM2_SEL_POSI)

#define ICU_PWM_CLK_PWM3_PWD_POSI           6
#define ICU_PWM_CLK_PWM3_PWD_MASK           (0x01UL << ICU_PWM_CLK_PWM3_PWD_POSI)

#define ICU_PWM_CLK_PWM3_SEL_POSI           7
#define ICU_PWM_CLK_PWM3_SEL_MASK           (0x01UL << ICU_PWM_CLK_PWM3_SEL_POSI)

#define ICU_PWM_CLK_PWM4_PWD_POSI           8
#define ICU_PWM_CLK_PWM4_PWD_MASK           (0x01UL << ICU_PWM_CLK_PWM4_PWD_POSI)

#define ICU_PWM_CLK_PWM4_SEL_POSI           9
#define ICU_PWM_CLK_PWM4_SEL_MASK           (0x01UL << ICU_PWM_CLK_PWM4_SEL_POSI)

#define ICU_PWM_CLK_PWM5_PWD_POSI           10
#define ICU_PWM_CLK_PWM5_PWD_MASK           (0x01UL << ICU_PWM_CLK_PWM5_PWD_POSI)

#define ICU_PWM_CLK_PWM5_SEL_POSI           11
#define ICU_PWM_CLK_PWM5_SEL_MASK           (0x01UL << ICU_PWM_CLK_PWM5_SEL_POSI)

#define ICU_PWM_CLK_PWM_3DS_PWD_POSI        12
#define ICU_PWM_CLK_PWM_3DS_PWD_MASK        (0x01UL << ICU_PWM_CLK_PWM_3DS_PWD_POSI)

#define ICU_PWM_CLK_PWM_X_PWD_POSI(x)       ((x) * 2)
#define ICU_PWM_CLK_PWM_X_PWD_MASK(x)       (0x01UL << ICU_PWM_CLK_PWM_X_PWD_POSI(x))

#define ICU_PWM_CLK_PWM_X_SEL_POSI(x)       ((x) * 2 + 1)
#define ICU_PWM_CLK_PWM_X_SEL_MASK(x)       (0x01UL << ICU_PWM_CLK_PWM_X_SEL_POSI(x))

#define ICU_PWM_CLK_PWM_X_PWD_CLEAR(x)      do {REG_ICU_PWM_CLK &= (~ICU_PWM_CLK_PWM_X_PWD_MASK(x));} while (0)
#define ICU_PWM_CLK_PWM_X_PWD_SET(x)        do {REG_ICU_PWM_CLK |= ( ICU_PWM_CLK_PWM_X_PWD_MASK(x));} while (0)

#define ICU_PWM_CLK_PWM_X_SEL_32KHZ(x)      do {REG_ICU_PWM_CLK &= (~ICU_PWM_CLK_PWM_X_SEL_MASK(x));} while (0)
#define ICU_PWM_CLK_PWM_X_SEL_16MHZ(x)      do {REG_ICU_PWM_CLK |= ( ICU_PWM_CLK_PWM_X_SEL_MASK(x));} while (0)
#endif      /* #if (CHIP_ID == CHIP_3433) */


#define REG_ICU_RTC_CLK_ADDR                (REG_ICU_BASE_ADDR + 0x0A * 4)
#define REG_ICU_RTC_CLK_MASK                0x01UL
#define REG_ICU_RTC_CLK                     (*((volatile unsigned long *) REG_ICU_RTC_CLK_ADDR))

#define ICU_RTC_CLK_PWD_POSI                0
#define ICU_RTC_CLK_PWD_MASK                (0x01UL << ICU_RTC_CLK_PWD_POSI)

#define ICU_RTC_CLK_PWD_CLEAR()             do {REG_ICU_RTC_CLK = 0x00;} while (0)
#define ICU_RTC_CLK_PWD_SET()               do {REG_ICU_RTC_CLK = 0x01;} while (0)


#define REG_ICU_LPO_CLK_ADDR                (REG_ICU_BASE_ADDR + 0x0B * 4)
#define REG_ICU_LPO_CLK_MASK                0x01UL
#define REG_ICU_LPO_CLK                     (*((volatile unsigned long *) REG_ICU_LPO_CLK_ADDR))

#define ICU_LPO_CLK_PWD_POSI                0
#define ICU_LPO_CLK_PWD_MASK                (0x01UL << ICU_LPO_CLK_PWD_POSI)


#define REG_ICU_32K_CLK_ADDR                (REG_ICU_BASE_ADDR + 0x0C * 4)
#define REG_ICU_32K_CLK_MASK                0x01UL
#define REG_ICU_32K_CLK                     (*((volatile unsigned long *) REG_ICU_32K_CLK_ADDR))

#define ICU_32K_CLK_SEL_POSI                0
#define ICU_32K_CLK_SEL_MASK                (0x01UL << ICU_32K_CLK_SEL_POSI)


#define REG_ICU_RW_CLK_ADDR                 (REG_ICU_BASE_ADDR + 0x0D * 4)
#define REG_ICU_RW_CLK_MASK                 0x81UL
#define REG_ICU_RW_CLK                      (*((volatile unsigned long *) REG_ICU_RW_CLK_ADDR))

#define ICU_RW_CLK_STATUS_POSI              0
#define ICU_RW_CLK_STATUS_MASK              (0x01UL << ICU_RW_CLK_STATUS_POSI)

#define ICU_RW_CLK_OSC_EN_SEL_POSI          7
#define ICU_RW_CLK_OSC_EN_SEL_MASK          (0x01UL << ICU_RW_CLK_OSC_EN_SEL_POSI)


#define REG_ICU_INT_ENABLE_ADDR             (REG_ICU_BASE_ADDR + 0x10 * 4)
#define REG_ICU_INT_ENABLE_MASK             (ICU_INT_ENABLE_IRQ_MASK | ICU_INT_ENABLE_FIQ_MASK)
#define REG_ICU_INT_ENABLE                  (*((volatile unsigned long *) REG_ICU_INT_ENABLE_ADDR))

#define ICU_INT_ENABLE_IRQ_PWM1_POSI        1
#define ICU_INT_ENABLE_IRQ_PWM2_POSI        2
#define ICU_INT_ENABLE_IRQ_PWM3_POSI        3
#define ICU_INT_ENABLE_IRQ_PWM4_POSI        4
#define ICU_INT_ENABLE_IRQ_UART0_POSI       5
#define ICU_INT_ENABLE_IRQ_SPI_POSI         6
#define ICU_INT_ENABLE_IRQ_I2C0_POSI         7
#define ICU_INT_ENABLE_IRQ_ADC_POSI         8
#define ICU_INT_ENABLE_IRQ_GPIO_POSI        9
#define ICU_INT_ENABLE_IRQ_USB_POSI         11
#define ICU_INT_ENABLE_IRQ_RTC_POSI         12
#define ICU_INT_ENABLE_FIQ_PWM0_POSI        0
#define ICU_INT_ENABLE_FIQ_LBD_POSI         10
#if ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))
#define ICU_INT_ENABLE_IRQ_PWM5_POSI        13
#define ICU_INT_ENABLE_IRQ_PWM_3DS_POSI     14
#endif      /* #if (CHIP_ID == CHIP_3435) */
#define ICU_INT_ENABLE_FIQ_BLE_POSI         15
#define ICU_INT_ENABLE_IRQ_UART1_POSI       16
#define ICU_INT_ENABLE_IRQ_SDM_POSI         17
#define ICU_INT_ENABLE_IRQ_I2S_POSI         18
#define ICU_INT_ENABLE_IRQ_PWM_X_POSI(x)    (((x) == 5) ? (13) : (x))

#define ICU_INT_ENABLE_IRQ_PWM1_MASK        (0x01UL << ICU_INT_ENABLE_IRQ_PWM1_POSI)
#define ICU_INT_ENABLE_IRQ_PWM2_MASK        (0x01UL << ICU_INT_ENABLE_IRQ_PWM2_POSI)
#define ICU_INT_ENABLE_IRQ_PWM3_MASK        (0x01UL << ICU_INT_ENABLE_IRQ_PWM3_POSI)
#define ICU_INT_ENABLE_IRQ_PWM4_MASK        (0x01UL << ICU_INT_ENABLE_IRQ_PWM4_POSI)
#define ICU_INT_ENABLE_IRQ_UART0_MASK       (0x01UL << ICU_INT_ENABLE_IRQ_UART0_POSI)
#define ICU_INT_ENABLE_IRQ_SPI_MASK         (0x01UL << ICU_INT_ENABLE_IRQ_SPI_POSI)
#define ICU_INT_ENABLE_IRQ_I2C0_MASK         (0x01UL << ICU_INT_ENABLE_IRQ_I2C0_POSI)
#define ICU_INT_ENABLE_IRQ_ADC_MASK         (0x01UL << ICU_INT_ENABLE_IRQ_ADC_POSI)
#define ICU_INT_ENABLE_IRQ_GPIO_MASK        (0x01UL << ICU_INT_ENABLE_IRQ_GPIO_POSI)
#define ICU_INT_ENABLE_IRQ_USB_MASK         (0x01UL << ICU_INT_ENABLE_IRQ_USB_POSI)
#define ICU_INT_ENABLE_IRQ_RTC_MASK         (0x01UL << ICU_INT_ENABLE_IRQ_RTC_POSI)
#define ICU_INT_ENABLE_FIQ_PWM0_MASK        (0x01UL << ICU_INT_ENABLE_FIQ_PWM0_POSI)
#define ICU_INT_ENABLE_FIQ_LBD_MASK         (0x01UL << ICU_INT_ENABLE_FIQ_LBD_POSI)
#define ICU_INT_ENABLE_FIQ_BLE_MASK         (0x01UL << ICU_INT_ENABLE_FIQ_BLE_POSI)
#if ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))
#define ICU_INT_ENABLE_IRQ_PWM5_MASK        (0x01UL << ICU_INT_ENABLE_IRQ_PWM5_POSI)
#define ICU_INT_ENABLE_IRQ_PWM_3DS_MASK     (0x01UL << ICU_INT_ENABLE_IRQ_PWM_3DS_POSI)
#endif      /* #if (CHIP_ID == CHIP_3435) */
#define ICU_INT_ENABLE_IRQ_PWM_X_MASK(x)    (0x01UL << ICU_INT_ENABLE_IRQ_PWM_X_POSI(x))
#define ICU_INT_ENABLE_IRQ_UART1_MASK       (0x01UL << ICU_INT_ENABLE_IRQ_UART1_POSI)
#define ICU_INT_ENABLE_IRQ_SDM_MASK         (0x01UL << ICU_INT_ENABLE_IRQ_SDM_POSI)
#define ICU_INT_ENABLE_IRQ_I2S_MASK         (0x01UL << ICU_INT_ENABLE_IRQ_I2S_POSI)
#define ICU_INT_ENABLE_IRQ_MASK             (0x00077BFEUL)
#define ICU_INT_ENABLE_FIQ_MASK             (ICU_INT_ENABLE_FIQ_PWM0_MASK | ICU_INT_ENABLE_FIQ_LBD_MASK | ICU_INT_ENABLE_FIQ_BLE_MASK)


#define REG_ICU_INT_GLOBAL_ENABLE_ADDR      (REG_ICU_BASE_ADDR + 0x11 * 4)
#define REG_ICU_INT_GLOBAL_ENABLE_MASK      0x0003UL
#define REG_ICU_INT_GLOBAL_ENABLE           (*((volatile unsigned long *) REG_ICU_INT_GLOBAL_ENABLE_ADDR))

#define ICU_INT_GLOBAL_ENABLE_IRQ_POSI      0
#define ICU_INT_GLOBAL_ENABLE_IRQ_MASK      (0x01UL << ICU_INT_GLOBAL_ENABLE_IRQ_POSI)
#define ICU_INT_GLOBAL_ENABLE_IRQ_SET       (0x01UL << ICU_INT_GLOBAL_ENABLE_IRQ_POSI)

#define ICU_INT_GLOBAL_ENABLE_FIQ_POSI      1
#define ICU_INT_GLOBAL_ENABLE_FIQ_MASK      (0x01UL << ICU_INT_GLOBAL_ENABLE_FIQ_POSI)
#define ICU_INT_GLOBAL_ENABLE_FIQ_SET       (0x01UL << ICU_INT_GLOBAL_ENABLE_FIQ_POSI)

#define ICU_INT_ENABLE_SET(x)               do {                                    \
                REG_ICU_INT_ENABLE |=  (x);                                         \
             /* if (ICU_INT_ENABLE_FIQ_MODEM_MASK > (x))                            \
                {                                                                   \
                    REG_ICU_INT_GLOBAL_ENABLE |= ICU_INT_GLOBAL_ENABLE_IRQ_MASK;    \
                }                                                                   \
                else                                                                \
                {                                                                   \
                    REG_ICU_INT_GLOBAL_ENABLE |= ICU_INT_GLOBAL_ENABLE_FIQ_MASK;    \
                } */                                                                \
            } while(0)
#define ICU_INT_ENABLE_CLEAR(x)             do {                                    \
                REG_ICU_INT_ENABLE &= (~(x));                                       \
            } while(0)


#define REG_ICU_INT_RAW_STATUS_ADDR         (REG_ICU_BASE_ADDR + 0x12 * 4)
#define REG_ICU_INT_RAW_STATUS_MASK         REG_ICU_INT_ENABLE_MASK
#define REG_ICU_INT_RAW_STATUS              (*((volatile unsigned long *) REG_ICU_INT_RAW_STATUS_ADDR))

#define ICU_INT_RAW_STATUS_BIT_GET(x)       (REG_ICU_INT_RAW_STATUS & (x))
#define ICU_INT_RAW_STATUS_BIT_CLEAR(x)     do {REG_ICU_INT_RAW_STATUS |= (x);} while(0)


#define REG_ICU_INT_STATUS_ADDR             (REG_ICU_BASE_ADDR + 0x12 * 4)
#define REG_ICU_INT_STATUS_MASK             REG_ICU_INT_ENABLE_MASK
#define REG_ICU_INT_STATUS                  (*((volatile unsigned long *) REG_ICU_INT_STATUS_ADDR))

#define ICU_INT_STATUS_BIT_GET(x)           (REG_ICU_INT_STATUS & (x))
#define ICU_INT_STATUS_BIT_CLEAR(x)         do {REG_ICU_INT_STATUS |= (x);} while(0)


#define REG_ICU_CEBCE2B_ADDR                (REG_ICU_BASE_ADDR + 0x13 * 4)
#define REG_ICU_CEBCE2B_MASK                0x0001FFFFUL
#define REG_ICU_CEBCE2B                     (*((volatile unsigned long *) REG_ICU_CEBCE2B_ADDR))

#define ICU_CEBCE2B_TIME_POSI               0
#define ICU_CEBCE2B_TIME_MASK               (0x00FFUL << ICU_CEBCE2B_TIME_POSI)

#define ICU_CEBCE2B_SEL_POSI                16
#define ICU_CEBCE2B_SEL_MASK                (0x01UL << ICU_CEBCE2B_SEL_POSI)


#define REG_ICU_ANALOG_TEST_ADDR            (REG_ICU_BASE_ADDR + 0x14 * 4)
#define REG_ICU_ANALOG_TEST_MASK            0xFFFFFFFFUL
#define REG_ICU_ANALOG_TEST                 (*((volatile unsigned long *) REG_ICU_ANALOG_TEST_ADDR))


#define REG_ICU_ANALOG_PWD_ADDR             (REG_ICU_BASE_ADDR + 0x15 * 4)
#define REG_ICU_ANALOG_PWD_MASK             0xFFFFFFFFUL
#define REG_ICU_ANALOG_PWD                  (*((volatile unsigned long *) REG_ICU_ANALOG_PWD_ADDR))


#define REG_ICU_ANALOG_TEST2_ADDR           (REG_ICU_BASE_ADDR + 0x16 * 4)
#define REG_ICU_ANALOG_TEST2_MASK           0x00FF3FFFUL
#define REG_ICU_ANALOG_TEST2                (*((volatile unsigned long *) REG_ICU_ANALOG_TEST2_ADDR))


#define REG_ICU_DIGITAL_PWD_ADDR            (REG_ICU_BASE_ADDR + 0x17 * 4)
#define REG_ICU_DIGITAL_PWD_MASK            0x00FFUL
#define REG_ICU_DIGITAL_PWD                 (*((volatile unsigned long *) REG_ICU_DIGITAL_PWD_ADDR))


#define REG_ICU_GPIO_DEEP_WAKE1_ADDR        (REG_ICU_BASE_ADDR + 0x18 * 4)
#define REG_ICU_GPIO_DEEP_WAKE1_MASK        0xFFFFFFFFUL
#define REG_ICU_GPIO_DEEP_WAKE1             (*((volatile unsigned long *) REG_ICU_GPIO_DEEP_WAKE1_ADDR))


#define REG_ICU_GPIO_DEEP_WAKE2_ADDR        (REG_ICU_BASE_ADDR + 0x19 * 4)
#define REG_ICU_GPIO_DEEP_WAKE2_MASK        0xFFFF00FFUL
#define REG_ICU_GPIO_DEEP_WAKE2             (*((volatile unsigned long *) REG_ICU_GPIO_DEEP_WAKE2_ADDR))

#define ICU_GPIO_DEEP_WAKE2_CMD_POSI        16
#define ICU_GPIO_DEEP_WAKE2_CMD_MASK        (0x0000FFFFUL << ICU_GPIO_DEEP_WAKE2_CMD_POSI)


#define REG_ICU_RC32K_CONFIG_ADDR           (REG_ICU_BASE_ADDR + 0x1A * 4)
#define REG_ICU_RC32K_CONFIG_MASK           0x0003FFFFUL
#define REG_ICU_RC32K_CONFIG                (*((volatile unsigned long *) REG_ICU_RC32K_CONFIG_ADDR))


#define REG_ICU_RC32K_SLEEP_TIME_ADDR       (REG_ICU_BASE_ADDR + 0x1B * 4)
#define REG_ICU_RC32K_SLEEP_TIME_MASK       0x00FFFFFFUL
#define REG_ICU_RC32K_SLEEP_TIME            (*((volatile unsigned long *) REG_ICU_RC32K_SLEEP_TIME_ADDR))


#define REG_ICU_EX_TIME_SLEEP_TIME_ADDR     (REG_ICU_BASE_ADDR + 0x1C * 4)
#define REG_ICU_EX_TIME_SLEEP_TIME_MASK     0x0000FFFFUL
#define REG_ICU_EX_TIME_SLEEP_TIME          (*((volatile unsigned long *) REG_ICU_EX_TIME_SLEEP_TIME_ADDR))
#endif      /* #if ((CHIP_ID == CHIP_3433) || (CHIP_ID == CHIP_3435)) */


typedef enum
{
    WAKE_UP_PER_UART0        = 0x01,
    WAKE_UP_PER_UART1        = 0x02,
    WAKE_UP_PER_ADC          = 0x03,
    WAKE_UP_PER_I2C0         = 0x04,
    WAKE_UP_PER_I2C1         = 0x05,
    WAKE_UP_PER_I2S          = 0x06,
    WAKE_UP_PER_SPI          = 0x07,
    WAKE_UP_PER_DMA          = 0x08,
    WAKE_UP_PER_RTC          = 0x09,
    WAKE_UP_PER_WDT          = 0x0A,
    WAKE_UP_PER_FFT          = 0x0B,
    WAKE_UP_PER_PWM          = 0x0C,
    WAKE_UP_PER_PWM1         = 0x0D,
    WAKE_UP_PER_PWM2         = 0x0E,
    WAKE_UP_PER_PWM3         = 0x0F,
    WAKE_UP_PER_GPIO         = 0x10,
    WAKE_UP_PER_I2C_FM       = 0x11,
} WakeUpPeripheral;


extern void ICU_init(void);
extern void ICU_Cali_32KHz(void);

extern void sys_sleep_mode_normal_voltage(void);
extern void sys_sleep_mode_low_voltage(void);
extern void sys_sleep_mode_deep_sleep(void);
extern void sys_wake_up1(void);

extern void sys_idle_mode_normal_voltage(unsigned char ucWakeUpPeripheral, unsigned char ucParameter);
extern void sys_idle_mode_low_voltage(unsigned char ucWakeUpPeripheral, unsigned char ucParameter);
extern void sys_deep_sleep(unsigned char ucWakeUpPeripheral, unsigned char ucParameter);
extern void sys_wake_up(unsigned char ucWakeUpPeripheral, unsigned char ucParameter);


#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif      /* __DRIVER_ICU_H__ */
