/*************************************************************
 * @file        driver_gpio.h
 * @brief       Header file of driver_gpio.c
 * @author      GuWenFu
 * @version     V1.0
 * @date        2016-09-29
 * @par
 * @attention
 *
 * @history     2016-09-29 gwf    create this file
 */

#ifndef __DRIVER_GPIO_H__

#define __DRIVER_GPIO_H__


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "BK_System.h"
#include <stdbool.h>

#if (CHIP_ID == CHIP_7211)
#define REG_GPIO_BASE_ADDR                  (0x0802800UL)
#define GPIO_CHANNEL_NUMBER_ALL             32
#define GPIO_CHANNEL_NUMBER_MAX             (GPIO_CHANNEL_NUMBER_ALL - 1)
#define GPIO_CHANNEL_NUMBER_MIN             0
#elif (CHIP_ID == CHIP_3262N)    /* #if (CHIP_ID == CHIP_7211) */
#define REG_GPIO_BASE_ADDR                  (0x00F40000UL)
#define GPIO_CHANNEL_NUMBER_ALL             (27)
#define GPIO_CHANNEL_NUMBER_MAX             (GPIO_CHANNEL_NUMBER_ALL - 1)
#define GPIO_CHANNEL_NUMBER_MIN             0
#elif (CHIP_ID == CHIP_3433)     /* #if (CHIP_ID == CHIP_7211) */
#define REG_GPIO_BASE_ADDR                  (0x0806500UL)
#define GPIO_CHANNEL_NUMBER_ALL             40
#define GPIO_CHANNEL_NUMBER_MAX             (GPIO_CHANNEL_NUMBER_ALL - 1)
#define GPIO_CHANNEL_NUMBER_MIN             0
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))     /* #if (CHIP_ID == CHIP_7211) */
#define REG_GPIO_BASE_ADDR                  (0x0806500UL)
#define GPIO_CHANNEL_NUMBER_ALL             32
#define GPIO_CHANNEL_NUMBER_MAX             (GPIO_CHANNEL_NUMBER_ALL - 1)
#define GPIO_CHANNEL_NUMBER_MIN             0
#elif (CHIP_ID == CHIP_7231)     /* #if (CHIP_ID == CHIP_7211) */
#define REG_GPIO_BASE_ADDR                  (0x0802800UL)
#define GPIO_CHANNEL_NUMBER_ALL             32
#define GPIO_CHANNEL_NUMBER_MAX             (GPIO_CHANNEL_NUMBER_ALL - 1)
#define GPIO_CHANNEL_NUMBER_MIN             0
#endif      /* #if (CHIP_ID == CHIP_7211) */


#ifdef GPIO_3254_STYLE
#define REG_GPIO_X_CONFIG_ADDR(x)           (REG_GPIO_BASE_ADDR + (x) * 4)
#define REG_GPIO_0_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x00 * 4)
#define REG_GPIO_1_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x01 * 4)
#define REG_GPIO_2_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x02 * 4)
#define REG_GPIO_3_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x03 * 4)
#define REG_GPIO_4_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x04 * 4)
#define REG_GPIO_5_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x05 * 4)
#define REG_GPIO_6_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x06 * 4)
#define REG_GPIO_7_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x07 * 4)
#define REG_GPIO_8_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x08 * 4)
#define REG_GPIO_9_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x09 * 4)
#define REG_GPIO_10_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x0A * 4)
#define REG_GPIO_11_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x0B * 4)
#define REG_GPIO_12_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x0C * 4)
#define REG_GPIO_13_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x0D * 4)
#define REG_GPIO_14_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x0E * 4)
#define REG_GPIO_15_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x0F * 4)
#define REG_GPIO_16_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x10 * 4)
#define REG_GPIO_17_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x11 * 4)
#define REG_GPIO_18_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x12 * 4)
#define REG_GPIO_19_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x13 * 4)
#define REG_GPIO_20_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x14 * 4)
#define REG_GPIO_21_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x15 * 4)
#define REG_GPIO_22_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x16 * 4)
#define REG_GPIO_23_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x17 * 4)
#define REG_GPIO_24_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x18 * 4)
#define REG_GPIO_25_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x19 * 4)
#define REG_GPIO_26_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x1A * 4)
#define REG_GPIO_27_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x1B * 4)
#define REG_GPIO_28_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x1C * 4)
#define REG_GPIO_29_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x1D * 4)
#define REG_GPIO_30_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x1E * 4)
#define REG_GPIO_31_CONFIG_ADDR             (REG_GPIO_BASE_ADDR + 0x1F * 4)

