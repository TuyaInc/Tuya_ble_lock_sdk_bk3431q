#include <string.h>

#include "co_bt.h"
#include "prf_types.h"               // Profile common types definition
#include "arch.h"                    // Platform Definitions
#include "prf.h"
#include "rf.h"
#include "prf_utils.h"
#include "util_encrypt.h"
#include "uart.h"
#include "flash.h"
#include "enc_key.h"
#include "co_utils.h"


extern uint8_t encrypt_key_array[16];

void code_sanity_check(void)
{
	uint8_t original_text[16];
	uint8_t plain_text[16];

	//read encrypted data from flash addr
	flash_read_data(&original_text[0], 0x400e3+6, 16);
	
	generate_key(co_default_bdaddr.addr, encrypt_key_array, plain_text);
	if(!memcmp(original_text, plain_text, 16))
	{
		//UART_PRINTF("procedure ok\r\n");
		return;
	}
	while(1)
	{
		//UART_PRINTF("Illegal procedure!\r\n");
	}
}








