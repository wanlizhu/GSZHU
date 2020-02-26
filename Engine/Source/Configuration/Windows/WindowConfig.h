#pragma once
#ifndef __ANDROID__

#include "Configuration/IConfig.h"

namespace Wanli
{
    struct WindowAttrbutes
    {
        bool fullscreen = false;
        bool resizable = true;
        bool borderless = false;
        bool floating = false;
    };

    class WANLI_API WindowConfig : public IConfig
    {
        DECL_SINGLETON_GET(WindowConfig);
        DECL_DATA_FIELD(Title, String, "Wanli Engine");
        DECL_DATA_FIELD(Size, glm::ivec2, glm::ivec2(0));
        DECL_DATA_FIELD(Pos, glm::ivec2, glm::ivec2(-1));
        DECL_DATA_FIELD(Attribs, WindowAttrbutes, {});
    };
}

#endif