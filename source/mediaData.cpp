//
//  mediaData.cpp
//  hymediacore
//
//  Created by brainpoint on 5/21/16.
//  Copyright © 2016 citong. All rights reserved.
//

#include "preheader.h"
#include "../include/mediaData.h"

#include "mediaData_p.hpp"

namespace hymediacore
{
    //-----------------------------------------------
    MediaData::MediaData()
    {
        PRIVATE_CLASS_CREATE;
    }

    //-----------------------------------------------
    MediaData::MediaData(const tchar* filename)
    {
        PRIVATE_CLASS_CREATE;
        PRIVATE_CLASS_GET(p);
        
        AVFormatContext* pFormatCtx = NULL;
        
#if defined(UNICODE) || defined(_UNICODE)
        cString cStr;
        const char* filenamec = StringUtils::StrToStr(filename, cStr);
#else
        const char* filenamec = filename;
#endif
        
        //-----------------------------------------------
        // open file.
        if (0 != avformat_open_input(&pFormatCtx, filenamec, NULL, NULL))
        {
            error_all(_T("can't open the media file %s"), filename);
            return;
        }
        
        // get stream.
        if (0 > avformat_find_stream_info(pFormatCtx, NULL))
        {
            error_all(_T("can't get the media stream %s"), filename);
            avformat_close_input(&pFormatCtx);
            return;
        }
        
        //-----------------------------------------------
        // find the first stream
        unsigned int i;
        p->audioStream = -1;
        p->videoStream = -1;
        
        for (i = 0; i < pFormatCtx->nb_streams; i++) {
            if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && p->audioStream < 0) {
                p->audioStream = i;
                continue;
            }
            if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && p->videoStream < 0) {
                p->videoStream = i;
                continue;
            }
        }
        
        if (p->audioStream == -1 && p->videoStream == -1)
        {
            error_all(_T("can't find media stream %s"), filename);
            avformat_close_input(&pFormatCtx);
            return;
        }
        
        //c = st->codec;
        AVCodecContext* pAudioCodecCtx = NULL;
        if (p->audioStream != -1)
        {
            pAudioCodecCtx = avcodec_alloc_context3(NULL);
            if (!pAudioCodecCtx) {
                error_all(_T("alloc codec context err"));
                return;
            }
            if (0 > avcodec_parameters_to_context(pAudioCodecCtx, pFormatCtx->streams[p->audioStream]->codecpar)) {
                error_all(_T("Could not copy the stream parameters"));
                avcodec_free_context(&pAudioCodecCtx);
                return;
            }
        }
        AVCodecContext* pVideoCodecCtx = NULL;
        if (p->videoStream != -1)
        {
            pVideoCodecCtx = avcodec_alloc_context3(NULL);
            if (!pVideoCodecCtx) {
                error_all(_T("alloc codec context err"));
                if (pAudioCodecCtx) avcodec_free_context(&pAudioCodecCtx);
                return;
            }
            if (0 > avcodec_parameters_to_context(pVideoCodecCtx, pFormatCtx->streams[p->videoStream]->codecpar)) {
                error_all(_T("Could not copy the stream parameters"));
                if (pAudioCodecCtx) avcodec_free_context(&pAudioCodecCtx);
                avcodec_free_context(&pVideoCodecCtx);
                return;
            }
        }
        
        // find the codec.
        if (pVideoCodecCtx)
        {
            AVCodec* pCodec = avcodec_find_decoder(pVideoCodecCtx->codec_id);
            if (NULL == pCodec)
            {
                error_all(_T("can't find the codec v %s"), filename);
                avformat_close_input(&pFormatCtx);
                if (pAudioCodecCtx) avcodec_free_context(&pAudioCodecCtx);
                if (pVideoCodecCtx) avcodec_free_context(&pVideoCodecCtx);
                return;
            }
            
            //        if (pCodec->capabilities & CODEC_CAP_DELAY)
            //            pVideoCodecCtx->flags |= CODEC_FLAG_LOW_DELAY;
            
            // deal truncated stream.
            if (pCodec->capabilities & CODEC_CAP_TRUNCATED)
                pVideoCodecCtx->flags |= CODEC_FLAG_TRUNCATED;
            
#ifdef CN_PLATFORM_MOBILE
            pVideoCodecCtx->idct_algo = FF_IDCT_ARM;
#endif
            
            // open codec.
            if (0 > avcodec_open2(pVideoCodecCtx, pCodec, NULL))
            {
                error_all(_T("can't open the codec %s"), filename);
                avformat_close_input(&pFormatCtx);
                if (pAudioCodecCtx) avcodec_free_context(&pAudioCodecCtx);
                if (pVideoCodecCtx) avcodec_free_context(&pVideoCodecCtx);
                return;
            }
        }
        if (pAudioCodecCtx)
        {
            AVCodec* pCodec = avcodec_find_decoder(pAudioCodecCtx->codec_id);
            if (NULL == pCodec)
            {
                error_all(_T("can't find the codec a %s"), filename);
                
                if (pAudioCodecCtx) avcodec_free_context(&pAudioCodecCtx);
                if (pVideoCodecCtx) { avcodec_close(pVideoCodecCtx); avcodec_free_context(&pVideoCodecCtx); }
                avformat_close_input(&pFormatCtx);
                return;
            }
            
            //        if (pCodec->capabilities & CODEC_CAP_DELAY)
            //            pAudioCodecCtx->flags |= CODEC_FLAG_LOW_DELAY;
            
            // deal truncated stream.
            if (pCodec->capabilities & CODEC_CAP_TRUNCATED)
                pAudioCodecCtx->flags |= CODEC_FLAG_TRUNCATED;
            
            // open codec.
            if (0 > avcodec_open2(pAudioCodecCtx, pCodec, NULL))
            {
                error_all(_T("can't open the codec %s"), filename);
                
                if (pAudioCodecCtx) avcodec_free_context(&pAudioCodecCtx);
                if (pVideoCodecCtx) { avcodec_close(pVideoCodecCtx); avcodec_free_context(&pVideoCodecCtx); }
                avformat_close_input(&pFormatCtx);
                return;
            }
        }
        
        p->pFormatCtx = pFormatCtx;
        p->pAudioCodecCtx = pAudioCodecCtx;
        p->pVideoCodecCtx = pVideoCodecCtx;
        
        //allocate video frame and set its fileds to default value
        p->_pFrame = av_frame_alloc();
        if (p->pVideoCodecCtx)
        {
            int numBytes =
            av_image_get_buffer_size(p->pVideoCodecCtx->pix_fmt, p->pVideoCodecCtx->width, p->pVideoCodecCtx->height, mem_align);
            //avpicture_get_size(p->pVideoCodecCtx->pix_fmt, p->pVideoCodecCtx->width, p->pVideoCodecCtx->height);
            av_new_packet(&p->_packet, numBytes);
            
            p->_infoVideo.width = p->pVideoCodecCtx->width;
            p->_infoVideo.height = p->pVideoCodecCtx->height;
            p->_infoVideo.bit_rate = p->pVideoCodecCtx->bit_rate;
            p->_infoVideo.frame_rate = p->pVideoCodecCtx->time_base.den;
        }
        
        if (p->pAudioCodecCtx)
        {
            p->_infoAudio.sample_fmt = (SampleFormat)p->pAudioCodecCtx->sample_fmt;
            p->_infoAudio.sample_rate = p->pAudioCodecCtx->sample_rate;
            p->_infoAudio.bit_rate = p->pAudioCodecCtx->bit_rate;
            p->_infoAudio.channels = p->pAudioCodecCtx->channels;
        }
        
        p->_skip_audio = 0;
        p->_skip_video = 0;
        p->_read_delay = false;
        
        //output file information 
