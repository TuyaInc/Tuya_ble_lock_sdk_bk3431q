/*************************************************************************
	> File Name: ty_string_op.c
	> Author: 
	> Mail: 
	> Created Time: Sat 08 Dec 2018 04:09:48 PM CST
 ************************************************************************/
#include "cpt_string_op.h"
#include "app_common.h"
//////////////////////////////////////////////////////////////////
//基本运算函数-字符数组拼接
//    len = func(buf, 1000, 4, "saaa", 4, "hello", 5);
//    buf[len] = 0;
//    printf("%d %s\n",len,buf);
//////////////////////////////////////////////////////////////////
/*
int ty_string_op_array_sprintf(char *out, int len, int num, ...){
    va_list ap;
    va_start(ap, num);

    int start = 0;
    for(int i=0;i<num;i+=2){
        char *str = va_arg(ap,char *);
        int str_len = va_arg(ap,int);
        if(str_len > (len-start)){
            va_end(ap);
            return 0;
        }

        memcpy(&out[start],str,str_len);
        start += str_len;

        printf("%s\n",str);
        printf("%d\n",str_len);
    }
    va_end(ap);
    return start;
}
*/

//////////////////////////////////////////////////////////////////
//基本运算函数-key-value字符串匹配
//////////////////////////////////////////////////////////////////
static s16 find_char(u8 *buf,u8 len,u8 data){
    for(u8 i=0;i<len;i++)
        if(buf[i] == data)return i;
    return -1;
}

u8 ty_string_op_get_key_value_by_key(u8 key_value_buf_len, u8 *key_value_buf,
                                u8 key_len, u8 *key,
                                u8 *value_len, u8 *value){

    if(key_value_buf_len == 0 || key_len == 0 || key_value_buf_len < key_len)
        return 0;

    u8 i = 0,temp = key_value_buf[0];
    key_value_buf[0] = ',';//easy for search: ,xx:xx,xx:xx,

    while(i<key_value_buf_len){
        u8 base = i;
        s16 index1 = find_char(&key_value_buf[base],key_value_buf_len,',');
        if(index1 == -1)return 0;
        s16 index2 = find_char(&key_value_buf[base+index1],key_value_buf_len,':');
        if(index2 == -1)return 0;
        if(index2 -1 != key_len){
            i = base + index2 + index1;
            continue;
        }

        u8 find = 1;
        for(u8 j=0;j<key_len;j++){
            if(key_value_buf[base+index1+1+j] != key[j]){
                find = 0;
                i = base + index2 + index1;
                break;
            }
        }

        if(find){
            s16 index3 = find_char(&key_value_buf[base+index1+index2],key_value_buf_len,',');
            if(index3 == -1)return 0;
            *value_len = index3 - 1;
            for(u8 k=0;k<*value_len;k++){
                value[k] = key_value_buf[base+index1+index2+1+k];
            }
            key_value_buf[0] = temp;
            return 1;
        }
    }
    key_value_buf[0] = temp;
    return 0;
}

/*
 * 从配置文件中获取key对应的int值
 */
u8 ty_string_op_get_key_value_by_key_to_int(u8 key_value_buf_len, u8 *key_value_buf, 
                        u8 *key, u8 key_len, 
                        int *result){
    u8 value[20];
    u8 value_len;
    u8 ret, ok = 0;

    ret = ty_string_op_get_key_value_by_key(key_value_buf_len, key_value_buf, key_len, key, &value_len, value);
    if(ret == 1){
        if(ty_string_op_intstr2int(value,value_len,result)){
            ok = 1;
        }
    }
    return ok;
}

/*
 * 从配置文件中获取key对应的hex值
 */
u8 ty_string_op_get_key_value_by_key_to_hex(u8 key_value_buf_len, u8 *key_value_buf, 
                        u8 *key, u8 key_len,
                        int *result){
    u8 value[20];
    u8 value_len;
    u8 ret, ok = 0;

    ret = ty_string_op_get_key_value_by_key(key_value_buf_len, key_value_buf, key_len, key, &value_len, value);
    if(ret == 1){
        if(ty_string_op_hexstr2int(value,value_len,result)){
            ok = 1;
        }
    }
    return ok;
}

/*
 * 从配置文件中获取key对应的bool的值
 */
