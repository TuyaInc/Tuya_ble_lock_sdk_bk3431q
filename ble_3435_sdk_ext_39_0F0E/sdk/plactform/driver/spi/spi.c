
/**
 ****************************************************************************************
 *
 * @file spi.c
 *
 * @brief spi driver
 *
 * Copyright (C) BeKen 2009-2017
 *
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 /*
#include <stddef.h>     // standard definition
#include <stdarg.h>
#include <stdio.h>
#include "driver_spi.h"       // uart definition
#include "BK3435_reg.h"
#include "reg_intc.h"
#include "intc.h"
#include "rwip.h"       // SW interface
#include "uart.h"
*/
#include "include.h"
#include "DeviceBtj_Header.h"



/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */
#define GPIO_SPI_CFG            		REG_APB5_GPIO0_CFG
#define GPIO_SPI_SCK_PIN          	4
#define GPIO_SPI_MOSI_PIN        	5
#define GPIO_SPI_MISO_PIN        	6
#define GPIO_SPI_NSS_PIN           	7

#define 	SPI_IDLE 	0
#define 	SPI_BUSY 	1

#define SPI_CLK_DIVID_SET     (NUMBER_ROUND_UP(SPI_DEFAULT_CLK/2, SPI_DEFAULT_BAUD) - 1)



///SPI Character format
enum SPI_CHARFORMAT
{
 	SPI_CHARFORMAT_8BIT = 0x00UL,
    	SPI_CHARFORMAT_16BIT = 0x01UL
};

enum CKPHA
{
	 CKPHA_CLK1 = 0x00UL,
	 CKPHA_CLK2 = 0x01UL
};

enum CKPOL
{
	 CKPOL_L = 0x00UL,
	 CKPOL_H = 0x01UL
};

enum NSSMD
{
	LINE3_NSS_NULL = 0,
	LINE4_NSS_CHIP = 1,
	LINE4_NSS_M = 2,
	LINE4_NSS_S_E =  3,
};


//SPI CLK = (Mclk/2)/(freq_div+1)  //Mclk is 16M 
//exapmle:freq_div:3   ->   SPI CLK  2M
//             freq_div:0   ->   SPI CLK  8M
void spi_init(uint8_t freq_div)
{
	
	 // SPI clock enable
	 REG_AHB0_ICU_SPICLKCON = 0x00;     

	 // Close JTAG to release GPIO to normal function
     	CLOSE_JTAG_MODE();

	 // Enable GPIO P0.4, P0.5, P0.6, P0.7 peripheral function for spi
     	GPIO_SPI_CFG &= ~((0x1<<GPIO_SPI_SCK_PIN)  | (0x1<<GPIO_SPI_MOSI_PIN)
                    | (0x1<<GPIO_SPI_MISO_PIN) | (0x1<<GPIO_SPI_NSS_PIN ));
	
      		UART_PRINTF("SPI_CTRL 0 = %x\r\n",REG_APB2_SPI_CTRL);
		
		REG_APB2_SPI_CTRL = (0x01UL << BIT_SPI_CTRL_SPIEN)
					|( 0x1 << BIT_SPI_CTRL_MSTEN)           //master mode 
					| (CKPHA_CLK1 << BIT_SPI_CTRL_CKPHA)
					| (CKPOL_L << BIT_SPI_CTRL_CKPOL)
					| (SPI_CHARFORMAT_8BIT << BIT_SPI_CTRL_BIT_WIDTH)
					| (0x03UL << BIT_SPI_CTRL_NSSMD)
					| (freq_div << BIT_SPI_CTRL_SPI_CKR) 
					//| (8 << BIT_SPI_CTRL_SPI_CKR)
					| (0x01UL << BIT_SPI_CTRL_RXINT_EN)
					| (0x00UL << BIT_SPI_CTRL_TXINT_EN)
					| (0x00UL << BIT_SPI_CTRL_RXOVR_EN)
					| (0x00UL << BIT_SPI_CTRL_TXOVR_EN)
					| (0x03UL << BIT_SPI_CTRL_RXINT_MODE)
					| (0x00UL << BIT_SPI_CTRL_TXINT_MODE);
	
		UART_PRINTF("SPI_CTRL 1 = %x\r\n",REG_APB2_SPI_CTRL);
}	


static void spi_tx_start(void)
{
	uint32_t reg ;
	reg = REG_APB2_SPI_CTRL;
	
	reg |= (0x03ul << BIT_SPI_CTRL_NSSMD) ;
	REG_APB2_SPI_CTRL = reg; 	
	
	reg &= ~(0x03ul << BIT_SPI_CTRL_NSSMD) ;
  	reg |= (0x02ul << BIT_SPI_CTRL_NSSMD) ;
	
  	REG_APB2_SPI_CTRL = reg; 

	UART_PRINTF("SPI_CTRL 2 = %x\r\n",REG_APB2_SPI_CTRL);
}

