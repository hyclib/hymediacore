//
//  mediaConverter.h
//  hymediacore
//
//  Created by brainpoint on 7/1/16.
//  Copyright © 2016 citong. All rights reserved.
//

#ifndef mediaConverter_h
#define mediaConverter_h

namespace hymediacore {
    
    //-----------------------------------------------
    // convert media data from a format to another format.
    //-----------------------------------------------
    class MediaConverter
    {
    public:
        MediaConverter();
        ~MediaConverter();
        
        /**
         * @desc 创建转换
         */
        ErrCode     install( PixelFormat from, PixelFormat to, int width, int height );
        
        /**
         * @desc 转换参数
         */
        PixelFormat pixDest() const;
        PixelFormat pixSrc() const;
        int         width() const;
        int         height() const;
        
    private:
        PRIVATE_CLASS_DECLARE;
    };
}

#endif /* mediaConverter_h */
