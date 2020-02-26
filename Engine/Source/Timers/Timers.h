#pragma once

#include "Core/IModule.h"

namespace Wanli
{
    class WANLI_API Timers
        : public IModule::Registrar<Timers, void, EModuleStage::Render>
    {
    public:
        Timers();
        virtual ~Timers();

        virtual void Update() override;
    };
}