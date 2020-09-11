#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//#include "port_mem.h"
#include "msg_pub.h"
//#include "os_port.h"
#include "msg.h"
#include "driver_icu.h"
#include "timer.h"
#include "driver_uart0.h"

#define UART_PRINTF	uart_printf
int uart_printf(const char *fmt,...);


volatile MESSAGE_ENTITY_T msg_entity;

extern void *ke_malloc(uint32_t size, uint8_t type);
void ke_free(void* mem_ptr);


void msg_init(void)
{
    uint8 *temp;
    uint32 *magic_hdr;
    uint32 *magic_tail;

    MSG_PRT("%s\r\n", __FUNCTION__);
//	memory_usage_show();
    memset((void *)&msg_entity, 0, sizeof(MESSAGE_ENTITY_T));
    
    temp = (uint8 *)ke_malloc((sizeof(MSG_T) * MSG_COUNT + MSG_SECURITY_BOUNDARY_NUM + MSG_SECURITY_BOUNDARY_NUM),3);
    if (temp == NULL)
    {
    	UART_PRINTF("msg_init: malloc failed!\r\n");
        return;
    }
    memset(temp, 0, (sizeof(MSG_T) * MSG_COUNT + MSG_SECURITY_BOUNDARY_NUM + MSG_SECURITY_BOUNDARY_NUM));
    msg_entity.borderline = (MSG_T *)temp;
    
    magic_hdr = (uint32 *)temp;
    msg_entity.pool = (MSG_T *)(temp + MSG_SECURITY_BOUNDARY_NUM);
    magic_tail = (uint32 *)(temp + sizeof(MSG_T) * MSG_COUNT + MSG_SECURITY_BOUNDARY_NUM);
    
    msg_entity.count  = MSG_COUNT;
    msg_entity.out_id = 0;
    msg_entity.in_id  = 0;
    msg_entity.status = MSG_POOL_INIT;
    
    *magic_hdr  = (uint32)MSG_HDR_MAGIC_WORD;
    *magic_tail = (uint32)MSG_TAIL_MAGIC_WORD;
//	memory_usage_show();
	
    #ifdef DELAY_MSG_PUT
//    jtask_init(&msg_entity.msg_task, J_TASK_TIMEOUT);
    if(UWE_NOMEM == jtask_init((os_timer_h *)&msg_entity.msg_task, J_TASK_TIMEOUT))
    {
        UART_PRINTF("msg_task_init_exception\r\n");
    }
    #endif
//    UART_PRINTF("msg_init: msg_entity.borderline = 0x%p\r\n", msg_entity.borderline);
//    UART_PRINTF("msg_init: msg_entity.pool = 0x%p\r\n", msg_entity.pool);
//    UART_PRINTF("msg_init: msg_entity.count = 0x%lx\r\n", msg_entity.count);
//    UART_PRINTF("msg_init: msg_entity.status = 0x%lx\r\n", msg_entity.status);
//    UART_PRINTF("msg_init: msg_entity.out_id = 0x%lx\r\n", msg_entity.out_id);
//    UART_PRINTF("msg_init: msg_entity.in_id = 0x%lx\r\n", msg_entity.in_id);
//    UART_PRINTF("msg_init: msg_entity.msg_task = 0x%p\r\n", msg_entity.msg_task);
//    UART_PRINTF("msg_init: msg_entity.arg.id = 0x%lx\r\n", msg_entity.arg.id);
//    UART_PRINTF("msg_init: msg_entity.arg.hdl = 0x%p\r\n", msg_entity.arg.hdl);
//    UART_PRINTF("msg_init: msg_entity.arg.arg = 0x%lx\r\n", msg_entity.arg.arg);
//	UART_PRINTF("msg_init end \r\n");
} 
  
void msg_uninit(void)
{
    msg_clear_all();
    
    MSG_PRT("msg_uninit\r\n");
    ke_free(msg_entity.borderline);
    msg_entity.borderline = 0;
    msg_entity.pool       = 0;
    
    msg_entity.status     = MSG_POOL_UNINIT;
    
    #ifdef DELAY_MSG_PUT
    jtask_stop(msg_entity.msg_task);
    jtask_uninit(msg_entity.msg_task);
    msg_entity.msg_task = 0;
    #endif
}

void msg_put(uint32 id)
{
    MSG_T msg;

    MSG_PRT("msg_put\r\n");
    msg.id  = id;
    msg.arg = 0;
    msg.hdl = 0;

    msg_lush_put(&msg);
}

