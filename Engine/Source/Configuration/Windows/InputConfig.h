#pragma once
#ifdef PLATFORM_WINDOWS

#include "Configuration/IConfig.h"

namespace Wanli
{
    class Window;

    class WANLI_API InputConfig : public IConfig::Registrar<InputConfig>
    {
        DECL_DATA_FIELD(Window, SharedPtr<Window>, nullptr);
    };
}

#endif