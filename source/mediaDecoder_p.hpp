//
//  mediaEncoder.hpp
//  hymediacore
//
//  Created by brainpoint on 7/2/16.
//  Copyright © 2016 citong. All rights reserved.
//

#ifndef mediaDecoder_hpp
#define mediaDecoder_hpp

namespace hymediacore {
    
    inline void AVFrameToVideoFrame( AVFrame* pFrame, MediaVideoFrame* pVFrame )
    {
        for (int i = 0; i < MC_NUM_DATA_POINTERS; i++) {
            pVFrame->data[i] = pFrame->data[i];
            pVFrame->linesize[i] = pFrame->linesize[i];
        }
        
        pVFrame->pts        = pFrame->pts;
        pVFrame->quality    = pFrame->quality;
        pVFrame->format     = PixelFormat(pFrame->format);
        pVFrame->width      = pFrame->width;
        pVFrame->height     = pFrame->height;
        pVFrame->key_frame  = (pFrame->key_frame == 1);
        pVFrame->frame_type  = FrameType(pFrame->pict_type);
        pVFrame->sample_aspect_ratio_width     = pFrame->sample_aspect_ratio.num;
        pVFrame->sample_aspect_ratio_height    = pFrame->sample_aspect_ratio.den;
    }
    
    inline void AVFrameToAudioFrame( AVFrame* pFrame, MediaAudioFrame* pVFrame )
    {
        for (int i = 0; i < MC_NUM_DATA_POINTERS; i++) {
            pVFrame->data[i] = pFrame->data[i];
            pVFrame->linesize[i] = pFrame->linesize[i];
        }
        
        pVFrame->pts        = pFrame->pts;
        pVFrame->quality    = pFrame->quality;
        pVFrame->format     = SampleFormat(pFrame->format);
        pVFrame->nb_samples      = pFrame->nb_samples;
        pVFrame->sample_rate     = pFrame->sample_rate;
        pVFrame->channel_layout  = pFrame->channel_layout;
        pVFrame->channels        = pFrame->channels;
    }
    
    
    //-----------------------------------------------
    // MediaDecoder::PrivateClass
    //-----------------------------------------------
    class MediaDecoder::PrivateClass
    {
    public:
        PrivateClass()
        {
            
        }
        
        ~PrivateClass()
        {
            
        }
        
    public:
        MediaVideoFrame frameVideo;
        MediaAudioFrame frameAudio;
    };
    
}

#endif /* mediaDecoder_hpp */
