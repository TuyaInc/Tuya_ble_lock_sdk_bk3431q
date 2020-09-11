/**
 ****************************************************************************************
 *
 * @file braces.c
 *
 * @brief barcelet Server Implementation.
 *
 * Copyright (C) beken 2009-2015
 *
 *
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_OADS_SERVER)
#include "attm.h"
#include "oads.h"
#include "oads_task.h"
#include "ke_mem.h"
#include "prf_utils.h"
#include "prf.h"
#include "co_utils.h"
#include "flash.h"
#include "uart.h"
#include "ke.h"
#include "icu.h"
#include "wdt.h"


/*
 * FFC0 ATTRIBUTES DEFINITION
 ****************************************************************************************
 */


static uint16_t oadBlkNum = 0, oadBlkTot = 0xFFFF; 
static uint8_t oad_uid_check_status = 0;
uint8_t oad_firmware_type = 0;

OAD_SECTION_T bsec;


const struct attm_desc oads_att_db[OADS_IDX_NB] =
{
	// REMIND Service Declaration
	[OADS_IDX_SVC]   		  =      {ATT_DECL_PRIMARY_SERVICE,  PERM(RD, ENABLE), 0, 0},

	[OADS_IDX_FFC1_LVL_CHAR]  =         {ATT_DECL_CHARACTERISTIC,  PERM(RD, ENABLE), 0, 0},
	//  Characteristic Value
	[OADS_IDX_FFC1_LVL_VAL]   =         {ATT_USER_SERVER_CHAR_IDENTIFY,  PERM(WRITE_REQ, ENABLE) | PERM(WRITE_COMMAND, ENABLE), PERM(UUID_LEN, UUID_128) | PERM(RI, ENABLE) | 0X10, 20},//OADS_FFC1_DATA_LEN *sizeof(uint8_t)

	// ffa3 Level Characteristic - Client Characteristic Configuration Descriptor
	[OADS_IDX_FFC1_LVL_NTF_CFG]   =   {ATT_DESC_CLIENT_CHAR_CFG,  PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},

	[OADS_IDX_FFC1_USER_DECL]     =   	{ATT_DESC_CHAR_USER_DESCRIPTION,  PERM(RD, ENABLE),  PERM(RI, ENABLE), 20},

	[OADS_IDX_FFC2_LVL_CHAR]  =         {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
	//  Characteristic Value
	[OADS_IDX_FFC2_LVL_VAL]   =         {ATT_USER_SERVER_CHAR_BLOCK,PERM(WRITE_REQ, ENABLE) | PERM(WRITE_COMMAND, ENABLE) ,PERM(UUID_LEN, UUID_128) |PERM(RI, ENABLE) | 0x10 , 20},// PERM(RI, ENABLE) OADS_FFC2_DATA_LEN *sizeof(uint8_t)

	[OADS_IDX_FFC2_LVL_NTF_CFG]     =   {ATT_DESC_CLIENT_CHAR_CFG,  PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},

	[OADS_IDX_FFC2_USER_DECL]     =   	{ATT_DESC_CHAR_USER_DESCRIPTION,  PERM(RD, ENABLE), PERM(RI, ENABLE), 20},
  
};




static uint8_t oads_init (struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task, uint8_t sec_lvl,  struct oads_db_cfg* params)
{
    // Service content flag
    uint32_t cfg_flag = OADS_OAD_MANDATORY_MASK;
    // DB Creation Status
    uint8_t status = ATT_ERR_NO_ERROR;
	
    //-------------------- allocate memory required for the profile  ---------------------
    struct oads_env_tag* oads_env =
            (struct oads_env_tag* ) ke_malloc(sizeof(struct oads_env_tag), KE_MEM_ATT_DB);

    // allocate BRACE required environment variable
    env->env = (prf_env_t*) oads_env;
	

  	status = attm_svc_create_db128(start_hdl, ATT_USER_SERVER_SVC_OAD, (uint8_t *)&cfg_flag,
               OADS_IDX_NB, NULL, env->task, &oads_att_db[0],
              (sec_lvl & (PERM_MASK_SVC_DIS | PERM_MASK_SVC_AUTH | PERM_MASK_SVC_EKS))
              | PERM(SVC_MI, DISABLE) );	

		
	UART_PRINTF("create_db status = %x\r\n",status);
    oads_env->oads_start_hdl     = *start_hdl;
    *start_hdl += OADS_IDX_NB;
		        
    if(status == ATT_ERR_NO_ERROR)
    {
		//Set optional permissions
		if(params->features == OADS_NTF_SUP)
		{
			// Level characteristic value permissions
			uint16_t perm = oads_att_db[OADS_IDX_FFC1_LVL_VAL].perm ;
			perm |= PERM(NTF, ENABLE);
			attm_att_set_permission(oads_env->oads_start_hdl + OADS_IDX_FFC1_LVL_VAL, perm, 0);
		
	  		perm = oads_att_db[OADS_IDX_FFC2_LVL_VAL].perm ;
		
			perm |= PERM(NTF, ENABLE);
	  		attm_att_set_permission(oads_env->oads_start_hdl + OADS_IDX_FFC2_LVL_VAL, perm, 0);
		}

				
		// set start handle to first allocated service value
		*start_hdl = oads_env->oads_start_hdl;

		oads_env->features          = params->features;
		oads_env->prf_env.app_task  = app_task
					 | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
		oads_env->prf_env.prf_task  = env->task | PERM(PRF_MI, DISABLE);

		// initialize environment variable
		env->id                     = TASK_ID_OADS;
		env->desc.idx_max           = OADS_IDX_MAX;
		env->desc.state             = oads_env->state;
		env->desc.default_handler   = &braces_default_handler;

		// service is ready, go into an Idle state
		ke_state_set(env->task, OADS_IDLE);
		//UART_PRINTF("go into OADS_IDLE");
    }
    return (status);
}



static void oads_destroy(struct prf_task_env* env)
{
    struct oads_env_tag* oads_env = (struct oads_env_tag*) env->env;

    // clear on-going operation
    if(oads_env->operation != NULL)
    {
        ke_free(oads_env->operation);
    }

    // free profile environment variables
    env->env = NULL;
    ke_free(oads_env);
}

static void oads_create(struct prf_task_env* env, uint8_t conidx)
{
    struct oads_env_tag* oads_env = (struct oads_env_tag*) env->env;
    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);

    // force notification config to zero when peer device is connected
	oads_env->ffc1_ntf_cfg[conidx] = 0;
	oads_env->ffc2_ntf_cfg[conidx] = 0;
}