#define REG_GPIO_X_CONFIG(x)                (*((volatile unsigned long *) REG_GPIO_X_CONFIG_ADDR(x)))
#define REG_GPIO_0_CONFIG                   (*((volatile unsigned long *) REG_GPIO_0_CONFIG_ADDR))
#define REG_GPIO_1_CONFIG                   (*((volatile unsigned long *) REG_GPIO_1_CONFIG_ADDR))
#define REG_GPIO_2_CONFIG                   (*((volatile unsigned long *) REG_GPIO_2_CONFIG_ADDR))
#define REG_GPIO_3_CONFIG                   (*((volatile unsigned long *) REG_GPIO_3_CONFIG_ADDR))
#define REG_GPIO_4_CONFIG                   (*((volatile unsigned long *) REG_GPIO_4_CONFIG_ADDR))
#define REG_GPIO_5_CONFIG                   (*((volatile unsigned long *) REG_GPIO_5_CONFIG_ADDR))
#define REG_GPIO_6_CONFIG                   (*((volatile unsigned long *) REG_GPIO_6_CONFIG_ADDR))
#define REG_GPIO_7_CONFIG                   (*((volatile unsigned long *) REG_GPIO_7_CONFIG_ADDR))
#define REG_GPIO_8_CONFIG                   (*((volatile unsigned long *) REG_GPIO_8_CONFIG_ADDR))
#define REG_GPIO_9_CONFIG                   (*((volatile unsigned long *) REG_GPIO_9_CONFIG_ADDR))
#define REG_GPIO_10_CONFIG                  (*((volatile unsigned long *) REG_GPIO_10_CONFIG_ADDR))
#define REG_GPIO_11_CONFIG                  (*((volatile unsigned long *) REG_GPIO_11_CONFIG_ADDR))
#define REG_GPIO_12_CONFIG                  (*((volatile unsigned long *) REG_GPIO_12_CONFIG_ADDR))
#define REG_GPIO_13_CONFIG                  (*((volatile unsigned long *) REG_GPIO_13_CONFIG_ADDR))
#define REG_GPIO_14_CONFIG                  (*((volatile unsigned long *) REG_GPIO_14_CONFIG_ADDR))
#define REG_GPIO_15_CONFIG                  (*((volatile unsigned long *) REG_GPIO_15_CONFIG_ADDR))
#define REG_GPIO_16_CONFIG                  (*((volatile unsigned long *) REG_GPIO_16_CONFIG_ADDR))
#define REG_GPIO_17_CONFIG                  (*((volatile unsigned long *) REG_GPIO_17_CONFIG_ADDR))
#define REG_GPIO_18_CONFIG                  (*((volatile unsigned long *) REG_GPIO_18_CONFIG_ADDR))
#define REG_GPIO_19_CONFIG                  (*((volatile unsigned long *) REG_GPIO_19_CONFIG_ADDR))
#define REG_GPIO_20_CONFIG                  (*((volatile unsigned long *) REG_GPIO_20_CONFIG_ADDR))
#define REG_GPIO_21_CONFIG                  (*((volatile unsigned long *) REG_GPIO_21_CONFIG_ADDR))
#define REG_GPIO_22_CONFIG                  (*((volatile unsigned long *) REG_GPIO_22_CONFIG_ADDR))
#define REG_GPIO_23_CONFIG                  (*((volatile unsigned long *) REG_GPIO_23_CONFIG_ADDR))
#define REG_GPIO_24_CONFIG                  (*((volatile unsigned long *) REG_GPIO_24_CONFIG_ADDR))
#define REG_GPIO_25_CONFIG                  (*((volatile unsigned long *) REG_GPIO_25_CONFIG_ADDR))
#define REG_GPIO_26_CONFIG                  (*((volatile unsigned long *) REG_GPIO_26_CONFIG_ADDR))
#define REG_GPIO_27_CONFIG                  (*((volatile unsigned long *) REG_GPIO_27_CONFIG_ADDR))
#define REG_GPIO_28_CONFIG                  (*((volatile unsigned long *) REG_GPIO_28_CONFIG_ADDR))
#define REG_GPIO_29_CONFIG                  (*((volatile unsigned long *) REG_GPIO_29_CONFIG_ADDR))
#define REG_GPIO_30_CONFIG                  (*((volatile unsigned long *) REG_GPIO_30_CONFIG_ADDR))
#define REG_GPIO_31_CONFIG                  (*((volatile unsigned long *) REG_GPIO_31_CONFIG_ADDR))

#define GPIO_CFG_INPUT_DATA_POSI            0
#define GPIO_CFG_INPUT_DATA_MASK            (0x01UL << GPIO_CFG_INPUT_DATA_POSI)
#define GPIO_X_CFG_INPUT_DATA_GET(x)        (REG_GPIO_X_CONFIG(x) & GPIO_CFG_INPUT_DATA_MASK)

#define GPIO_CFG_OUTPUT_DATA_POSI           1
#define GPIO_CFG_OUTPUT_DATA_MASK           (0x01UL << GPIO_CFG_OUTPUT_DATA_POSI)
#define GPIO_X_CFG_OUTPUT_DATA_SET(x)       (REG_GPIO_X_CONFIG(x) |=  GPIO_CFG_OUTPUT_DATA_MASK)
#define GPIO_X_CFG_OUTPUT_DATA_CLEAR(x)     (REG_GPIO_X_CONFIG(x) &= (~GPIO_CFG_OUTPUT_DATA_MASK))

#define GPIO_CFG_INPUT_ENABLE_POSI          2
#define GPIO_CFG_INPUT_ENABLE_MASK          (0x01UL << GPIO_CFG_INPUT_ENABLE_POSI)
#define GPIO_CFG_INPUT_ENABLE_SET           (0x01UL << GPIO_CFG_INPUT_ENABLE_POSI)

#define GPIO_CFG_OUTPUT_ENABLE_POSI         3
#define GPIO_CFG_OUTPUT_ENABLE_MASK         (0x01UL << GPIO_CFG_OUTPUT_ENABLE_POSI)

#define GPIO_CFG_PULL_MODE_POSI             4
#define GPIO_CFG_PULL_MODE_MASK             (0x01UL << GPIO_CFG_PULL_MODE_POSI)
#define GPIO_CFG_PULL_MODE_PULL_DOWN        (0x00UL << GPIO_CFG_PULL_MODE_POSI)
#define GPIO_CFG_PULL_MODE_PULL_UP          (0x01UL << GPIO_CFG_PULL_MODE_POSI)

