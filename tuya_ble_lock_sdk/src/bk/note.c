功耗：
低压模式：2uA左右
sleep模式：6uA左右


IRQ: Interrupt Request
FIQ: Fast Interrupt Request


官方源码中，在400e3地址写入Mac地址，后面写了16字节其他数据，该页无其他数据

HZ32000 —— 32K 时钟源切换开关








编译冲突改动的位置:
peripheral_stack_symbol.txt
#0x00017e81 T __aeabi_memset
#0x00017e81 T __aeabi_memset4
#0x00017e81 T __aeabi_memset8
#0x00017e91 T __aeabi_memclr
#0x00017e91 T __aeabi_memclr4
#0x00017e91 T __aeabi_memclr8
#0x00017e95 T _memset$wrapper