static void oads_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{
    struct oads_env_tag* oads_env = (struct oads_env_tag*) env->env;

    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);
    // force notification config to zero when peer device is disconnected
	oads_env->ffc1_ntf_cfg[conidx] = 0;
	oads_env->ffc2_ntf_cfg[conidx] = 0;
		
}


void oad_save_reciveData(uint8_t *pValue)
{
	bsec.data_cnt++;
	memcpy(&(bsec.data[(bsec.data_cnt - 1)*16]), pValue + 2, 16);
//	for(uint8_t i = 0;i < 16;i++)
//	{
//		UART_PRINTF("%02x ",pValue[ i+ 2]);
//	}
//	UART_PRINTF("\r\n");
//	
	bsec.flag_write = 1;
}


uint32_t oad_updating_user_section_begin(uint32_t version, uint32_t total_len)
{
	UART_PRINTF("%s\r\n",__func__);
	bsec.sec_hdr.len = total_len;
	bsec.sec_hdr.ver = version;
    bsec.update_offset = 0;
	bsec.data_cnt = 0;
	bsec.flag_sleep = 1;
	
	return bsec.sec_hdr.len;

}



uint32_t oad_updating_user_section_pro(void)
{
	uint32_t wr_addr;
	uint32_t len ;
	OAD_SECTION_PTR sec_ptr;

	
	if(bsec.flag_write == 1)
	{
		sec_ptr = &bsec;
		wr_addr = sec_ptr->update_offset + (SEC_BACKUP_OAD_HEADER_FADDR);
		len = sec_ptr->data_cnt * 16;
	  	flash_write(FLASH_MAIN_BASE_ADDR, wr_addr, len, sec_ptr->data, NULL);	
		sec_ptr->update_offset += len;
		sec_ptr->data_cnt = 0;		
		bsec.flag_write = 0;
		
	}
	
	if(bsec.flag_write == 2)
	{	
		sec_ptr = &bsec;
		wr_addr = sec_ptr->update_offset + (SEC_BACKUP_OAD_HEADER_FADDR);
		len = sec_ptr->data_cnt * 16;
	  	flash_write(FLASH_MAIN_BASE_ADDR, wr_addr, len, sec_ptr->data, NULL);
		sec_ptr->update_offset += len;
		sec_ptr->data_cnt = 0;		
		for(int i = 0 ;i < 10;i++)
		{
			UART_PRINTF("wait for reset!!!\r\n");
		}	
		bsec.flag_write = 0;
		wdt_enable(10);
		while(1);

	}
    return 0; 
} 


