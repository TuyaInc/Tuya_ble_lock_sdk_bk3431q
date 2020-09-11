/**
 *************************************************************************************
 * @file    sbc_decoder.h
 * @brief   An implementation of SBC decoder for bluetooth.
 *
 * @author  Aixing.Li
 * @version V1.0.0
 *
 * &copy; 2017 BEKEN Corporation Ltd. All rights reserved.
 *
 *************************************************************************************
 */

#ifndef __SBC_DECODER_H__
#define __SBC_DECODER_H__

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "sbc_common.h"

/**
 * SBC decoder error code
 */
typedef enum _SBC_DECODER_ERROR_CODE
{
    SBC_DECODER_ERRORS = -128,
    SBC_DECODER_ERROR_BUFFER_OVERFLOW,          /**< buffer overflow       */
    SBC_DECODER_ERROR_SYNC_INCORRECT,           /**< synchronize incorrect */
    SBC_DECODER_ERROR_BITPOOL_OUT_BOUNDS,       /**< bitpool out of bounds */
    SBC_DECODER_ERROR_CRC8_INCORRECT,           /**< CRC8 check incorrect  */
    SBC_DECODER_ERROR_STREAM_EMPTY,             /**< stream empty          */

    SBC_DECODER_ERROR_OK = 0,                   /**< no error              */

}SBC_DECODER_ERROR_CODE;

/**
 * @brief SBC decoder context
 */
typedef struct _SbcDecoderContext
{
    SbcCommonContext frame;

    int8_t   num_channels;              /**< channels number    */
    uint8_t  pcm_length;                /**< PCM length         */
    uint16_t sample_rate;               /**< sample rate        */

    int32_t  pcm_sample[2][128];        /**< PCM frame buffer   */

    int32_t  vfifo[2][170];             /**< FIFO V for subbands synthesis calculation. */ 

    int32_t  offset[2][16];

}SbcDecoderContext;

/**
 * @brief  SBC decoder initialize
 * @param  sbc SBC decoder context pointer
 * @return error code, @see SBC_DECODER_ERROR_CODE
 */
int32_t sbc_decoder_init(SbcDecoderContext* sbc);

/**
 * @brief  SBC decoder decode one frame
 * @param  sbc    SBC decoder context pointer
 * @param  data   buffer to be decoded
 * @param  length input buffer legth
 * @return consumed buffer length by decoder if no error ocurs,
 *         else error code (always small than 0) will be return, @see SBC_DECODER_ERROR_CODE
 *         the output PCM data please refer to the follows variables:
 *         sbc->pcm_sample   means output PCM data address
 *         sbc->pcm_length   means output PCM data length in sample
 *         sbc->num_channels means output PCM data channels
 */
int32_t sbc_decoder_frame_decode(SbcDecoderContext* sbc, const uint8_t* data, int32_t length);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//__SBC_DECODER_H__