#define GPIO_CFG_PULL_ENABLE_POSI           5
#define GPIO_CFG_PULL_ENABLE_MASK           (0x01UL << GPIO_CFG_PULL_ENABLE_POSI)
#define GPIO_CFG_PULL_ENABLE_SET            (0x01UL << GPIO_CFG_PULL_ENABLE_POSI)

#define GPIO_CFG_FUNCTION_ENABLE_POSI       6
#define GPIO_CFG_FUNCTION_ENABLE_MASK       (0x01UL << GPIO_CFG_FUNCTION_ENABLE_POSI)
#define GPIO_CFG_FUNCTION_ENABLE_SET        (0x01UL << GPIO_CFG_FUNCTION_ENABLE_POSI)

#define GPIO_CFG_INPUT_MONITOR_POSI         7
#define GPIO_CFG_INPUT_MONITOR_MASK         (0x01UL << GPIO_CFG_INPUT_MONITOR_POSI)
#define GPIO_CFG_INPUT_MONITOR_SET          (0x01UL << GPIO_CFG_INPUT_MONITOR_POSI)


#define REG_GPIO_FUNTION_MODE_ADDR          (REG_GPIO_BASE_ADDR + 0x20 * 4)
#define REG_GPIO_FUNTION_MODE_MASK          0xFFFFFFFFUL
#define REG_GPIO_FUNTION_MODE               (*((volatile unsigned long *) REG_GPIO_FUNTION_MODE_ADDR))

#define GPIO_X_FUNTION_MODE_POSI(x)         (x)
#define GPIO_X_FUNTION_MODE_MASK(x)         (0x01UL << GPIO_X_FUNTION_MODE_POSI(x))
#define GPIO_X_FUNTION_MODE_2_FUNC(x)       (0x00UL << GPIO_X_FUNTION_MODE_POSI(x))
#define GPIO_X_FUNTION_MODE_3_FUNC(x)       (0x01UL << GPIO_X_FUNTION_MODE_POSI(x))


#define REG_GPIO_INT_ENABLE_ADDR            (REG_GPIO_BASE_ADDR + 0x21 * 4)
#define REG_GPIO_INT_ENABLE_MASK            0xFFFFFFFFUL
#define REG_GPIO_INT_ENABLE                 (*((volatile unsigned long *) REG_GPIO_INT_ENABLE_ADDR))

#define GPIO_X_INT_ENABLE_POSI(x)           (x)
#define GPIO_X_INT_ENABLE_MASK(x)           (0x01UL << GPIO_X_INT_ENABLE_POSI(x))


#define REG_GPIO_INT_TYPE_1_ADDR            (REG_GPIO_BASE_ADDR + 0x22 * 4)
#define REG_GPIO_INT_TYPE_1_MASK            0xFFFFFFFFUL
#define REG_GPIO_INT_TYPE_1                 (*((volatile unsigned long *) REG_GPIO_INT_TYPE_1_ADDR))

#define GPIO_X_INT_TYPE_1_POSI(x)           ((x) *2)
#define GPIO_X_INT_TYPE_1_MASK(x)           (0x03UL << GPIO_X_INT_TYPE_1_POSI(x))
#define GPIO_X_INT_TYPE_1_LOW_LEVEL(x)      (0x00UL << GPIO_X_INT_TYPE_1_POSI(x))
#define GPIO_X_INT_TYPE_1_HIGH_LEVEL(x)     (0x01UL << GPIO_X_INT_TYPE_1_POSI(x))
#define GPIO_X_INT_TYPE_1_POS_EDGE(x)       (0x02UL << GPIO_X_INT_TYPE_1_POSI(x))
#define GPIO_X_INT_TYPE_1_NEG_EDGE(x)       (0x03UL << GPIO_X_INT_TYPE_1_POSI(x))


#define REG_GPIO_INT_TYPE_2_ADDR            (REG_GPIO_BASE_ADDR + 0x23 * 4)
#define REG_GPIO_INT_TYPE_2_MASK            0xFFFFFFFF
#define REG_GPIO_INT_TYPE_2                 (*((volatile unsigned long *) REG_GPIO_INT_TYPE_2_ADDR))

#define GPIO_X_INT_TYPE_2_POSI(x)           (((x) - 16) *2)
#define GPIO_X_INT_TYPE_2_MASK(x)           (0x03UL << GPIO_X_INT_TYPE_2_POSI(x))
#define GPIO_X_INT_TYPE_2_LOW_LEVEL(x)      (0x00UL << GPIO_X_INT_TYPE_2_POSI(x))
#define GPIO_X_INT_TYPE_2_HIGH_LEVEL(x)     (0x01UL << GPIO_X_INT_TYPE_2_POSI(x))
#define GPIO_X_INT_TYPE_2_POS_EDGE(x)       (0x02UL << GPIO_X_INT_TYPE_2_POSI(x))
#define GPIO_X_INT_TYPE_2_NEG_EDGE(x)       (0x03UL << GPIO_X_INT_TYPE_2_POSI(x))


#define REG_GPIO_INT_STATUS_ADDR            (REG_GPIO_BASE_ADDR + 0x24 * 4)
#define REG_GPIO_INT_STATUS_MASK            0xFFFFFFFFUL
#define REG_GPIO_INT_STATUS                 (*((volatile unsigned long *) REG_GPIO_INT_STATUS_ADDR))

#define GPIO_X_INT_STATUS_POSI(x)           (x)
#define GPIO_X_INT_STATUS_MASK(x)           (0x01UL << GPIO_X_INT_STATUS_POSI(x))
#define GPIO_X_INT_STATUS_GET(x)            (REG_GPIO_INT_STATUS | GPIO_X_INT_STATUS_MASK(x))
#define GPIO_X_INT_STATUS_CLEAR(x)          do {REG_GPIO_INT_STATUS |= GPIO_X_INT_STATUS_MASK(x);} while(0)


