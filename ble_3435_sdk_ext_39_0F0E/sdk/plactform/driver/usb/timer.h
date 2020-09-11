
#ifndef _TIMER_H_
#define _TIMER_H_

#include "BK_System.h"


typedef enum {
    UWE_OK              =  0,

    /* General and I/O Errors */
    UWE_NOTSTARTED      =  1,
    UWE_INPROGRESS      =  2,
    UWE_PERM            =  3,
    UWE_NOENT           =  4,
    UWE_IO              =  5,
    UWE_NXIO            =  6,
    UWE_NOMEM           =  7,
    UWE_BUSY            =  8,
    UWE_NODEV           =  9,
    UWE_INVAL           = 10,
    UWE_NOTSUP          = 11,
    UWE_TIMEDOUT        = 12,
    UWE_SUSPENDED       = 13,
    UWE_UNKNOWN         = 14,
    UWE_TEST_FAILED     = 15,
    UWE_STATE           = 16,
    UWE_STALLED         = 17,
    UWE_PARAM           = 18,
    UWE_ABORTED         = 19,
    UWE_SHORT           = 20,
    UWE_WOULDBLOCK      = 21,
    UWE_ALREADY         = 22,
    UWE_EXPIRED         = 23,
    UWE_FULL            = 24,

    /* Network/IPC Errors */
    UWE_DESTADDRREQ     = 41,
    UWE_ADDRNOTAVAIL    = 42,
    UWE_MSGSIZE         = 43,
    UWE_NETDOWN         = 44,
    UWE_NETUNREACH      = 45,
    UWE_NETRESET        = 46,
    UWE_CONNABORTED     = 47,
    UWE_CONNRESET       = 48,
    UWE_ISCONN          = 49,
    UWE_NOTCONN         = 50,
    UWE_CONNREFUSED     = 51,
    UWE_HOSTDOWN        = 52,
    UWE_HOSTUNREACH     = 53,
    UWE_NOLINK          = 54,
    UWE_PROTO           = 55,
    UWE_NOPROTOOPT      = 56,
    UWE_INTR            = 57,
    UWE_PROTOTYPE       = 58
} result_t;

#define MSEC_IN_TICK               (10)

typedef void *os_timer_h;
typedef void (*jthread_func)(void *arg);

typedef struct os_time {
    int32_t tv_sec;     /* seconds */
    int32_t tv_usec;    /* microseconds */
} os_time_t;

typedef struct os_timer {
    bool is_pending;
    uint64_t ticks;
    jthread_func f;
    void *arg;
    struct os_timer *next;
} os_timer_t;

#define PWM0_COUNT         320     // 32KHz clock 10ms timer
//extern void timer_pwm0_pt2_init(void);
extern void timer_pwm0_init(void);
extern void  RAM_CODE timer_pwm1_pt2_swi(void);
extern uint64_t os_get_tick_counter(void);
void os_tick_delay(uint32 count);
void compute_cpu_speed(void);
void timer_polling_handler(void);
int os_timer_is_pending(os_timer_h timer_h);

result_t os_timer_init(os_timer_h *timer_h);
void os_timer_stop(os_timer_h timer_h);
void os_timer_uninit(os_timer_h timer_h);
void enable_timeout_timer(uint8 flag);
void disable_timeout_timer(void);
uint8 timeout_handle(void);
result_t os_timer_reset(os_timer_h timer_h, uint32_t msec, jthread_func func,
                        void *arg);

#endif // _TIMER_H_

//EOF
