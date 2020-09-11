//
// Created by qianh on 2019/8/21.
//

#ifndef FPE_FF1_COMMON_H
#define FPE_FF1_COMMON_H

#include <stdint.h>
#include "fpe_config.h"

#ifdef FPE_LOG_PRINT

typedef int (*printf_fn)(const char *format, ...);

#endif //FPE_LOG_PRINT

typedef struct {
    uint32_t max_tweak_len;
    int ret;
#ifdef FPE_LOG_PRINT
    printf_fn sys_printf;
#endif //FPE_LOG_PRINT
} ff1_context;

#endif //FPE_FF1_COMMON_H
