#pragma once

#include "Wanlix/Core3D/Utility/NonCopyable.h"
#include "Wanlix/Core3D/Platform/Win32/Win32LeanAndMean.h"
#include <Windows.h>
#include <stdexcept>

namespace Wanlix
{
    class Win32WindowClass : public NonCopyable
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