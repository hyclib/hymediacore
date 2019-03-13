//
//  mediaEncoder_helper.hpp
//  hymediacore
//
//  Created by brainpoint on 7/3/16.
//  Copyright © 2016 citong. All rights reserved.
//

#ifndef mediaEncoder_helper_h
#define mediaEncoder_helper_h

namespace hymediacore {
    
    
#define STREAM_FRAME_RATE   25 /* 25 images/s */
#define STREAM_PIX_FMT      AV_PIX_FMT_YUV420P /* default pix_fmt */

    /* check that a given sample format is supported by the encoder */
    inline bool check_sample_fmt(AVCodec *codec, enum AVSampleFormat sample_fmt)
    {
        const enum AVSampleFormat *p = codec->sample_fmts;
        
        while (*p != AV_SAMPLE_FMT_NONE) {
            if (*p == sample_fmt)
                return true;
            p++;
        }
        return false;
    }
    
    /* just pick the highest supported samplerate */
    inline int select_sample_rate(AVCodec *codec)
    {
        const int *p;
        int best_samplerate = 0;
        
        if (!codec->supported_samplerates)
            return 44100;
        
        p = codec->supported_samplerates;
        while (*p) {
            best_samplerate = FFMAX(*p, best_samplerate);
            p++;
        }
        return best_samplerate;
    }
    
    /* select layout with the highest channel count */
    inline int select_channel_layout(AVCodec *codec)
    {
        const uint64_t *p;
        uint64_t best_ch_layout = 0;
        int best_nb_channels   = 0;
        
        if (!codec->channel_layouts)
            return AV_CH_LAYOUT_STEREO;
        
        p = codec->channel_layouts;
        while (*p) {
            int nb_channels = av_get_channel_layout_nb_channels(*p);
            
            if (nb_channels > best_nb_channels) {
                best_ch_layout    = *p;
                best_nb_channels = nb_channels;
            }
            p++;
        }
        return (int)best_ch_layout;
    }
    
