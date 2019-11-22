#pragma once

#include "EngineConfig.h"

namespace Wanlix
{
    class Context;

    class IModule
    {
    public:
        IModule(Context* context)
            : mContext(context)
        {}
        virtual ~IModule() = default;
        virtual bool Initialize() { return true; }
        virtual void Tick(float delta) {}

    protected:
        Context* mContext = nullptr;
    };
}