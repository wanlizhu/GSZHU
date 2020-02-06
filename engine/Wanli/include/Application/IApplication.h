#pragma once

#include "Application/IWindow.h"
#include "Application/IEventListener.h"

namespace Wanli
{
    struct ApplicationCreateInfo
    {
        String title = "Wanli Host";
        Version version;
        WindowCreateInfo windowInfo;
    };

    class DLLDECL IApplication
    {
    public:
        virtual ~IApplication() = default;
        virtual bool Initialize(const ApplicationCreateInfo& info) = 0;
        virtual void Destroy() = 0;
        virtual void Tick(double time, double delta) = 0;
        virtual void Quit() = 0;
        virtual bool RecreateWindow(const WindowCreateInfo& info) = 0;
        virtual IMonitor* GetPrimaryMonitor() const = 0;
        virtual IWindow* GetWindow() const = 0;
        virtual const char* GetPath() const = 0;
        virtual const char* GetTitle() const = 0;
    };

    SharedPtr<IApplication> DLLDECL MakeApplication();
}