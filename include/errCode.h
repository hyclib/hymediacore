//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// hymediacore
// author:       lipengxiang
// description:
//

#ifndef __BP_errcode_H_20120307180744__
#define __BP_errcode_H_20120307180744__



namespace hymediacore
{
    enum ErrCode {
        ErrCode_ok          = 0,
        
        ErrCode_err         = 1,
        ErrCode_param,
        ErrCode_noStream,
        ErrCode_noMemory,
        ErrCode_fileLock,       // 文件被锁住无法操作.
    };
    
} // end namespace hymediacore


#endif // end of define __BP_errcode_H_20120307180744__