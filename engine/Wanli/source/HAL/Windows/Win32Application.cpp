#include "HAL/Windows/Win32Application.h"

namespace Wanli
{
    Win32Application::Win32Application(HINSTANCE hInstance)
    {
        std::vector<Win32Monitor> monitors;
        Win32Monitor::GetMonitors(monitors);
        for (auto& monitor : monitors)
        {
            if (monitor.IsPrimary())
            {
                mPrimaryMonitor.reset(new Win32Monitor(monitor));
            }
        }
    }

    Win32Application::~Win32Application()
    {
        Quit();
    }

    bool Win32Application::Initialize() 
    {
        return true;
    }

    void Win32Application::Tick(double time, double delta) 
    {}

    void Win32Application::Quit()
    {}

    bool Win32Application::CreateCanvasWindow(const char* title, int width, int height, uint32_t flags = 0) 
    {
        return true;
    }

    Win32Monitor* Win32Application::GetPrimaryMonitor() const 
    {
        return mPrimaryMonitor.get();
    }

    Win32Window* Win32Application::GetWindow() const
    {
        return mCanvasWindow.get();
    }
}