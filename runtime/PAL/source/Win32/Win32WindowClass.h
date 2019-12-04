#pragma once

#include <Windows.h>
#include <stdexcept>

namespace Wanlix
{
    class Win32WindowClass 
    {
    public:
        static LRESULT CALLBACK WindowProc(
            HWND wnd,
            UINT msg,
            WPARAM wParam,
            LPARAM lParam
        );
        static Win32WindowClass* Instance();

        ~Win32WindowClass();
        const wchar_t* GetName() const;

    private:
        Win32WindowClass();
    };
}