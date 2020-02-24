#pragma once

#ifdef PLATFORM_WINDOWS
#include <Windows.h>

namespace Wanli
{
    using WindowHandle = HWND;
    using LibraryHandle = HMODULE;
}

#endif