/*************************************************************
 * @file        driver_usb.c
 * @brief       code of USB driver of BK3435_v2
 * @author      GuWenFu
 * @version     V1.0
 * @date        2016-09-29
 * @par
 * @attention
 *
 * @history     2016-09-29 gwf    create this file
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rwip_config.h" 

#include "msg.h"
#include "timer.h"

#include "BK_System.h"
#include "driver_icu.h"
//#include "driver_xvr.h"
#include "driver_usb.h"
#include "driver_gpio.h"

#include "mu_impl.h"

/*************************************************
 * DelayNops
 */
void DelayNops(volatile unsigned long nops)
{
    while (nops --)
    {
    }
}

/*************************************************
 * DelayMS
 * 测试结果，MCU主频为16MHz
 * DelayMS(1) = 1.003ms
 * DelayMS(100)平均DelayUS(1) = 1.001ms
 */
void DelayMS(volatile unsigned long timesMS)
{
	volatile unsigned long i;
#if (CHIP_ID == CHIP_7211)
	////	unsigned long tick_cnt = 12800;   // 24MHz, DelayMS(100)平均DelayMS(1) = 1.004ms
	//	  unsigned long tick_cnt = 3720;   // 26MHz, DelayMS(100)平均DelayMS(1) = 1.002ms
	#if (MCU_CLK == MCU_CLK_26MHz)
	#define tick_cnt_ms 		3720
	#elif (MCU_CLK == MCU_CLK_120MHz)		/* #if (MCU_CLK == MCU_CLK_26MHz) */
	#define tick_cnt_ms 		29500
	#endif		/* #if (MCU_CLK == MCU_CLK_26MHz) */
#elif (CHIP_ID == CHIP_3433)
	#if (MCU_CLK == MCU_CLK_32KHz)
		#define 	tick_cnt_ms 		8
	#elif (MCU_CLK == MCU_CLK_16MHz)
		#define 	tick_cnt_ms 		1111
	#endif
#elif ((CHIP_ID == CHIP_3435) || (CHIP_ID == CHIP_3435_v2))
	#if (MCU_CLK == MCU_CLK_32KHz)
		#define 	tick_cnt_ms 		8
	#elif (MCU_CLK == MCU_CLK_16MHz)
		#define 	tick_cnt_ms 		1660
	#elif (MCU_CLK == MCU_CLK_48MHz)
		#define 	tick_cnt_ms 		3000
	#else
		#define 	tick_cnt_ms 		(1660 * 3)
	#endif
#else		/* #if (CHIP_ID == CHIP_7211) */
	//	  unsigned long tick_cnt = (unsigned long)((float)1333*MCU_CLK/16000000 + 1);
	#define tick_cnt_ms 		((unsigned long)((float)1333*MCU_CLK/16000000 + 1))
#endif		/* #if (CHIP_ID == CHIP_7211) */
//	  printf("tick_cnt = 0x%x\r\n", tick_cnt);
	while (timesMS --)
	{
		i = 0;
		while (i < tick_cnt_ms)
		{
			i++;
		}
	}
}

extern int usb_sw_init(void);
extern void MGC_AfsUdsIsr(uint16_t wIntrTxValue,uint16_t wIntrRxValue,uint8_t bIntrUsbValue);
void usb_init(USB_MODE usb_mode)
{
	#if 0//SYSTEM_SLEEP
        ICU_USB_PWD_SET();   //close usb //190219
        DelayNops(500);
	#else
	unsigned char ucUSBIntStatus;

        //    GPIO_Output(26, TRUE);
        os_printf("usb_init: REG_ICU_INT_ENABLE = 0x%lx, REG_ICU_INT_GLOBAL_ENABLE = 0x%lx\r\n",
        REG_ICU_INT_ENABLE, REG_ICU_INT_GLOBAL_ENABLE);
        ICU_USB_PWD_SET();
        DelayNops(500);
        //DelayNops(50000);
        ICU_USB_PWD_CLEAR();

        REG_USB_INTRRX1E = 0x0;
        REG_USB_INTRTX1E = 0x0;
        REG_USB_INTRUSBE = 0x0;
        REG_AHB2_USB_VTH &= ~(1<<7);

        if (usb_mode == USB_HOST_MODE)
        {
            REG_AHB2_USB_OTG_CFG = 0x50;        // host
            REG_AHB2_USB_DEV_CFG = 0x00;
        }
        else
        {
            REG_USB_INTRRX1E = 0x07;
            REG_USB_INTRTX1E = 0x07;
            REG_USB_INTRUSBE = 0x3F;
            REG_AHB2_USB_OTG_CFG = 0x08;        // device
            REG_AHB2_USB_DEV_CFG = 0xF4;
            REG_AHB2_USB_OTG_CFG |= 0x01;
        }

    ucUSBIntStatus = REG_AHB2_USB_INT;
    DelayNops(500);
    REG_AHB2_USB_INT = ucUSBIntStatus;
    DelayNops(500);

    REG_AHB2_USB_GEN = (0x7<<4) | (0x7<<0);   //  DP_EN DN_EN

    //    GPIO_Output(26, FALSE);
    if (usb_sw_init() == 0)
    {
	  //    GPIO_Output(26, TRUE);
        ICU_INT_ENABLE_SET(ICU_INT_ENABLE_IRQ_USB_MASK);
        uart_printf("usb_sw_init OK\r\n");
    }
    else
    {
        uart_printf("usb_sw_init failed!\r\n");
    }
    os_printf("usb_init: REG_AHB2_USB_OTG_CFG = 0x%x, REG_AHB2_USB_DEV_CFG = 0x%x\r\n",
            REG_AHB2_USB_OTG_CFG, REG_AHB2_USB_DEV_CFG);
    os_printf("usb_init: REG_ICU_INT_ENABLE = 0x%lx, REG_ICU_INT_GLOBAL_ENABLE = 0x%lx\r\n",
        REG_ICU_INT_ENABLE, REG_ICU_INT_GLOBAL_ENABLE);
		#endif
}

void USB_InterruptHandler(void)
{
	//uint8 ucUSBIntStatus = REG_AHB2_USB_INT;
	uint8 bIntrUsbValue = REG_USB_INTRUSB;
	uint16 wIntrTxValue = REG_USB_INTRTX1 | (REG_USB_INTRTX2 << 8);
	uint16 wIntrRxValue = REG_USB_INTRRX1 | (REG_USB_INTRRX2 << 8);

	//GPIO_Output_Reverse(9);
	os_printf("------------------- USB_InterruptHandler in  ---------------------\r\n");
	//    UART_PRINTF("bIntrUsbValue = 0x%x, wIntrTxValue = 0x%x, wIntrRxValue = 0x%x, REG_USB_DEVCTL = 0x%x, REG_AHB2_USB_INT = 0x%x\r\n",
	//            bIntrUsbValue, wIntrTxValue, wIntrRxValue, REG_USB_DEVCTL, ucUSBIntStatus);
	//    if (wIntrTxValue == 1)
	//    {
	//        wIntrTxValue = 1;
	//    }
	bIntrUsbValue &= ~(0xc0);
	MGC_AfsUdsIsr(wIntrTxValue,wIntrRxValue,bIntrUsbValue);
	os_printf("------------------- USB_InterruptHandler out ---------------------\r\n");
}
