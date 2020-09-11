#include <stddef.h>     // standard definition
#include "rwip_config.h"
#include "user_config.h"
#include "BK3435_reg.h"
#include "icu.h"      // timer definition
#include "rf.h"

int Get_utf8_size(unsigned char pInput)
{
	unsigned char c = pInput;
	if(c < 0x80) return 0;  
	if(c >= 0x80 && c < 0xc0) return -1;
	if(c >= 0xc0 && c < 0xe0) return 2;  //0xc0  = 1100 0000,2 bytes
	if(c >= 0xe0 && c < 0xf0) return 3;  //0xe0  = 1110 0000,3 bytes
	if(c >= 0xf0 && c < 0xf8) return 4;  //0xf0  = 1111 0000,4 bytes
	if(c >= 0xf8 && c < 0xfc) return 5;  //0xf8  = 1111 1000,5 bytes
	if(c >= 0xfc ) return 6;		      //0xfc  = 1111 1100,6 bytes	
	return 0;
}

int utf8_to_unicode_one(unsigned char* pInput,unsigned short *Unic)
{
       char b1,b2,b3,b4,b5,b6;
	*Unic = 0x0;
	int utfbytes = Get_utf8_size(*pInput);
	unsigned char *pOutput = (unsigned char *)Unic; //return data in Unic
	switch(utfbytes)
	{
		case 0:
			*pOutput = *pInput;
			utfbytes +=1;
			break;
		case 2:
			b1 = *pInput;
			b2 = *(pInput +1);
			if((b2 & 0xc0) != 0x80)
				return 0;
			*pOutput = (b1 << 6) +(b2 & 0x3F); 
			*(pOutput + 1) = (b1 >> 2) & 0x07;
			break;
		case 3:
			b1 = *pInput;
			b2 = *(pInput +1);
			b3 = *(pInput + 2);
			if(((b2 & 0xC0) != 0x80) || ((b3 & 0xc0) != 0x80))
				return 0;
			*pOutput = (b2 << 6) + (b3 & 0x3f);
			*(pOutput + 1) = (b1 << 4) + ((b2 >> 2) & 0x0f);
			break;
		case 4:
			b1 = *pInput;
			b2 = *(pInput +1);
			b3 = *(pInput + 2);
			b4 = *(pInput + 3);
			if(((b2 & 0xC0) != 0x80) || ((b3 & 0xc0) != 0x80) 
				|| ((b4 & 0xC0) != 0x80))
				return 0;
			*pOutput = (b3 << 6) + (b4 & 0x3f);
			*(pOutput + 1) = (b2 << 4) + ((b3 >> 2) & 0x0f);
			*(pOutput + 2) = ((b1 << 2) & 0x1c) + ((b2 >> 4) & 0x03); //OK
			break;
		case 5:
			b1 = *pInput;
			b2 = *(pInput +1);
			b3 = *(pInput + 2);
			b4 = *(pInput + 3);
			b5 = *(pInput + 4);
			if(((b2 & 0xC0) != 0x80) || ((b3 & 0xc0) != 0x80) 
				|| ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80))
				return 0;
			*pOutput = (b4 << 6) + (b5 & 0x3f);
			*(pOutput + 1) = (b3 << 4) + ((b4 >> 2) & 0x0f);
			*(pOutput + 2) = (b2 << 2)  + ((b3 >> 4) & 0x03);
			*(pOutput + 3) = (b1 << 6) ;  //? shift to hig bit
			//*(pOutput + 3) = b1 & 0x03 ;  //? shift to hig bit
			break;
		case 6:
			b1 = *pInput;
			b2 = *(pInput +1);
			b3 = *(pInput + 2);
			b4 = *(pInput + 3);
			b5 = *(pInput + 4);
			b5 = *(pInput + 5);
			if(((b2 & 0xC0) != 0x80) || ((b3 & 0xc0) != 0x80) 
				|| ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
				|| ((b6 & 0xC0) != 0x80))
				return 0;
			*pOutput = (b5 << 6) + (b6 & 0x3f);
			*(pOutput + 1) = (b5 << 4) + ((b6 >> 2) & 0x0f);
			*(pOutput + 2) = (b3 << 2) + ((b4 >> 4) & 0x03);
			*(pOutput + 3) = ((b1 << 6) & 0x40) + (b2 & 0x3f) ; //OK
			break;
		default:
			return 0;
			//break;
			
	}
	return utfbytes;
}

extern void flash_read_data (uint8_t *buffer, uint32_t address, uint32_t len);
uint16_t Read_GBK2312_From_FlashData(uint16_t pInput) //input unicode value
{
	uint16_t gbk2312_data = 0;
	#define UNICODE_BASE_DATA 0x4e00
	#define UNICODE_BASE1_ADDRESS 0x48000  //216580 
	#define UNICODE_BASE2_ADDRESS 0x5228a  //258190   calc  0x48000 + (258190 - 216580)
	if(pInput >= 0x4e00 && pInput <= 0xe814)
	{
		if(pInput <= 0x9f44)
		{
			//uart_printf("flash read1!\r\n");
			//app_flash_read((uint8_t*)&gbk2312_data,((pInput - UNICODE_BASE_DATA)*2 + UNICODE_BASE1_ADDRESS),2);
			flash_read_data((uint8_t*)&gbk2312_data,((pInput - UNICODE_BASE_DATA)*2 + UNICODE_BASE1_ADDRESS),2);
		}
		else if(pInput >= 0x9f50)
		{
			uint16_t unicode_to_gbk2312_data[24][2]; 
			//uart_printf("flash read2!\r\n");
			flash_read_data((uint8_t*)&gbk2312_data,UNICODE_BASE2_ADDRESS,sizeof(unicode_to_gbk2312_data));
			for(int index = 0; index < 24; index++)
			{
				if(pInput == unicode_to_gbk2312_data[index][0])
				{
					gbk2312_data = unicode_to_gbk2312_data[index][1];
				}
			}
			
		}
		return gbk2312_data;
	}
	return 0;
}


