
/**
 *************************************************************************************
 * @file    sbc_decoder.c
 * @brief   An implementation of SBC decoder for bluetooth.
 *
 * @author  Aixing.Li
 * @version V1.0.0
 *
 * &copy; 2017 BEKEN Corporation Ltd. All rights reserved.
 *
 *************************************************************************************
 */

#include <string.h>
#include "sbc_decoder.h"

static const int32_t SBC_DECODER_PROTO_4_40M0[] =
{
    SS4(0x00000000), SS4(0xffa6982f), SS4(0xfba93848), SS4(0x0456c7b8),
    SS4(0x005967d1), SS4(0xfffb9ac7), SS4(0xff589157), SS4(0xf9c2a8d8),
    SS4(0x027c1434), SS4(0x0019118b), SS4(0xfff3c74c), SS4(0xff137330),
    SS4(0xf81b8d70), SS4(0x00ec1b8b), SS4(0xfff0b71a), SS4(0xffe99b00),
    SS4(0xfef84470), SS4(0xf6fb4370), SS4(0xffcdc351), SS4(0xffe01dc7)
};

static const int32_t SBC_DECODER_PROTO_4_40M1[] =
{
    SS4(0xffe090ce), SS4(0xff2c0475), SS4(0xf694f800), SS4(0xff2c0475),
    SS4(0xffe090ce), SS4(0xffe01dc7), SS4(0xffcdc351), SS4(0xf6fb4370),
    SS4(0xfef84470), SS4(0xffe99b00), SS4(0xfff0b71a), SS4(0x00ec1b8b),
    SS4(0xf81b8d70), SS4(0xff137330), SS4(0xfff3c74c), SS4(0x0019118b),
    SS4(0x027c1434), SS4(0xf9c2a8d8), SS4(0xff589157), SS4(0xfffb9ac7)
};

static const int32_t SBC_DECODER_PROTO_8_80M0[] =
{
    SS8(0x00000000), SS8(0xfe8d1970), SS8(0xee979f00), SS8(0x11686100),
    SS8(0x0172e690), SS8(0xfff5bd1a), SS8(0xfdf1c8d4), SS8(0xeac182c0),
    SS8(0x0d9daee0), SS8(0x00e530da), SS8(0xffe9811d), SS8(0xfd52986c),
    SS8(0xe7054ca0), SS8(0x0a00d410), SS8(0x006c1de4), SS8(0xffdba705),
    SS8(0xfcbc98e8), SS8(0xe3889d20), SS8(0x06af2308), SS8(0x000bb7db),
    SS8(0xffca00ed), SS8(0xfc3fbb68), SS8(0xe071bc00), SS8(0x03bf7948),
    SS8(0xffc4e05c), SS8(0xffb54b3b), SS8(0xfbedadc0), SS8(0xdde26200),
    SS8(0x0142291c), SS8(0xff960e94), SS8(0xff9f3e17), SS8(0xfbd8f358),
    SS8(0xdbf79400), SS8(0xff405e01), SS8(0xff7d4914), SS8(0xff8b1a31),
    SS8(0xfc1417b8), SS8(0xdac7bb40), SS8(0xfdbb828c), SS8(0xff762170)
};

static const int32_t SBC_DECODER_PROTO_8_80M1[] =
{
    SS8(0xff7c272c), SS8(0xfcb02620), SS8(0xda612700), SS8(0xfcb02620),
    SS8(0xff7c272c), SS8(0xff762170), SS8(0xfdbb828c), SS8(0xdac7bb40),
    SS8(0xfc1417b8), SS8(0xff8b1a31), SS8(0xff7d4914), SS8(0xff405e01),
    SS8(0xdbf79400), SS8(0xfbd8f358), SS8(0xff9f3e17), SS8(0xff960e94),
    SS8(0x0142291c), SS8(0xdde26200), SS8(0xfbedadc0), SS8(0xffb54b3b),
    SS8(0xffc4e05c), SS8(0x03bf7948), SS8(0xe071bc00), SS8(0xfc3fbb68),
    SS8(0xffca00ed), SS8(0x000bb7db), SS8(0x06af2308), SS8(0xe3889d20),
    SS8(0xfcbc98e8), SS8(0xffdba705), SS8(0x006c1de4), SS8(0x0a00d410),
    SS8(0xe7054ca0), SS8(0xfd52986c), SS8(0xffe9811d), SS8(0x00e530da),
    SS8(0x0d9daee0), SS8(0xeac182c0), SS8(0xfdf1c8d4), SS8(0xfff5bd1a)
};

