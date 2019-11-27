#pragma once

#include "Config.h"
#include "Types.h"
#include "Context.h"
#include "IModule.h"
#include "Platform/IWindow.h"

namespace Wanlix
{
    class Engine final : public NonCopyable
    {
    public:
        using Ptr = std::shared_ptr<Engine>;
        using ConstPtr = std::shared_ptr<const Engine>;

        static Ptr Create(ISurface::Ptr surface);
        ~Engine();

        void Tick();
        Context::Ptr GetContext();
        Context::ConstPtr GetContext() const;

    private:
        Engine();
        bool LoadConfigFile();
        bool InitializeModules();

    private:
        Context::Ptr mContext;
    };
}