u8 ty_string_op_get_key_value_by_key_to_bool(u8 key_value_buf_len, u8 *key_value_buf,
                        u8 *key, u8 key_len,
                        int *result){
    u8 value[20];
    u8 value_len;
    u8 ret, ok = 0;

    ret = ty_string_op_get_key_value_by_key(key_value_buf_len, key_value_buf, key_len, key, &value_len, value);
    if(ret == 1){
        if(0 == memcmp(value,"false",5)){
            *result = 0;
            ok = 1;
        }else if(0 == memcmp(value,"true",4)){
            *result = 1;
            ok = 1;
        }
    }
    return ok;
}

/*
 * 从配置文件中获取key对应的string值
 */
u8 ty_string_op_get_key_value_by_key_to_string(u8 key_value_buf_len, u8 *key_value_buf,
                        u8 *key, u8 key_len, 
                        u8 *value, u8 *value_len){
    u8 ret, ok = 0;

    ret = ty_string_op_get_key_value_by_key(key_value_buf_len, key_value_buf, key_len, key, value_len, value);
    if(ret == 1){
        ok = 1;
    }
    return ok;
}

//////////////////////////////////////////////////////////////////
//基本运算函数-将tuya_dps字符串结构化打印出来
//////////////////////////////////////////////////////////////////
void ty_string_op_print_dp(tuya_dp_s *dp){
    if(dp == NULL)return;
//    char *type[] = {"RAW","BOOL","INT","STRING","ENUM","BITMAP"};
    
//    PR_DEBUG_RAW("{\"id\":%d,\"type\":\"%s\",\"length\":%d,\"data\":",dp->id,type[dp->type],dp->len);
//    PR_DEBUG_HEX_ARRAY(dp->data,dp->len);
//    PR_DEBUG_RAW("}");
}

/*
 * param:
 *      hex_array[]
 *      hex_array_len
 *      *dp
 * ret:
 *      the length of this dp occupied hexs
 */
int ty_string_op_hex2dp(u8 *hex_array, int hex_array_len, tuya_dp_s *dp){
	#define TUYA_DP_EXCEPT_DATA_LEN 4
    int dp_len = 0;
    if(hex_array_len > TUYA_DP_EXCEPT_DATA_LEN){
        dp->id = hex_array[0];
        dp->type = (dp_kind_e)hex_array[1];
        dp->len = (u16)(hex_array[2]<<8) | hex_array[3];

        //check length
        int temp = TUYA_DP_EXCEPT_DATA_LEN+dp->len;
        if(hex_array_len >= temp){
            dp->data = &hex_array[4];
            dp_len = temp;
        }
    }
    return dp_len;
}

/*
 * param:
 *      hex_array[]
 *      hex_array_len
 *      dps[x], x must be budget maximum
 * ret:
 *      the num of effective dp
 */
int ty_string_op_hex2dps(u8 *hex_array, int hex_array_len, tuya_dp_s *dps){
    int ret = 1;
    int num = 0;
    int base_index = 0;
    int left_len = hex_array_len;
    while(ret){
        ret = ty_string_op_hex2dp(&hex_array[base_index],left_len,&dps[num]);
        if(ret){
            ty_string_op_print_dp(&dps[num]);
//            PR_DEBUG_RAW("\r\n");
            base_index += ret;
            left_len -= ret;
            num++;
        }
    }
    return num;
}

/*
 * param:
 *      dps[]
 *      dps_num
 *      out[256]
 * ret:
 *      the len of effective hex in out[]
 */
int ty_string_op_dps2hex(tuya_dp_s *dps, int dps_num, u8 *out){
    int index = 0;
    for(int i=0;i<dps_num;i++){
        out[index++] = dps[i].id;
        out[index++] = (u8)dps[i].type;
        out[index++] = dps[i].len >> 8;
        out[index++] = dps[i].len & 0xFF;
        memcpy(&out[index],dps[i].data,dps[i].len);
        index += dps[i].len;
    }
//    PR_DEBUG_HEX_ARRAY(out,index);
//    PR_DEBUG_RAW("\r\n");
    return index;
}

/*int main(void){
    tuya_dp_s dps[10];
    
    u8 dp_array[] = {0x04, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0xB1, 0x04, 0x02, 0x00, 0x04, 0x00, 0x02, 0x00, 0xB1};
    int num = ty_string_op_hex2dps(dp_array,sizeof(dp_array),dps);

    u8 dp_array_temp[256];
    int dp_array_temp_len = ty_string_op_dps2hex(dps,num,dp_array_temp);
    return 0;
}*/

