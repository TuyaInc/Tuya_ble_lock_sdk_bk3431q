////
//// Created by qianh on 2019/7/29.
////
//#include <stdlib.h>
//#include <stdio.h>
//#include "ff1.h"
//#include "fpe_str.h"
//#include "nrf_log.h"
//
///*## 调用方法
//include ff1.h文件，调用decrypt函数，可以参考main.c
//```
// num_str decrypt(byte_str key, byte_str tweak, num_str cipher, ff1_context ctx) ;
//```
//* key：密钥，可以使用16，24和32字节的密钥
//* tweak：由于我们是门锁密码，没法tweak，加密和解密方可以约定一个值，也可以为空的
//* cipher：为用户输入的密码，最长不能超过14个字节，因为我们把大整数去掉了，所以，如果超过14外字节的密码，会导致溢出，从而导致结果不对
//* 返回类型：num_str，里面包含一个uint32_t类型的数组，即解密出来的值
//
//## 配置
//* 配置文件 fpe_config.h
//* FPE_RADIX 统一为10,不用修改
//* FPE_LOG_PRINT 如果定义了这个宏,表示打日志,否则不打日志
//
//## AES
//  AES用了mbed-crypto库,各自可以换成自己的库
//*/
//
//int fpe_example(void) {
//    uint8_t key_buf[] = {112, 70, 35, 189, 161, 86, 161, 229, 71, 53, 98, 114, 185, 114, 138, 115};
//    byte_str key = {.buf=key_buf, .len=16};
//    byte_str tweak = create_byte_str("", 0);
//    //明文[1, 8, 5, 8, 1, 2, 3, 5, 6, 7, 6, 3]
//    uint8_t cipher_buf[] = {9, 8, 4, 0, 8, 4, 6, 6, 1, 3, 4, 0};
//    num_str cipher = {.buf=cipher_buf, .len=12};
//
//    ff1_context ctx;
//    ctx.ret = 0;
//    ctx.max_tweak_len = 20;
//#ifdef FPE_LOG_PRINT
//    ctx.sys_printf = printf;
//#endif
//    FPE_PRINT_STR(cipher, -1, "Cipher:", ctx);
//    num_str result = decrypt(key, tweak, cipher, ctx);
//    if (result.len <= 0) {
//        //printf("error");
//		NRF_LOG_INFO("error");
//        release_str(key);
//        return -1;
//    }
//	/*
//    printf("decrypt result:\t");
//    for (int i = 0; i < result.len; i++) {
//        printf("%d", result.buf[i]);
//    }
//    printf("\n");
//	*/
//	
//	NRF_LOG_INFO("decrypt result->[");
//	for (int i = 0; i < result.len; i++) {
//        NRF_LOG_RAW_INFO("%d", result.buf[i]);
//    }
//	NRF_LOG_RAW_INFO("]\r\n");
//	
//    release_str(result);
//    release_str(tweak);
//    return 0;
//}
//