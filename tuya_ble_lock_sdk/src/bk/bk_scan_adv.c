#include "bk_scan_adv.h"
//#include "app_port.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */
//广播和扫描响应数据
#define     DEFAULT_ADV_DATA            \
            {                           \
                0x02,                   \
                GAP_AD_TYPE_FLAGS,      \
                GAP_LE_GEN_DISCOVERABLE_FLG | GAP_BR_EDR_NOT_SUPPORTED, \
            }
            
#define     DEFAULT_SCAN_RSP            \
            {                           \
                0x04,                   \
                GAP_AD_TYPE_COMPLETE_NAME, \
                'L',                    \
                'K',                    \
                'E'                     \
            }

//广播和扫描参数 //1285
#define     DEFAULT_ADV_PARAM           \
            {                           \
                .adv_interval_min = BK_ADV_INTERVAL_MIN, \
                .adv_interval_max = BK_ADV_INTERVAL_MAX, \
                .adv_type     = GAPM_ADV_UNDIRECT,        \
                .adv_power    = 0x00,                     \
                .adv_channal_map   = APP_ADV_CHMAP,               \
            }

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */
adv_param_t             g_adv_param                    = DEFAULT_ADV_PARAM;
uint32_t                g_adv_data_len                 = DEFAULT_ADV_DATA_LEN;
uint8_t                 g_adv_data[NRFS_ADV_MAX_LEN]   = DEFAULT_ADV_DATA;
uint32_t                g_scan_rsp_len                 = DEFAULT_SCAN_RSP_LEN;
uint8_t                 g_scan_rsp[NRFS_ADV_MAX_LEN]   = DEFAULT_SCAN_RSP;

//更新广播参数
volatile bool g_adv_restart_glag = false;




/*********************************************************
FN: 启动广播
*/
void bk_adv_start(void)
{
    appm_start_advertising();
}

/*********************************************************
FN: 停止广播
*/
void bk_adv_stop(void)
{
    appm_stop_advertising();
}

/*********************************************************
FN: 更新广播和扫描响应数据
*/
void bk_adv_update_advDataAndScanRsp(void)
{
    appm_update_adv_data(g_adv_data, g_adv_data_len, g_scan_rsp, g_scan_rsp_len);
}

/*********************************************************
FN: 设置广播参数，结合 g_adv_param 使用
*/
void bk_adv_param_set(void)
{
    if(ke_state_get(TASK_APP) == APPM_ADVERTISING)
    {
        bk_adv_stop();
        g_adv_restart_glag = true;
    }
}
































