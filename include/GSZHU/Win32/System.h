#pragma once

#include <GSZHU/Config.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#ifdef CreateWindow
#undef CreateWindow
#endif

namespace GSZHU::System {
    GSZHU_API HWND CreateWindow(const char* Name, LONG Width, LONG Height, WNDPROC WinProc, bool VSync, HINSTANCE Instance);
}