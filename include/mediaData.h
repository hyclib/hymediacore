//
//  hymcMediaData.h
//  hymediacore
//
//  Created by brainpoint on 5/17/16.
//  Copyright © 2016 citong. All rights reserved.
//

#ifndef hymcMediaData_h
#define hymcMediaData_h

namespace hymediacore {
    
    //-----------------------------------------------
    // 视频信息.
    //-----------------------------------------------
    struct MediaVideoInfo {
        s64           bit_rate;
        int           frame_rate;
        int           width;
        int           height;
    };

    //-----------------------------------------------
    // 音频信息.
    //-----------------------------------------------
    struct MediaAudioInfo {
        s64           bit_rate;
        SampleFormat  sample_fmt;
        int           sample_rate;
        int           channels;
    };
    
    //-----------------------------------------------
    // 表示媒体数据, 可以是一段文件,流媒体,或数据.
    //-----------------------------------------------
    class MediaData {
    public:
        MediaData();
        MediaData( const tchar* filename );
        ~MediaData();

        /**
         * @desc 判断媒体数据是否有效.
         */
        bool                    isValidate() const;
        
        /**
         * @desc media info.
         */
        const MediaVideoInfo&   videoInfo() const;
        
        /**
         * @desc 判断视频是否有效.
         */
        bool                    videoValidate() const;
        
        
        const MediaAudioInfo&   audioInfo() const;
        bool                    audioValidate() const;
        
    private:
        PRIVATE_CLASS_DECLARE;
        
        friend class MediaDecoder;
    };
    
}

#endif /* hymcMediaData_h */