int msg_lush_put(MSG_T *msg_ptr)
{
    MSG_T *element_ptr;
//	uint32 interrupts_info;

    MSG_PRT("%s\r\n", __FUNCTION__);
    if(NULL == msg_ptr)
    {
        MSG_PRT("msg_lush_put_null_ptr\r\n");
    }
    
    //MSG_PRT("msg:%p\r\n", msg_ptr->id);
    
//    VICMR_disable_interrupts(&interrupts_info);
    REG_ICU_INT_GLOBAL_ENABLE = 0;
    /* step 0: check if the pool is full*/
    if(MSG_POOL_FULL == msg_entity.status)
    {
//        VICMR_restore_interrupts(interrupts_info);
        static uint8 cnt=0;
    REG_ICU_INT_GLOBAL_ENABLE = ICU_INT_GLOBAL_ENABLE_IRQ_SET | ICU_INT_GLOBAL_ENABLE_FIQ_SET;
		if(cnt < 10)
		{
			cnt++;
        	MSG_PRT("f\r\n"); 
		}
		
        return MSG_FAILURE_RET;
    }
    
    /* step 1: put the msg into container*/
    element_ptr = &(msg_entity.pool[msg_entity.in_id & (MSG_COUNT - 1)]);
    memcpy(element_ptr, msg_ptr, sizeof(MSG_T));
    
    /* step 3: update the variable*/
    msg_entity.in_id = (msg_entity.in_id + 1) % MSG_COUNT;

    if((msg_entity.in_id & (MSG_COUNT - 1)) == (msg_entity.out_id & (MSG_COUNT - 1)))
    {
        msg_entity.status = MSG_POOL_FULL;
    }
    else
    {
        msg_entity.status = MSG_POOL_ADSUM;
    }
    
//    VICMR_restore_interrupts(interrupts_info);
    REG_ICU_INT_GLOBAL_ENABLE = ICU_INT_GLOBAL_ENABLE_IRQ_SET | ICU_INT_GLOBAL_ENABLE_FIQ_SET;

    return MSG_SUCCESS_RET;
}

#ifdef DELAY_MSG_PUT
void msg_put_func(void *id) {
    MSG_PRT("%s\r\n", __FUNCTION__);
    msg_put(*(uint32 *)id);
}

int msg_delay_put(uint32 os_tick_count, MSG_T *msg_ptr)
{
    uint32 msec;
    
    MSG_PRT("%s\r\n", __FUNCTION__);
    if(jtask_is_pending(msg_entity.msg_task))
    {
        return MSG_FAILURE_RET;
    }

    msec = os_tick_count * OS_TICK_MSEC;
    memcpy((char *)&msg_entity.arg, msg_ptr, sizeof(MSG_T));
    jtask_schedule(msg_entity.msg_task, 
    		        msec,
                    msg_put_func, 
                    (void *)&msg_entity.arg);

    return MSG_SUCCESS_RET;
}

#endif

int msg_priority_put(MSG_T *msg_ptr)
{
    MSG_T *element_ptr;
//	uint32 interrupts_info;
    
    MSG_PRT("%s\r\n", __FUNCTION__);
//    VICMR_disable_interrupts(&interrupts_info);
    REG_ICU_INT_GLOBAL_ENABLE = 0;
    
    /* step 0: check if the pool is full*/
    if(MSG_POOL_FULL == msg_entity.status)
    {
//        VICMR_restore_interrupts(interrupts_info);
    REG_ICU_INT_GLOBAL_ENABLE = ICU_INT_GLOBAL_ENABLE_IRQ_SET | ICU_INT_GLOBAL_ENABLE_FIQ_SET;
        
        MSG_PRT("pmsg_pool_full\r\n");
        
        return MSG_FAILURE_RET;
    }

    /* step 1: add msg into pool*/
    msg_entity.out_id -= 1;    
    element_ptr = &(msg_entity.pool[msg_entity.out_id & (MSG_COUNT - 1)]);
    memcpy(element_ptr, msg_ptr, sizeof(MSG_T));
    
    /* step 2: update varaible*/
    if((msg_entity.in_id & (MSG_COUNT - 1)) == (msg_entity.out_id & (MSG_COUNT - 1)))
    {
        msg_entity.status = MSG_POOL_FULL;
    }
    else
    {
        msg_entity.status = MSG_POOL_ADSUM;
    }
    
//    VICMR_restore_interrupts(interrupts_info);
    REG_ICU_INT_GLOBAL_ENABLE = ICU_INT_GLOBAL_ENABLE_IRQ_SET | ICU_INT_GLOBAL_ENABLE_FIQ_SET;

    return MSG_SUCCESS_RET;
}

