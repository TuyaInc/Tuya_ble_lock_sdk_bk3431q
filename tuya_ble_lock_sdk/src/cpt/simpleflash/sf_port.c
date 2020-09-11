#include "sf_port.h"




/*********************************************************************
 * LOCAL CONSTANT
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLE
 */

/*********************************************************************
 * VARIABLE
 */

/*********************************************************************
 * LOCAL FUNCTION
 */




/*********************************************************
FN: 
*/
u32 sf_port_flash_read(u32 addr, void* buf, u32 size)
{
    bk_flash_read(addr, buf, size);
    return 0;
}

/*********************************************************
FN: 
*/
u32 sf_port_flash_write(u32 addr, void* buf, u32 size)
{
    bk_flash_write(addr, buf, size);
    return 0;
}

/*********************************************************
FN: 
*/
u32 sf_port_flash_erase(u32 addr, u32 num)
{
    bk_flash_erase(addr, num);
    return 0;
}

/*********************************************************
FN: 
*/
void sf_mem_init(void)
{
#if SF_MEM_EN
    sd_mem_init();
#else
    //add custom mem function
#endif
}

/*********************************************************
FN: 
*/
void* sf_malloc(u32 size)
{
#if SF_MEM_EN
    return sd_malloc(size);
#else
    //add custom mem function
#endif
}

/*********************************************************
FN: 
*/
u32 sf_free(void* buf)
{
    if(buf) {
#if SF_MEM_EN
        sd_free(buf);
#else
        //add custom mem function
#endif
    }
    return 0;
}

/*********************************************************
FN: 
*/
void sf_log_hexdump(const char *name, uint8_t *buf, uint16_t size)
{
    elog_hexdump(name, 8, buf, size);
}
/*********************************************************
FN: 
*/
void sf_log_hexdump_empty(const char *name, uint8_t width, uint8_t *buf, uint16_t size)
{
    //empty
}




