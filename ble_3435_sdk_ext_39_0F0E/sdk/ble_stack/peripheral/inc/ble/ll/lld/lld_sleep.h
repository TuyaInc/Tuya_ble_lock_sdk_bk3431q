/**
 ****************************************************************************************
 *
 * @file lld_sleep.h
 *
 * @brief Functions for RWBLE core sleep mode management
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef LLD_SLEEP_H_
#define LLD_SLEEP_H_

/**
 ****************************************************************************************
 * @addtogroup LLDSLEEP LLDSLEEP
 * @ingroup LLD
 * @brief Functions for RWBLE core sleep mode management
 *
 * This module implements the function that manages deep sleep of BLE core.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"

#include <stdint.h>
#include <stdbool.h>

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

#if (!BT_DUAL_MODE)

/**
 ****************************************************************************************
 * @brief Initialize sleep module
 ****************************************************************************************
 */
void lld_sleep_init(void);

/**
 ****************************************************************************************
 * @brief The Sleep function. Enter BLE Core in deep sleep
 *
 * @param[in]    sleep_duration  Duration of deep sleep (slot time duration)
 * @param[in]    ext_wakeup      False: external wake-up disabled / True: enabled
 ****************************************************************************************
 */
void lld_sleep_enter(uint32_t sleep_duration, bool ext_wakeup);

/**
 ****************************************************************************************
 * @brief Function to wake up BLE core
 ****************************************************************************************
 */
void lld_sleep_wakeup(void);

/**
 ****************************************************************************************
 * @brief Function to handle the end of BLE core wake up
 ****************************************************************************************
 */
void lld_sleep_wakeup_end(void);

#endif // !BT_DUAL_MODE


/// @} LLDSLEEP

#endif // LLD_SLEEP_H_
