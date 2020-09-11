#include "rwip_config.h" // RW SW configuration
#include "arch.h"      // architectural platform definitions
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include "boot.h"      // boot definition
#include "rwip.h"      // RW SW initialization
#include "syscntl.h"   // System control initialization
#include "emi.h"       // EMI initialization
#include "intc.h"      // Interrupt initialization
#include "timer.h"     // TIMER initialization
#include "icu.h"
#include "flash.h"
#include "uart.h"      	// UART initialization
#include "flash.h"     // Flash initialization

#if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
#include "rf.h"        // RF initialization
#endif // BLE_EMB_PRESENT || BT_EMB_PRESENT
#if (BLE_APP_PRESENT)
#include "app.h"       // application functions
#endif // BLE_APP_PRESENT

#include "nvds.h"         // NVDS definitions
#include "reg_assert_mgr.h"
#include "BK3435_reg.h"
#include "RomCallFlash.h"
#include "gpio.h"
#include "pwm.h"
#include "audio.h"
#include "app_task.h"
#include "ir.h"
#include "oads.h"
#include "prf.h"
#include "wdt.h"
#include "user_config.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION
 */
static void Stack_Integrity_Check(void);
extern void code_sanity_check(void);
extern void rw_fcc_enter(void);
extern void rw_pn9_enter(void);
extern void uart_stack_register(void *cb);
void rwip_eif_api_init(void);

/*********************************************************************
 * VARIABLES
 */
// Creation of uart external interface api
struct rwip_eif_api uart_api;
extern struct rom_env_tag rom_env;
extern uint8_t system_mode;




/*********************************************************
FN: 
*/
#if ((UART_PRINTF_EN) &&(UART_DRIVER))
void assert_err(const char *condition, const char * file, int line)
{
	BK_PRINTF("%s,condition %s,file %s,line = %d",__func__,condition,file,line);
}

void assert_param(int param0, int param1, const char * file, int line)
{
	BK_PRINTF("%s,param0 = %d,param1 = %d,file = %s,line = %d",__func__,param0,param1,file,line);
}

void assert_warn(int param0, int param1, const char * file, int line)
{
	BK_PRINTF("%s,param0 = %d,param1 = %d,file = %s,line = %d",__func__,param0,param1,file,line);
}

void dump_data(uint8_t* data, uint16_t length)
{
	BK_PRINTF("%s,data = %d,length = %d,file = %s,line = %d",__func__,data,length);
}
#else
void assert_err(const char *condition, const char * file, int line)
{
}

void assert_param(int param0, int param1, const char * file, int line)
{
}

void assert_warn(int param0, int param1, const char * file, int line)
{
}

void dump_data(uint8_t* data, uint16_t length)
{
}
#endif //UART_PRINTF_EN

/*********************************************************
FN: 
*/
void platform_reset(uint32_t error)
{
	BK_PRINTF("error = %x", error);

	// Disable interrupts
	GLOBAL_INT_STOP();

#if UART_PRINTF_EN
	// Wait UART transfer finished
	uart_finish_transfers();
#endif //UART_PRINTF_EN

	if(error == RESET_AND_LOAD_FW || error == RESET_TO_ROM)
	{
		// Not yet supported
	} else {
		//Restart FW
		//pReset = (void * )(0x0);
		//pReset();
		wdt_enable(10);
		while(1);
	}
}

