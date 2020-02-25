#pragma once
#ifdef PLATFORM_WINDOWS

#include "Configuration/IConfig.h"

namespace Wanli
{
    class WANLI_API GraphicsConfig : public IConfig::Registrar<GraphicsConfig>
    {
        DECL_DATA_FIELD(NativeWindow, WindowHandle, 0);
        DECL_DATA_FIELD(EnableDebug, bool, (_DEBUG == 1));
        DECL_DATA_FIELD(BackbufferCount, int, 2);
        DECL_DATA_FIELD(BackgroundColor, Color, Color_AutoCAD);
        DECL_DATA_FIELD(AsyncDraw, bool, true);
    };
}

#endif