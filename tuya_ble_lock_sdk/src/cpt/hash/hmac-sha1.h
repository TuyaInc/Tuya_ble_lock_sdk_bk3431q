#ifndef __HMAC_SHA1_H__
#define __HMAC_SHA1_H__

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdint.h>
    
#define HMAC_BLOCK_SIZE 		( 64 )
    
void HMAC_SHA1(uint8_t *key, unsigned int key_len, uint8_t *message, unsigned int msg_len, uint8_t *digest);

#ifdef __cplusplus
}
#endif

#endif // __HMAC_SHA1_H__
