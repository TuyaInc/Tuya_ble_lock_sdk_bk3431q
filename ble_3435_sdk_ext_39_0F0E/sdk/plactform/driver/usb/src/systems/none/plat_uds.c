/******************************************************************
 *                                                                *
 *        Copyright Mentor Graphics Corporation 2004              *
 *                                                                *
 *                All Rights Reserved.                            *
 *                                                                *
 *    THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION *
 *  WHICH IS THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS   *
 *  LICENSORS AND IS SUBJECT TO LICENSE TERMS.                    *
 *                                                                *
 ******************************************************************/

/*
 * No-OS implementation of a system for Controller Driver
 * $Revision: 1.17 $
 */

#include "mu_impl.h"
#include "mu_none.h"
#include "mu_stat.h"

#include "mu_mem.h"

#include "plat_cnf.h"

#ifndef MUSB_MAX_CONTROLLERS
#define MUSB_MAX_CONTROLLERS 1
#endif



#define UART_PRINTF	uart_printf
int uart_printf(const char *fmt,...);
/***************************** TYPES ******************************/

/* System */
typedef struct _MGC_NoneSystem
{
    MUSB_Controller* pController;
    void* pBoardPrivateData;
    uint16_t wNextQueueItem;
    uint16_t wNextDequeueItem;
    uint8_t* pQueueData;
    MUSB_SystemServices Services;
    uint8_t bBsrRequest;
    uint8_t bBsrOver;
    uint8_t bBsrRan;
} MGC_NoneSystem;

/**************************** GLOBALS *****************************/

static MGC_NoneSystem* MGC_apNoneSystem[MUSB_MAX_CONTROLLERS];

static uint8_t MGC_bNoneSystemCount = 0;

extern void MUSB_Start_Fun(void* pParam);
extern void MUSB_HfiDeviceRemoved(void);
extern void MUSB_Disconn_Fun(void *pParam);

/*************************** FUNCTIONS ****************************/

/*
* Controller calls this to enqueue a background item
*/
static uint8_t MGC_NoneQueueBackgroundItem(void* pPrivate, const void* pItem)
{
    MGC_NoneSystem* pSystem = (MGC_NoneSystem*)pPrivate;
	
    uint8_t* pQueueItem = pSystem->pQueueData + 
    	(pSystem->pController->wQueueItemSize * pSystem->wNextQueueItem++);
    MUSB_MemCopy(pQueueItem, pItem, pSystem->pController->wQueueItemSize);
    if (pSystem->wNextQueueItem >= pSystem->pController->wQueueLength)
    {
		pSystem->wNextQueueItem = 0;
    }
    return TRUE;
}

/*
* Controller calls this to dequeue a background item
*/
static uint8_t MGC_NoneDequeueBackgroundItem(void* pPrivate, void* pItem)
{
    MGC_NoneSystem* pSystem = (MGC_NoneSystem*)pPrivate;
    uint8_t* pQueueItem = NULL;

    if (pSystem->wNextDequeueItem != pSystem->wNextQueueItem)
    {
		pQueueItem = pSystem->pQueueData + 
		(pSystem->pController->wQueueItemSize * pSystem->wNextDequeueItem++);
    }

    if (pSystem->wNextDequeueItem >= pSystem->pController->wQueueLength)
    {
		pSystem->wNextDequeueItem = 0;
    }

    if (pQueueItem)
    {
		MUSB_MemCopy(pItem, pQueueItem, pSystem->pController->wQueueItemSize);
		return TRUE;
    }
    return FALSE;
}

/*
 * Controller calls this to flush the ISR/BSR queue
 */
static void MGC_NoneFlushBackgroundQueue(void* pPrivate)
{
    MGC_NoneSystem* pSystem = (MGC_NoneSystem*)pPrivate;
    pSystem->wNextQueueItem = 0;
    pSystem->wNextDequeueItem = 0;
}

/*
* Controller calls this to enter a lock
*/
static uint8_t MGC_NoneLock(void* pPrivate, uint16_t wIndex)
{
    /* 
     * No locking needed in non-concurrent environment 
     * (ISR restores any "damage" it does)
     */
    return TRUE;
}

