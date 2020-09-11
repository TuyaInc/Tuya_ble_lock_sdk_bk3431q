#include "sf_nv.h"




/*********************************************************************
 * LOCAL CONSTANT
 */
//固定两个区域，轮换使用
#define SF_AREA_DIVIDE_NUM      (2)

#define SF_BIT_VALID            0x00
#define SF_BIT_INVALID          0x01

//当前区域的起始/结束地址
#define S_START_ADDR(id)        (s_area_base[id] + (s_start_area[id])*SF_ERASE_MIN_SIZE)
#define S_END_ADDR(id)          (s_area_base[id] + (s_start_area[id]+1)*SF_ERASE_MIN_SIZE)
//备份区域的起始地址
#define S_START_ADDR_SHADOW(id) (s_area_base[id] + (sf_next_area(s_start_area[id]))*SF_ERASE_MIN_SIZE)
#define S_END_ADDR_SHADOW(id)   (s_area_base[id] + (sf_next_area(s_start_area[id])+1)*SF_ERASE_MIN_SIZE)

//header size
#define AREA_HDR_SIZE           sizeof(sf_area_hdr_t)
#define UNIT_HDR_SIZE           sizeof(sf_unit_hdr_t)
//align，SF_WRITE_MIN_SIZE字节对齐
#define WRITE_ALIGN(len)        ((len + (SF_WRITE_MIN_SIZE - 1)) & ~(SF_WRITE_MIN_SIZE - 1))


/*********************************************************************
 * LOCAL STRUCT
 */
#pragma pack(1)
typedef struct
{
    u32 occupied_flag:1;
    u32 full_flag:1;
    u32 reserve:30;
} sf_area_hdr_t;

typedef struct
{
    u8 unuse:1;
    u8 valid:1;
    u8 reserve:6;
    u16 id;
    u8 len;
} sf_unit_hdr_t;
#pragma pack()

/*********************************************************************
 * LOCAL VARIABLE
 */
static u32 s_start_area[SF_AREA_NUM] = {0};
static u32 s_area_base[SF_AREA_NUM]  = {
    SF_AREA0_BASE,
    SF_AREA1_BASE,
    SF_AREA2_BASE,
    SF_AREA3_BASE,
    SF_AREA4_BASE,
};

/*********************************************************************
 * VARIABLE
 */

/*********************************************************************
 * LOCAL FUNCTION
 */
static u32 nv_read(u32 addr, void* buf, u32 size);
static u32 nv_write(u32 addr, void* buf, u32 size);
static u32 nv_erase(u32 addr, u32 num);




/*********************************************************
FN: 
*/
static u32 nv_read(u32 addr, void* buf, u32 size)
{
    if(size == 0) {
        SF_PRINTF("Error: size");
        return SF_ERROR_PARAM;
    }
    
    if(buf == NULL) {
        SF_PRINTF("Error: buf");
        return SF_ERROR_PARAM;
    }
    
    if(addr%SF_WRITE_MIN_SIZE != 0) {
        SF_PRINTF("Error: addr");
        return SF_ERROR_PARAM;
    }
    
    u32 read_size = WRITE_ALIGN(size);
    void* tmp = sf_malloc(read_size);
    if(tmp) {
        sf_port_flash_read(addr, tmp, read_size);
        memcpy(buf, tmp, size);
        
        sf_free(tmp);
    }
    
    return SF_SUCCESS;
}

/*********************************************************
FN: 
*/
static u32 nv_write(u32 addr, void* buf, u32 size)
{
    if(size == 0) {
        SF_PRINTF("Error: size");
        return SF_ERROR_PARAM;
    }
    
    if(buf == NULL) {
        SF_PRINTF("Error: buf");
        return SF_ERROR_PARAM;
    }
    
    if(addr%SF_WRITE_MIN_SIZE != 0) {
        SF_PRINTF("Error: addr");
        return SF_ERROR_PARAM;
    }
    
    u32 write_size = WRITE_ALIGN(size);
    void* tmp = sf_malloc(write_size);
    if(tmp) {
        memset(tmp, 0, write_size);
        memcpy(tmp, buf, size);
        sf_port_flash_write(addr, tmp, write_size);
        
        sf_free(tmp);
    }
    return SF_SUCCESS;
}