uint32_t oad_updating_user_section_end(void)
{
	bsec.flag_sleep = 0;
	bsec.flag_write = 2;	
    latency_disable_state=0;
	return 0;
}


uint8_t oadImgIdentifyWrite( uint16_t connHandle, uint16_t length,uint8_t *pValue )
{
  	UART_PRINTF("%s\r\n",__func__);

  	img_hdr_t rxHdr;
	img_hdr_t ImgHdr;

	rxHdr.ver = co_read16p(&(pValue[4]));
	rxHdr.len = co_read16p(&(pValue[6]));
	rxHdr.uid = co_read32p(&(pValue[8]));
	rxHdr.rom_ver = co_read16p(&(pValue[14]));
	UART_PRINTF("rxHdr.ver = %x \r\n", rxHdr.ver);
	UART_PRINTF("rxHdr.len = %x \r\n", rxHdr.len);
	UART_PRINTF("rxHdr.uid = %x \r\n", rxHdr.uid);
	UART_PRINTF("rxHdr.rom_ver = %x \r\n", rxHdr.rom_ver);			
	
	if(rxHdr.uid == OAD_APP_PART_UID)
	{
		oad_firmware_type = 1;
		UART_PRINTF("app part upgrade\r\n");
		
		//only app can be upgraded.
		flash_read(0,SEC_IMAGE_OAD_HEADER_APP_FADDR, sizeof(img_hdr_t),(uint8_t *)&ImgHdr, NULL);
		UART_PRINTF("ImgHdr.rom_ver = %x \r\n", ImgHdr.rom_ver);
		UART_PRINTF("ImgHdr.uid = %x \r\n", ImgHdr.uid);
		UART_PRINTF("ImgHdr.ver = %x \r\n", ImgHdr.ver);
		oadBlkTot = rxHdr.len / (OAD_BLOCK_SIZE / HAL_FLASH_WORD_SIZE);
    	UART_PRINTF("oadBlkTot = %x \r\n", oadBlkTot);
		if(( ImgHdr.ver != rxHdr.ver ) && (oadBlkTot <= OAD_BLOCK_APP_MAX)&&(oadBlkTot != 0) && (rxHdr.rom_ver == ImgHdr.rom_ver))
		{
			oad_uid_check_status = 1;
		}
	
	}
	else if(rxHdr.uid == OAD_APP_STACK_UID)
	{
		oad_firmware_type = 2;
		UART_PRINTF("app and stack upgrade\r\n");

		//both app and stack can be upgrade.
		flash_read(0,SEC_IMAGE_OAD_HEADER_STACK_FADDR, sizeof(img_hdr_t),(uint8_t *)&ImgHdr, NULL);
		UART_PRINTF("ImgHdr.uid = %x \r\n", ImgHdr.uid);
		UART_PRINTF("ImgHdr.ver = %x \r\n", ImgHdr.ver);
		oadBlkTot = rxHdr.len / (OAD_BLOCK_SIZE / HAL_FLASH_WORD_SIZE);
    	UART_PRINTF("oadBlkTot = %x \r\n", oadBlkTot);
		if(((rxHdr.rom_ver != ImgHdr.rom_ver)) && (oadBlkTot <= OAD_BLOCK_STACK_MAX) && (oadBlkTot != 0))
		{
			oad_uid_check_status = 1;
		}
	}
	else
	{
		oad_firmware_type = 0;
		flash_read(0,SEC_IMAGE_OAD_HEADER_APP_FADDR, sizeof(img_hdr_t),(uint8_t *)&ImgHdr, NULL);
		UART_PRINTF("ImgHdr.rom_ver = %x \r\n", ImgHdr.rom_ver);
		UART_PRINTF("ImgHdr.uid = %x \r\n", ImgHdr.uid);
		UART_PRINTF("ImgHdr.ver = %x \r\n", ImgHdr.ver);
	}
	
	if(oad_uid_check_status == 1)
	{
		oadBlkNum = 0;  
		latency_disable_state = 1;
		oad_uid_check_status = 0;

		//update oad connect parameter.
        bk_conn_param_update(12, 14, 0, 300);
		
		oadImgBlockReq(connHandle, 0);
	}
	else
	{
		oadImgIdentifyReq(connHandle, &ImgHdr);
	}

    return ( 0x00 );//SUCCESS
}