#define REG_GPIO_DPLL_UNLOCK_ADDR           (REG_GPIO_BASE_ADDR + 0x26 * 4)
#define REG_GPIO_DPLL_UNLOCK_MASK           0x01UL
#define REG_GPIO_DPLL_UNLOCK                (*((volatile unsigned long *) REG_GPIO_DPLL_UNLOCK_ADDR))


#define REG_GPIO_DECTECT_FLAG_ADDR          (REG_GPIO_BASE_ADDR + 0x27 * 4)
#define REG_GPIO_DECTECT_FLAG_MASK          0x03UL
#define REG_GPIO_DECTECT_FLAG               (*((volatile unsigned long *) REG_GPIO_DECTECT_FLAG_ADDR))


#define REG_GPIO_ENCRYPT_WORD_ADDR          (REG_GPIO_BASE_ADDR + 0x28 * 4)
#define REG_GPIO_ENCRYPT_WORD_MASK          0xFFFFFFFFUL
#define REG_GPIO_ENCRYPT_WORD               (*((volatile unsigned long *) REG_GPIO_ENCRYPT_WORD_ADDR))


#define REG_GPIO_DEBUG_MSG_ADDR             (REG_GPIO_BASE_ADDR + 0x29 * 4)
#define REG_GPIO_DEBUG_MSG_MASK             0x000000FFUL
#define REG_GPIO_DEBUG_MSG                  (*((volatile unsigned long *) REG_GPIO_DEBUG_MSG_ADDR))


#define REG_GPIO_DEBUG_OUTPUT_SEL_ADDR      (REG_GPIO_BASE_ADDR + 0x2A * 4)
#define REG_GPIO_DEBUG_OUTPUT_SEL_MASK      0x00000007UL
#define REG_GPIO_DEBUG_OUTPUT_SEL           (*((volatile unsigned long *) REG_GPIO_DEBUG_OUTPUT_SEL_ADDR))


#define REG_GPIO_DEBUG_ENABLE_ADDR          (REG_GPIO_BASE_ADDR + 0x2B * 4)
#define REG_GPIO_DEBUG_ENABLE_MASK          0xFFFFFFFFUL
#define REG_GPIO_DEBUG_ENABLE               (*((volatile unsigned long *) REG_GPIO_DEBUG_ENABLE_ADDR))


#define REG_GPIO_DEBUG_REPORT_ADDR          (REG_GPIO_BASE_ADDR + 0x2C * 4)
#define REG_GPIO_DEBUG_REPORT_MASK          0xFFFFFFFFUL
#define REG_GPIO_DEBUG_REPORT               (*((volatile unsigned long *) REG_GPIO_DEBUG_REPORT_ADDR))


#define REG_GPIO_ANALOG_REG_0_ADDR          (REG_GPIO_BASE_ADDR + 0x30 * 4)
#define REG_GPIO_ANALOG_REG_0_MASK          0xFFFFFFFFUL
#define REG_GPIO_ANALOG_REG_0               (*((volatile unsigned long *) REG_GPIO_ANALOG_REG_0_ADDR))


#define REG_GPIO_ANALOG_REG_1_ADDR          (REG_GPIO_BASE_ADDR + 0x31 * 4)
#define REG_GPIO_ANALOG_REG_1_MASK          0xFFFFFFFFUL
#define REG_GPIO_ANALOG_REG_1               (*((volatile unsigned long *) REG_GPIO_ANALOG_REG_1_ADDR))


#define REG_GPIO_ANALOG_REG_2_ADDR          (REG_GPIO_BASE_ADDR + 0x32 * 4)
#define REG_GPIO_ANALOG_REG_2_MASK          0xFFFFFFFFUL
#define REG_GPIO_ANALOG_REG_2               (*((volatile unsigned long *) REG_GPIO_ANALOG_REG_2_ADDR))


#define REG_GPIO_ANALOG_REG_3_ADDR          (REG_GPIO_BASE_ADDR + 0x33 * 4)
#define REG_GPIO_ANALOG_REG_3_MASK          0xFFFFFFFFUL
#define REG_GPIO_ANALOG_REG_3               (*((volatile unsigned long *) REG_GPIO_ANALOG_REG_3_ADDR))


#define REG_GPIO_ANALOG_REG_4_ADDR          (REG_GPIO_BASE_ADDR + 0x34 * 4)
#define REG_GPIO_ANALOG_REG_4_MASK          0xFFFFFFFFUL
#define REG_GPIO_ANALOG_REG_4               (*((volatile unsigned long *) REG_GPIO_ANALOG_REG_4_ADDR))


#define REG_GPIO_ANALOG_REG_5_ADDR          (REG_GPIO_BASE_ADDR + 0x35 * 4)
#define REG_GPIO_ANALOG_REG_5_MASK          0xFFFFFFFFUL
#define REG_GPIO_ANALOG_REG_5               (*((volatile unsigned long *) REG_GPIO_ANALOG_REG_5_ADDR))


#define REG_GPIO_ANALOG_REG_6_ADDR          (REG_GPIO_BASE_ADDR + 0x36 * 4)
#define REG_GPIO_ANALOG_REG_6_MASK          0xFFFFFFFFUL
#define REG_GPIO_ANALOG_REG_6               (*((volatile unsigned long *) REG_GPIO_ANALOG_REG_6_ADDR))


