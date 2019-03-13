//
//  pixelFormatConvert.hpp
//  hymediacore
//
//  Created by brainpoint on 5/17/16.
//  Copyright © 2016 citong. All rights reserved.
//

#ifndef pixelformatConvert_h
#define pixelformatConvert_h

#include "preheader.h"

namespace hymediacore {
    
    // 转换对应的值到ffmpeg pixel format
    inline enum AVPixelFormat pf2ffpf(enum PixelFormat pf)
    {
        switch (pf) {
            case PixelFormat_NONE:
                return AV_PIX_FMT_NONE;
                
                // 565.
            case PixelFormat_RGB565BE:
                return AV_PIX_FMT_RGB565BE;
            case PixelFormat_RGB565LE:
                return AV_PIX_FMT_RGB565LE;
            case PixelFormat_BGR565BE:
                return AV_PIX_FMT_BGR565BE;
            case PixelFormat_BGR565LE:
                return AV_PIX_FMT_BGR565LE;

                // RGB.
            case PixelFormat_RGB24:
                return AV_PIX_FMT_RGB24;
            case PixelFormat_BGR24:
                return AV_PIX_FMT_BGR24;
                
                // ARGB.
            case PixelFormat_ARGB:
                return AV_PIX_FMT_ARGB;
            case PixelFormat_RGBA:
                return AV_PIX_FMT_RGBA;
            case PixelFormat_ABGR:
                return AV_PIX_FMT_ABGR;
            case PixelFormat_BGRA:
                return AV_PIX_FMT_BGRA;
                
                // YUV420.
            case PixelFormat_YUV420P:
                return AV_PIX_FMT_YUV420P;
            case PixelFormat_NV12:
                return AV_PIX_FMT_NV12;
            case PixelFormat_NV21:
                return AV_PIX_FMT_NV21;
                
            case PixelFormat_YUV420P9BE:
                return AV_PIX_FMT_YUV420P9BE;
            case PixelFormat_YUV420P9LE:
                return AV_PIX_FMT_YUV420P9LE;
                
            case PixelFormat_YUV420P10BE:
                return AV_PIX_FMT_YUV420P10BE;
            case PixelFormat_YUV420P10LE:
                return AV_PIX_FMT_YUV420P10LE;
                
            case PixelFormat_YUV420P12BE:
                return AV_PIX_FMT_YUV420P12BE;
            case PixelFormat_YUV420P12LE:
                return AV_PIX_FMT_YUV420P12LE;
                
            case PixelFormat_YUV420P14BE:
                return AV_PIX_FMT_YUV420P14BE;
            case PixelFormat_YUV420P14LE:
                return AV_PIX_FMT_YUV420P14LE;
                
            case PixelFormat_YUV420P16LE:
                return AV_PIX_FMT_YUV420P16LE;
            case PixelFormat_YUV420P16BE:
                return AV_PIX_FMT_YUV420P16BE;
                
            default:
                return AV_PIX_FMT_NONE;
        }
    }
    
}

#endif /* pixelformatConvert_h */