/*********************************************************************
 * @fn      oadImgBlockWrite
 *
 * @brief   Process the Image Block Write.
 *
 * @param   connHandle - connection message was received on
 * @param   pValue - pointer to data to be written
 *
 * @return  status
 */
uint8_t oadImgBlockWrite( uint16_t connHandle, uint8_t *pValue )
{
    static uint16_t  validBlockCnt = 0x0 ;
		
    uint16_t blkNum = co_read16p( pValue);
		
    // make sure this is the image we're expecting
    if ( blkNum == 0 )
    {
		img_hdr_t ImgHdr;
		uint16_t ver;
		uint16_t rom_ver;
		uint32_t uid;

		if(oad_firmware_type == 1)
		{
			flash_read(0,SEC_IMAGE_OAD_HEADER_APP_FADDR, sizeof(img_hdr_t),(uint8_t *)&ImgHdr, NULL);
	
			ver = co_read16p( pValue + 6);
			rom_ver = co_read16p( pValue + 16);
			uid = co_read32p(pValue + 10);
			
			ImgHdr.ver = ver + 1;
			validBlockCnt = 0x0;

			UART_PRINTF("uid = %x\r\n", uid);
			UART_PRINTF("blkNum = %x ,oadBlkNum %x,oadBlkTot %x,ver %x \r\n",blkNum,oadBlkNum,oadBlkTot,ver);
			UART_PRINTF("ImgHdr.ver = %x, ver = %x\r\n",ImgHdr.ver, ver);
			if (( oadBlkNum != blkNum )||( ImgHdr.rom_ver != rom_ver ))
			{
				return ( 0x03 );
			}
		}
		else if(oad_firmware_type == 2)
		{
			flash_read(0,SEC_IMAGE_OAD_HEADER_STACK_FADDR, sizeof(img_hdr_t),(uint8_t *)&ImgHdr, NULL);
	
			ver = co_read16p( pValue + 6);
			rom_ver = co_read16p( pValue + 16);
			uid = co_read32p(pValue + 10);
			
			ImgHdr.ver = ver + 1;
			validBlockCnt = 0x0;

			UART_PRINTF("uid = %x\r\n", uid);
			UART_PRINTF("blkNum = %x ,oadBlkNum %x,oadBlkTot %x,ver %x \r\n",blkNum,oadBlkNum,oadBlkTot,ver);
			UART_PRINTF("ImgHdr.ver = %x, ver = %x\r\n",ImgHdr.ver, ver);
			if ( (oadBlkNum != blkNum) || (ImgHdr.rom_ver== rom_ver))
			{
				return ( 0x03 );
			}
		}
		else 
		{
			UART_PRINTF("oadImgBlockWrite: UNKNOWN UID\r\n");
			return (0x03);
		}

		oad_updating_user_section_begin(ver , oadBlkTot * 4 - 16); 	
		oad_save_reciveData(pValue);		
    }

    if (oadBlkNum == blkNum)
    {
		if(oadBlkNum != 0)
		{
			validBlockCnt++;
			oad_save_reciveData(pValue);
	
			if(validBlockCnt == (oadBlkTot - 1))
			{
				UART_PRINTF("last block \r\n");
				
				oadImgBlockReq(connHandle, oadBlkNum);
			}
		}		
  
		oadBlkNum++;  
    }
	else if(oadBlkNum != blkNum) // Request the next OTA Image block.
    {			
		oadImgBlockReq(connHandle, oadBlkNum);	 
    }
    if (oadBlkNum == oadBlkTot)  // If the OTA Image is complete.
    {
		UART_PRINTF("update down!\r\n");
		oad_updating_user_section_end();
    } 

    return ( 0x00 );
}



