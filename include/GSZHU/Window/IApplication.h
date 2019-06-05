#pragma once

#include <GSZHU/Config.h>
#include <GSZHU/IWindow.h>
#include <GSZHU/IRenderer.h>

namespace GSZHU {
    class IApplication {
    public:
        virtual void Initialize(void* userData) = 0;
        virtual void Destroy() = 0;

        virtual std::shared_ptr<IWindow> CreateWindow(STRING const& name, int width, int height, bool isVSync = true) = 0;
        virtual void RemoveWindow(HANDLE handle) = 0;
        virtual std::shared_ptr<IWindow> FindWindowByName(STRING const& name) const = 0;
        virtual std::shared_ptr<IWindow> FindWindowByHandle(HANDLE handle) const = 0;

        virtual int Run() = 0;
        virtual void Quit(int code = 0) = 0;
    };

    IApplication* CreateApplication(void* userData);
}