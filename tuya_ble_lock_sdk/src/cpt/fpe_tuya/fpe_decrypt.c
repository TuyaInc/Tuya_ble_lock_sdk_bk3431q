//
// Created by qianh on 2019/7/29.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ff1.h"
#include "fpe_str.h"
#include "elog.h"

int fpe_decrypt(uint8_t *input_key, uint8_t key_len, uint8_t *input_cipher, uint16_t cipher_len, uint8_t *output, uint8_t *output_len) {
    byte_str key = {.buf=input_key, .len=key_len};
    byte_str tweak = create_byte_str("", 0);
    num_str cipher = {.buf=input_cipher, .len=cipher_len};

    ff1_context ctx;
    ctx.ret = 0;
    ctx.max_tweak_len = 20;
#ifdef FPE_LOG_PRINT
    ctx.sys_printf = printf;
#endif
    FPE_PRINT_STR(cipher, -1, "Cipher:", ctx);
    num_str result = decrypt(key, tweak, cipher, ctx);
    if (result.len <= 0) {
        release_str(key);
        return -1;
    }

#ifdef FPE_LOG_PRINT
	ctx.sys_printf("decrypt result->[");
	for (int i = 0; i < (int)result.len; i++) {
        ctx.sys_printf("%d", result.buf[i]);
    }
	ctx.sys_printf("]\r\n");
#endif
    
	*output_len = result.len;
	memcpy(output, result.buf, result.len);
	
    release_str(result);
    release_str(tweak);
    return 0;
}