////////////////////////////////////////////
//基本运算函数-字符串转hex/int
////////////////////////////////////////////
u8 ty_string_op_hex2int(u8 mhex){
    switch(mhex){
        case '0':return 0;
        case '1':return 1;
        case '2':return 2;
        case '3':return 3;
        case '4':return 4;
        case '5':return 5;
        case '6':return 6;
        case '7':return 7;
        case '8':return 8;
        case '9':return 9;
        case 'a':
        case 'A':return 10;
        case 'b':
        case 'B':return 11;
        case 'c':
        case 'C':return 12;
        case 'd':
        case 'D':return 13;
        case 'e':
        case 'E':return 14;
        case 'f':
        case 'F':return 15;
        default:return (u8)-1;
    }
}
u8 ty_string_op_int2hex(u8 mint){
    switch(mint){
        case 0:return '0';
        case 1:return '1';
        case 2:return '2';
        case 3:return '3';
        case 4:return '4';
        case 5:return '5';
        case 6:return '6';
        case 7:return '7';
        case 8:return '8';
        case 9:return '9';
        case 10:return 'a';
        case 11:return 'b';
        case 12:return 'c';
        case 13:return 'd';
        case 14:return 'e';
        case 15:return 'f';
//        case 10:return 'A';
//        case 11:return 'B';
//        case 12:return 'C';
//        case 13:return 'D';
//        case 14:return 'E';
//        case 15:return 'F';
        default:return (u8)-1;
    }
}

u8 ty_string_op_hexstr2int(u8 *hexstr,int len,int *sum){
    *sum = 0;
    int value;
    for(int i=0;i<len;i++){
        value = ty_string_op_hex2int(hexstr[i]);
        if(value == -1)return 0;
        (*sum)=(*sum)<<4;
        (*sum)+=value;
    }
    return 1;
}

u8 ty_string_op_intstr2int(u8 *intstr,int len,int *sum){
    *sum = 0;
    int value;
    for(int i= 0;i<len;i++){
        value = ty_string_op_hex2int(intstr[i]);
        if(value == -1 || value > 9)return 0;
        (*sum)=(*sum)*10;
        (*sum)+=value;
    }
    
    return 1;
}

u8 ty_string_op_hexstr2hex(u8 *hexstr,int len,u8 *hex){
    u8 value = 0;
	for(u8 i=0;i<len;i++){
        u8 tmp = ty_string_op_hex2int(hexstr[i]);
        if(tmp == (u8)-1)return 0;
        value <<= 4;
        value |= tmp;
        
        if((i & 1) == 1){
            hex[i>>1] = value;
            value = 0;
        }
	}
	return 1;
}

u8 ty_string_op_hex2hexstr(u8 *hex,int len,u8 *hexstr){
	for(u8 i=0;i<len;i++){
        u8 high = hex[i]>>4;
        u8 low  = hex[i]&0x0F;
        hexstr[i*2] = ty_string_op_int2hex(high);
        hexstr[i*2+1] = ty_string_op_int2hex(low);
	}
	return 1;
}

////////////////////////////////////////////
//蓝牙广播包解析函数
////////////////////////////////////////////
u8 ty_string_op_adv_report_parse(u8 type, ty_string_op_data_s *p_advdata, ty_string_op_data_s *p_typedata){
    u8 index = 0;
    u8 * p_data;
    p_data = p_advdata->data;
    while (index < p_advdata->len){
        u8 field_length = p_data[index];
        u8 field_type   = p_data[index + 1];

        if (field_type == type){
            p_typedata->data = &(p_data[index + 2]);
            p_typedata->len = field_length - 1;
            return 0;//OPRT_OK
        }
        index += field_length + 1;
    }
    return 1;//OPRT_ERROR_NOT_FOUND
}

//////////////////////////////////////////////////////////////////
//基本运算函数-CRC校验
//////////////////////////////////////////////////////////////////
const unsigned char CRC7_POLY = 0x91;
u8 ty_string_op_get_crc(u8 *msg, u8 len){
    u8 i, j, crc = 0;

    for(i=0; i<len; i++){
        crc ^= msg[i];
        for(j = 0; j < 8; j++){
            if(crc & 1)
                crc ^= CRC7_POLY;
            crc >>= 1;
        }
    }
    return crc;
}

