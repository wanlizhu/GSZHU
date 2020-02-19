#pragma once

#include "Core/BasicTypes.h"
#include "Core/IModule.h"
#include "Utilities/NonCopyable.h"

namespace Wanli
{
    class DLLDECL PhysicsModule final : public IModule
    {
    public:
        using ConfigType = PhysicsConfig;

        PhysicsModule()
            : IModule(EModuleStage::Update, "PhysicsModule")
        {}

        virtual ~PhysicsModule() = default;
        virtual void Initialize(IConfig * config) override final;
        virtual void Update(double elapsedTime) override final;
        virtual void Shutdown() override final;
    };
}