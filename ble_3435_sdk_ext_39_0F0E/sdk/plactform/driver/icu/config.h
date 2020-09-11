/*************************************************************
 * @file        config.h
 * @brief       Header file for system configuration
 * @author      GuWenFu
 * @version     V1.0
 * @date        2016-09-29
 * @par         
 * @attention   
 *
 * @history     2016-09-29 gwf    create this file
 */


#ifndef __CONFIG_H__

#define __CONFIG_H__


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


//#define CHIP_3231S          1
//#define CHIP_7211           2
//#define CHIP_3262N          3
//#define CHIP_3433           4
//#define CHIP_3435           5
#define CHIP_3435_v2        7

#define CHIP_ID             CHIP_3435_v2


#if (CHIP_ID == CHIP_7211)
#define ADC_3231_STYLE
#define I2C_3231_STYLE
#define I2S_3252_STYLE
#define SPI_3231_STYLE
#define PWM_3231_STYLE
#define WDT_3231_STYLE
#define FFT_7211_STYLE
#define IRDA_7211_STYLE
#define UART_3231_STYLE
#define UART0_3231_STYLE
#define UART1_3231_STYLE
#define GPIO_3254_STYLE
#define FLASH_3231_STYLE
#define MAILBOX_7211_STYLE


#define MCU_CLK_32KHz       32000
#define MCU_CLK_16MHz       16000000
#define MCU_CLK_26MHz       26000000
#define MCU_CLK_48MHz       48000000
#define MCU_CLK_96MHz       96000000
#define MCU_CLK_120MHz      120000000

#define MCU_CLK             MCU_CLK_26MHz


#define PER_CLK_32KHz       32000
#define PER_CLK_16MHz       16000000
#define PER_CLK_26MHz       26000000
#define PER_CLK_48MHz       48000000
#define PER_CLK_96MHz       96000000
#define PER_CLK_120MHz      120000000

#define PER_CLK             PER_CLK_26MHz

#elif (CHIP_ID == CHIP_3262N)       /* #if (CHIP_ID == CHIP_7211) */

#define ADC_3262N_STYLE
#define DMA_3262N_STYLE
#define I2C_FM_7211_STYLE
#define I2C0_3231_STYLE
#define I2S_3252_STYLE
#define SPI_3231_STYLE
#define PWM0_3262N_STYLE
#define PWM1_3262N_STYLE
#define PWM2_3262N_STYLE
#define PMU_3262N_STYLE
#define WDT_3231_STYLE
#define FFT_7211_STYLE
#define XVR_3262N_STYLE
#define IRDA_7211_STYLE
#define UART0_3231_STYLE
#define UART1_3231_STYLE
#define GPIO_3262N_STYLE
#define AUDIO_3251_STYLE
#define FLASH_3231_STYLE
#define SPI_DMA_7211_STYLE
#define MAILBOX_7211_STYLE


#define MCU_CLK_32KHz       32000
#define MCU_CLK_16MHz       16000000
#define MCU_CLK_26MHz       26000000
#define MCU_CLK_48MHz       48000000
#define MCU_CLK_96MHz       96000000
#define MCU_CLK_120MHz      120000000

#define MCU_CLK             MCU_CLK_26MHz


#define PER_CLK_32KHz       32000
#define PER_CLK_16MHz       16000000
#define PER_CLK_26MHz       26000000
#define PER_CLK_48MHz       48000000
#define PER_CLK_96MHz       96000000
#define PER_CLK_120MHz      120000000

#define PER_CLK             PER_CLK_26MHz

#elif (CHIP_ID == CHIP_3433)        /* #if (CHIP_ID == CHIP_7211) */

#define ADC_3231_STYLE
#define I2C0_3231_STYLE
#define SPI_3231_STYLE
#define PWM_3433_STYLE
#define WDT_3231_STYLE
#define RTC_3231_STYLE
#define MFC_3431_STYLE
#define XVR_3431_STYLE
#define UART0_3231_STYLE
#define GPIO_3231_STYLE


#define MCU_CLK_32KHz       32000
#define MCU_CLK_16MHz       16000000
#define MCU_CLK_24MHz       24000000
#define MCU_CLK_48MHz       48000000

#define MCU_CLK             MCU_CLK_16MHz

#elif (CHIP_ID == CHIP_3435)        /* #if (CHIP_ID == CHIP_7211) */

#define ADC_3262N_STYLE
#define I2C0_3231_STYLE
#define SPI_3231_STYLE
#define PWM_3231_STYLE
#define WDT_3231_STYLE
#define RTC_3231_STYLE
#define MFC_3431_STYLE
#define XVR_3431_STYLE
#define FLASH_3231_STYLE
#define UART0_3231_STYLE
#define GPIO_3231_STYLE
#define PWM_3DS_3231_STYLE


#define MCU_CLK_32KHz       32000
#define MCU_CLK_16MHz       16000000
#define MCU_CLK_24MHz       24000000
#define MCU_CLK_48MHz       48000000

#define MCU_CLK             MCU_CLK_48MHz

#define PER_CLK             MCU_CLK_16MHz

#elif (CHIP_ID == CHIP_3435_v2)        /* #if (CHIP_ID == CHIP_7211) */

#define ADC_3435_v2_STYLE
#define USB_3435_v2_STYLE
#define I2C0_3231_STYLE
#define I2S_3252_STYLE
#define SPI_3231_STYLE
#define PWM_3231_STYLE
#define WDT_3231_STYLE
#define RTC_3231_STYLE
#define MFC_3431_STYLE
#define XVR_3431_STYLE
#define FLASH_3231_STYLE
#define UART0_3231_STYLE
#define UART1_3231_STYLE
#define GPIO_3231_STYLE
#define PWM_3DS_3231_STYLE


#define MCU_CLK_32KHz       32000
#define MCU_CLK_16MHz       16000000
#define MCU_CLK_24MHz       24000000
#define MCU_CLK_48MHz       48000000

#define MCU_CLK             MCU_CLK_16MHz

#define PER_CLK             MCU_CLK_16MHz

#endif      /* #if (CHIP_ID == CHIP_7211) */


#define DEBUG_PORT_UART0        1
#define DEBUG_PORT_UART1        2
#define DEBUG_PORT_SPI          3
#define DEBUG_PORT_I2C0         4
#define DEBUG_PORT_I2C1         5
#define DEBUG_PORT_IO_SIM_UART  10

#define DEBUG_PORT              DEBUG_PORT_UART0
#define PRINT_PORT              DEBUG_PORT_UART0


#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif      /* __CONFIG_H__ */