uint8_t CharUtf8_to_GBK2312(unsigned char* pInput)
{
	uint16_t Unic;
	uint8_t buf;
	utf8_to_unicode_one(pInput,&Unic); //utf8 change to unicode
	//brpintf("Unic:%x \r\n",Unic);
	if(Unic >= 0xff01 && Unic <= 0xffe5)
	{
		buf = Unic - 0xff01; 	////blog.csdn.net/a369000753/article/details/51612170
		buf = buf + 0x21;   	//string start 0x21
		return buf;
	}
	
	if(Unic == 0x3002) //。 
	{
		buf = 13 + 0x21;
		return buf;
	}

	if(Unic == 0x3001) //'
	{
		buf = 11 + 0x21;
		return buf;
	}

	if(Unic == 0x2026)//...
	{
		buf = 0x2e;//.
		return buf;
	}
	return 0;
}

uint16_t ChineseUtf8_to_GBK2312(unsigned char* pInput)
{
	uint16_t Unic;
	uint16_t Gbk232 = 0;
	utf8_to_unicode_one(pInput, &Unic);
	#if 0
		uint16_t i =0;
		if(Unic >= 0x4e00 && Unic <= 0xE814)
		{
			i = Unicode_GetIdx(Unic); //find out GBK232 number
			return GB_TO_UNI[i][1];
		}
	#endif
	Gbk232 = Read_GBK2312_From_FlashData(Unic);
	if(Gbk232 != 0)
	{
		return Gbk232;
	}
	return 0;
}

//decode uft8 to unicode,then to gbk2312 
uint16_t app_utf8_to_gbk2312_decode(unsigned char* pInput,unsigned char* Gbk2312) //only one gbk2312 code 
{
	uint16_t Gbk2312buf = 0;
	Gbk2312buf = ChineseUtf8_to_GBK2312(pInput);  
	if(Gbk2312buf != 0)
	{
		Gbk2312[0] = (Gbk2312buf & 0xff00) >> 8;
		Gbk2312[1] = (Gbk2312buf & 0x00ff);
		//Gbk2312[0] = (Gbk2312buf & 0x00ff);  //180322
		//Gbk2312[1] = (Gbk2312buf & 0xff00) >> 8;
		//uart_printf("%x",Gbk2312[1]);
		//uart_printf("%x",Gbk2312[0]);
             //uart_printf("\r\n");
		return 2;
	}
	else
	{
		Gbk2312buf = CharUtf8_to_GBK2312(pInput);
		if(Gbk2312buf != 0)
		{
			*Gbk2312 = Gbk2312buf & 0xff;
			//uart_printf("%x",Gbk2312[1]);
			//uart_printf("%x",Gbk2312[0]);
             		//uart_printf("\r\n");
			return 1;
		}
	}
	*Gbk2312 = 0x20;  //空间符表示
	return 1;
}

uint16_t app_phone_message_decode2GB2312(uint8_t *p_dst,uint8_t max_size,uint8_t *p_src,uint16_t length)
{
	uint16_t message_index = 0;
	uint8_t utf8_temp[3];
       uint8_t size;
	unsigned char GBK2312[2];
	
	for(int index = 0; index < length; index++)
	{
	}

       for(int index = 0; index < length; index++)
       {
       	if(message_index < max_size)
       	{
	       	if(p_src[index] < 0x80)
	       	{
	       		p_dst[message_index]=p_src[index] ;
				//uart_printf("%x\r\n",p_dst[message_index]);
				message_index++;
	       	}
			else
			{
				for(int i =0; i < 3; i++)
				{
					utf8_temp[i] = p_src[index+i];
					//uart_printf(" %x",utf8_temp[i]);
				}
				//uart_printf("\r\n");
				size = app_utf8_to_gbk2312_decode(&utf8_temp[0],&GBK2312[0]);
				index += 2;
				if(size == 1)
				{
					p_dst[message_index]= GBK2312[0];
					message_index++;
				}
				else if(size == 2)
				{
					p_dst[message_index] = GBK2312[0]; //tow byte 
					p_dst[message_index+1] = GBK2312[1];
					message_index += 2;
				}
			}
       	}
       }
	return message_index;
}

uint16_t app_phone_message_decode2unicode(uint8_t *p_dst,uint16_t max_size,uint8_t *p_src,uint16_t length)//uif-8转unicode
{
	uint16_t message_index = 0;
	uint16_t unicode = 0;
	for(int index = 0; index < length; index++)
	{
		if(message_index < max_size)
		{
			uint8_t size = utf8_to_unicode_one(&p_src[index],&unicode);
			p_dst[message_index] = unicode & 0x00ff; //tow byte 
			p_dst[message_index+1] = unicode>>8 & 0xff;
			index += (size-1);
			message_index += 2;
		}
	}
	return message_index;
}