/*********************************************************
FN: 
*/
static u32 nv_erase(u32 addr, u32 num)
{
    sf_port_flash_erase(addr, num);
    return SF_SUCCESS;
}

/*********************************************************
FN: 
*/
static __inline u32 sf_next_area(u32 area_id)
{
    return (area_id == 0) ? 1 : 0;
}
static __inline u32 sf_next_unit_addr(u32 area_id, u32 addr, u32 shift_addr)
{
    shift_addr = WRITE_ALIGN(shift_addr);
    return (addr + shift_addr == (s_area_base[area_id] + SF_AREA_SIZE)) ? (s_area_base[area_id]) : (addr + shift_addr);
}

/*********************************************************
FN: 获取当前 area 的序号
*/
static u32 get_current_area_idx(u32 area_id)
{
    for(u32 idx=0; idx<SF_AREA_DIVIDE_NUM; idx++) {
        sf_area_hdr_t hdr;
        nv_read(s_area_base[area_id] + idx*SF_ERASE_MIN_SIZE, &hdr, AREA_HDR_SIZE);
        if(hdr.occupied_flag == SF_BIT_VALID && hdr.full_flag == SF_BIT_INVALID) {
            return idx;
        }
    }
    return SF_AREA_DIVIDE_NUM;
}

/*********************************************************
FN: 获取空的 area 的序号
*/
static u32 get_empty_area_idx(u32 area_id)
{
    for(u32 idx=0; idx<SF_AREA_DIVIDE_NUM; idx++) {
        sf_area_hdr_t hdr;
        nv_read(s_area_base[area_id] + idx*SF_ERASE_MIN_SIZE, &hdr, AREA_HDR_SIZE);
        if(hdr.occupied_flag == SF_BIT_INVALID && hdr.full_flag == SF_BIT_INVALID) {
            return idx;
        }
    }
    return SF_AREA_DIVIDE_NUM;
}

/*********************************************************
FN: 更新 area 头
*/
static u32 update_area_header(u32 addr, bool occupied_flag, bool full_flag)
{
    sf_area_hdr_t hdr;
    nv_read(addr, &hdr, AREA_HDR_SIZE);
    hdr.occupied_flag = occupied_flag;
    hdr.full_flag = full_flag;
    nv_write(addr, &hdr, AREA_HDR_SIZE);
    return 0;
}

/*********************************************************
FN: 判断某个地址是否在可用区间内
*/
static u32 addr_is_valid(u32 area_id, u32 addr)
{
    if((addr >= S_START_ADDR(area_id)) && (addr < S_END_ADDR(area_id))) {
        return 1;
    }
    return 0;
}

/*********************************************************
FN: nv初始化
*/
u32 sf_nv_init(u32 area_id)
{
    sf_mem_init();
    SF_PRINTF("simpleflash area[%d] start addr: 0x%x, size: %d", area_id, s_area_base[area_id], SF_AREA_SIZE);
    
    s_start_area[area_id] = get_current_area_idx(area_id);
    if(s_start_area[area_id] == SF_AREA_DIVIDE_NUM) //不存在当前 area （空）/满
    {
        u32 empty_area_idx = get_empty_area_idx(area_id);
        if(empty_area_idx == SF_AREA_DIVIDE_NUM) {
            SF_PRINTF("simpleflash is full");
            return SF_ERROR_FULL;
        }
        else {
            SF_PRINTF("simpleflash is empty");
            s_start_area[area_id] = 0;
            update_area_header(S_START_ADDR(area_id), SF_BIT_VALID, SF_BIT_INVALID);
        }
    }
    return SF_SUCCESS;
}