#define REG_GPIO_ANALOG_REG_7_ADDR          (REG_GPIO_BASE_ADDR + 0x37 * 4)
#define REG_GPIO_ANALOG_REG_7_MASK          0xFFFFFFFFUL
#define REG_GPIO_ANALOG_REG_7               (*((volatile unsigned long *) REG_GPIO_ANALOG_REG_7_ADDR))


#elif defined GPIO_3231_STYLE       /* #ifdef GPIO_3254_STYLE */


#define REG_GPIO_A_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x00 * 4)
#define REG_GPIO_A_CONFIG_MASK              0xFFFFFFFF
#define REG_GPIO_A_CONFIG                   (*((volatile unsigned long *) REG_GPIO_A_CONFIG_ADDR))


#define REG_GPIO_A_DATA_ADDR                (REG_GPIO_BASE_ADDR + 0x01 * 4)
#define REG_GPIO_A_DATA_MASK                0x00FFFFFF
#define REG_GPIO_A_DATA                     (*((volatile unsigned long *) REG_GPIO_A_DATA_ADDR))


#define REG_GPIO_B_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x02 * 4)
#define REG_GPIO_B_CONFIG_MASK              0xFFFFFFFF
#define REG_GPIO_B_CONFIG                   (*((volatile unsigned long *) REG_GPIO_B_CONFIG_ADDR))


#define REG_GPIO_B_DATA_ADDR                (REG_GPIO_BASE_ADDR + 0x03 * 4)
#define REG_GPIO_B_DATA_MASK                0x00FFFFFF
#define REG_GPIO_B_DATA                     (*((volatile unsigned long *) REG_GPIO_B_DATA_ADDR))


#define REG_GPIO_C_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x04 * 4)
#define REG_GPIO_C_CONFIG_MASK              0xFFFFFFFF
#define REG_GPIO_C_CONFIG                   (*((volatile unsigned long *) REG_GPIO_C_CONFIG_ADDR))


#define REG_GPIO_C_DATA_ADDR                (REG_GPIO_BASE_ADDR + 0x05 * 4)
#define REG_GPIO_C_DATA_MASK                0x00FFFFFF
#define REG_GPIO_C_DATA                     (*((volatile unsigned long *) REG_GPIO_C_DATA_ADDR))


#define REG_GPIO_D_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x06 * 4)
#define REG_GPIO_D_CONFIG_MASK              0xFFFFFFFF
#define REG_GPIO_D_CONFIG                   (*((volatile unsigned long *) REG_GPIO_D_CONFIG_ADDR))


#define REG_GPIO_D_DATA_ADDR                (REG_GPIO_BASE_ADDR + 0x07 * 4)
#define REG_GPIO_D_DATA_MASK                0x00FFFFFF
#define REG_GPIO_D_DATA                     (*((volatile unsigned long *) REG_GPIO_D_DATA_ADDR))


#define REG_GPIO_E_CONFIG_ADDR              (REG_GPIO_BASE_ADDR + 0x08 * 4)
#define REG_GPIO_E_CONFIG_MASK              0xFFFFFFFF
#define REG_GPIO_E_CONFIG                   (*((volatile unsigned long *) REG_GPIO_E_CONFIG_ADDR))


#define REG_GPIO_E_DATA_ADDR                (REG_GPIO_BASE_ADDR + 0x09 * 4)
#define REG_GPIO_E_DATA_MASK                0x00FFFFFF
#define REG_GPIO_E_DATA                     (*((volatile unsigned long *) REG_GPIO_E_DATA_ADDR))


#define GPIO_CONFIG_SECOND_FUNCTION_POSI        0
#define GPIO_CONFIG_X_SECOND_FUNCTION_POSI(x)   (x)
#define GPIO_CONFIG_SECOND_FUNCTION_MASK        (0x00FFUL << GPIO_CONFIG_SECOND_FUNCTION_POSI)
#define GPIO_CONFIG_X_SECOND_FUNCTION_MASK(x)   (0x0001UL << GPIO_CONFIG_X_SECOND_FUNCTION_POSI(x))

#define GPIO_CONFIG_OUTPUT_ENABLE_POSI          8
#define GPIO_CONFIG_X_OUTPUT_ENABLE_POSI(x)     ((x) + 8)
#define GPIO_CONFIG_OUTPUT_ENABLE_MASK          (0x00FFUL << GPIO_CONFIG_OUTPUT_ENABLE_POSI)
#define GPIO_CONFIG_X_OUTPUT_ENABLE_MASK(x)     (0x0001UL << GPIO_CONFIG_X_OUTPUT_ENABLE_POSI(x))

#define GPIO_CONFIG_PULL_UP_POSI                16
#define GPIO_CONFIG_X_PULL_UP_POSI(x)           ((x) + 16)
#define GPIO_CONFIG_PULL_UP_MASK                (0x00FFUL << GPIO_CONFIG_PULL_UP_POSI)
#define GPIO_CONFIG_X_PULL_UP_MASK(x)           (0x0001UL << GPIO_CONFIG_X_PULL_UP_POSI(x))

#define GPIO_CONFIG_PULL_DOWN_POSI              24
#define GPIO_CONFIG_X_PULL_DOWN_POSI(x)         ((x) + 24)
#define GPIO_CONFIG_PULL_DOWN_MASK              (0x00FFUL << GPIO_CONFIG_PULL_DOWN_POSI)
#define GPIO_CONFIG_X_PULL_DOWN_MASK(x)         (0x0001UL << GPIO_CONFIG_X_PULL_DOWN_POSI(x))