/*********************************************************
FN: 
*/
uint8_t rc_sleep_flag = 0;
void power_up_rc_cali_path(void)
{
#if(HZ32000==0)    
    icu_init();
	REG_AHB0_ICU_INT_ENABLE 	= 0x00008000;        
	REG_AHB0_ICU_IRQ_ENABLE 	= 0x00000002;  
	REG_AHB0_ICU_WDTCLKCON 		= 0x0;
	REG_AHB0_ICU_BLECLKCON  	= 0x0;//open BLE clock
	REG_AHB10_RW_DEEPSLTIME		= 0x10000;//sleep time
	REG_AHB10_RW_ENBPRESET 		= (0x40<<RW_ENBPRESET_TWEXT_bit)|(0x40<<RW_ENBPRESET_TWOSC_bit)|(0x40<<RW_ENBPRESET_TWRW_bit);	
    REG_AHB10_RW_DEEPSLCNTL		= 0x00000007;//BLE sleep
	GLOBAL_INT_START();

	Delay_ms(20);  

	mHWreg_Assign_XVR_Regsiter(00, 0xC3A03210);
	mHWreg_Assign_XVR_Regsiter(01, 0x8295C100);
	mHWreg_Assign_XVR_Regsiter(02, 0x2F42AF00);
	mHWreg_Assign_XVR_Regsiter(03, 0x6A5C9C62);
	mHWreg_Assign_XVR_Regsiter(04, 0x5F9156B3);
	mHWreg_Assign_XVR_Regsiter(05, 0x48285500);
	mHWreg_Assign_XVR_Regsiter(06, 0x88D68C00);
	mHWreg_Assign_XVR_Regsiter(07, 0x00023FC3);
	mHWreg_Assign_XVR_Regsiter(08, 0x2FB84005);
	mHWreg_Assign_XVR_Regsiter(09, 0x70203C08);
	mHWreg_Assign_XVR_Regsiter(0A, 0x1607EC25);
	mHWreg_Assign_XVR_Regsiter(0B, 0x61FD240C);
	mHWreg_Assign_XVR_Regsiter(0C, 0x80009008);
	mHWreg_Assign_XVR_Regsiter(0D, 0x84413F23);
	mHWreg_Assign_XVR_Regsiter(0E, 0x00309350);
	mHWreg_Assign_XVR_Regsiter(0F, 0x3126E978);
	Delay_ms(3);  //3ms

	BK_PRINTF("RW sleep");
	
	rc_sleep_flag = 1;
	REG_AHB0_ICU_DIGITAL_PWD |= 0x80;
	REG_AHB0_ICU_CPU_STATUS  =  0x661;
	REG_AHB0_ICU_CORECLKCON = 0x0 ;  
	REG_AHB0_ICU_DIGITAL_PWD |= SET_CLK48M_PWD;
	REG_AHB0_ICU_DIGITAL_PWD |= SET_CB_BAIS_PWD;
	REG_AHB0_ICU_DIGITAL_PWD |=SET_HP_LDO_PWD;
	uint32_t temp_reg = REG_AHB0_ICU_DIGITAL_PWD | SET_CLK16M_PWD;
	REG_AHB0_ICU_CLKSRCSEL  = 0x1f0;	
	REG_AHB0_ICU_DIGITAL_PWD = temp_reg;					  
	REG_AHB0_ICU_CORECLKCON  = 0x1; // Power down MCU
		
	BK_PRINTF("RW Wakeup");
	
	mHWreg_Assign_XVR_Regsiter(09, 0x74203C08);
	Delay_ms(20);
#endif
}

/*********************************************************
FN: 
*/
void rw_dut_enter(void)
{
	while(1)
	{
		// schedule all pending events
		rwip_schedule();
	}
}

/*********************************************************
FN: 
*/
void rw_app_enter(void)
{
#if SYSTEM_SLEEP
	uint8_t sleep_type = 0;
#endif

    //启动看门狗
    //0x3FFF*250us = 4 095 750us = 4s
    //0x7FFF*250us = 8 191 750us = 8s
    wdt_enable(WATCH_DOG_COUNT);
    
	while(1)
	{
		//schedule all pending events
		rwip_schedule();
        {
            tuya_ble_main_tasks_exec();
            bk_ble_notify_handler();
        }
        bk_wdt_feed();
        
		// Checks for sleep have to be done with interrupt disabled
		GLOBAL_INT_DISABLE();

#if SYSTEM_SLEEP
        if(g_system_sleep)
        {
            // Check if the processor clock can be gated
            sleep_type = rwip_sleep();
            if((sleep_type & RW_MCU_DEEP_SLEEP) == RW_MCU_DEEP_SLEEP)
            {
                // 1:idel  0:reduce voltage
                if(icu_get_sleep_mode()) {
                    cpu_idle_sleep();
                } else {
                    cpu_reduce_voltage_sleep();
                }
            }
            else if((sleep_type & RW_MCU_IDLE_SLEEP) == RW_MCU_IDLE_SLEEP)
            {
                cpu_idle_sleep();
            }
        }
#endif
		Stack_Integrity_Check();
		GLOBAL_INT_RESTORE();
	}
}

/*********************************************************
FN: 
*/
void sys_mode_init(void)
{
	system_mode |= RW_NO_MODE;
}

