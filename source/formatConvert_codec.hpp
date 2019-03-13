//
//  formatConvert_codec.hpp
//  hymediacore
//
//  Created by brainpoint on 7/3/16.
//  Copyright © 2016 citong. All rights reserved.
//

#ifndef formatConvert_codec_h
#define formatConvert_codec_h

#include "preheader.h"

namespace hymediacore {
    
    inline enum AVCodecID cd2ffcd(enum CodecID cd)
    {
        switch (cd) {
                /* video codecs */
            case CodecID_VIDEO_MPEG1VIDEO: return AV_CODEC_ID_MPEG1VIDEO;
            case CodecID_VIDEO_MPEG2VIDEO: return AV_CODEC_ID_MPEG2VIDEO;
            case CodecID_VIDEO_MPEG4: return AV_CODEC_ID_MPEG4;
            case CodecID_VIDEO_RAWVIDEO: return AV_CODEC_ID_RAWVIDEO;
            case CodecID_VIDEO_FLV1: return AV_CODEC_ID_FLV1;
            case CodecID_VIDEO_H263: return AV_CODEC_ID_H263;
            case CodecID_VIDEO_H264: return AV_CODEC_ID_H264;
            case CodecID_VIDEO_H265: return AV_CODEC_ID_H265;
                
            case CodecID_VIDEO_VP6: return AV_CODEC_ID_VP6;
            case CodecID_VIDEO_VP6F: return AV_CODEC_ID_VP6F;
            case CodecID_VIDEO_VP6A: return AV_CODEC_ID_VP6A;
            case CodecID_VIDEO_VP7: return AV_CODEC_ID_VP7;
                
            case CodecID_VIDEO_MJPEG: return AV_CODEC_ID_MJPEG;
            case CodecID_VIDEO_LJPEG: return AV_CODEC_ID_LJPEG;
            case CodecID_VIDEO_JPEGLS: return AV_CODEC_ID_JPEGLS;
                
            case CodecID_VIDEO_PNG: return AV_CODEC_ID_PNG;
            case CodecID_VIDEO_BMP: return AV_CODEC_ID_BMP;
            case CodecID_VIDEO_JPEG2000: return AV_CODEC_ID_JPEG2000;
            case CodecID_VIDEO_TIFF: return AV_CODEC_ID_TIFF;
            case CodecID_VIDEO_GIF: return AV_CODEC_ID_GIF;
                
                /* audio codecs */
            case CodecID_AUDIO_MP1: return AV_CODEC_ID_MP1;
            case CodecID_AUDIO_MP2: return AV_CODEC_ID_MP2;
            case CodecID_AUDIO_MP3: return AV_CODEC_ID_MP3;
            case CodecID_AUDIO_AAC: return AV_CODEC_ID_AAC;
            case CodecID_AUDIO_AC3: return AV_CODEC_ID_AC3;
            case CodecID_AUDIO_DTS: return AV_CODEC_ID_DTS;
            case CodecID_AUDIO_WMAV1: return AV_CODEC_ID_WMAV1;
            case CodecID_AUDIO_WMAV2: return AV_CODEC_ID_WMAV2;
            case CodecID_AUDIO_FLAC: return AV_CODEC_ID_FLAC;
            case CodecID_AUDIO_MP3ADU: return AV_CODEC_ID_MP3ADU;
            case CodecID_AUDIO_MP3ON4: return AV_CODEC_ID_MP3ON4;
            case CodecID_AUDIO_APE: return AV_CODEC_ID_APE;
                
            default:
                return AV_CODEC_ID_NONE;
        }
    }
    
}

#endif /* formatConvert_codec_h */
