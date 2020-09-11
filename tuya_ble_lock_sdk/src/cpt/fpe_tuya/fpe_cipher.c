//
// Created by qianh on 2019/7/29.
//
#include <stdlib.h>
#include <string.h>
#include "fpe_cipher.h"
#include "fpe_str.h"
#include "mbedtls/aes.h"
#if defined (USED_STDLIB_MEMALLOC)
#include <stdlib.h>
#else 
#include "tuya_ble_heap.h"
#endif 

#if 0
byte_str prf(byte_str key, byte_str blocks) {
    mbedcrypto_aes_context ctx;
    mbedcrypto_aes_init(&ctx);
    mbedcrypto_aes_setkey_enc(&ctx, key.buf, key.len * 8);
    uint8_t *output = calloc(blocks.len, 1);
    uint8_t iv[16] = {0};
    mbedcrypto_aes_crypt_cbc(&ctx, MBEDCRYPTO_AES_ENCRYPT, blocks.len, iv, blocks.buf, output);
    mbedcrypto_aes_free(&ctx);
    uint8_t *ret_buf = calloc(16, 1);
    memcpy(ret_buf, output + (blocks.len - 16), 16);
    free(output);
    byte_str ret = {.buf=ret_buf, .len=16};
    return ret;
}
#endif 

byte_str prf(byte_str key, byte_str blocks) {
	mbedtls_aes_context aes_ctx;
	uint8_t iv[16] = {0};
#if defined (USED_STDLIB_MEMALLOC)	
	uint8_t *output = calloc(blocks.len, 1);
#else 	
	uint8_t *output = pvTuyaPortMalloc(blocks.len*1);
	memset(output, 0x00, blocks.len*1);
#endif 
	
    mbedtls_aes_init(&aes_ctx);
    mbedtls_aes_setkey_enc(&aes_ctx, key.buf, key.len * 8);
    mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_ENCRYPT, blocks.len, iv, blocks.buf, output);
	mbedtls_aes_free(&aes_ctx);
	
#if defined (USED_STDLIB_MEMALLOC)	
	uint8_t *ret_buf = calloc(16, 1);
#else	
	uint8_t *ret_buf = pvTuyaPortMalloc(16*1);
	memset(ret_buf, 0x00, 16*1);
#endif 	
    memcpy(ret_buf, output + (blocks.len - 16), 16);

#if defined (USED_STDLIB_MEMALLOC)	
    free(output);
#else 	
	vTuyaPortFree(output);
#endif 
	
    byte_str ret = {.buf=ret_buf, .len=16};
	return ret;
}
