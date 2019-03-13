//
//  mediaConverter.cpp
//  hymediacore
//
//  Created by brainpoint on 7/2/16.
//  Copyright © 2016 citong. All rights reserved.
//

#include "preheader.h"
#include "../include/mediaConverter.h"
#include "mediaConverter_p.hpp"

namespace hymediacore {
    
    MediaConverter::MediaConverter()
    {
        PRIVATE_CLASS_CREATE;
    }
    
    MediaConverter::~MediaConverter()
    {
        PRIVATE_CLASS_DESTROY;
    }
    
    //--------------------------------------
    ErrCode MediaConverter::install( PixelFormat from, PixelFormat to, int width, int height )
    {
        PRIVATE_CLASS_GET(p);
        return p->install(from, to, width, height);
    }
    
    //--------------------------------------
    PixelFormat MediaConverter::pixDest() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->pfTo;
    }
    //--------------------------------------
    PixelFormat MediaConverter::pixSrc() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->pfFrom;
    }
    //--------------------------------------
    int         MediaConverter::width() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->width;
    }
    //--------------------------------------
    int         MediaConverter::height() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->height;
    }

    
    
    
} // end namespace hymediacore.