#define GPIO_DATA_OUTPUT_VALUE_POSI             0
#define GPIO_DATA_X_OUTPUT_VALUE_POSI(x)        (x)
#define GPIO_DATA_OUTPUT_VALUE_MASK             (0x00FFUL << GPIO_DATA_OUTPUT_VALUE_POSI)
#define GPIO_DATA_X_OUTPUT_VALUE_MASK(x)        (0x0001UL << GPIO_DATA_X_OUTPUT_VALUE_POSI(x))

#define GPIO_DATA_INPUT_VALUE_POSI              8
#define GPIO_DATA_X_INPUT_VALUE_POSI(x)         ((x) + 8)
#define GPIO_DATA_INPUT_VALUE_MASK              (0x00FFUL << GPIO_DATA_INPUT_VALUE_POSI)
#define GPIO_DATA_X_INPUT_VALUE_MASK(x)         (0x0001UL << GPIO_DATA_X_INPUT_VALUE_POSI(x))

#define GPIO_DATA_INPUT_ENABLE_POSI             16
#define GPIO_DATA_X_INPUT_ENABLE_POSI(x)        ((x) + 16)
#define GPIO_DATA_INPUT_ENABLE_MASK             (0x00FFUL << GPIO_DATA_INPUT_ENABLE_POSI)
#define GPIO_DATA_X_INPUT_ENABLE_MASK(x)        (0x0001UL << GPIO_DATA_X_INPUT_ENABLE_POSI(x))


#define REG_GPIO_ABCD_WU_TYPE_ADDR          (REG_GPIO_BASE_ADDR + 0x10 * 4)
#define REG_GPIO_ABCD_WU_TYPE_MASK          0xFFFFFFFFUL
#define REG_GPIO_ABCD_WU_TYPE               (*((volatile unsigned long *) REG_GPIO_ABCD_WU_TYPE_ADDR))


#define REG_GPIO_E_WU_TYPE_ADDR             (REG_GPIO_BASE_ADDR + 0x11 * 4)
#define REG_GPIO_E_WU_TYPE_MASK             0x00FFUL
#define REG_GPIO_E_WU_TYPE                  (*((volatile unsigned long *) REG_GPIO_E_WU_TYPE_ADDR))


#define REG_GPIO_ABCD_WU_ENABLE_ADDR        (REG_GPIO_BASE_ADDR + 0x12 * 4)
#define REG_GPIO_ABCD_WU_ENABLE_MASK        0xFFFFFFFFUL
#define REG_GPIO_ABCD_WU_ENABLE             (*((volatile unsigned long *) REG_GPIO_ABCD_WU_ENABLE_ADDR))


#define REG_GPIO_E_WU_ENABLE_ADDR           (REG_GPIO_BASE_ADDR + 0x13 * 4)
#define REG_GPIO_E_WU_ENABLE_MASK           0x00FFUL
#define REG_GPIO_E_WU_ENABLE                (*((volatile unsigned long *) REG_GPIO_E_WU_ENABLE_ADDR))


#define REG_GPIO_ABCD_WU_STATUS_ADDR        (REG_GPIO_BASE_ADDR + 0x14 * 4)
#define REG_GPIO_ABCD_WU_STATUS_MASK        0xFFFFFFFFUL
#define REG_GPIO_ABCD_WU_STATUS             (*((volatile unsigned long *) REG_GPIO_ABCD_WU_STATUS_ADDR))


#define REG_GPIO_E_WU_STATUS_ADDR           (REG_GPIO_BASE_ADDR + 0x15 * 4)
#define REG_GPIO_E_WU_STATUS_MASK           0x00FFUL
#define REG_GPIO_E_WU_STATUS                (*((volatile unsigned long *) REG_GPIO_E_WU_STATUS_ADDR))
#endif      /* #ifdef GPIO_3254_STYLE */


#if (CHIP_ID == CHIP_7211)
#define GPIO_UART0_RX_PIN                   10
#define GPIO_UART0_TX_PIN                   11
#define GPIO_UART0_CTS_PIN                  12
#define GPIO_UART0_RTS_PIN                  13
#define GPIO_UART0_RX_CONFIG                REG_GPIO_10_CONFIG
#define GPIO_UART0_TX_CONFIG                REG_GPIO_11_CONFIG
#define GPIO_UART0_CTS_CONFIG               REG_GPIO_12_CONFIG
#define GPIO_UART0_RTS_CONFIG               REG_GPIO_13_CONFIG

#define GPIO_UART1_TX_PIN                   0
#define GPIO_UART1_RX_PIN                   1
#define GPIO_UART1_TX_CONFIG                REG_GPIO_0_CONFIG
#define GPIO_UART1_RX_CONFIG                REG_GPIO_1_CONFIG

#define GPIO_I2C0_SCL_PIN                   20
#define GPIO_I2C0_SDA_PIN                   21
#define GPIO_I2C0_SCL_CONFIG                REG_GPIO_20_CONFIG
#define GPIO_I2C0_SDA_CONFIG                REG_GPIO_21_CONFIG

#define GPIO_2F_I2C1_SCL_PIN                0
#define GPIO_2F_I2C1_SDA_PIN                1
#define GPIO_2F_I2C1_SCL_CONFIG             REG_GPIO_0_CONFIG
#define GPIO_2F_I2C1_SDA_CONFIG             REG_GPIO_1_CONFIG

#define GPIO_I2S_CLK_PIN                    2
#define GPIO_I2S_SYNC_PIN                   3
#define GPIO_I2S_DIN_PIN                    4
#define GPIO_I2S_DOUT_PIN                   5
#define GPIO_I2S_CLK_CONFIG                 REG_GPIO_2_CONFIG
#define GPIO_I2S_SYNC_CONFIG                REG_GPIO_3_CONFIG
#define GPIO_I2S_DIN_CONFIG                 REG_GPIO_4_CONFIG
#define GPIO_I2S_DOUT_CONFIG                REG_GPIO_5_CONFIG

