#pragma once

#include <GSZHU/IApplication.h>
#include <GSZHU/IWindow.h>
#include "Win32/CWindow_Win32.h"

namespace GSZHU {
    class CApplication_Win32 : public IApplication {
    public:
        static CApplication_Win32* GetInstance();

        virtual void Initialize(void* userData) override;
        virtual void Destroy() override;
        HINSTANCE GetHandle() const;

        virtual std::shared_ptr<IWindow> CreateWindow(STRING const& name, int width, int height, bool isVSync = true) override;
        virtual void RemoveWindow(HANDLE handle) override;
        virtual std::shared_ptr<IWindow> FindWindowByName(STRING const& name) const override;
        virtual std::shared_ptr<IWindow> FindWindowByHandle(HANDLE handle) const override;

        virtual int Run() override;
        virtual void Quit(int code = 0) override;
        
    private:
        HINSTANCE mInstance = NULL;
        HASH_MAP<HWND, std::weak_ptr<CWindow_Win32>> mWindowTable;
    };
}