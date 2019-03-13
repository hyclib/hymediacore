//
//  hymcReader.h
//  hymediacore
//
//  Created by brainpoint on 5/17/16.
//  Copyright © 2016 citong. All rights reserved.
//

#ifndef hymcMediaDecoder_h
#define hymcMediaDecoder_h


namespace hymediacore {
    
#define MC_NUM_DATA_POINTERS 8
    
    //-----------------------------------------------
    // a media frame data.
    //-----------------------------------------------
    struct MediaFrame {
        /**
         * pointer to the picture/channel planes.
         */
        u8*     data[MC_NUM_DATA_POINTERS];
        
        /**
         * For video, size in bytes of each picture line.
         * For audio, size in bytes of each plane.
         *
         * For audio, only linesize[0] may be set. For planar audio, each channel
         * plane must be the same size.
         *
         * For video the linesizes should be multiples of the CPUs alignment
         * preference, this is 16 or 32 for modern desktop CPUs.
         * Some code requires such alignment other code can be slower without
         * correct alignment, for yet other it makes no difference.
         *
         * @note The linesize may be larger than the size of usable data -- there
         * may be extra padding present for performance reasons.
         */
        int     linesize[MC_NUM_DATA_POINTERS];
        
        /**
         * Presentation timestamp in time_base units (time when frame should be shown to user).
         */
        s64     pts;
        
        /**
         * quality (between 1 (good) and 256*128-1 (bad))
         */
        int     quality;
    };
    
    //-----------------------------------------------
    // a video media frame data.
    //-----------------------------------------------
    struct MediaVideoFrame : MediaFrame {
        
        PixelFormat     format;
        int             width, height;
        bool            key_frame;
        FrameType       frame_type;

        /**
         * Sample aspect ratio for the video frame, 0/1 if unknown/unspecified.
         */
        int             sample_aspect_ratio_width;
        int             sample_aspect_ratio_height;
    };
    
    
    //-----------------------------------------------
    // a audio media frame data.
    //-----------------------------------------------
    struct MediaAudioFrame : MediaFrame {
        
        SampleFormat    format;
        
        /**
         * number of audio samples (per channel) described by this frame.
         */
        int             nb_samples;
        
        /**
         * Sample rate of the audio data.
         */
        int             sample_rate;
        
        /**
         * Channel layout of the audio data.
         */
        u64             channel_layout;
        
        /**
         * number of audio channels, only used for audio.
         */
        int             channels;
    };
    
    //-----------------------------------------------
    // 解码器.
    //-----------------------------------------------
    class MediaDecoder {
    public:
        MediaDecoder();
        ~MediaDecoder();
        
        /**
         * @brief read frames from media.
         *        if pAudioFrame and pVideoFrame == NULL, return ErrCode_ok means decode to end.
         * @return.
         */
        ErrCode    readOneFrame( MediaData* pData, OUT MediaAudioFrame*& pAudioFrame, OUT MediaVideoFrame*& pVideoFrame );
    
    private:
        PRIVATE_CLASS_DECLARE;
    };
    
    
} // end of namespace hymediacore.



#endif /* hymcMediaDecoder_h */


