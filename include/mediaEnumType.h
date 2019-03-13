//
//  mediaEnumType.h
//  hymediacore
//
//  Created by brainpoint on 7/3/16.
//  Copyright © 2016 citong. All rights reserved.
//

#ifndef mediaEnumType_h
#define mediaEnumType_h

namespace hymediacore {
    
    //-----------------------------------------------
    // 影像的存储像素格式
    //-----------------------------------------------
    enum PixelFormat
    {
        PixelFormat_NONE = -1,
        
        // 565.
        PixelFormat_RGB565BE,  ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), big-endian
        PixelFormat_RGB565LE,  ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), little-endian
        
        PixelFormat_BGR565BE,  ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), big-endian
        PixelFormat_BGR565LE,  ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), little-endian
        
        // RGB.
        PixelFormat_RGB24,     ///< packed RGB 8:8:8, 24bpp, RGBRGB...
        PixelFormat_BGR24,     ///< packed RGB 8:8:8, 24bpp, BGRBGR...
        
        // ARGB.
        PixelFormat_ARGB,      ///< packed ARGB 8:8:8:8, 32bpp, ARGBARGB...
        PixelFormat_RGBA,      ///< packed RGBA 8:8:8:8, 32bpp, RGBARGBA...
        PixelFormat_ABGR,      ///< packed ABGR 8:8:8:8, 32bpp, ABGRABGR...
        PixelFormat_BGRA,      ///< packed BGRA 8:8:8:8, 32bpp, BGRABGRA...
        
        // YUV420.
        PixelFormat_YUV420P,   ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
        PixelFormat_NV12,      ///< planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)
        PixelFormat_NV21,      ///< as above, but U and V bytes are swapped
        
        PixelFormat_YUV420P9BE, ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
        PixelFormat_YUV420P9LE, ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
        
        PixelFormat_YUV420P10BE,///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
        PixelFormat_YUV420P10LE,///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
        
        PixelFormat_YUV420P12BE, ///< planar YUV 4:2:0,18bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
        PixelFormat_YUV420P12LE, ///< planar YUV 4:2:0,18bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
        
        PixelFormat_YUV420P14BE, ///< planar YUV 4:2:0,21bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
        PixelFormat_YUV420P14LE, ///< planar YUV 4:2:0,21bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
        
        PixelFormat_YUV420P16LE,  ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
        PixelFormat_YUV420P16BE,  ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
    };
    
    //-----------------------------------------------
    // 帧格式.
    //-----------------------------------------------
    enum FrameType
    {
        FrameType_NONE = 0, ///< Undefined
        FrameType_I,     ///< Intra
        FrameType_P,     ///< Predicted
        FrameType_B,     ///< Bi-dir predicted
        FrameType_S,     ///< S(GMC)-VOP MPEG4
        FrameType_SI,    ///< Switching Intra
        FrameType_SP,    ///< Switching Predicted
        FrameType_BI,    ///< BI type
    };
    
    
    //-----------------------------------------------
    // 音频采样格式.
    //-----------------------------------------------
    enum SampleFormat
    {
        SampleFormat_NONE = -1,
        SampleFormat_U8,          ///< unsigned 8 bits
        SampleFormat_S16,         ///< signed 16 bits
        SampleFormat_S32,         ///< signed 32 bits
        SampleFormat_FLT,         ///< float
        SampleFormat_DBL,         ///< double
        
        SampleFormat_U8P,         ///< unsigned 8 bits, planar
        SampleFormat_S16P,        ///< signed 16 bits, planar
        SampleFormat_S32P,        ///< signed 32 bits, planar
        SampleFormat_FLTP,        ///< float, planar
        SampleFormat_DBLP,        ///< double, planar
    };
    
    //-----------------------------------------------
    // 音频通道数.
    //-----------------------------------------------
    enum Channels
    {
        Channels_stereo   = 0x00000001|0x00000002,
        Channels_mono     = 0x00000004,
    };
    
    //-----------------------------------------------
    // 编码类型.
    //-----------------------------------------------
    enum CodecID {
        CodecID_NONE,
        
        /* video codecs */
        CodecID_VIDEO_MPEG1VIDEO,
        CodecID_VIDEO_MPEG2VIDEO, ///< preferred ID for MPEG-1/2 video decoding
        CodecID_VIDEO_MPEG4,
        CodecID_VIDEO_RAWVIDEO,
        CodecID_VIDEO_FLV1,
        CodecID_VIDEO_H263,
        CodecID_VIDEO_H264,
        CodecID_VIDEO_H265,
        
        CodecID_VIDEO_VP6,
        CodecID_VIDEO_VP6F,
        CodecID_VIDEO_VP6A,
        CodecID_VIDEO_VP7,
        
        CodecID_VIDEO_MJPEG,
        CodecID_VIDEO_LJPEG,
        CodecID_VIDEO_JPEGLS,
        
        CodecID_VIDEO_PNG,
        CodecID_VIDEO_BMP,
        CodecID_VIDEO_JPEG2000,
        CodecID_VIDEO_TIFF,
        CodecID_VIDEO_GIF,
        
        /* audio codecs */
        CodecID_AUDIO_MP1,
        CodecID_AUDIO_MP2,
        CodecID_AUDIO_MP3, ///< preferred ID for decoding MPEG audio layer 1, 2 or 3
        CodecID_AUDIO_AAC,
        CodecID_AUDIO_AC3,
        CodecID_AUDIO_DTS,
        CodecID_AUDIO_WMAV1,
        CodecID_AUDIO_WMAV2,
        CodecID_AUDIO_FLAC,
        CodecID_AUDIO_MP3ADU,
        CodecID_AUDIO_MP3ON4,
        CodecID_AUDIO_APE,
    };

} // end namespace hymediacore.

#endif /* mediaEnumType_h */
