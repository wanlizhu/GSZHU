#pragma once

#include <string>
#include "Application/IWindow.h"
#include <memory>

namespace NTGS {
    class IApplication;
    typedef std::shared_ptr<IApplication> IApplicationPtr;

    class IApplication : public std::enable_shared_from_this<IApplication> {
    public:
        static IApplicationPtr Create();
        virtual ~IApplication() = default;

        virtual void Initialize() = 0;
        virtual void Finalize() = 0;

        virtual void Run() = 0;
        virtual void Quit() = 0;

        virtual IWindowPtr CreateNamedWindow(const std::string& name, const WindowConfig& config, void* nativeWindow) = 0;
        virtual IWindowPtr GetWindow(const std::string& name) const = 0;

    protected:
        IApplication() = default;
        IApplication(const IApplication&) = default;
        IApplication& operator=(const IApplication&) = default;
    };
}