#define GPIO_2F_ADC1_PIN                    4
#define GPIO_2F_ADC2_PIN                    5
//#define GPIO_ADC2_PIN                       23
#define GPIO_2F_ADC1_CONFIG                 REG_GPIO_4_CONFIG
#define GPIO_2F_ADC2_CONFIG                 REG_GPIO_5_CONFIG
//#define GPIO_ADC2_CONFIG                    REG_GPIO_23_CONFIG

#define GPIO_2F_PWM0_PIN                    6
#define GPIO_2F_PWM1_PIN                    7
#define GPIO_2F_PWM2_PIN                    8
#define GPIO_2F_PWM3_PIN                    9
#define GPIO_2F_PWM4_PIN                    18
#define GPIO_2F_PWM5_PIN                    19
#define GPIO_2F_PWM0_CONFIG                 REG_GPIO_6_CONFIG
#define GPIO_2F_PWM1_CONFIG                 REG_GPIO_7_CONFIG
#define GPIO_2F_PWM2_CONFIG                 REG_GPIO_8_CONFIG
#define GPIO_2F_PWM3_CONFIG                 REG_GPIO_9_CONFIG
#define GPIO_2F_PWM4_CONFIG                 REG_GPIO_18_CONFIG
#define GPIO_2F_PWM5_CONFIG                 REG_GPIO_19_CONFIG

#define GPIO_SD_CLK_PIN                     14
#define GPIO_SD_CMD_PIN                     15
#define GPIO_SD_DATA0_PIN                   16
#define GPIO_SD_DATA1_PIN                   17
#define GPIO_SD_DATA2_PIN                   18
#define GPIO_SD_DATA3_PIN                   19
#define GPIO_SD_CLK_CONFIG                  REG_GPIO_14_CONFIG
#define GPIO_SD_CMD_CONFIG                  REG_GPIO_15_CONFIG
#define GPIO_SD_DATA0_CONFIG                REG_GPIO_16_CONFIG
#define GPIO_SD_DATA1_CONFIG                REG_GPIO_17_CONFIG
#define GPIO_SD_DATA2_CONFIG                REG_GPIO_18_CONFIG
#define GPIO_SD_DATA3_CONFIG                REG_GPIO_19_CONFIG

#define GPIO_2F_SPI_SCK_PIN                 14
#define GPIO_2F_SPI_CSN_PIN                 15
#define GPIO_2F_SPI_MOSI_PIN                16
#define GPIO_2F_SPI_MISO_PIN                17
#define GPIO_2F_SPI_SCK_CONFIG              REG_GPIO_14_CONFIG
#define GPIO_2F_SPI_CSN_CONFIG              REG_GPIO_15_CONFIG
#define GPIO_2F_SPI_MOSI_CONFIG             REG_GPIO_16_CONFIG
#define GPIO_2F_SPI_MISO_CONFIG             REG_GPIO_17_CONFIG

#define GPIO_2F_JTAG_TCK_PIN                20
#define GPIO_2F_JTAG_TMS_PIN                21
#define GPIO_2F_JTAG_TDI_PIN                22
#define GPIO_2F_JTAG_TDO_PIN                23
#define GPIO_2F_JTAG_TCK_CONFIG             REG_GPIO_20_CONFIG
#define GPIO_2F_JTAG_TMS_CONFIG             REG_GPIO_21_CONFIG
#define GPIO_2F_JTAG_TDI_CONFIG             REG_GPIO_22_CONFIG
#define GPIO_2F_JTAG_TDO_CONFIG             REG_GPIO_23_CONFIG

#define GPIO_CLK_13M_PIN                    6
#define GPIO_CLK_26M_PIN                    22
#define GPIO_CLK_LPO_PIN                    24
#define GPIO_CLK_13M_CONFIG                 REG_GPIO_6_CONFIG
#define GPIO_CLK_26M_CONFIG                 REG_GPIO_22_CONFIG
#define GPIO_CLK_LPO_CONFIG                 REG_GPIO_24_CONFIG

#define GPIO_WIFI_ACTIVE_CONFIG             REG_GPIO_7_CONFIG
#define GPIO_BT_ACTIVE_CONFIG               REG_GPIO_8_CONFIG
#define GPIO_BT_PRIORITY_CONFIG             REG_GPIO_9_CONFIG

#define GPIO_TXEN_CONFIG                    REG_GPIO_25_CONFIG
#define GPIO_RXEN_CONFIG                    REG_GPIO_28_CONFIG

#elif (CHIP_ID == CHIP_3262N)       /* #if (CHIP_ID == CHIP_7211) */

#define GPIO_UART0_TX_PIN		0
#define GPIO_UART0_RX_PIN		1
#define GPIO_UART0_TX_CONFIG	REG_GPIO_0_CONFIG
#define GPIO_UART0_RX_CONFIG	REG_GPIO_1_CONFIG

#define GPIO_I2C_FM_SCL_PIN			25
#define GPIO_I2C_FM_SDA_PIN			26
#define GPIO_I2C_FM_SCL_CONFIG		REG_GPIO_25_CONFIG
#define GPIO_I2C_FM_SDA_CONFIG		REG_GPIO_26_CONFIG

#define GPIO_2F_I2C1_SCL_PIN		0
#define GPIO_2F_I2C1_SDA_PIN		1
#define GPIO_2F_I2C1_SCL_CONFIG		REG_GPIO_0_CONFIG
#define GPIO_2F_I2C1_SDA_CONFIG		REG_GPIO_1_CONFIG

