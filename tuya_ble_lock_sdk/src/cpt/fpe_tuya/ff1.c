#include "ff1.h"
#include "fpe_math.h"
#include "fpe_cipher.h"
#include <stdio.h>

uint32_t calcb(uint32_t v) {
    //  int b = ceil(ceil(v * log2(ctx.radix)) / 8.0);
    return ceil2(ceil2(v * 332192, 100000), 8);
}

uint32_t calcd(uint32_t b) {
    return 4 * ceil2(b, 4) + 4;
}

uint32_t calcc(byte_str num_B, byte_str y, uint32_t m) {
    uint32_t mod = fpe_int_pow(FPE_RADIX, m);
    uint32_t modb = fpe_mod_pow_radix(num_B, m);
    uint32_t mody = fpe_mod_pow_radix(y, m);
    if (modb > mody) {
        return (modb - mody) % mod;
    } else if (mody > modb) {
        return (mod + modb - mody) % mod;
    }
    return 0;
}

num_str decrypt(byte_str key, byte_str tweak, num_str cipher, ff1_context ctx) {
    FPE_PRINT_STR(tweak, -1, "tweak", ctx);
    FPE_PRINT_STR(cipher, -1, "cipher", ctx);
    num_str empty_num_str = {.buf=NULL, .len=0};
    if (key.len != 16 && key.len != 24 && key.len != 32) {
        ctx.ret = INVALID_KEY_LENGTH;
        return empty_num_str;
    }
    if (tweak.len > ctx.max_tweak_len) {
        ctx.ret = INVALID_TWEAK_LENGTH;
        return empty_num_str;
    }
    if (cipher.len < INPUT_MIN_LENGTH || cipher.len > INPUT_MAX_LENGTH) {
        ctx.ret = INVALID_INPUT_LENGTH;
        return empty_num_str;
    }
    uint32_t n = cipher.len;
    uint32_t t = tweak.len;
    uint32_t u = n / 2;
    uint32_t v = n - u;
    num_str num_str_A = num_str_copy_of_range(cipher, 0, u);
    FPE_PRINT_STR(num_str_A, -1, "A", ctx);
    num_str num_str_B = num_str_copy_of_range(cipher, u, n);
    FPE_PRINT_STR(num_str_B, -1, "B", ctx);
    uint32_t b = calcb(v);
    uint32_t d = calcd(b);
    byte_str tbr = to_byte_str(FPE_RADIX, 3);
    byte_str fbn = to_byte_str(n, 4);
    byte_str fbt = to_byte_str(t, 4);
    uint8_t arrP[] = {0x01, 0x02, 0x01, tbr.buf[0], tbr.buf[1], tbr.buf[2], 0x0a, u % 256, fbn.buf[0],
                      fbn.buf[1], fbn.buf[2], fbn.buf[3],
                      fbt.buf[0], fbt.buf[1], fbt.buf[2], fbt.buf[3]};
    release_str(tbr);
    release_str(fbn);
    release_str(fbt);
    byte_str byte_str_P = create_byte_str(arrP, sizeof(arrP));
    FPE_PRINT_STR(byte_str_P, -1, "P", ctx);
    byte_str tmp = to_byte_str(0, fpe_mod(-t - b - 1, 16));
    byte_str byte_str_Q_start = byte_str_concatenate(tweak, tmp, false);
    release_str(tmp);
    for (int i = 9; i >= 0; i--) {
        tmp = to_byte_str(i, 1);
        byte_str byte_str_Q = byte_str_concatenate(byte_str_Q_start, tmp, false);
        release_str(tmp);
        byte_str x = num_str_to_num(num_str_A, FPE_RADIX);
        byte_str_Q = byte_str_concatenate(byte_str_Q, to_byte_str_with_bignum(x, b), true);
        FPE_PRINT_STR(byte_str_Q, i, "Q", ctx);
        release_str(x);
        tmp = byte_str_concatenate(byte_str_P, byte_str_Q, false);
        release_str(byte_str_Q);
        byte_str byte_str_R = prf(key, tmp);
        FPE_PRINT_STR(byte_str_R, i, "R", ctx);
        release_str(tmp);
        byte_str byte_str_S = byte_str_R;
        byte_str_S = byte_str_copy_of_range(byte_str_S, 0, d, true);
        FPE_PRINT_STR(byte_str_S, i, "S", ctx);
        byte_str y = byte_str_to_num(byte_str_S);
        uint32_t m = i % 2 == 0 ? u : v;
        byte_str num_B = num_str_to_num(num_str_B, FPE_RADIX);
        release_str(num_str_B);
        uint32_t c = calcc(num_B, y, m);
#ifdef FPE_LOG_PRINT
        ctx.sys_printf("Round #%d,%s is:\t", i, "c");
#endif
        release_str(num_B);
        release_str(y);
        num_str num_str_C = num_to_num_str(c, FPE_RADIX, m);

        num_str_B = num_str_A;
        num_str_A = num_str_C;
    }
    release_str(byte_str_P);
    release_str(byte_str_Q_start);
    num_str ret = num_str_concatenate(num_str_A, num_str_B, true);
    FPE_PRINT_STR(ret, -1, "Result", ctx);
    return ret;
}