static const int32_t SBC_DECODER_SYNMATRIX_4[8][4] =
{
    { SN4(0x05a82798), SN4(0xfa57d868), SN4(0xfa57d868), SN4(0x05a82798) },
    { SN4(0x030fbc54), SN4(0xf89be510), SN4(0x07641af0), SN4(0xfcf043ac) },
    { SN4(0x00000000), SN4(0x00000000), SN4(0x00000000), SN4(0x00000000) },
    { SN4(0xfcf043ac), SN4(0x07641af0), SN4(0xf89be510), SN4(0x030fbc54) },
    { SN4(0xfa57d868), SN4(0x05a82798), SN4(0x05a82798), SN4(0xfa57d868) },
    { SN4(0xf89be510), SN4(0xfcf043ac), SN4(0x030fbc54), SN4(0x07641af0) },
    { SN4(0xf8000000), SN4(0xf8000000), SN4(0xf8000000), SN4(0xf8000000) },
    { SN4(0xf89be510), SN4(0xfcf043ac), SN4(0x030fbc54), SN4(0x07641af0) }
};

static const int32_t SBC_DECODER_SYNMATRIX_8[16][8] =
{
    { SN8(0x05a82798), SN8(0xfa57d868), SN8(0xfa57d868), SN8(0x05a82798),
      SN8(0x05a82798), SN8(0xfa57d868), SN8(0xfa57d868), SN8(0x05a82798) },
    { SN8(0x0471ced0), SN8(0xf8275a10), SN8(0x018f8b84), SN8(0x06a6d988),
      SN8(0xf9592678), SN8(0xfe70747c), SN8(0x07d8a5f0), SN8(0xfb8e3130) },
    { SN8(0x030fbc54), SN8(0xf89be510), SN8(0x07641af0), SN8(0xfcf043ac),
      SN8(0xfcf043ac), SN8(0x07641af0), SN8(0xf89be510), SN8(0x030fbc54) },
    { SN8(0x018f8b84), SN8(0xfb8e3130), SN8(0x06a6d988), SN8(0xf8275a10),
      SN8(0x07d8a5f0), SN8(0xf9592678), SN8(0x0471ced0), SN8(0xfe70747c) },
    { SN8(0x00000000), SN8(0x00000000), SN8(0x00000000), SN8(0x00000000),
      SN8(0x00000000), SN8(0x00000000), SN8(0x00000000), SN8(0x00000000) },
    { SN8(0xfe70747c), SN8(0x0471ced0), SN8(0xf9592678), SN8(0x07d8a5f0),
      SN8(0xf8275a10), SN8(0x06a6d988), SN8(0xfb8e3130), SN8(0x018f8b84) },
    { SN8(0xfcf043ac), SN8(0x07641af0), SN8(0xf89be510), SN8(0x030fbc54),
      SN8(0x030fbc54), SN8(0xf89be510), SN8(0x07641af0), SN8(0xfcf043ac) },
    { SN8(0xfb8e3130), SN8(0x07d8a5f0), SN8(0xfe70747c), SN8(0xf9592678),
      SN8(0x06a6d988), SN8(0x018f8b84), SN8(0xf8275a10), SN8(0x0471ced0) },
    { SN8(0xfa57d868), SN8(0x05a82798), SN8(0x05a82798), SN8(0xfa57d868),
      SN8(0xfa57d868), SN8(0x05a82798), SN8(0x05a82798), SN8(0xfa57d868) },
    { SN8(0xf9592678), SN8(0x018f8b84), SN8(0x07d8a5f0), SN8(0x0471ced0),
      SN8(0xfb8e3130), SN8(0xf8275a10), SN8(0xfe70747c), SN8(0x06a6d988) },
    { SN8(0xf89be510), SN8(0xfcf043ac), SN8(0x030fbc54), SN8(0x07641af0),
      SN8(0x07641af0), SN8(0x030fbc54), SN8(0xfcf043ac), SN8(0xf89be510) },
    { SN8(0xf8275a10), SN8(0xf9592678), SN8(0xfb8e3130), SN8(0xfe70747c),
      SN8(0x018f8b84), SN8(0x0471ced0), SN8(0x06a6d988), SN8(0x07d8a5f0) },
    { SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000),
      SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000) },
    { SN8(0xf8275a10), SN8(0xf9592678), SN8(0xfb8e3130), SN8(0xfe70747c),
      SN8(0x018f8b84), SN8(0x0471ced0), SN8(0x06a6d988), SN8(0x07d8a5f0) },
    { SN8(0xf89be510), SN8(0xfcf043ac), SN8(0x030fbc54), SN8(0x07641af0),
      SN8(0x07641af0), SN8(0x030fbc54), SN8(0xfcf043ac), SN8(0xf89be510) },
    { SN8(0xf9592678), SN8(0x018f8b84), SN8(0x07d8a5f0), SN8(0x0471ced0),
      SN8(0xfb8e3130), SN8(0xf8275a10), SN8(0xfe70747c), SN8(0x06a6d988) }
};