/*********************************************************
FN: 写 nv
*/
u32 sf_nv_write(u32 area_id, u16 id, void *buf, u8 size)
{
    if(size == 0) {
        SF_PRINTF("Error: size");
        return SF_ERROR_PARAM;
    }
    
    if(buf == NULL) {
        SF_PRINTF("Error: buf");
        return SF_ERROR_PARAM;
    }
    
    u32 addr;
    u32 addr_shadow;
    u8 *pBuf;
    u8 *pBuf_shadow;
    sf_unit_hdr_t hdr;
    
    pBuf = sf_malloc(size);
    if(pBuf == NULL) {
        return SF_ERROR_PARAM;
    }
    
    memcpy(pBuf, buf, size);
    
    //write data
    for(addr=S_START_ADDR(area_id)+AREA_HDR_SIZE; addr!=S_START_ADDR_SHADOW(area_id); )
    {
        // 读取 item 头数据
        nv_read(addr, &hdr, UNIT_HDR_SIZE);

        // 作废旧数据
        if(hdr.id==id && !hdr.unuse && hdr.valid) {
            hdr.valid = 0;
            nv_write(addr, &hdr, UNIT_HDR_SIZE);// 写入 item 头数据
        }

        // 写入新数据
        if(hdr.unuse)
        {
            if(sf_next_unit_addr(area_id, addr, UNIT_HDR_SIZE + size) <= S_END_ADDR(area_id))
            {
                hdr.unuse = 0;
                hdr.valid = 1;
                hdr.id = id;
                hdr.len = size;
                nv_write(addr, (void*)&hdr, UNIT_HDR_SIZE);// 写入 item 头数据
                nv_write(addr+UNIT_HDR_SIZE, (void*)pBuf, hdr.len);// 写入数据
                
                sf_free(pBuf);
                return 0;
            } else {
                //填充该 area 尾部
                hdr.unuse = 0;
                hdr.valid = 0;
                hdr.id = 0xFFFF;
                hdr.len = S_END_ADDR(area_id) - (addr + UNIT_HDR_SIZE);
                nv_write(addr, (void*)&hdr, UNIT_HDR_SIZE);// 写入 item 头数据
                break;
            }
        }
        else {
            addr = sf_next_unit_addr(area_id, addr, UNIT_HDR_SIZE + hdr.len);
            if(!addr_is_valid(area_id, addr)) {
                break;
            }
        }
    }

    // 写满
    // 搬移
    addr_shadow = S_START_ADDR_SHADOW(area_id) + AREA_HDR_SIZE;
    for(addr=S_START_ADDR(area_id)+AREA_HDR_SIZE; addr!=S_START_ADDR_SHADOW(area_id); 
        addr=sf_next_unit_addr(area_id, addr, UNIT_HDR_SIZE + hdr.len))
    {
        // 读取 item 头数据
        nv_read(addr, &hdr, UNIT_HDR_SIZE);

        // 找到有效数据, 搬移
        if(!hdr.unuse && hdr.valid)
        {
            pBuf_shadow = sf_malloc(hdr.len);
            if(pBuf_shadow) {
                nv_read(addr+UNIT_HDR_SIZE, pBuf_shadow, hdr.len);// 读取数据

                nv_write(addr_shadow, &hdr, UNIT_HDR_SIZE);// 写入 item 头数据
                nv_write(addr_shadow+UNIT_HDR_SIZE, pBuf_shadow, hdr.len);// 写入数据
                
                sf_free(pBuf_shadow);
                
                addr_shadow = sf_next_unit_addr(area_id, addr_shadow, UNIT_HDR_SIZE + hdr.len);
            } else {
                sf_free(pBuf);
                return 1;
            }
        }
    }
    
    // 搬移完成
    {
        // 擦除原有数据
        nv_erase(S_START_ADDR(area_id), 1);
        //这个 area 满
//        update_area_header(S_START_ADDR(area_id), SF_BIT_VALID, SF_BIT_VALID);
        //到下一个 area
        s_start_area[area_id] = sf_next_area(s_start_area[area_id]);
        update_area_header(S_START_ADDR(area_id), SF_BIT_VALID, SF_BIT_INVALID);
        
        // 检查是否写满了，没有写满
        if(addr_is_valid(area_id, addr_shadow) && (sf_next_unit_addr(area_id, addr_shadow, UNIT_HDR_SIZE + size) <= S_END_ADDR(area_id)))
        {
            // 写入新数据
            if(sf_nv_write(area_id, id, pBuf, size) == SF_SUCCESS)
            {
                sf_free(pBuf);
                return SF_SUCCESS;
            }
        }
        else {
            SF_PRINTF("simpleflash is full");
            sf_free(pBuf);
            return SF_ERROR_FULL;
        }
    }
    return SF_SUCCESS;
}

