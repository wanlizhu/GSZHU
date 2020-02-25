#pragma once
#ifdef PLATFORM_WINDOWS

#include "Inputs/InputManager.h"

namespace Wanli
{
    class WANLI_API WindowsInputManager final : public InputManagerImpl
    {
    public:
        WindowsInputManager() = default;
        virtual ~WindowsInputManager();

        virtual void Initialize() override final;
        virtual void Update() override final;
        virtual void Destroy() override final;

    private:
        static LRESULT CALLBACK InputMessageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}

#endif