#pragma once

#include "Win32/System_Win32.h"

namespace GSZHU {
    LRESULT CALLBACK WinProc_Win32(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
}