#define GPIO_SPI_CSN_PIN		6
#define GPIO_SPI_SCK_PIN		7
#define GPIO_SPI_MOSI_PIN		8
#define GPIO_SPI_MISO_PIN		9
#define GPIO_SPI_CSN_CONFIG		REG_GPIO_6_CONFIG
#define GPIO_SPI_SCK_CONFIG		REG_GPIO_7_CONFIG
#define GPIO_SPI_MOSI_CONFIG	REG_GPIO_8_CONFIG
#define GPIO_SPI_MISO_CONFIG	REG_GPIO_9_CONFIG

#define GPIO_2F_I2S_SYNC_PIN		6
#define GPIO_2F_I2S_CLK_PIN			7
#define GPIO_2F_I2S_DIN_PIN			8
#define GPIO_2F_I2S_DOUT_PIN		9
#define GPIO_2F_I2S_SYNC_CONFIG		REG_GPIO_6_CONFIG
#define GPIO_2F_I2S_CLK_CONFIG		REG_GPIO_7_CONFIG
#define GPIO_2F_I2S_DIN_CONFIG		REG_GPIO_8_CONFIG
#define GPIO_2F_I2S_DOUT_CONFIG		REG_GPIO_9_CONFIG

#define GPIO_PWM0_PIN			10
#define GPIO_PWM1_PIN			11
#define GPIO_2F_PWM2_PIN		18
#define GPIO_2F_PWM3_PIN		19
#define GPIO_2F_PWM4_PIN		20
#define GPIO_2F_PWM5_PIN		21
#define GPIO_PWM0_CONFIG		REG_GPIO_10_CONFIG
#define GPIO_PWM1_CONFIG		REG_GPIO_11_CONFIG
#define GPIO_2F_PWM2_CONFIG		REG_GPIO_18_CONFIG
#define GPIO_2F_PWM3_CONFIG		REG_GPIO_19_CONFIG
#define GPIO_2F_PWM4_CONFIG		REG_GPIO_20_CONFIG
#define GPIO_2F_PWM5_CONFIG		REG_GPIO_21_CONFIG

#define GPIO_ADC1_PIN           4
#define GPIO_ADC2_PIN           5
#define GPIO_2F_ADC3_PIN        19
#define GPIO_ADC1_CONFIG        REG_GPIO_4_CONFIG
#define GPIO_ADC2_CONFIG        REG_GPIO_5_CONFIG
#define GPIO_2F_ADC3_CONFIG     REG_GPIO_19_CONFIG
#endif      /* #if (CHIP_ID == CHIP_7211) */


/*************************************************************
 * GPIO_Int_Enable
 * Description: set GPIO int enable
 * Parameters:  ucChannel:  GPIO channel
 *              ucMode:     GPIO mode, 0: GPIO low voltage interrupt
 *                                     1: GPIO high voltage interrupt
 *                                     2: GPIO positive edge interrupt
 *                                     3: GPIO negative edge interrupt
 *              p_Int_Handler: int handler
 * return:      none
 * error:       none
 */
extern void GPIO_Int_Enable(unsigned char ucChannel, unsigned char ucMode,
                     void (*p_Int_Handler)(unsigned char));
/*************************************************************
 * GPIO_Int_Disable
 * Description: set GPIO int disable
 * Parameters:  ucChannel:  GPIO channel
 * return:      none
 * error:       none
 */
extern void GPIO_Int_Disable(unsigned char ucChannel);
/*************************************************************
 * GPIO_int_handler_clear
 * Description: clear GPIO int handler
 * Parameters:  ucChannel:  GPIO channel
 * return:      none
 * error:       none
 */
extern void GPIO_int_handler_clear(unsigned char ucChannel);


/*************************************************************
 * GPIO_InterruptHandler
 * Description: GPIO interrupt handler
 * Parameters:  none
 * return:      none
 * error:       none
 */
extern void GPIO_InterruptHandler(void);


/*************************************************************
 * GPIO_Set_Mode
 * Description: set GPIO mode
 * Parameters:  ucChannel:   GPIO channel
 *              ucDirection: GPIO direction, bit[0:1]: 0: input, 1: output
 *                                                     2/3: high-impedance state
 *              bPullEnable: GPIO pull enable, FALSE: disable, TRUE: enable
 *              bPullmode:   GPIO pull up/down, FALSE: pull down, TRUE: pull up
 * return:      STATUS
 * error:       none
 */
extern STATUS GPIO_Set_Mode(unsigned char ucChannel, unsigned char ucDirection, bool bPullEnable, bool bPullmode);
extern void GPIO_Output(unsigned char ucChannel, bool bOutputData);
extern void GPIO_Output_Reverse(unsigned char ucChannel);
extern unsigned char GPIO_Input(unsigned char ucChannel);


extern void GPIO_UART_function_enable(unsigned char ucChannel);

extern void GPIO_I2C0_function_enable(void);
extern void GPIO_I2C_FM_function_enable(void);
extern void GPIO_I2C1_function_enable(void);

extern void GPIO_SPI_function_enable(unsigned char ucChannel);
extern void GPIO_SPI_DMA_function_enable(unsigned char ucChannel);

extern void GPIO_I2S_function_enable(unsigned char ucChannel);

extern void GPIO_PWM_function_enable(unsigned char ucChannel);
extern void GPIO_PWM2_function_enable(unsigned char ucChannel);

extern void GPIO_ADC_function_enable(unsigned char ucChannel);


#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif      /* __DRIVER_GPIO_H__ */
