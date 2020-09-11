//
// Created by qianh on 2019/7/29.
//

#ifndef FPE_FPE_MATH_H
#define FPE_FPE_MATH_H

#include "fpe_str.h"
#include <stdint.h>

uint32_t fpe_int_pow(uint32_t base, uint32_t exp);

uint32_t fpe_mod(int32_t x, uint32_t m);

uint32_t fpe_mod_pow_radix(byte_str x, uint32_t m);


uint32_t ceil2(uint32_t numerator, uint32_t denominator);

#endif //FPE_FPE_MATH_H