static int32_t sbc_decoder_frame_length_calc(SbcDecoderContext* sbc)
{
    int32_t frame_length;
    int32_t blocks       = (int32_t)sbc->frame.blocks;
    int32_t subbands     = (int32_t)sbc->frame.subbands;
    int32_t num_channels = (int32_t)sbc->num_channels;

    frame_length = 4 + ((4 * subbands * num_channels) >> 3);

    //for MONO and DUAL_CHANNEL
    if(sbc->frame.channel_mode < 2)
    {
        frame_length += (blocks * num_channels * (int32_t)(sbc->frame.bitpool) + 7) >> 3;
    }
    //for STEREO and JOINT_STEREO
    else
    {
        frame_length += ((sbc->frame.channel_mode == SBC_CHANNEL_MODE_JOINT_STEREO) * subbands + blocks * ((int32_t)sbc->frame.bitpool) + 7) >> 3;
    }
    
    return frame_length;
}

static inline void sbc_decoder_synthesize_four(SbcDecoderContext* sbc)
{
    int32_t i, j, k, ch, blk, idx;

    for(ch = 0; ch < sbc->num_channels; ch++)
    {
        int32_t* vfifo  = sbc->vfifo[ch];
        int32_t* offset = sbc->offset[ch];

        for(blk = 0; blk < sbc->frame.blocks; blk++)
        {
            int32_t  s;
            int32_t* pcm = &sbc->pcm_sample[ch][blk * 4];

            for(i = 0; i < 8; i++)
            {
                int32_t* synmatrix = (int32_t*)SBC_DECODER_SYNMATRIX_4[i];

                /* Shifting */
                if(--offset[i] < 0)
                {
                    offset[i] = 79;
                    memcpy(vfifo + 80, vfifo, 9 * sizeof(*vfifo));
                }

                /* Distribute the new matrix value to the shifted position */
                s  = synmatrix[0] * pcm[0];
                s += synmatrix[1] * pcm[1];
                s += synmatrix[2] * pcm[2];
                s += synmatrix[3] * pcm[3];

                vfifo[offset[i]] = SCALE4_STAGED1(s);
            }

            /* Compute the samples */
            for(idx = 0, i = 0; i < 4; i++, idx += 5)
            {
                j = offset[i];
                k = offset[i + 4];

                /* Store in output, Q0 */
                s  = vfifo[j + 0] * SBC_DECODER_PROTO_4_40M0[idx + 0];
                s += vfifo[k + 1] * SBC_DECODER_PROTO_4_40M1[idx + 0];
                s += vfifo[j + 2] * SBC_DECODER_PROTO_4_40M0[idx + 1];
                s += vfifo[k + 3] * SBC_DECODER_PROTO_4_40M1[idx + 1];
                s += vfifo[j + 4] * SBC_DECODER_PROTO_4_40M0[idx + 2];
                s += vfifo[k + 5] * SBC_DECODER_PROTO_4_40M1[idx + 2];
                s += vfifo[j + 6] * SBC_DECODER_PROTO_4_40M0[idx + 3];
                s += vfifo[k + 7] * SBC_DECODER_PROTO_4_40M1[idx + 3];
                s += vfifo[j + 8] * SBC_DECODER_PROTO_4_40M0[idx + 4];
                s += vfifo[k + 9] * SBC_DECODER_PROTO_4_40M1[idx + 4];

                *pcm++ = SCALE4_STAGED2(s);
            }
        }
    }
}

