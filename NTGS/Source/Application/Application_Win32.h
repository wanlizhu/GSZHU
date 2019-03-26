#pragma once

#include "Application/IApplication.h"
#include "Window_Win32.h"

namespace NTGS {
    class Application_Win32;
    typedef std::shared_ptr<Application_Win32> Application_Win32Ptr;

    class Application_Win32 : public IApplication {
    public:
        static Application_Win32Ptr Create();
        virtual ~Application_Win32();

        virtual void Initialize() override;
        virtual void Finalize() override;

        virtual void Run() override;
        virtual void Quit() override;

        virtual IWindowPtr CreateNamedWindow(const std::string& name, const WindowConfig& config, void* nativeWindow) override;
        virtual IWindowPtr GetWindow(const std::string& name) const override;

    private:
        std::vector<Window_Win32Ptr> mWindowList;
    };
}