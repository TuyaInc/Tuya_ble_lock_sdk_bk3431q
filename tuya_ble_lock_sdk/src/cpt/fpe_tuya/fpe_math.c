//
// Created by qianh on 2019/7/29.
//

#include "fpe_math.h"
#include "fpe_config.h"


uint32_t fpe_int_pow(uint32_t base, uint32_t exp) {
    uint32_t r = 1;
    for (int i = 0; i < exp; i++) {
        // r *= n;
        r = (r * base);
    }
    return r;
}

uint32_t fpe_mod(int32_t x, uint32_t m) {
    int32_t base = x % m;
    if (base < 0) {
        base += m;
    }
    return base;
}

uint32_t fpe_mod_pow(uint32_t n, uint32_t exp, uint32_t mod) {
    uint32_t r = 1;
    for (int i = 0; i < exp; i++) {
        r = (r * n) % mod;
    }
    return r;
}

uint32_t fpe_mod_pow_radix(byte_str x, uint32_t m) {
    uint32_t mod = fpe_int_pow(FPE_RADIX, m);
    uint32_t tmp = 0;
    for (int i = 0; i < x.len; i++) {
        uint32_t d = (uint32_t) x.buf[i];
        tmp += (((d % mod) * fpe_mod_pow(16 % mod, 2 * (x.len - 1 - i), mod)) % mod);
    }
    return tmp % mod;
}


uint32_t ceil2(uint32_t numerator, uint32_t denominator) {
    if (numerator <= denominator) {
        return 1;
    }
    if (numerator % denominator == 0) {
        return numerator / denominator;
    }
    return numerator / denominator + 1;
}
