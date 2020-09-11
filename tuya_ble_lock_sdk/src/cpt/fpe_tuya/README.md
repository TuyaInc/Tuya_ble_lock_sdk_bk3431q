# libfpe
给门锁的fpe算法的参考实现

# 说明

## 调用方法
include ff1.h文件，调用decrypt函数，可以参考main.c
```
 num_str decrypt(byte_str key, byte_str tweak, num_str cipher, ff1_context ctx) ;
```
* key：密钥，可以使用16，24和32字节的密钥
* tweak：由于我们是门锁密码，没法tweak，加密和解密方可以约定一个值，也可以为空的
* cipher：为用户输入的密码，最长不能超过14个字节，因为我们把大整数去掉了，所以，如果超过14外字节的密码，会导致溢出，从而导致结果不对
* 返回类型：num_str，里面包含一个uint32_t类型的数组，即解密出来的值

## 配置
* 配置文件 fpe_config.h
* FPE_RADIX 统一为10,不用修改
* FPE_LOG_PRINT 如果定义了这个宏,表示打日志,否则不打日志

## AES
  AES用了mbed-crypto库,各自可以换成自己的库
