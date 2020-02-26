#pragma once

#ifndef __ANDROID__
#include <Windows.h>

namespace Wanli
{
    using WindowHandle = HWND;
    using LibraryHandle = HMODULE;
}

#endif