    //----------------------------------------------
    inline AVStream *add_video_stream(AVFormatContext *oc, enum AVCodecID codec_id, const MediaEncodeInfo& writerInfo, OUT AVCodecContext*& c)
    {
//        AVCodecContext *c;
        AVStream *st;
        AVCodec *codec;
        
        st = avformat_new_stream(oc, NULL);
        if (!st) {
            error_all(_T("Could not alloc stream"));
            return NULL;
        }
        
//        c = st->codec;
        
        /* find the video encoder */
        codec = avcodec_find_encoder(codec_id);
        if (!codec) {
            error_all(_T("codec not found"));
            return NULL;
        }
        //avcodec_get_context_defaults3(c, codec);
        c = avcodec_alloc_context3(codec);
        if (!c) {
            error_all(_T("alloc codec context err"));
            return NULL;
        }
        
        AVRational rate = av_make_q(1,writerInfo.video_frame_rate);
        c->codec_id = codec_id;
        
        /* put sample parameters */
        c->bit_rate = writerInfo.video_bit_rate;
        c->bit_rate_tolerance = 4000000;
        
        /* resolution must be a multiple of two */
        c->width = writerInfo.video_width;
        c->height = writerInfo.video_height;
        
        /* time base: this is the fundamental unit of time (in seconds) in terms
         of which frame timestamps are represented. for fixed-fps content,
         timebase should be 1/framerate and timestamp increments should be
         identically 1. */
        c->time_base = rate;
        st->time_base = rate;
        c->gop_size = 12; /* emit one intra frame every twelve frames at most */
        c->pix_fmt = STREAM_PIX_FMT;
        
        if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
            /* just for testing, we also add B frames */
            c->max_b_frames = 2;
        }
        if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO){
            /* Needed to avoid using macroblocks in which some coeffs overflow.
             This does not happen with normal video, it just happens here as
             the motion of the chroma plane does not match the luma plane. */
            c->mb_decision=2;
        }
        
        // some formats want stream headers to be separate
        if (oc->oformat->flags & AVFMT_GLOBALHEADER)
            c->flags |= CODEC_FLAG_GLOBAL_HEADER;
        
        //av_opt_set(c->priv_data, "preset", "fast", 0);
        //av_opt_set(c->priv_data, "preset", "superfast", 0);
        av_opt_set(c->priv_data, "preset", "ultrafast", 0);
        av_opt_set(c->priv_data, "tune", "zerolatency", 0);
        
        /* copy the stream parameters to the muxer */
        if (0 > avcodec_parameters_from_context(st->codecpar, c)) {
            error_all(_T("Could not copy the stream parameters"));
            avcodec_free_context(&c);
            return NULL;
        }
        
        return st;
    }
    
    //----------------------------------------------
    inline AVStream *add_audio_stream(AVFormatContext *oc, enum AVCodecID codec_id, const MediaEncodeInfo& writerInfo, OUT AVCodecContext*& c)
    {
//        AVCodecContext *c;
        AVStream *st;
        
        st = avformat_new_stream(oc, NULL);
        if (!st) {
            error_all(_T("Could not alloc stream"));
            return NULL;
        }
        st->id = 1;
        
        st->time_base.num = 1;
        st->time_base.den = writerInfo.audio_sample_rate;
        
        //c = st->codec;
        c = avcodec_alloc_context3(NULL);
        if (!c) {
            error_all(_T("alloc codec context err"));
            return NULL;
        }
        c->codec_id = codec_id;
        c->codec_type = AVMEDIA_TYPE_AUDIO;
        
        /* put sample parameters */
        c->sample_fmt   = AVSampleFormat(writerInfo.audio_sample_fmt);
        c->bit_rate     = writerInfo.audio_bit_rate;
        c->sample_rate  = writerInfo.audio_sample_rate;
        c->channel_layout = c->channels;
        c->channels     = (writerInfo.audio_channels == AV_CH_LAYOUT_MONO) ? 1 : 2;
        
        // some formats want stream headers to be separate
        if (oc->oformat->flags & AVFMT_GLOBALHEADER)
            c->flags |= CODEC_FLAG_GLOBAL_HEADER;
        
        /* copy the stream parameters to the muxer */
        if (0 > avcodec_parameters_from_context(st->codecpar, c)) {
            error_all(_T("Could not copy the stream parameters"));
            avcodec_free_context(&c);
            return NULL;
        }
        
        return st;
    }

    
    //----------------------------------------------
    inline bool open_audio(AVFormatContext *oc, AVStream *st, AVCodecContext* c, OUT AVFrame*& pFrame )
    {
        AVCodecParameters* cp;
        AVCodec *codec;
        
//        c = st->codec;
        cp = st->codecpar;
        
        /* find the audio encoder */
        codec = avcodec_find_encoder(cp->codec_id);
        if (!codec) {
            error_all(_T("codec not found"));
            return false;
        }
        
        if (!check_sample_fmt(codec, (AVSampleFormat)cp->format))
        {
            error_all(_T("no support sample fmt%d"), (int)cp->format);
            return false;
        }
        
        /* select other audio parameters supported by the encoder */
        int bestSample = select_sample_rate(codec);
        if (cp->sample_rate > bestSample)
            cp->sample_rate = bestSample;
        
        int bestChannel = select_channel_layout(codec);
        if (bestChannel == AV_CH_LAYOUT_MONO)
            cp->channel_layout = AV_CH_LAYOUT_MONO;
        cp->channels       = av_get_channel_layout_nb_channels(cp->channel_layout);
        
        if (0 > avcodec_parameters_to_context(c, cp)) {
            error_all(_T("Could not copy the stream parameters"));
            return false;
        }
        
        /* open it */
        if (avcodec_open2(c, codec, NULL) < 0) {
            error_all(_T("could not open codec"));
            return false;
        }
        
        assert(!pFrame);
        pFrame = av_frame_alloc();
        if (!pFrame) {
            fprintf(stderr, "Could not allocate audio frame\n");
            avcodec_close(c);
            return false;
        }
        
        pFrame->nb_samples = c->frame_size;
        pFrame->format = c->sample_fmt;
        pFrame->channel_layout = c->channel_layout;
        
        return true;
    }
    
    //-----------------------------------------------
    inline bool open_video(AVFormatContext *oc, AVStream *st, AVCodecContext* c, OUT AVFrame*& pFrame)
    {
        AVCodecParameters* cp;
        AVCodec *codec;
//        AVCodecContext *c;
        
//        c = st->codec;
        cp = st->codecpar;
        
        /* find the video encoder */
        codec = avcodec_find_encoder(cp->codec_id);
        if (!codec) {
            error_all(_T("codec not found"));
            return false;
        }
        
        /* open the codec */
        if (avcodec_open2(c, codec, NULL) < 0) {
            error_all(_T("could not open codec"));
            return false;
        }
        
        assert(!pFrame);
        pFrame = av_frame_alloc();
        if (!pFrame) {
            error_all(_T("Could not allocate video frame"));
            avcodec_close(c);
            return false;
        }
        
        pFrame->format = c->pix_fmt;
        pFrame->width  = c->width;
        pFrame->height = c->height;
        
        /* Allocate the encoded raw picture. */
        if (0 > av_frame_get_buffer(pFrame, mem_align))
        {
            error_all(_T("Could not allocate picture"));
            av_frame_free(&pFrame);
            avcodec_close(c);
            return false;
        }
//        int ret = avpicture_alloc(pPicture, c->pix_fmt, c->width, c->height);
//        if (ret < 0) {
//            error_all(_T("Could not allocate picture"));
//            av_frame_free(&pFrame);
//            return false;
//        }
//        
//        /* copy data and linesize picture pointers to frame */
//        *((AVPicture *)pFrame) = *pPicture;
        
        return true;
    }
    
}

#endif /* mediaEncoder_helper_h */
