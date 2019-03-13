//
//  mediaDecoder_helper.hpp
//  hymediacore
//
//  Created by brainpoint on 7/5/16.
//  Copyright © 2016 citong. All rights reserved.
//

#ifndef mediaDecoder_helper_h
#define mediaDecoder_helper_h

namespace hymediacore {
    
    // The flush packet is a non-NULL packet with size 0 and data NULL
    inline int decode(AVCodecContext *avctx, AVFrame *frame, int *got_frame, AVPacket *pkt)
    {
        int ret;
        
        *got_frame = 0;
        
        if (pkt) {
            ret = avcodec_send_packet(avctx, pkt);
            // In particular, we don't expect AVERROR(EAGAIN), because we read all
            // decoded frames with avcodec_receive_frame() until done.
            if (ret < 0)
                return ret == AVERROR_EOF ? 0 : ret;
        }
        
        ret = avcodec_receive_frame(avctx, frame);
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
            return ret;
        if (ret >= 0)
            *got_frame = 1;
        
        return 0;
    }
    
}


#endif /* mediaDecoder_helper_h */
