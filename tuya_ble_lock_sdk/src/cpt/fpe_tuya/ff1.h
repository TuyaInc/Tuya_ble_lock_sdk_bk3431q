#ifndef FPE_FF1_H
#define FPE_FF1_H

#include <stdlib.h>
#include "fpe_str.h"
#include "ff1_common.h"

#define INPUT_MIN_LENGTH 2
#define INPUT_MAX_LENGTH 18


#define OK 0
#define INVALID_KEY_LENGTH 1
#define INVALID_INPUT_LENGTH 2
#define INVALID_TWEAK_LENGTH 3


num_str decrypt(byte_str key, byte_str tweak, num_str cipher, ff1_context ctx);

#endif //FPE_FF1_H

