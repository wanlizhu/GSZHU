#pragma once

#include "Core/BasicTypes.h"
#include "Core/IModule.h"
#include "Inputs/InputFlags.h"
#include "Configurations/InputConfig.h"

namespace Wanli
{
    class DLLDECL InputModule : public IModule
    {
    public:
        using ConfigType = InputConfig;

        InputModule()
            : IModule(EModuleStage::PreUpdate, "InputModule")
        {}

        virtual ~InputModule() = default;
        virtual void Initialize(IConfig* config) override final;
        virtual void Update(double elapsedTime) override final;
        virtual void Shutdown() override final;
    };
}