/*
* Controller calls this to exit a lock
*/
static uint8_t MGC_NoneUnlock(void* pPrivate, uint16_t wIndex)
{
    /* 
     * No locking needed in non-concurrent environment 
     * (ISR restores any "damage" it does)
     */
    return TRUE;
}

/*
* Utilities provided to controller
*/
static MUSB_SystemUtils MGC_gNoneUtils = {
    MUSB_SYSTEM_UTILS_VERSION,
    MUSB_BoardMessageString,
    MUSB_BoardMessageNumber,
    MUSB_BoardGetTime
};



/*
 * non-OS ISR, arranged to be called by board-specific code.
 */
/*static*/ uint8_t MGC_NoneControllerIsr(void* pPrivateData,uint16_t wIntrTxValue,
            uint16_t wIntrRxValue,uint8_t bIntrUsbValue)
{
    int isrValue = 0;
    volatile MGC_NoneSystem* pSystem = (MGC_NoneSystem*)pPrivateData;
    os_printf("MGC_NoneControllerIsr\r\n");
//os_printf("isr usb\r\n");
    if (pSystem)
    {
        if (pSystem->pController)
        {
            if (pSystem->pController->pfIsr)
            {
                // MGC_FdrcIsr()
                isrValue = pSystem->pController->pfIsr(pSystem->pController->pIsrParam
                    ,wIntrTxValue,wIntrRxValue,bIntrUsbValue);
            }
        }
    }

    if (isrValue < 0)
    {
    	return FALSE;
    }
    
    if (isrValue > 0)
    {
    	/* flag for main loop */
    	pSystem->bBsrRequest++;
    	if (!pSystem->bBsrRequest)
    	{
    	    pSystem->bBsrOver++;
    	}
    }
    return TRUE;
}

/*
 * Arm a timer.
 * Call the board-specific function.
 */
uint8_t MGC_NoneArmTimer(void* pPrivateData, uint16_t wIndex, 
			 uint32_t dwTime, uint8_t bPeriodic, 
			 MUSB_pfTimerExpired pfExpireCallback)
{
    MGC_NoneSystem* pSystem = (MGC_NoneSystem*)pPrivateData;

//    UART_PRINTF("MGC_NoneArmTimer: pfExpireCallback = 0x%p\r\n", pfExpireCallback);
    return MUSB_BoardArmTimer(pSystem->pBoardPrivateData, wIndex, dwTime,
	bPeriodic, pfExpireCallback, pSystem->pController->pPrivateData);
}

/*
 * Cancel a timer.
 * Call the board-specific function.
 */
uint8_t MGC_NoneCancelTimer(void* pPrivateData, uint16_t wIndex)
{
    MGC_NoneSystem* pSystem = (MGC_NoneSystem*)pPrivateData;

    return MUSB_BoardCancelTimer(pSystem->pBoardPrivateData, wIndex);
}

/*
 * Implementation
 */
uint32_t MUSB_NoneRunBackground(void)
{
    uint8_t bBsrOver;
    volatile MGC_NoneSystem* pSystem;
    uint8_t bController;
    uint32_t RetVal = 0;
	
//    os_printf("MUSB_NoneRunBackground: MGC_bNoneSystemCount = %d\r\n", MGC_bNoneSystemCount);
    for (bController = 0; bController < MGC_bNoneSystemCount; bController++)
    {
    	pSystem = MGC_apNoneSystem[bController];
		MUSB_BoardRunBackground(pSystem->pBoardPrivateData);
    	bBsrOver = pSystem->bBsrOver;
    	if ((pSystem->bBsrRequest > pSystem->bBsrRan) || bBsrOver)
    	{
    		os_printf("MUSB_NoneRunBackground: pSystem->bBsrRequest = 0x%x, pSystem->bBsrRan = 0x%x, bBsrOver = 0x%x\r\n",
    				pSystem->bBsrRequest, pSystem->bBsrRan, bBsrOver);
    	    pSystem->bBsrRan++;
            // MGC_DrcBsr()
    	    RetVal = pSystem->pController->pfBsr(pSystem->pController->pBsrParam);
	    	if (!pSystem->bBsrRan) 
				pSystem->bBsrOver = 0;
    	}
    }
	return RetVal;//just one controller
}