/*********************************************************************
 * @fn      oadImgIdentifyReq
 *
 * @brief   Process the Image Identify Request.
 *
 * @param   connHandle - connection message was received on
 * @param   pImgHdr - Pointer to the img_hdr_t data to send.
 *
 * @return  None
 */
static void oadImgBlockReq(uint16_t connHandle, uint16_t blkNum)
{
   // UART_PRINTF("%s\r\n",__func__);
	struct oads_env_tag* oads_env = PRF_ENV_GET(OADS, oads);

	
	uint8_t noti[2];

	noti[0] = LO_UINT16(blkNum);
	noti[1] = HI_UINT16(blkNum);

	struct gattc_send_evt_cmd *cmd = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
								KE_BUILD_ID(TASK_GATTC, connHandle), prf_src_task_get(&(oads_env->prf_env),0),
								gattc_send_evt_cmd, sizeof(uint8_t) * 2);

	// Fill in the parameter structure
	cmd->operation = GATTC_NOTIFY;
	cmd->handle = oads_env->oads_start_hdl + OADS_IDX_FFC2_LVL_VAL;

	//	cmd->seq_num = cmd->handle;
	// pack measured value in database
	cmd->length = 2;

	memcpy(&cmd->value[0],&noti[0],2);
	// send notification to peer device
	ke_msg_send(cmd);


}

/*********************************************************************
 * @fn      oadImgIdentifyReq
 *
 * @brief   Process the Image Identify Request.
 *
 * @param   connHandle - connection message was received on
 * @param   pImgHdr - Pointer to the img_hdr_t data to send.
 *
 * @return  None
 */
static void oadImgIdentifyReq(uint16_t connHandle, img_hdr_t *pImgHdr)
{
    UART_PRINTF("%s\r\n",__func__);  
	
    struct oads_env_tag* oads_env = PRF_ENV_GET(OADS, oads);
	
	uint8_t noti[4];
	uint8_t rom_ver[2];

  	UART_PRINTF("noti  = 0x%x\r\n",&noti[0]);
	noti[0] = LO_UINT16(pImgHdr->ver);
	noti[1] = HI_UINT16(pImgHdr->ver);

	noti[2] = LO_UINT16(pImgHdr->len);
	noti[3] = HI_UINT16(pImgHdr->len);

	rom_ver[0] = LO_UINT16(pImgHdr->rom_ver);
	rom_ver[1] = HI_UINT16(pImgHdr->rom_ver);

	struct gattc_send_evt_cmd *cmd = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
								KE_BUILD_ID(TASK_GATTC, connHandle), prf_src_task_get(&(oads_env->prf_env),0),
								gattc_send_evt_cmd, sizeof(uint8_t) * 10);

	if(cmd == NULL)
	{
		while(1)
		{
			UART_PRINTF("KE_MSG_ALLOC_DYN GATTC_SEND_EVT_CMD  == NULL \r\n");
		}
		
	}

	// Fill in the parameter structure
	cmd->operation = GATTC_NOTIFY;
	cmd->handle = oads_env->oads_start_hdl + OADS_IDX_FFC1_LVL_VAL;

	cmd->seq_num = cmd->handle;
	// pack measured value in database
	cmd->length = 10;
	
	memcpy(&cmd->value[0],&noti[0],4);
	memcpy(&cmd->value[4], (uint8_t*)(&pImgHdr->uid),  4);
	memcpy(&cmd->value[8], &rom_ver[0], 2);
	
	

	// send notification to peer device
	ke_msg_send(cmd);
	
}


/// BRACE Task interface required by profile manager
const struct prf_task_cbs oads_itf =
{
    (prf_init_fnct) oads_init,
    oads_destroy,
    oads_create,
    oads_cleanup,
};



const struct prf_task_cbs* oads_prf_itf_get(void)
{
   return &oads_itf;
}


#endif // (BLE_BRACELET_REPORTER)


 
