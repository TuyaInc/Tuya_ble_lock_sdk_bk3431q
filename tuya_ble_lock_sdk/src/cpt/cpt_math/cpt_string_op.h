/*************************************************************************
	> File Name: ty_string_op.h
	> Author: 
	> Mail: 
	> Created Time: Sat 08 Dec 2018 04:10:00 PM CST
 ************************************************************************/

#ifndef _SD_STRING_OP_H
#define _SD_STRING_OP_H

#include "tuya_ble_utils.h"
//extern int ty_string_op_array_sprintf(char *out, int len, int num, ...);
//

////////////////////////////////////////////////////////////////////////
//KEY-VALUE
//#define KEY_VALUE_STRING  "{version:1.0.0,category:0101,cpin:6,clv:1,max_lum:100,min_lum:10,pwmhz:8000,crc:97,}"
extern u8 ty_string_op_get_key_value_by_key(u8 key_value_buf_len, u8 *key_value_buf,
                                u8 key_len, u8 *key,
                                u8 *value_len, u8 *value);

extern u8 ty_string_op_get_key_value_by_key_to_int(u8 key_value_buf_len, u8 *key_value_buf, 
                                u8 *key, u8 key_len, 
                                int *result);//get int from key-value string
extern u8 ty_string_op_get_key_value_by_key_to_hex(u8 key_value_buf_len, u8 *key_value_buf, 
                                u8 *key, u8 key_len,
                                int *result);//get hex form key-value string
extern u8 ty_string_op_get_key_value_by_key_to_bool(u8 key_value_buf_len, u8 *key_value_buf,
                                u8 *key, u8 key_len,
                                int *result);//get bool from key-value string
extern u8 ty_string_op_get_key_value_by_key_to_string(u8 key_value_buf_len, u8 *key_value_buf,
                                u8 *key, u8 key_len, 
                                u8 *value, u8 *value_len);//get string for key-value string

////////////////////////////////////////////////////////////////////////
//DP-HEX
typedef enum{
    DP_KIND_RAW = 0x00,
    DP_KIND_BOOL = 0x01,
    DP_KIND_INT = 0x02,
    DP_KIND_STRING = 0x03,
    DP_KIND_ENUM = 0x04,
    DP_KIND_BITMAP = 0x05,
}dp_kind_e;
typedef struct{
    u8 id;
    dp_kind_e type;
    u16 len;
    u8 *data;
}tuya_dp_s;

typedef struct{
    u8 len;
    u8 *data;
}ty_string_op_data_s;

extern void ty_string_op_print_dp(tuya_dp_s *dp);
extern int ty_string_op_hex2dps(u8 *hex_array, int hex_array_len, tuya_dp_s *dps);
extern int ty_string_op_dps2hex(tuya_dp_s *dps, int dps_num, u8 *out);

extern u8 ty_string_op_hex2int(u8 mhex);
extern u8 ty_string_op_hexstr2int(u8 *hexstr,int len,int *sum);
extern u8 ty_string_op_intstr2int(u8 *intstr, int len, int *sum);
extern u8 ty_string_op_hexstr2hex(u8 *hexstr,int len,u8 *hex);
extern u8 ty_string_op_hex2hexstr(u8 *hex,int len,u8 *hexstr);

extern u8 ty_string_op_adv_report_parse(u8 type, ty_string_op_data_s *p_advdata, ty_string_op_data_s *p_typedata);

extern u8 ty_string_op_get_crc(u8 *msg, u8 len);

#endif