RAM_CODE int msg_get(MSG_T *msg_ptr)
{
    MSG_T *element_ptr;
//	uint32 interrupts_info;
    
    MSG_PRT("%s\r\n", __FUNCTION__);
//    VICMR_disable_interrupts(&interrupts_info);
    REG_ICU_INT_GLOBAL_ENABLE = 0;
    /* step 0: check if the pool is empty*/
    if ((MSG_POOL_EMPTY == msg_entity.status) || (MSG_POOL_INIT == msg_entity.status) || (MSG_POOL_UNINIT == msg_entity.status))
    {
//        VICMR_restore_interrupts(interrupts_info);
    REG_ICU_INT_GLOBAL_ENABLE = ICU_INT_GLOBAL_ENABLE_IRQ_SET | ICU_INT_GLOBAL_ENABLE_FIQ_SET;

        return MSG_FAILURE_RET;
    }

    /* step 1: put the msg into container*/
    element_ptr = &(msg_entity.pool[msg_entity.out_id & (MSG_COUNT - 1)]);
    memcpy(msg_ptr, element_ptr, sizeof(MSG_T));
    
    /* step 3: update the variable*/
    msg_entity.out_id = (msg_entity.out_id + 1) % MSG_COUNT;

    if((msg_entity.in_id & (MSG_COUNT - 1)) == (msg_entity.out_id & (MSG_COUNT - 1)))
    {
        msg_entity.status = MSG_POOL_EMPTY;
    }
    
//    VICMR_restore_interrupts(interrupts_info);
    REG_ICU_INT_GLOBAL_ENABLE = ICU_INT_GLOBAL_ENABLE_IRQ_SET | ICU_INT_GLOBAL_ENABLE_FIQ_SET;

    return MSG_SUCCESS_RET;
}

uint32 msg_get_pool_status(void)
{
    MSG_PRT("%s\r\n", __FUNCTION__);
    return msg_entity.status;
}

void msg_clear_all(void)
{
//	uint32 interrupts_info;

    MSG_PRT("msg_clear_all\r\n");
//    VICMR_disable_interrupts(&interrupts_info);
    REG_ICU_INT_GLOBAL_ENABLE = 0;
    msg_entity.out_id = 0;
    msg_entity.in_id  = 0;
    msg_entity.status = MSG_POOL_EMPTY;
//    VICMR_restore_interrupts(interrupts_info);
    REG_ICU_INT_GLOBAL_ENABLE = ICU_INT_GLOBAL_ENABLE_IRQ_SET | ICU_INT_GLOBAL_ENABLE_FIQ_SET;
}

void msg_dump(void)
{
    MSG_PRT("%s\r\n", __FUNCTION__);
//    uint32 i;
//    MSG_T *element_ptr;    

//    element_ptr = msg_entity.pool;
//    for(i = 0; i < MSG_COUNT; i ++)
//    {
//        MSG_PRT("msg:%ld, %p:%p:%p\r\n", i, element_ptr[i].id, element_ptr[i].hdl, element_ptr[i].arg);
//    }

//    MSG_PRT("msg-in:%d:%d out:%d:%d\r\n", msg_entity.in_id
//                                        , msg_entity.out_id
//                                        , msg_entity.in_id & (MSG_COUNT - 1)
//                                        , msg_entity.out_id & (MSG_COUNT - 1));
//    
//    MSG_PRT("msg-border:%p pool:%p\r\n", msg_entity.borderline, msg_entity.pool);
}

uint32 msg_pool_is_polluted(void)
{
    uint8 *temp;
    uint32 *magic_hdr;
    uint32 *magic_tail;

    MSG_PRT("%s\r\n", __FUNCTION__);
    temp = (uint8 *)msg_entity.borderline;
    
    magic_hdr = (uint32 *)temp;
    magic_tail = (uint32 *)(temp + sizeof(MSG_T) * MSG_COUNT + MSG_SECURITY_BOUNDARY_NUM);
    
    return ((MSG_HDR_MAGIC_WORD != *magic_hdr) 
            || (MSG_TAIL_MAGIC_WORD != *magic_tail));
}
// EOF
