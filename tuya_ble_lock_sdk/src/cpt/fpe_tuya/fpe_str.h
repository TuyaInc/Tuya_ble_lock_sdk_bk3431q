//
// Created by qianh on 2019/7/28.
//

#ifndef FPE_FPE_STR_H
#define FPE_FPE_STR_H

#include <stdbool.h>
#include <stdint.h>
#include "ff1_common.h"

typedef struct {
    uint8_t *buf;
    uint32_t len;
} byte_str ,num_str;

byte_str create_byte_str(uint8_t *data, uint32_t len);

void release_str(byte_str s);

byte_str byte_str_copy_of_range(byte_str source, uint32_t start, uint32_t end, bool release);

byte_str to_byte_str(uint32_t x, uint32_t s);

byte_str to_byte_str_with_bignum(byte_str x, uint32_t s);

byte_str byte_str_concatenate(byte_str x, byte_str y, bool release);

byte_str byte_str_to_num(byte_str str);

#ifdef FPE_LOG_PRINT
#define  FPE_PRINT_STR(s, round, desc, ctx) \
    ctx.sys_printf("Round #%d,%s is:\t[", round, desc);\
    for (int i = 0; i < s.len; i++) {\
        ctx.sys_printf("%d,", s.buf[i]);\
    }\
    ctx.sys_printf("]\n");
#else
#define FPE_PRINT_STR(s, round, desc, ctx)
#endif

num_str num_str_copy_of_range(num_str source, uint32_t start, uint32_t end);

byte_str num_str_to_num(num_str str, uint32_t radix);

num_str num_to_num_str(uint32_t num, uint32_t radix, uint32_t m);

num_str num_str_concatenate(num_str x, num_str y, bool release);


#endif //FPE_FPE_STR_H
