#ifndef __FPE_DECRYPT_H__
#define __FPE_DECRYPT_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int fpe_decrypt(uint8_t *input_key, uint8_t key_len, uint8_t *input_cipher, uint16_t cipher_len, uint8_t *output, uint8_t *output_len);
	
#ifdef __cplusplus
}
#endif

#endif // __FPE_DECRYPT_H__