/*********************************************************
FN: This function is called right after the booting process has completed.
*/
void rw_main(void)
{
	/* ********** 平台初始化 ********** */
	//随机数处理初始化 ？？？
	srand(1);
    //系统模式初始化
	sys_mode_init();
	//系统休眠标识初始化
	system_sleep_init();
	//交换内存接口初始化（exchange memory interface）
	emi_init();
	//定时器初始化，未使用
	timer_init();

    //串口api注册 ？？？
	rwip_eif_api_init();

	//中断控制器初始化
	intc_init();
	//串口组件初始化
    bk_uart1_init();
    bk_uart2_init();
    bk_log_init();
    //串口api注册 ？？？
    uart_stack_register(uart_printf);
    uart_stack_register(uart2_printf); //干嘛用的？

    //Flash初始化，写保护
	flash_advance_init();
    //蓝牙设备地址初始化
    bk_init_bt_mac_addr();

	//nvds组件初始化
	struct nvds_env_tag env;
	env.flash_read = &flash_read;
	env.flash_write = &flash_write;
	env.flash_erase = &flash_erase;
	nvds_init(env);

    //ROM env初始化 ？？？
	rom_env_init(&rom_env);
//    power_up_rc_cali_path();


	/* ********** RW SW stack 初始化 ********** */
	rwip_init(0); // main_road -2
	icu_init();
    //Flash初始化
	flash_init();
    
    //
	REG_AHB0_ICU_INT_ENABLE |= (0x01 << 15); //BLE INT
	REG_AHB0_ICU_IRQ_ENABLE = 0x03;
    
	//启动中断处理
	GLOBAL_INT_START();


	/* ********** 程序入口选择 ********** */
	if((system_mode & RW_DUT_MODE) == RW_DUT_MODE) {
		BK_PRINTF("dut mode start");
		rw_dut_enter();
	} else if((system_mode & RW_FCC_MODE) == RW_FCC_MODE) {
		BK_PRINTF("fcc mode start");
		rw_fcc_enter();
	} else if((system_mode & RW_PN9_MODE) == RW_PN9_MODE) {
		BK_PRINTF("pn9 mode start");
		rw_pn9_enter();
	} else {
		BK_PRINTF("normal mode start");
		rw_app_enter();
	}
}

/*********************************************************
FN: 
*/
void rwip_eif_api_init(void)
{
	uart_api.read = &uart_read;
	uart_api.write = &uart_write;
	uart_api.flow_on = &uart_flow_on;
	uart_api.flow_off = &uart_flow_off;
}

/*********************************************************
FN: 
*/
const struct rwip_eif_api* rwip_eif_get(uint8_t type)
{
	const struct rwip_eif_api* ret = NULL;
	switch(type)
	{
        case RWIP_EIF_AHI: {
            ret = &uart_api;
        } break;
        
#if (BLE_EMB_PRESENT) || (BT_EMB_PRESENT)
        case RWIP_EIF_HCIC: {
            ret = &uart_api;
        } break;
#elif !(BLE_EMB_PRESENT) || !(BT_EMB_PRESENT)
        case RWIP_EIF_HCIH: {
            ret = &uart_api;
        } break;
#endif
        default: {
            ASSERT_INFO(0, type, 0);
        } break;
	}
	return ret;
}

/*********************************************************
FN: 
*/
static void Stack_Integrity_Check(void)
{
	if ((REG_PL_RD(STACK_BASE_UNUSED)!= BOOT_PATTERN_UNUSED)) {
		while(1) {
			uart_putchar("Stack_Integrity_Check STACK_BASE_UNUSED fail!\r\n");
		}
	}

	if ((REG_PL_RD(STACK_BASE_SVC)!= BOOT_PATTERN_SVC)) {
		while(1) {
			uart_putchar("Stack_Integrity_Check STACK_BASE_SVC fail!\r\n");
		}
	}

	if ((REG_PL_RD(STACK_BASE_FIQ)!= BOOT_PATTERN_FIQ)) {
		while(1) {
			uart_putchar("Stack_Integrity_Check STACK_BASE_FIQ fail!\r\n");
		}
	}

	if ((REG_PL_RD(STACK_BASE_IRQ)!= BOOT_PATTERN_IRQ)) {
		while(1) {
			uart_putchar("Stack_Integrity_Check STACK_BASE_IRQ fail!\r\n");
		}
	}
}

/*********************************************************
FN: 
*/
void rom_env_init(struct rom_env_tag *api)
{
	memset(&rom_env,0,sizeof(struct rom_env_tag));
	rom_env.prf_get_id_from_task = prf_get_id_from_task;
	rom_env.prf_get_task_from_id = prf_get_task_from_id;
	rom_env.prf_init = prf_init;
	rom_env.prf_create = prf_create;
	rom_env.prf_cleanup = prf_cleanup;
	rom_env.prf_add_profile = prf_add_profile;
	rom_env.rwble_hl_reset = rwble_hl_reset;
	rom_env.rwip_reset = rwip_reset;
#if SYSTEM_SLEEP
	rom_env.rwip_prevent_sleep_set = rwip_prevent_sleep_set;
	rom_env.rwip_prevent_sleep_clear = rwip_prevent_sleep_clear;
	rom_env.rwip_sleep_lpcycles_2_us = rwip_sleep_lpcycles_2_us;
	rom_env.rwip_us_2_lpcycles = rwip_us_2_lpcycles;
	rom_env.rwip_wakeup_delay_set = rwip_wakeup_delay_set;
#endif
	rom_env.platform_reset = platform_reset;
	rom_env.assert_err = assert_err;
	rom_env.assert_param = assert_param;
	rom_env.Read_Uart_Buf = Read_Uart_Buf;
	rom_env.uart_clear_rxfifo = uart_clear_rxfifo;
}





