#pragma once

#include "Wanlix/Core3D/Types.h"
#include <string>

namespace Wanlix
{
    struct WindowDescriptor
    {
        std::weak_ptr<Context> context;
        std::wstring title = L"Untitled";
        Offset position = { 0, 0 };
        Extent size = { 0, 0 };
        bool visible = true;
        bool borderless = false;
        bool resizable = true;
        bool acceptDropFiles = true;
        bool centered = true;
        void* parent = nullptr;
    };
}