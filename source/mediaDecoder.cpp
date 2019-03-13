//
//  mediaDecoder.cpp
//  hymediacore
//
//  Created by brainpoint on 7/2/16.
//  Copyright © 2016 citong. All rights reserved.
//

#include "preheader.h"
#include "../include/mediaDecoder.h"
#include "mediaDecoder_p.hpp"
#include "mediaData_p.hpp"
#include "mediaDecoder_helper.hpp"

namespace hymediacore {
    
    MediaDecoder::MediaDecoder()
    {
        PRIVATE_CLASS_CREATE;
    }
    
    MediaDecoder::~MediaDecoder()
    {
        PRIVATE_CLASS_DESTROY;
    }
    
    //--------------------------------------
    ErrCode MediaDecoder::readOneFrame( MediaData* pData, OUT MediaAudioFrame*& pAudioFrame, OUT MediaVideoFrame*& pVideoFrame )
    {
        PRIVATE_CLASS_GET(p);
        PRIVATE_CLASS_GET_ANY(pData, ppData, MediaData);
        
        pAudioFrame = NULL;
        pVideoFrame = NULL;
        
        //allocate video frame and set its fileds to default value
        AVFrame*& pFrame = ppData->_pFrame;
        AVPacket& packet = ppData->_packet;
        
        int frameFinished = false;
        int ret = 0;
        
        if (ppData->_read_delay)
        {
            if (ppData->pVideoCodecCtx && ppData->pVideoCodecCtx->codec->capabilities & CODEC_CAP_DELAY)
            {
                uint8_t* data = packet.data;
                int size      = packet.size;
                
                if (ppData->_skip_video > 0)
                {
                    packet.data = NULL;
                    packet.size = 0;
                    
                    //ret = avcodec_decode_video2(ppData->pVideoCodecCtx, pFrame, &frameFinished, &packet);
                    ret = decode(ppData->pVideoCodecCtx, pFrame, &frameFinished, &packet);
                    
                    // we get a video frame.
                    if (ret >= 0)
                    {
                        if (frameFinished){
                            AVFrameToVideoFrame(pFrame, &p->frameVideo);
                            pVideoFrame = &p->frameVideo;
                        }
                    }
                    
                    ppData->_skip_video--;
                    
                } // if.
                
                if (ppData->_skip_audio > 0)
                {
                    packet.data = NULL;
                    packet.size = 0;
                    
                    //ret = avcodec_decode_audio4(ppData->pAudioCodecCtx, pFrame, &frameFinished, &packet);
                    ret = decode(ppData->pAudioCodecCtx, pFrame, &frameFinished, &packet);
                    
                    // we get a video frame.
                    if (ret >= 0)
                    {
                        if (frameFinished){
                            AVFrameToVideoFrame(pFrame, &p->frameVideo);
                            pVideoFrame = &p->frameVideo;
                        }
                    }
                    
                    ppData->_skip_audio--;
                }
                
                packet.data = data;
                packet.size = size;
                
                if (ppData->_skip_audio <= 0 && ppData->_skip_video <= 0)
                    return ErrCode_ok;
                
                return ErrCode_ok;
            }
            
            return ErrCode_ok;
        }
        else
        {
            //Read the next frame of a stream
            while (av_read_frame(ppData->pFormatCtx, &packet) >= 0)
            {
                //Is this a packet from the video stream?
                if (packet.stream_index == ppData->videoStream)
                {
                    //decode video frame of size packet.size from packet.data into picture
                    //ret = avcodec_decode_video2(ppData->pVideoCodecCtx, pFrame, &frameFinished, &packet);
                    ret = decode(ppData->pVideoCodecCtx, pFrame, &frameFinished, &packet);
                    //pFrame->pts = packet.pts;
                    
                    // we get a video frame.
                    if (ret >= 0)
                    {
                        if (frameFinished)
                        {
                            //                     print_all_line(_T("%d %d"), (int)pFrame->pkt_dts, (int)pFrame->coded_picture_number);
                            
                            
                            AVFrameToVideoFrame(pFrame, &p->frameVideo);
                            pVideoFrame = &p->frameVideo;
                            
                            if (ppData->videoStream > ppData->audioStream)
                                return ErrCode_ok;
                        }
                        else
                        {
                            ppData->_skip_video++;
                        }
                    }
                    else
                    {
                        error_all(_T("decode video error"));
                        //
//                        av_free_packet(&packet);
//                        av_frame_free(&pFrame);
                        
                        return ErrCode_err;
                    }
                }
                else if (packet.stream_index == ppData->audioStream)
                {
                    //ret = avcodec_decode_audio4(ppData->pAudioCodecCtx, pFrame, &frameFinished, &packet);
                    ret = decode(ppData->pAudioCodecCtx, pFrame, &frameFinished, &packet);
                    //pFrame->pts = packet.pts;
                    
                    // we get a audio frame.
                    if (ret >= 0)
                    {
                        if (frameFinished)
                        {
                            AVFrameToAudioFrame(pFrame, &p->frameAudio);
                            pAudioFrame = &p->frameAudio;
                            
                            if (ppData->videoStream < ppData->audioStream)
                                return ErrCode_ok;
                        }
                        else
                        {
                            ppData->_skip_audio++;
                        }
                    }
                    else
                    {
                        error_all(_T("decode audio error"));
//                            av_free_packet(&packet);
//                            av_frame_free(&pFrame);
                        return ErrCode_err;
                    }
                }
            } // while.
            
            ppData->_read_delay = true;
        } // if.
        
        return ErrCode_ok;
    }
    
    
} // end namespace hymediacore