#ifdef _DEBUG
        av_dump_format(p->pFormatCtx, 0, filenamec, 0);
#endif // _DEBUG
        
        return;
    }
    
    //-----------------------------------------------
    MediaData::~MediaData()
    {
        PRIVATE_CLASS_GET(p);
        
        if (p->_pFrame)
        {
            // TODO: av_packet_free.
            av_packet_unref(&p->_packet);
            av_frame_free(&p->_pFrame);
        }
        
        if (p->pVideoCodecCtx) { avcodec_close(p->pVideoCodecCtx); avcodec_free_context(&p->pVideoCodecCtx); }
        if (p->pAudioCodecCtx) { avcodec_close(p->pAudioCodecCtx); avcodec_free_context(&p->pAudioCodecCtx); }
        
        if (p->pFormatCtx)
        {
            avformat_close_input(&p->pFormatCtx);
        }

        
        PRIVATE_CLASS_DESTROY;
    }
    
    //-----------------------------------------------
    bool MediaData::isValidate() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->pFormatCtx != NULL;
    }
    
    //-----------------------------------------------
    const MediaVideoInfo& MediaData::videoInfo() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->_infoVideo;
    }
    
    //-----------------------------------------------
    const MediaAudioInfo& MediaData::audioInfo() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->_infoAudio;
    }
    
    //-----------------------------------------------
    bool MediaData::videoValidate() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->pVideoCodecCtx != NULL;
    }
    
    //-----------------------------------------------
    bool MediaData::audioValidate() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->pAudioCodecCtx != NULL;
    }
}
