//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// author:       lipengxiang
// description:
//

#ifndef __BP_mediaconverter_p_hpp__
#define __BP_mediaconverter_p_hpp__

#include "preheader.h"

namespace hymediacore
{
    class MediaConverter::PrivateClass
    {
    public:
        PrivateClass()
        : pfFrom(PixelFormat_NONE)
        , pfTo(PixelFormat_NONE)
        , _pFrame(NULL)
        , _pBuf(NULL)
        , _pImg_convert_ctx(NULL)
        {
            
        }
        
        ~PrivateClass()
        {
            uninstall();
        }
        
        /**
         * @desc
         * @return
         */
        ErrCode install( PixelFormat from, PixelFormat to, int width, int height )
        {
            uninstall();
            
            //allocate video frame and set its fileds to default value
            AVFrame* pFrame = av_frame_alloc();
            
            AVPixelFormat pix_fmt_dest = pf2ffpf(to);
            AVPixelFormat pix_fmt_src = pf2ffpf(from);
            
            int numBytes =
            av_image_get_buffer_size(pix_fmt_dest, width, height, mem_align);
            //avpicture_get_size( pix_fmt_dest, width, height);
            u8 *out_buffer = (u8*)av_malloc(numBytes*sizeof(u8));
            if (!out_buffer)
            {
                pFrame = NULL;
                return ErrCode_noMemory;
            }
            
            //Assign appropriate parts of buffer to image planes in pFrame
            //Note that pFrame is an AVFrame, but AVFrame is a superset of AVPicture
            int len = av_image_fill_arrays(pFrame->data, pFrame->linesize, out_buffer, pix_fmt_dest, width, height, mem_align);
//            int len = avpicture_fill((AVPicture*)pFrame, out_buffer, pix_fmt_dest, width, height);
            if (0 > len)
            {
                av_frame_free(&pFrame);
                av_free(out_buffer);
                return ErrCode_err;
            }
            
            pFrame->width = width;
            pFrame->height = height;
            pFrame->format = pix_fmt_dest;
            pFrame->pkt_size = numBytes;
            
            if (pix_fmt_src == AV_PIX_FMT_YUV420P && pix_fmt_dest == AV_PIX_FMT_RGB24)
            {
                _pImg_convert_ctx = NULL;
            }
            //     else if (pix_fmt_src == AV_PIX_FMT_RGB24 && pix_fmt_dest == AV_PIX_FMT_YUV420P)
            //     {
            //         frame.pImg_convert_ctx = NULL;
            //     }
            else
            {
                _pImg_convert_ctx
                = sws_getContext(
                                 width, height,
                                 pix_fmt_src,
                                 width, height,
                                 pix_fmt_dest, SWS_BICUBIC, NULL, NULL, NULL);
            }
            
            _pBuf = out_buffer;
            _pFrame = pFrame;
            pfTo = to;
            pfFrom = from;
            this->width = width;
            this->height = height;
            return ErrCode_ok;
        }
        
        /**
         * @desc
         * @return
         */
        void uninstall()
        {
            if (_pBuf)
            {
                av_free(_pBuf);
                _pBuf = NULL;
            }
            if (_pFrame)
            {
                av_frame_free(&_pFrame);
            }
            if (_pImg_convert_ctx)
            {
                sws_freeContext(_pImg_convert_ctx);
                _pImg_convert_ctx = NULL;
            }
            pfFrom = pfTo = PixelFormat_NONE;
            width = height = 0;
        }
        
        
//        //-----------------------------------------------
//        void vc_helper_convert( Media_convert& frame, u32 offsetY, u32 width, u32 height, u8* src[], int srcStride[], u8* dst[], int dstStride[] )
//        {
//            //-----------------------------------------------
//            // YUV420p -> RGB24
//            if (frame.pix_fmt_src == AV_PIX_FMT_YUV420P
//                && frame.pix_fmt_dest == AV_PIX_FMT_RGB24)
//            {
//                pix_helper_YUV420P_To_RGB888(
//                                             src[0], src[1], src[2], dst[0],
//                                             srcStride[0], srcStride[1], srcStride[2], dstStride[0],
//                                             width, height);
//            }
//            //-----------------------------------------------
//            //     // RGB24 -> YUV420p
//            //     else if (frame.pix_fmt_src == AV_PIX_FMT_RGB24
//            //         && frame.pix_fmt_dest == AV_PIX_FMT_YUV420P)
//            //     {
//            //         pix_helper_RGB888_To_YUV420P(
//            //             pFrameDest->data[0], pFrameDest->data[1], pFrameDest->data[2], pFrameSrc->data[0],
//            //             pFrameDest->linesize[0], pFrameDest->linesize[1], pFrameDest->linesize[2], pFrameSrc->linesize[0],
//            //             pFrameSrc->width, pFrameSrc->height);
//            //     }
//            //-----------------------------------------------
//            // other
//            else
//            {
//                sws_scale(
//                          frame.pImg_convert_ctx, src, srcStride,
//                          0, height, dst, dstStride);
//            }
//        }
        

        
        
    public:
        PixelFormat         pfFrom;
        PixelFormat         pfTo;
        int                 width, height;
        
        AVFrame*            _pFrame;
        u8*                 _pBuf;
        struct SwsContext*  _pImg_convert_ctx;
    };
    
} // end namespace hymediacore


#endif // end of define __BP_mediaconverter_p_hpp__