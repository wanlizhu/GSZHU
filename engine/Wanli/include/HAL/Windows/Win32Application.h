#pragma once

#include "Application/IApplication.h"
#include "HAL/Windows/Win32Window.h"

namespace Wanli
{
    class Win32Application : public IApplication
    {
    public:
        Win32Application(HINSTANCE hInstance);
        Win32Application(const Win32Application&) = delete;
        Win32Application& operator=(const Win32Application&) = delete;
        virtual ~Win32Application();

        virtual bool Initialize() override final;
        virtual void Tick(double time, double delta) override final;
        virtual void Quit() override final;
        virtual bool CreateCanvasWindow(const char* title, int width, int height, uint32_t flags = 0) override final;
        virtual Win32Monitor* GetPrimaryMonitor() const override final;
        virtual Win32Window* GetWindow() const override final;

    private:
        HINSTANCE mHInstance = NULL;
        std::unique_ptr<Win32Window> mCanvasWindow;
        std::unique_ptr<Win32Monitor> mPrimaryMonitor;
    };
}