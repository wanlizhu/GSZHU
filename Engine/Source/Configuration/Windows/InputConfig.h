#pragma once
#ifndef __ANDROID__

#include "Configuration/IConfig.h"

namespace Wanli
{
    class Window;

    class WANLI_API InputConfig : public IConfig
    {
        DECL_SINGLETON_GET(InputConfig);
    };
}

#endif