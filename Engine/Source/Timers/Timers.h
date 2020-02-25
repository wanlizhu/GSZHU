#pragma once

#include "Core/IModule.h"

namespace Wanli
{
    class WANLI_API Timers
        : public IModule::Registrar<Timers, EModuleStage::Render>
    {
    public:
        virtual void Initialize() override;
        virtual void Update() override;
        virtual void Destroy() override;
    };
}