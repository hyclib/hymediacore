//
//  hymcMediaWriter.h
//  hymediacore
//
//  Created by brainpoint on 5/17/16.
//  Copyright © 2016 citong. All rights reserved.
//

#ifndef hymcMediaEncoder_h
#define hymcMediaEncoder_h


namespace hymediacore {
    
    //-----------------------------------------------
    // MediaEncodeInfo
    //-----------------------------------------------
    struct MediaEncodeInfo
    {
        bool            audio_enable;
        SampleFormat    audio_sample_fmt;
        int             audio_bit_rate;
        int             audio_sample_rate;
        Channels        audio_channels;
        
        bool            video_enable;
        int             video_bit_rate;
        int             video_width;
        int             video_height;
        int             video_frame_rate;   // frame per second.
    };
    
    //-----------------------------------------------
    // 编码器.
    //-----------------------------------------------
    class MediaEncoder {
    public:
        MediaEncoder();
        ~MediaEncoder();
        
        /**
         * @desc 设置编码信息.
         * @param codec:    如果为CodecID_NONE则根据文件扩展名自动选择编码格式,否则使用指定编码格式.
         * @return
         */
        ErrCode install( const MediaEncodeInfo& info, const tchar* outputfilename, CodecID codec = CodecID_NONE );
        
        ErrCode encodeHeader();
        ErrCode encodeVideoFrame(const u8 *src_data[4], const int src_linesizes[4], int width, int height);
        ErrCode encodeAudioFrame(const u8 *src_data[4], const int src_linesizes[4], int nb_samples);
        ErrCode encodeTrailer();
        
    private:
        PRIVATE_CLASS_DECLARE;
    };
    
    
} // end of namespace hymediacore.


#endif /* hymcMediaEncoder_h */