static inline void sbc_decoder_synthesize_eight(SbcDecoderContext* sbc)
{
    int32_t i, j, k, ch, blk, idx;

    for(ch = 0; ch < sbc->num_channels; ch++)
    {
        int32_t* vfifo  = sbc->vfifo[ch];
        int32_t* offset = sbc->offset[ch];

        for(blk = 0; blk < sbc->frame.blocks; blk++)
        {
            int32_t  s;
            int32_t* pcm = &sbc->pcm_sample[ch][blk * 8];

            for(i = 0; i < 16; i++)
            {
                int32_t* synmatrix = (int32_t*)SBC_DECODER_SYNMATRIX_8[i];

                /* Shifting */
                if(--offset[i] < 0)
                {
                    offset[i] = 159;
                    memcpy(vfifo + 160, vfifo, 9 * sizeof(*vfifo));
                }

                /* Distribute the new matrix value to the shifted position */
                s  = synmatrix[0] * pcm[0];
                s += synmatrix[1] * pcm[1];
                s += synmatrix[2] * pcm[2];
                s += synmatrix[3] * pcm[3];
                s += synmatrix[4] * pcm[4];
                s += synmatrix[5] * pcm[5];
                s += synmatrix[6] * pcm[6];
                s += synmatrix[7] * pcm[7];

                vfifo[offset[i]] = SCALE8_STAGED1(s);
            }

            /* Compute the samples */
            for(idx = 0, i = 0; i < 8; i++, idx += 5)
            {
                j = offset[i];
                k = offset[i + 8];

                /* Store in output */
                s  = vfifo[j + 0] * SBC_DECODER_PROTO_8_80M0[idx + 0];
                s += vfifo[k + 1] * SBC_DECODER_PROTO_8_80M1[idx + 0];
                s += vfifo[j + 2] * SBC_DECODER_PROTO_8_80M0[idx + 1];
                s += vfifo[k + 3] * SBC_DECODER_PROTO_8_80M1[idx + 1];
                s += vfifo[j + 4] * SBC_DECODER_PROTO_8_80M0[idx + 2];
                s += vfifo[k + 5] * SBC_DECODER_PROTO_8_80M1[idx + 2];
                s += vfifo[j + 6] * SBC_DECODER_PROTO_8_80M0[idx + 3];
                s += vfifo[k + 7] * SBC_DECODER_PROTO_8_80M1[idx + 3];
                s += vfifo[j + 8] * SBC_DECODER_PROTO_8_80M0[idx + 4];
                s += vfifo[k + 9] * SBC_DECODER_PROTO_8_80M1[idx + 4];

                *pcm++ = SCALE8_STAGED2(s);
            }
        }
    }
}

static void sbc_decoder_subband_synthesis_filter(SbcDecoderContext* sbc)
{
    switch(sbc->frame.subbands)
    {
    case 4:
        sbc_decoder_synthesize_four(sbc);
        break;
    case 8:
        sbc_decoder_synthesize_eight(sbc);
        break;
    default:
        break;
    }
}

