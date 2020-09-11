/*************************************************************
 * @file        driver_usb.h
 * @brief       Header file of driver_usb.c
 * @author      GuWenFu
 * @version     V1.0
 * @date        2016-09-29
 * @par
 * @attention
 *
 * @history     2016-09-29 gwf    create this file
 */

#ifndef __DRIVER_USB_H__

#define __DRIVER_USB_H__


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#include "BK_System.h"


#if (CHIP_ID == CHIP_3435_v2)
#define REG_USB_BASE_ADDR               (0x00804000UL)
#endif      /* #if (CHIP_ID == CHIP_3435_v2) */


#ifdef USB_3435_v2_STYLE
#define REG_USB_FADDR                     (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x00)))
#define REG_USB_POWER                     (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x01)))
#define REG_USB_INTRTX1                   (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x02)))
#define REG_USB_INTRTX2                   (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x03)))
#define REG_USB_INTRRX1                   (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x04)))
#define REG_USB_INTRRX2                   (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x05)))
#define REG_USB_INTRUSB                   (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x06)))
#define REG_USB_INTRTX1E                  (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x07)))
#define REG_USB_INTRTX2E                  (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x08)))
#define REG_USB_INTRRX1E                  (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x09)))
#define REG_USB_INTRRX2E                  (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x0A)))
#define REG_USB_INTRUSBE                  (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x0B)))
#define REG_USB_FRAME1                    (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x0C)))
#define REG_USB_FRAME2                    (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x0D)))
#define REG_USB_INDEX                     (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x0E)))
#define REG_USB_DEVCTL                    (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x0F)))
#define REG_USB_TXMAXP                    (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x10)))
#define REG_USB_CSR0                      (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x11)))
#define REG_USB_CSR02                     (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x12)))
#define REG_USB_RXMAXP                    (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x13)))
#define REG_USB_RXCSR1                    (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x14)))
#define REG_USB_RXCSR2                    (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x15)))
#define REG_USB_COUNT0                    (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x16)))
#define REG_USB_RXCOUNT2                  (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x17)))
#define REG_USB_TXTYPE                    (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x18)))
#define REG_USB_NAKLIMIT0                 (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x19)))
#define REG_USB_RXTYPE                    (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x1A)))
#define REG_USB_RXINTERVAL                (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x1B)))

#define REG_USB_FIFOSIZE                  (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x1F)))

#define REG_USB_FIFO0                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x20)))
#define REG_USB_FIFO1                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x24)))
#define REG_USB_FIFO2                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x28)))
#define REG_USB_FIFO3                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x2C)))
#define REG_USB_FIFO4                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x30)))
#define REG_USB_FIFO5                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x34)))
#define REG_USB_FIFO6                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x38)))
#define REG_USB_FIFO7                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x3C)))
#define REG_USB_FIFO8                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x40)))
#define REG_USB_FIFO9                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x44)))
#define REG_USB_FIFOA                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x48)))
#define REG_USB_FIFOB                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x4C)))
#define REG_USB_FIFOC                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x50)))
#define REG_USB_FIFOD                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x54)))
#define REG_USB_FIFOE                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x58)))
#define REG_USB_FIFOF                     (*((volatile unsigned long *) (REG_USB_BASE_ADDR + 0x5C)))

#define REG_AHB2_USB_OTG_CFG              (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x80)))
#define REG_AHB2_USB_DMA_ENDP             (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x84)))
#define REG_AHB2_USB_VTH                  (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x88)))
#define REG_AHB2_USB_GEN                  (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x8C)))
#define REG_AHB2_USB_STAT                 (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x90)))
#define REG_AHB2_USB_INT                  (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x94)))
#define REG_AHB2_USB_RESET                (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x98)))
#define REG_AHB2_USB_DEV_CFG              (*((volatile unsigned char *) (REG_USB_BASE_ADDR + 0x9C)))
#endif      /* #ifdef USB_3435_v2_STYLE */


#define USB_RET_OK			0
#define USB_RET_ERROR 		1
#define USB_RET_CONNECT 	2
#define USB_RET_DISCONNECT  3
#define USB_RET_READ_OK 	4
#define USB_RET_WRITE_OK 	5


typedef enum        // by gwf
{
    USB_HOST_MODE   = 0,
    USB_DEVICE_MODE = 1
} USB_MODE;

typedef struct __driver_udisk_s
{
    uint32        total_block;
	uint16		  block_size;
	uint16		  InitFlag;
} driver_udisk_t;


extern void usb_init(USB_MODE usb_mode);
extern void test_usb_device(void);
extern void USB_InterruptHandler(void);



#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif      /* __DRIVER_USB_H__ */
