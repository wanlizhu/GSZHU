#pragma once
#ifndef __ANDROID__

#include "Inputs/Inputs.h"

namespace Wanli
{
    class WANLI_API WindowsInputManager final : public InputManagerImpl
    {
    public:
        WindowsInputManager();
        virtual ~WindowsInputManager();

        virtual void Update() override final;

    private:
        static LRESULT CALLBACK InputMessageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}

#endif