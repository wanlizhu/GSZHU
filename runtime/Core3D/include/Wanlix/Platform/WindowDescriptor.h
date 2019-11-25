#pragma once

#include "Wanlix/Core/Types.h"
#include <string>

namespace Wanlix
{
    struct WindowDescriptor
    {
        std::wstring title;
        Offset position;
        Extent size;
        bool visible = true;
        bool borderless = false;
        bool resizable = true;
        bool acceptDropFiles = true;
        bool centered = true;
        void* parent = nullptr;
    };
}