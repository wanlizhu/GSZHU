#include "Win32WindowClass.h"
#include <Windows.h>
#include <stdexcept>

namespace Wanlix
{
    Win32WindowClass* Win32WindowClass::Instance() {
        static Win32WindowClass _instance;
        return &_instance;
    }

    Win32WindowClass::~Win32WindowClass() {
        ::UnregisterClassW(GetName(), GetModuleHandle(nullptr));
    }

    const wchar_t* Win32WindowClass::GetName() const {
        return L"__Wanlix_Win32_Window_Class__";
    }

    Win32WindowClass::Win32WindowClass()
    {
        // Setup window class information
        WNDCLASSW wc;
        ZeroMemory(&wc, sizeof(WNDCLASSW));
        HINSTANCE hinst = ::GetModuleHandleW(nullptr);

        wc.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS);
        wc.hInstance = hinst;
        wc.lpfnWndProc = reinterpret_cast<WNDPROC>(&Win32WindowClass::WindowProc);
        wc.hIcon = ::LoadIconW(hinst, IDI_APPLICATION);
        wc.hCursor = ::LoadCursorW(hinst, IDC_ARROW);
        #ifdef WANLIX_ARCH_ARM
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
        #else
        wc.hbrBackground = reinterpret_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
        #endif
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = GetName();

        // Register window class 
        if (!::RegisterClassW(&wc)) {
            throw std::runtime_error("failed to register window class");
        }
    }
}