void MUSB_Host_init(void)
{
    volatile MGC_NoneSystem* pSystem;
 //   uint8_t bController;

    pSystem = MGC_apNoneSystem[0];
	MUSB_Start_Fun(pSystem->pController->pBsrParam);
}
void MUSB_Host_uninit(void)
{
    volatile MGC_NoneSystem* pSystem;
	MUSB_HfiDeviceRemoved();

	MUSB_NoneRunBackground();
    pSystem = MGC_apNoneSystem[0];
	MUSB_Disconn_Fun(pSystem->pController->pBsrParam);
}
/*
void MUSB_Flush_Queue(void)
{
	volatile MGC_NoneSystem* pSystem;
    pSystem = MGC_apNoneSystem[0];
	MUSB_FlushBackground_queue(pSystem->pController->pBsrParam);
}*/
static void* MGC_NoneInitController(const MUSB_NoneController* pControllerInfo)
{
    static MGC_NoneSystem pSystem;
	static uint8_t QueueData[640];
    uint32_t dwStatus;
    uint8_t* pBaseIsr;
    uint8_t* pBaseBsr;

//    pSystem = (MGC_NoneSystem*)MUSB_MemAlloc(sizeof(MGC_NoneSystem));
 //   if (pSystem)
    {
    	MUSB_MemSet(&pSystem, 0, sizeof(MGC_NoneSystem));
    	pSystem.Services.wVersion = MUSB_SYSTEM_SERVICES_VERSION;
    	pSystem.Services.pPrivateData = (void*)(&pSystem);
    	pSystem.Services.pfSystemToBusAddress = MUSB_BoardSystemToBusAddress;
    	pSystem.Services.pfQueueBackgroundItem = MGC_NoneQueueBackgroundItem;
    	pSystem.Services.pfDequeueBackgroundItem = MGC_NoneDequeueBackgroundItem;
    	pSystem.Services.pfFlushBackgroundQueue = MGC_NoneFlushBackgroundQueue;
    	pSystem.Services.pfArmTimer = MGC_NoneArmTimer;
    	pSystem.Services.pfCancelTimer = MGC_NoneCancelTimer;
    	pSystem.Services.pfLock = MGC_NoneLock;
    	pSystem.Services.pfUnlock = MGC_NoneUnlock;
    	pSystem.Services.pfPrintDiag = MUSB_BoardPrintDiag;
    
	/* for structured error handling: */
	do
	{
	    pBaseIsr = pBaseBsr = pControllerInfo->pBase;
//        UART_PRINTF("pControllerInfo->wType 1 = %d\r\n", pControllerInfo->wType);
	    pSystem.pBoardPrivateData = MUSB_BoardInitController(&pSystem,
                    MGC_NoneControllerIsr, pControllerInfo, &pBaseIsr, &pBaseBsr);
//        UART_PRINTF("pControllerInfo->wType 2 = %d\r\n", pControllerInfo->wType);
	    if (!pSystem.pBoardPrivateData)
	    {
//		    MUSB_MemFree(pSystem);
//		    pSystem = NULL;
            break;
	    }

	    /* try UCD init */
		os_printf("MGC_NoneInitController: --------1\r\n");
//        UART_PRINTF("pControllerInfo->wType 3 = %d\r\n", pControllerInfo->wType);
	    pSystem.pController = MUSB_NewController(&MGC_gNoneUtils,
	        pControllerInfo->wType, pBaseIsr, pBaseBsr);
//        UART_PRINTF("pSystem.pController->wQueueLength = %d, pSystem.pController->wQueueItemSize = %d\r\n",
//            pSystem.pController->wQueueLength, pSystem.pController->wQueueItemSize);
	    if (!pSystem.pController)
	    {
		MUSB_BoardDestroyController(pSystem.pBoardPrivateData);
//		MUSB_MemFree(pSystem);
//		pSystem = NULL;
		break;
	    }
		os_printf("MGC_NoneInitController: --------2\r\n");

	    /* try target-specific timer init now that we know the requirements */
	    if (!MUSB_BoardInitTimers(pSystem.pBoardPrivateData,
		pSystem.pController->wTimerCount,
		pSystem.pController->adwTimerResolutions))
	    {
//		MUSB_DestroyController(pSystem->pController);
		MUSB_BoardDestroyController(pSystem.pBoardPrivateData);
//		MUSB_MemFree(pSystem);
//		pSystem = NULL;
		break;
	    }

	    /* prepare queue */
		os_printf("pQueueData:\r\n");
		pSystem.pQueueData = & QueueData[0];
/*	    pSystem.pQueueData = MUSB_MemAlloc(
		pSystem.pController->wQueueLength *
		pSystem.pController->wQueueItemSize);
	    if (!pSystem.pQueueData)
	    {
		MUSB_DestroyController(pSystem->pController);
		MUSB_BoardDestroyController(pSystem->pBoardPrivateData);
		MUSB_MemFree(pSystem);
		pSystem = NULL;
		break;
	    }*/
//        UART_PRINTF("pSystem.pController->wQueueLength = %d, pSystem.pController->wQueueItemSize = %d\r\n",
//            pSystem.pController->wQueueLength, pSystem.pController->wQueueItemSize);
	    MUSB_MemSet(pSystem.pQueueData, 0,
		pSystem.pController->wQueueLength *
		pSystem.pController->wQueueItemSize);
 
	    /* finally, start the controller */
	    dwStatus = MUSB_StartController(pSystem.pController,
		&(pSystem.Services));
	    if (MUSB_STATUS_OK != dwStatus)
	    {
	    	/*	MUSB_DestroyController(pSystem.pController);
		MUSB_BoardDestroyController(pSystem.pBoardPrivateData);
		MUSB_MemFree(pSystem.pQueueData);
		MUSB_MemFree(pSystem);
		pSystem = NULL;
		break;*/
	    }
	} while (FALSE);
    }
   
    return &pSystem;

}
#if 0
static void MGC_NoneDestroyController(MGC_NoneSystem* pSystem)
{
    /* stop controller */    
    MUSB_StopController(pSystem->pController);

    /* destroy controller */
    MUSB_DestroyController(pSystem->pController);

    /* destroy ISR/BSR queue */
    MUSB_MemFree(pSystem->pQueueData);

    MUSB_MemFree(pSystem);
}
#endif
/*
 * Startup code calls this.
 * Loop through all controllers and initialize them.
 */
