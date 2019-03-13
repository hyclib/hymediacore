//
//  preheader.h
//  hymediacore
//
//  Created by brainpoint on 5/17/16.
//  Copyright © 2016 citong. All rights reserved.
//

#ifndef preheader_h
#define preheader_h

//-----------------------------------------------
// ffmpeg
#ifdef __cplusplus
extern "C"
{
#endif
    
#define __STDC_CONSTANT_MACROS
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
    
#ifdef __cplusplus
};
#endif


#include "../include/hymediacorePlatform.h"
#include "../include/mediaData.h"

#include "formatConvert_pixel.hpp"
#include "formatConvert_codec.hpp"

#define mem_align   32


#endif /* preheader_h */