int32_t sbc_decoder_init(SbcDecoderContext* sbc)
{
    int32_t ch, i;

    memset((void*)sbc, 0, sizeof(SbcDecoderContext));

    for(ch = 0; ch < 2; ch++)
    {
        for(i = 0; i < 8 * 2; i++)
        {
            sbc->offset[ch][i] = (10 * i + 10);
        }
    }

    return SBC_DECODER_ERROR_OK;
}

int32_t sbc_decoder_frame_decode(SbcDecoderContext* sbc, const uint8_t* data, int32_t length)
{
    int32_t ch;
    int32_t sb;
    int32_t blk;
    int32_t consumed;

    SbcCommonContext* frame = &sbc->frame;

    #if defined(USE_JAVA)

    /* frame_header */
    switch(data[0])
    {
    case SBC_SYNCWORD:
        frame->sample_rate_index  = (data[1] >> 6) & 0x03;
        frame->blocks             = (((data[1] >> 4) & 0x03) + 1) << 2;
        frame->channel_mode       = (data[1] >> 2) & 0x03;
        frame->allocation_method  = (data[1] >> 1) & 0x01;
        frame->subbands           = ((data[1] & 0x01) + 1) << 2;
        frame->bitpool            = data[2];
        break;
    case MSBC_SYNCWORD:
        frame->blocks = 15;
        if(data[1] || data[2])
        {
            frame->sample_rate_index  = (data[1] >> 6) & 0x03;
            frame->channel_mode       = (data[1] >> 2) & 0x03;
            frame->allocation_method  = (data[1] >> 1) & 0x01;
            frame->subbands           = ((data[1] & 0x01) + 1) << 2;
            frame->bitpool            = data[2];
        }
        else
        {
            frame->sample_rate_index  = 0;
            frame->channel_mode       = 0;
            frame->allocation_method  = 0;
            frame->subbands           = 8;
            frame->bitpool            = 26;
        }
        break;
    default:
        return SBC_DECODER_ERROR_SYNC_INCORRECT;
    }

    #else

    /* frame_header */
    SbcFrameHeader header;

    *(uint32_t*)&header = *(uint32_t*)data;

    switch(header.syncword)
    {
    case SBC_SYNCWORD:
        frame->sample_rate_index  = header.sample_rate_index;
        frame->blocks             = (header.block_mode + 1) << 2;
        frame->channel_mode       = header.channel_mode;
        frame->allocation_method  = header.allocation_method;
        frame->subbands           = (header.subband_mode + 1) << 2;
        frame->bitpool            = header.bitpool;
        break;
    case MSBC_SYNCWORD:
        frame->blocks = 10;
        if(data[1] || data[2])
        {
            frame->sample_rate_index  = header.sample_rate_index;
            frame->channel_mode       = header.channel_mode;
            frame->allocation_method  = header.allocation_method;
            frame->subbands           = (header.subband_mode + 1) << 2;
            frame->bitpool            = header.bitpool;
        }
        else
        {
            frame->sample_rate_index  = 0;
            frame->channel_mode       = 0;
            frame->allocation_method  = 0;
            frame->subbands           = 8;
            frame->bitpool            = 26;
        }
        break;
    default:
        return SBC_DECODER_ERROR_SYNC_INCORRECT;
    }

    #endif

    sbc->num_channels = frame->channel_mode == SBC_CHANNEL_MODE_MONO ? 1 : 2;
    sbc->pcm_length   = frame->blocks * frame->subbands;
    sbc->sample_rate  = sbc_common_sample_rate_get(frame->sample_rate_index);

    if(sbc_decoder_frame_length_calc(sbc) > length)
    {
        return SBC_DECODER_ERROR_STREAM_EMPTY;
    }

    if(((frame->channel_mode == SBC_CHANNEL_MODE_MONO   || frame->channel_mode == SBC_CHANNEL_MODE_DUAL_CHANNEL) && (frame->bitpool > (frame->subbands << 4))) ||
       ((frame->channel_mode == SBC_CHANNEL_MODE_STEREO || frame->channel_mode == SBC_CHANNEL_MODE_JOINT_STEREO) && (frame->bitpool > (frame->subbands << 5))))
    {
        return SBC_DECODER_ERROR_BITPOOL_OUT_BOUNDS;
    }

    consumed = 32;

    if(frame->channel_mode == SBC_CHANNEL_MODE_JOINT_STEREO)
    {
        int32_t  sb;
        uint32_t join = 0;

        for(sb = 0; sb < frame->subbands - 1; sb++)
        {
            join |= ((data[4] >> (7 - sb)) & 0x01) << sb;
        }

        frame->join = (uint8_t)join;

        consumed += frame->subbands;
    }

    /* scale_factor */
    for(ch = 0; ch < sbc->num_channels; ch++)
    {
        int8_t* sf = frame->scale_factor[ch];

        for(sb = 0; sb < frame->subbands; sb++)
        {
            sf[sb] = (data[consumed >> 3] >> (4 - (consumed & 0x7))) & 0x0F;;
            consumed += 4;
        }
    }

    /* bit_allocation */
    sbc_common_bit_allocation(frame);

    /* audio_samples & reconstruction */
    for(ch = 0; ch < sbc->num_channels; ch++)
    {
        int8_t*  bits   = frame->bits[ch];
        int32_t* levels = frame->mem[ch];

        for(sb = 0; sb < frame->subbands; sb++)
        {
            levels[sb] = (0x1 << bits[sb]) - 1;
        }
    }

    for(blk = 0; blk < frame->blocks; blk++)
    {
        for(ch = 0; ch < sbc->num_channels; ch++)
        {
            int8_t*  bits   = frame->bits[ch];
            int8_t*  sf     = frame->scale_factor[ch];
            int32_t* levels = frame->mem[ch];
            int32_t* pcm    = &sbc->pcm_sample[ch][blk * frame->subbands];

            for(sb = 0; sb < frame->subbands; sb++)
            {
                if(levels[sb] > 0)
                {
                    int32_t bit;
                    int32_t value = 0;

                    for(bit = 0; bit < bits[sb]; bit++)
                    {
                        if((data[consumed >> 3] >> (7 - (consumed & 0x7))) & 0x01)
                        {
                            value |= 1 << (bits[sb] - bit - 1);
                        }

                        consumed++;
                    }

                    *pcm++ = (((value << 1) | 1) << sf[sb]) / levels[sb] - (1 << sf[sb]);
                }
                else
                {
                    *pcm++ = 0;
                }
            }
        }
    }

    /* joint_stereo */
     if(frame->channel_mode == SBC_CHANNEL_MODE_JOINT_STEREO)
    {
        int32_t  idx, t0, t1;
        int32_t* pcm0 = sbc->pcm_sample[0];
        int32_t* pcm1 = sbc->pcm_sample[1];

        for(blk = 0; blk < frame->blocks; blk++)
        {
            idx = blk * frame->subbands;

            for(sb = 0; sb < frame->subbands; sb++)
            {
                if(frame->join & (0x01 << sb))
                {
                    t0 = pcm0[idx];
                    t1 = pcm1[idx];

                    pcm0[idx] = t0 + t1;
                    pcm1[idx] = t0 - t1;
                }

                idx++;
            }
        }
    }

    /* padding */
    consumed = ((consumed + 7) & 0xFFFFFFF8) >> 3;

    sbc_decoder_subband_synthesis_filter(sbc);

    if(sbc->num_channels == 2)
    {
        int32_t  i;
        int16_t* pcm16 = (int16_t*)sbc->pcm_sample + 1;
        int32_t* pcm32 = sbc->pcm_sample[1];

        for(i = 0; i < sbc->pcm_length; i++)
        {
            *pcm16 = *pcm32++;
            pcm16 += 2;
        }
    }
    else
    {
        int32_t  i;
        int16_t* pcm16 = (int16_t*)sbc->pcm_sample;
        int32_t* pcm32 = sbc->pcm_sample[0];

        for(i = 0; i < sbc->pcm_length; i++)
        {
            *pcm16++ = *pcm32++;
        }
    }

    return consumed;
}