void spi_tx_end(void)
{
	uint32_t reg ;
	uint32_t state;
	reg  = REG_APB2_SPI_CTRL;

	reg |=(0x01UL << BIT_SPI_CTRL_TXINT_EN);	
	
	REG_APB2_SPI_CTRL = reg;

	state = REG_APB2_SPI_STAT;
	
	while(!(state & (0x01 << BIT_SPI_STAT_TXINT)))
	{
		state = REG_APB2_SPI_STAT;
	}		
	
	reg &=~(0x01UL << BIT_SPI_CTRL_TXINT_EN);	
	
  	REG_APB2_SPI_CTRL = reg;
	
	REG_APB2_SPI_STAT = state;

	reg |= (0x03UL << BIT_SPI_CTRL_NSSMD);	
	
	REG_APB2_SPI_CTRL = reg;

	UART_PRINTF("SPI_CTRL 3 = %x\r\n",REG_APB2_SPI_CTRL);
}


static void spi_write_byte(uint8_t data)
{
	while(spi_is_busy());
	
	while(SPI_TX_FIFO_FULL);	
	
	REG_APB2_SPI_DAT = data;
}

static uint8_t spi_read_byte(void)
{
	while(spi_is_busy());
	
	while(SPI_RX_FIFO_EMPTY);	
	
	return (REG_APB2_SPI_DAT & 0xff);
}


void spi_write_read(uint8_t *wbuf, uint32_t w_size, uint8_t *rbuf, uint32_t r_size,uint32_t r_off)
{
	uint32_t max_len;
	
	max_len = (w_size > (r_size + r_off)) ? w_size : (r_size + r_off);

	spi_tx_start();

	while(max_len--)
	{
		if(w_size > 0)
		{
			spi_write_byte(*wbuf);
			wbuf++;
			w_size--;
		}
		else
		{
			spi_write_byte(0x00);
		}
		if(r_off > 0)
		{	
			spi_read_byte();
			r_off--;
		}
		else if(r_size > 0)
		{
			*rbuf = spi_read_byte();
			rbuf++;
			r_size--;
		}else
		{		
			spi_read_byte();
		}
	}
	spi_tx_end();
}


uint8_t spi_is_busy(void)
{
  	return REG_APB2_SPI_STAT & (0x01<<BIT_SPI_STAT_SPI_BUSY);
}

uint8_t spi_buf[80],spi_rxbuf[80];
uint8_t tx_length = 80;
uint8_t spi_status = 0;

unsigned char    *p_spi_txdata;
unsigned char    *p_spi_rxdata;

/*************************************************************************
**函数名称:void spi_master_test(void)
**功能描述:直接调用，可进行做主测试
*************************************************************************/
void spi_master_test(void)
{
	/****************************************************
	**SPI master test
	****************************************************/
	int i;
	for(i=0;i<80;i++)
	{
		spi_buf[i]=i+0x56;//+0x56;
		spi_rxbuf[i]=i+0x20;//0x78;
	}
	
	spi_init(7);  //8M for master ok //4M
	//spi_init(42); 
	while(1)
	{
		spi_write_read(&spi_buf[0],60,&spi_rxbuf[0],60,0);
		for(i=0;i<80;i++)
		{
				bprintf("buf%d:%x,  rx:%x;\r\n",i,spi_buf[i],spi_rxbuf[i]);
		}
		memset(spi_rxbuf,0,80);
		Delay_ms(100);
	}
	
}



