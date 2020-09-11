//
// Created by qianh on 2019/7/28.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fpe_str.h"
#include "fpe_config.h"
#if defined (USED_STDLIB_MEMALLOC)
#include <stdlib.h>
#else
#include "tuya_ble_heap.h"
#endif 

byte_str create_byte_str(uint8_t *data, uint32_t len) {
#if defined (USED_STDLIB_MEMALLOC)
    uint8_t *buf = malloc(len);
#else 
	uint8_t *buf = pvTuyaPortMalloc(len);
#endif 
	
    memcpy(buf, data, len);
    byte_str ret = {.len = len, .buf = buf};
    return ret;
}

void release_str(byte_str s){
    if (s.buf != NULL) {
#if defined (USED_STDLIB_MEMALLOC)		
        free(s.buf);
#else 
		vTuyaPortFree(s.buf);
#endif 
        s.buf = NULL;
    }
    s.len = 0;
}

byte_str byte_str_copy_of_range(byte_str source, uint32_t start, uint32_t end, bool release) {
    if (!(start >= 0 && end >= 0 && end >= start && end <= source.len)) {
        byte_str invalid = {.buf=NULL, .len=-1};
        return invalid;
    }

    int new_len = end - start;    
#if defined (USED_STDLIB_MEMALLOC)	
    uint8_t *new_buf = malloc(new_len);
#else 	
	uint8_t *new_buf = pvTuyaPortMalloc(new_len);
#endif 	
	
    memcpy(new_buf, source.buf + start, new_len);
    if (release) {
        release_str(source);
    }
    byte_str ret = {.buf = new_buf, .len = new_len};
    return ret;
}

byte_str to_byte_str(uint32_t x, uint32_t s) {
#if defined (USED_STDLIB_MEMALLOC)		
    uint8_t *buf = calloc(1, s);
#else 	
	uint8_t *buf = pvTuyaPortMalloc(1*s);
	memset(buf, 0x00, 1*s);
#endif 	

    for (int i = s - 1; i >= 0 && x > 0; i--) {
        buf[i] = (uint8_t) x;
        x >>= 8;
    }
    byte_str ret = {.buf=buf, .len=s};
    return ret;
}


byte_str to_byte_str_with_bignum(byte_str x, uint32_t s) {
#if defined (USED_STDLIB_MEMALLOC)		
    uint8_t *buf = calloc(1, s);
#else 
	uint8_t *buf = pvTuyaPortMalloc(1*s);
	memset(buf, 0x00, 1*s);
#endif 
	
    uint8_t *src_start = x.buf + (x.len > s ? x.len - s : 0);
    uint8_t *dst_start = buf + (s > x.len ? s - x.len : 0);
    uint32_t copy_len = x.len < s ? x.len : s;
    memcpy(dst_start, src_start, copy_len);
    byte_str ret = {.buf=buf, .len=s};
    return ret;
}


byte_str byte_str_concatenate(byte_str x, byte_str y, bool release) {
#if defined (USED_STDLIB_MEMALLOC)		
    uint8_t *buf = calloc(x.len + y.len, 1);
#else 	
	uint8_t *buf = pvTuyaPortMalloc((x.len + y.len));
	memset(buf, 0x00, (x.len + y.len));
#endif
	
    memcpy(buf, x.buf, x.len);
    memcpy(buf + x.len, y.buf, y.len);
    byte_str ret = {.buf = buf, .len = x.len + y.len};
    if (release) {
        release_str(x);
        release_str(y);
    }
    return ret;
}

byte_str byte_str_to_num(byte_str str) {
    return str;
}

num_str num_str_copy_of_range(num_str source, uint32_t start, uint32_t end) {
    if (!(start >= 0 && end >= 0 && end >= start && end <= source.len)) {
        num_str invalid = {.buf=NULL, .len=-1};
        return invalid;
    }
	
    uint32_t new_len = end - start;
#if defined (USED_STDLIB_MEMALLOC)		
    uint8_t *new_buf = malloc(new_len * sizeof(uint8_t));
#else	
	uint8_t *new_buf = pvTuyaPortMalloc(new_len * sizeof(uint8_t));
#endif 
	
    memcpy(new_buf, source.buf + start, new_len * sizeof(uint8_t));
    num_str ret = {.buf = new_buf, .len = new_len};
    return ret;
}

byte_str num_str_to_num(num_str str, uint32_t radix) {
    uint32_t x = 0;
    for (int i = 0; i < str.len; i++) {
        x = x * radix + str.buf[i];
    }
    return to_byte_str(x, 4);
}

num_str num_to_num_str(uint32_t num, uint32_t radix, uint32_t m) {
#if defined (USED_STDLIB_MEMALLOC)		
    uint8_t *buf = calloc(m, sizeof(uint8_t));
#else 
	uint8_t *buf = pvTuyaPortMalloc(m*sizeof(uint8_t));
	memset(buf, 0x00, m*sizeof(uint8_t));
#endif 	
	
    for (int i = 1; i <= m; i++) {
        buf[m - i] = (uint8_t)(num % FPE_RADIX);
        num = num / FPE_RADIX;
    }
    num_str ret = {.buf=buf, .len=m};
    return ret;
}

num_str num_str_concatenate(num_str x, num_str y, bool release) {
#if defined (USED_STDLIB_MEMALLOC)		
    uint8_t *buf = calloc(x.len + y.len, sizeof(uint8_t));
#else 	
	uint8_t *buf = pvTuyaPortMalloc((x.len + y.len) * sizeof(uint8_t));
	memset(buf, 0x00, (x.len + y.len) * sizeof(uint8_t));
#endif 
	
    memcpy(buf, x.buf, x.len * sizeof(uint8_t));
    memcpy(buf + x.len, y.buf, y.len * sizeof(uint8_t));
    num_str ret = {.buf = buf, .len = x.len + y.len};
    if (release) {
        release_str(x);
        release_str(y);
    }
    return ret;
}
