//
//  mediaData_p.hpp
//  hymediacore
//
//  Created by brainpoint on 5/21/16.
//  Copyright © 2016 citong. All rights reserved.
//

#ifndef mediaData_p_hpp
#define mediaData_p_hpp

namespace hymediacore {
    
    class MediaData::PrivateClass
    {
    public:
        PrivateClass()
            : pFormatCtx(NULL)
            , pAudioCodecCtx(NULL)
            , audioStream(0)
            , _skip_audio(0)
            , pVideoCodecCtx(NULL)
            , videoStream(0)
            , _skip_video(0)
            , _pFrame(NULL)
            , _read_delay(false)
        {
            ZeroMemory(&_infoVideo, sizeof(_infoVideo));
            ZeroMemory(&_infoAudio, sizeof(_infoAudio));
        }
    public:
        AVFormatContext*    pFormatCtx;
        
        AVCodecContext*     pAudioCodecCtx;
        int                 audioStream;
        int                 _skip_audio;
        
        AVCodecContext*     pVideoCodecCtx;
        int                 videoStream;
        int                 _skip_video;
        
        AVFrame*            _pFrame;
        AVPacket            _packet;
        bool                _read_delay;
        
        MediaVideoInfo      _infoVideo;
        MediaAudioInfo      _infoAudio;
    };
    
}

#endif /* mediaData_p_hpp */