uint8_t MUSB_InitSystem(unsigned long dwBsrPriority)
{
    MGC_NoneSystem* pSystem;
    uint8_t bIndex;

    MGC_bNoneSystemCount = 0;
    MUSB_MemSet(MGC_apNoneSystem, 0, sizeof(MGC_apNoneSystem));

	os_printf("---MUSB_InitSystem fun----\r\n");
    for (bIndex = 0; 
	bIndex < (uint8_t)MUSB_MAX_CONTROLLERS;
	bIndex++)
    {
		pSystem = MGC_NoneInitController(&(MUSB_aNoneController[bIndex]));
		if (pSystem)
		{
		    MGC_apNoneSystem[MGC_bNoneSystemCount++] = pSystem;
		}
    }
	/*
    if (MGC_bNoneSystemCount < (uint8_t)MUSB_MAX_CONTROLLERS)
    {
		MUSB_DestroySystem();
		return FALSE;
    }*/
    return TRUE;
}

/*
 * Loop through all initialized controller and destroy them.
 */
 /*
uint8_t MUSB_DestroySystem(void)
{
    uint8_t bIndex;

    for (bIndex = 0; bIndex < MGC_bNoneSystemCount; bIndex++)
    {
		MGC_NoneDestroyController(MGC_apNoneSystem[bIndex]);
    }
    return TRUE;
}*/
