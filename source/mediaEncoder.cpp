//
//  mediaEncoder.cpp
//  hymediacore
//
//  Created by brainpoint on 7/2/16.
//  Copyright © 2016 citong. All rights reserved.
//

#include "preheader.h"
#include "../include/mediaEncoder.h"
#include "mediaEncoder_p.hpp"

namespace hymediacore {
    
    MediaEncoder::MediaEncoder()
    {
        PRIVATE_CLASS_CREATE;
    }
    
    MediaEncoder::~MediaEncoder()
    {
        PRIVATE_CLASS_DESTROY;
    }
    
    ErrCode MediaEncoder::install( const MediaEncodeInfo& info, const tchar* outputfilename, CodecID codec )
    {
        PRIVATE_CLASS_GET(p);
        return p->install(info, outputfilename, codec);
    }
    
    ErrCode MediaEncoder::encodeHeader()
    {
        PRIVATE_CLASS_GET(p);
        
        if (p->pVideoFrame)
            p->pVideoFrame->pts = 0;
        if (p->pAudioFrame)
            p->pAudioFrame->pts = 0;
        
        /* write the stream header, if any */
        if (0 > avformat_write_header(p->pFormatCtx, NULL))
        {
            return ErrCode_err;
        }
        
        return ErrCode_ok;
    }
    
    ErrCode MediaEncoder::encodeVideoFrame(const u8 *src_data[4], const int src_linesizes[4], int width, int height)
    {
        PRIVATE_CLASS_GET(p);
        
        //     assert(pFrame->format == STREAM_PIX_FMT);
        if (!p->pVideoStream)
            return ErrCode_noStream;
        
        if (width != p->pVideoCodec->width || height != p->pVideoCodec->height)
            return ErrCode_param;
        
        av_image_copy(p->pVideoFrame->data, p->pVideoFrame->linesize,
                      src_data, src_linesizes,
                      STREAM_PIX_FMT, width, height);
        
        int ret;
        AVStream* st = p->pVideoStream;
        AVCodecContext *c = p->pVideoCodec;
        
        //writer.pFrame->pts = pFrame->pts;
        p->pVideoFrame->pts += av_rescale_q(1, c->time_base, st->time_base);
        
        if (p->pFormatCtx->oformat->flags & AVFMT_RAWPICTURE) {
            /* Raw video case - directly store the picture in the packet */
            AVPacket pkt;
            av_init_packet(&pkt);
            
            pkt.flags |= AV_PKT_FLAG_KEY;
            pkt.stream_index = st->index;
            pkt.data = p->pVideoFrame->data[0];
            pkt.size = sizeof(AVPicture);
            
            ret = av_interleaved_write_frame(p->pFormatCtx, &pkt);
            av_packet_unref(&pkt);
            //av_free_packet(&pkt);
        } else {
            AVPacket pkt = { 0 };
            av_init_packet(&pkt);
            int got_packet;
            
            /* encode the image */
            ret = avcodec_encode_video2(c, &pkt, p->pVideoFrame, &got_packet);
            if (ret < 0) {
                error_all(_T("Error encoding video frame"));
                av_packet_unref(&pkt);
                //av_free_packet(&pkt);
                return ErrCode_err;
            }
            
            /* If size is zero, it means the image was buffered. */
            if (!ret && got_packet && pkt.size) {
                pkt.stream_index = st->index;
                
                /* Write the compressed frame to the media file. */
                ret = av_interleaved_write_frame(p->pFormatCtx, &pkt);
                
                //             if (c->codec->capabilities & CODEC_CAP_DELAY)
                //             {
                //                 /* encode the image */
                //                 ret = avcodec_encode_video2(c, &pkt, NULL, &got_packet);
                //                 if (ret < 0) {
                //                     error_all(_T("Error encoding video frame"));
                //                     return;
                //                 }
                //
                //                 /* If size is zero, it means the image was buffered. */
                //                 if (!ret && got_packet && pkt.size) {
                //                     pkt.stream_index = st->index;
                // 
                //                     /* Write the compressed frame to the media file. */
                //                     ret = av_interleaved_write_frame(writer.pFormatCtx, &pkt);
                //                 } else {
                //                     ret = 0;
                //                 }
                //             }
                
            }
            
            av_packet_unref(&pkt);
            //av_free_packet(&pkt);
        }
        if (ret != 0) {
            error_all(_T("Error while writing video frame"));
            return ErrCode_err;
        }
        
        return ErrCode_ok;
    }
    
    ErrCode MediaEncoder::encodeAudioFrame(const u8 *src_data[4], const int src_linesizes[4], int nb_samples)
    {
        PRIVATE_CLASS_GET(p);
        if (!p->pAudioStream)
            return ErrCode_noStream;
        
        AVStream *st = p->pAudioStream;
        AVCodecContext *c = p->pAudioCodec;
        
        int bufLen = src_linesizes[0];
        const u8* buf = src_data[0];
        int buffer_size = av_samples_get_buffer_size(NULL, c->channels, c->frame_size,
                                                     c->sample_fmt, 0);
        while (bufLen >= buffer_size)
        {
            AVPacket pkt = { 0 };
            av_init_packet(&pkt);
            int got_packet;
            
            /* encode the audio */
            AVRational rate;
            rate.num = 1;
            rate.den = c->sample_rate;
            
            p->pAudioFrame->pts = av_rescale_q(p->samples_count, rate, c->time_base);
            p->samples_count += p->pAudioFrame->nb_samples;
            
            
            /* setup the data pointers in the AVFrame */
            int ret = avcodec_fill_audio_frame(p->pAudioFrame, c->channels, c->sample_fmt,
                                               buf, buffer_size, 0);
            buf += buffer_size;
            bufLen -= buffer_size;
            
            if (ret < 0) {
                error_all(_T("Could not setup audio frame"));
                av_packet_unref(&pkt);
                //av_free_packet(&pkt);
                return ErrCode_err;
            }
            
            ret = avcodec_encode_audio2(c, &pkt, p->pAudioFrame, &got_packet);
            if (ret < 0) {
                error_all(_T("Error encoding audio frame"));
                av_packet_unref(&pkt);
                //av_free_packet(&pkt);
                return ErrCode_err;
            }
            
            if (!ret && got_packet)
            {
                //av_packet_rescale_ts(&pkt, c->time_base, oStream->time_base);
                pkt.stream_index = st->index;
                //pkt.pts = destFrame->pts;
                /* Write the compressed frame to the media file. */
                ret = av_interleaved_write_frame(p->pFormatCtx, &pkt);
            }
            
            if (ret != 0) {
                error_all(_T("Error while writing audio frame"));
                av_packet_unref(&pkt);
                //av_free_packet(&pkt);
                return ErrCode_err;
            }
            
            av_packet_unref(&pkt);
            //av_free_packet(&pkt);
            
        } // while.
        
        return ErrCode_ok;
    }
    
    ErrCode MediaEncoder::encodeTrailer()
    {
        PRIVATE_CLASS_GET(p);
        
        /* write the stream trailer */
        if (0 == av_write_trailer(p->pFormatCtx))
            return ErrCode_ok;
        return ErrCode_err;
    }
    
    
} // end namespace hymediacore
