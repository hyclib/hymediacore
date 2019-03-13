//
// Copyright(c) 1999-2016 hyrj. All rights reserved.
// hy media core library
// create at:    2016/05/10
// author:       lipengxiang
// description:  
//

#include "preheader.h"


//-----------------------------------------------
inline void dll_init()
{
}

//-----------------------------------------------
inline void dll_destory()
{
}


#if CN_PLATFORM == CN_PLATFORM_WINDOWS
//----------------------------------------------------------------------------------------------------
//*	dll初始化
//----------------------------------------------------------------------------------------------------

class DLLInit
{
public:
    /*---------------------------
    初始化
    ---------------------------*/
    DLLInit()
    {
        dll_init();
    }

    /*---------------------------
    析构
    ---------------------------*/
    ~DLLInit()
    {
        dll_destory();

    }
};
static DLLInit g_dllInit;

#else

void dll_main_init() __attribute__((constructor));
void dll_main_destory() __attribute__((destructor));

void dll_main_init() {
    dll_init();
}

void dll_main_destory() {
    dll_destory();
}
#endif