/*********************************************************
FN: 读 nv
*/
u32 sf_nv_read(u32 area_id, u16 id, void *buf, u8 size)
{
    u32 addr;
    u8* pBuf;
    sf_unit_hdr_t hdr;

    for(addr=S_START_ADDR(area_id)+AREA_HDR_SIZE; addr!=S_START_ADDR_SHADOW(area_id); )
    {
        nv_read(addr, &hdr, UNIT_HDR_SIZE);
        
        if(hdr.id==id && !hdr.unuse && hdr.valid && hdr.len==size)
        {
            pBuf = sf_malloc(size);
            if(pBuf) {
                // 读取数据
                nv_read(addr+UNIT_HDR_SIZE, pBuf, hdr.len);
                memcpy(buf, pBuf, hdr.len);
                
                sf_free(pBuf);
                return SF_SUCCESS;
            }
        }
        
        addr = sf_next_unit_addr(area_id, addr, UNIT_HDR_SIZE + hdr.len);
        if(!addr_is_valid(area_id, addr)) {
            return SF_ERROR_NOT_FOUND;
        }
        if((hdr.id == 0xFFFF) || (addr%SF_ERASE_MIN_SIZE == 0)) {
            addr += AREA_HDR_SIZE;
        }
    }
    return 1;
}

/*********************************************************
FN: 删除 nv
*/
u32 sf_nv_delete(u32 area_id, u16 id)
{
    u32 addr;
    sf_unit_hdr_t hdr;

    for(addr=S_START_ADDR(area_id)+AREA_HDR_SIZE; addr!=S_START_ADDR_SHADOW(area_id); )
    {
        nv_read(addr, &hdr, UNIT_HDR_SIZE);
        // 作废旧数据
        if(hdr.id==id && !hdr.unuse && hdr.valid)
        {
            hdr.valid = 0;
            nv_write(addr, (void*)&hdr, UNIT_HDR_SIZE);// 写入 item 头数据
            return 0;
        }
        
        addr = sf_next_unit_addr(area_id, addr, UNIT_HDR_SIZE + hdr.len);
        if(!addr_is_valid(area_id, addr)) {
            return SF_ERROR_NOT_FOUND;
        }
        if((hdr.id == 0xFFFF) || (addr%SF_ERASE_MIN_SIZE == 0)) {
            addr += AREA_HDR_SIZE;
        }
    }
    return 1;
}




/*********************************************************
FN: nv 测试
*/
#define SF_NV_TEST_LEN  255
static u8 tmp_buf1[SF_NV_TEST_LEN] = {0};
static u8 tmp_buf2[SF_NV_TEST_LEN] = {0};

void sf_nv_test(u32 area_id)
{
    u32 idx;
    u32 num = ((SF_ERASE_MIN_SIZE-AREA_HDR_SIZE) / (UNIT_HDR_SIZE+WRITE_ALIGN(SF_NV_TEST_LEN)));
    
    nv_erase(s_area_base[area_id], SF_AREA_DIVIDE_NUM);
    for(idx=0; idx<SF_NV_TEST_LEN; idx++) {
        tmp_buf1[idx] = idx;
    }
    
    sf_nv_init(area_id);
    
    for(idx=0; idx<5*num; idx++)
    {
        memset(tmp_buf2, 0, SF_NV_TEST_LEN);
        sf_nv_write(area_id, 0, tmp_buf1, SF_NV_TEST_LEN);
        sf_nv_read(area_id, 0, tmp_buf2, SF_NV_TEST_LEN);
        if(memcmp(tmp_buf1, tmp_buf2, SF_NV_TEST_LEN) != 0) {
            SF_PRINTF("Error: read");
        }
    }
    
    for(idx=num; idx<2*num; idx++)
    {
        memset(tmp_buf2, 0, SF_NV_TEST_LEN);
        sf_nv_write(area_id, idx, tmp_buf1, SF_NV_TEST_LEN);
        sf_nv_read(area_id, idx, tmp_buf2, SF_NV_TEST_LEN);
        if(memcmp(tmp_buf1, tmp_buf2, SF_NV_TEST_LEN) != 0) {
            SF_PRINTF("Error: read");
        }
    }
    
    memset(tmp_buf2, 0, SF_NV_TEST_LEN);
    sf_nv_write(area_id, idx, tmp_buf1, SF_NV_TEST_LEN);
    sf_nv_read(area_id, idx, tmp_buf2, SF_NV_TEST_LEN);
    if(memcmp(tmp_buf1, tmp_buf2, SF_NV_TEST_LEN) != 0) {
        SF_PRINTF("Error: read");
    }
    
    __nop();
}