/*************************************************************************
below SPI Slave code
*************************************************************************/
void spi_slave_init(void)
{
		int i;
    REG_AHB0_ICU_SPICLKCON    = 0x0 ;                     //SPI clock enable
    REG_AHB0_ICU_INT_ENABLE  |= INT_STATUS_SPI;      //IRQ UART interrupt enable
    //REG_APB5_GPIOA_CFG &= ~(0xF0<<BIT_GPIO_NORMAL);// Enable GPIO-A[7:4] peripheral functions
   GPIO_SPI_CFG &= ~((0x1<<GPIO_SPI_SCK_PIN)  | (0x1<<GPIO_SPI_MOSI_PIN)
                    | (0x1<<GPIO_SPI_MISO_PIN) | (0x1<<GPIO_SPI_NSS_PIN ));

	REG_APB2_SPI_CTRL = 0;
    REG_APB2_SPI_CTRL = (0x01UL << BIT_SPI_CTRL_SPIEN)
					//|( 0x1 << BIT_SPI_CTRL_MSTEN)           //master mode //set slave mode 
					| (CKPHA_CLK1 << BIT_SPI_CTRL_CKPHA)
					| (CKPOL_L << BIT_SPI_CTRL_CKPOL)
					| (SPI_CHARFORMAT_8BIT << BIT_SPI_CTRL_BIT_WIDTH)
					| (0x01UL << BIT_SPI_CTRL_NSSMD)
					//| (0x01UL << BIT_SPI_CTRL_NSSMD)  //
					//| (freq_div << BIT_SPI_CTRL_SPI_CKR)  //clk div
					//| (0x00UL << BIT_SPI_CTRL_RXINT_EN) 
					| (0x01UL << BIT_SPI_CTRL_RXINT_EN)  // enable rxint
					| (0x00UL << BIT_SPI_CTRL_TXINT_EN)  
					| (0x00UL << BIT_SPI_CTRL_RXOVR_EN)
					| (0x00UL << BIT_SPI_CTRL_TXOVR_EN)
					//| (0x00UL << BIT_SPI_CTRL_RXINT_MODE)
					| (0x03UL << BIT_SPI_CTRL_RXINT_MODE)  //rx 
					| (0x03UL << BIT_SPI_CTRL_TXINT_MODE);
	for(i = 0;i < 10; i++)
	{
		REG_APB2_SPI_DAT = *p_spi_txdata;  
            	p_spi_txdata ++;
	}

	UART_PRINTF("SPI_CTRL 1 slave = %x\r\n",REG_APB2_SPI_CTRL);

	
}

void spi_slave_start(void)
{
	    REG_APB2_SPI_CTRL |= 0x20;  //open rxint_en
}

void spi_slave_stop()
{
	REG_APB2_SPI_CTRL = 0;
	REG_AHB0_ICU_SPICLKCON    = 0x1 ;                     //SPI clock enable
    	REG_AHB0_ICU_INT_ENABLE  &= ~INT_STATUS_SPI;      //IRQ UART interrupt enable
       REG_APB2_SPI_STAT = 0;
}

bool spi_slave_getSLVSEL() //bit 14
{
	return(REG_APB2_SPI_STAT & 0x4000);
}

//after cs high，init REG_APB2_SPI_DAT(10byte)
void spi_slave_DoneData(void)
{
	int i;
	if(spi_slave_getSLVSEL() && (spi_status == SPI_IDLE))  //CS active
	{
		spi_status = SPI_BUSY;
	}
	else if((!spi_slave_getSLVSEL()) &&  (spi_status == SPI_BUSY)) //cs inactinve
	{
		spi_slave_stop();
		for(i=0;i<60;i++)
		{
			UART_PRINTF("buf%d:%x,rx:%x;\r\n",i,spi_buf[i],spi_rxbuf[i]);
		}
		memset(spi_rxbuf,0,80);
		p_spi_txdata =spi_buf;
   		p_spi_rxdata = spi_rxbuf;
		spi_slave_init();
		spi_status = SPI_IDLE;
	}
}

void spi_slave_isr(void)
{
	unsigned int            rxint;
	unsigned char           rxfifo_empty = 0;

 
	rxint = REG_APB2_SPI_STAT & 0x0200;  
	
	if(rxint)
	{
		rxfifo_empty = REG_APB2_SPI_STAT & 0x0004;
		while(!rxfifo_empty)
		{		
			REG_APB2_SPI_DAT = *p_spi_txdata;  
            		p_spi_txdata ++;
            		
			*p_spi_rxdata = REG_APB2_SPI_DAT;
			p_spi_rxdata ++;
			rxfifo_empty = REG_APB2_SPI_STAT & 0x0004;		
		}

             REG_APB2_SPI_STAT |= 0x0200; //clear rxint
	}
}

/*************************************************************************
**函数名称:void spi_master_test(void)
**功能描述:直接调用，可进行做从测试
*************************************************************************/
void spi_slave_test(void)
{
	/****************************************************
	**SPI slave test
	****************************************************/
	int i;
  	for(i=0;i<80;i++)
	{
		spi_buf[i]=i+0x0;//+0x56;
		spi_rxbuf[i]=i+0x20;//0x78;
	}
       p_spi_txdata =spi_buf;
       p_spi_rxdata = spi_rxbuf;
	spi_slave_init();
	spi_slave_start();
	while(1)
	{		            
		spi_slave_DoneData();
	}
}



/// @} SPI DRIVER
