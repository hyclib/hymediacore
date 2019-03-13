//
//  mediaEncoder.hpp
//  hymediacore
//
//  Created by brainpoint on 7/2/16.
//  Copyright © 2016 citong. All rights reserved.
//

#ifndef mediaEncoder_hpp
#define mediaEncoder_hpp

#include "mediaEncoder_helper.hpp"

namespace hymediacore {
    
    
    class MediaEncoder::PrivateClass
    {
    public:
        PrivateClass()
        :pFormatCtx(NULL)
        ,pAudioStream(NULL)
        ,pVideoStream(NULL)
        ,pAudioCodec(NULL)
        ,pVideoCodec(NULL)
        ,pVideoFrame(NULL)
        ,pAudioFrame(NULL)
        ,samples_count(0)
        {
        }
        
        ~PrivateClass()
        {
            uninstall();
        }
        
        //-----------------------------------------------
        ErrCode install( const MediaEncodeInfo& info, const tchar* outputfilename, CodecID codec )
        {
            uninstall();
            
            AVFrame* pFrameAudio = NULL;
            AVFrame* pFrameVideo = NULL;
            AVCodecContext* pAudioCodec = NULL;
            AVCodecContext* pVideoCodec = NULL;
            
#if defined(UNICODE) || defined(_UNICODE)
            cString cname;
            StringUtils::StrToStr(outputfilename, cname);
            const char* fn = cname.c_str();
#else
            const char* fn = outputfilename;
#endif
            AVOutputFormat *fmt = NULL;
            AVFormatContext *oc = NULL;
            AVStream *audio_st = NULL, *video_st = NULL;
            
            /* allocate the output media context */
            avformat_alloc_output_context2(&oc, NULL, NULL, fn);
            if (!oc) {
                error_all(_T("Could not deduce output format from file extension: using MPEG; %s"), outputfilename);
                return ErrCode_err;
            }
            
            fmt = oc->oformat;
            
#ifdef _DEBUG
            av_dump_format(oc, 0, fn, 1);
#endif // _DEBUG
            
            /* open the output file, if needed */
            if (!(fmt->flags & AVFMT_NOFILE)) {
                if (avio_open(&oc->pb, fn, AVIO_FLAG_WRITE) < 0) {
                    error_all(_T("Could not open '%s'"), outputfilename);
                    avformat_free_context(oc);
                    //av_free(oc);
                    return ErrCode_fileLock;
                }
            }
            
            if (info.video_enable && fmt->video_codec != AV_CODEC_ID_NONE) {
                video_st = add_video_stream(oc, fmt->video_codec, info, pVideoCodec);
            }
            if (info.audio_enable && fmt->audio_codec != AV_CODEC_ID_NONE) {
                audio_st = add_audio_stream(oc, fmt->audio_codec, info, pAudioCodec);
            }
            
            if (video_st)
            {
                if (!open_video(oc, video_st, pVideoCodec, pFrameVideo))
                {
                    if (pVideoCodec)
                    {
                        avcodec_free_context(&pVideoCodec);
                    }
                    if (pAudioCodec)
                    {
                        avcodec_free_context(&pAudioCodec);
                    }
                    
                    if (oc)
                    {
                        if (!(oc->oformat->flags & AVFMT_NOFILE))
                        {
                            avio_close(oc->pb);
                        }
                        
                        avformat_free_context(oc);
                        //                av_free(oc);
                        oc = NULL;
                    }
                    
                    //av_free(oc);
                    return ErrCode_err;
                }
            }
            if (audio_st)
            {
                if (!open_audio(oc, audio_st, pAudioCodec, pFrameAudio))
                {
                    if (pVideoCodec)
                    {
                        avcodec_close(pVideoCodec);
                        avcodec_free_context(&pVideoCodec);
                    }
                    if (pAudioCodec)
                    {
                        avcodec_free_context(&pAudioCodec);
                    }
                    if (pFrameVideo)
                    {
                        av_frame_free(&pFrameVideo);
                    }
                    
                    if (oc)
                    {
                        if (!(oc->oformat->flags & AVFMT_NOFILE))
                        {
                            avio_close(oc->pb);
                        }
                        
                        avformat_free_context(oc);
                        //                av_free(oc);
                        oc = NULL;
                    }
                    
                    //av_free(oc);
                    
                    return ErrCode_err;
                }
            }
            
            this->pFormatCtx = oc;
            this->pAudioStream = audio_st;
            this->pVideoStream = video_st;
            this->pVideoFrame = pFrameVideo;
            this->pAudioFrame = pFrameAudio;
            this->pVideoCodec = pVideoCodec;
            this->pAudioCodec = pAudioCodec;
            this->samples_count = 0;
            return ErrCode_ok;
        }
        
        void uninstall()
        {
            samples_count = 0;
            
            if (pAudioCodec)
            {
                avcodec_close(pAudioCodec);
                avcodec_free_context(&pAudioCodec);
            }
            if (pAudioStream)
            {
                //av_free_packet(&writer.pkta);
                pAudioStream = NULL;
            }
            if (pAudioFrame)
                av_frame_free(&pAudioFrame);
            
            if (pVideoCodec)
            {
                avcodec_close(pVideoCodec);
                avcodec_free_context(&pVideoCodec);
            }
            if (pVideoStream)
            {
                pVideoStream = NULL;
            }
            if (pVideoFrame)
            {
                av_frame_free(&pVideoFrame);
            }
            
            if (pFormatCtx)
            {
                if (!(pFormatCtx->oformat->flags & AVFMT_NOFILE))
                {
                    avio_close(pFormatCtx->pb);
                }
                
                avformat_free_context(pFormatCtx);
//                av_free(pFormatCtx);
                pFormatCtx = NULL;
            }
        }
        
    public:
        AVFormatContext*    pFormatCtx;
        AVStream*           pAudioStream;
        AVStream*           pVideoStream;
        
        AVCodecContext*     pAudioCodec;
        AVCodecContext*     pVideoCodec;
        
        AVFrame*            pVideoFrame;
        AVFrame*            pAudioFrame;
        int                 samples_count;
    };
    
}

#endif /* mediaEncoder_hpp */
