#pragma once

#include "Core/BasicTypes.h"
#include "Core/IModule.h"
#include "Utilities/NonCopyable.h"

namespace Wanli
{
    class DLLDECL GraphicsModule final : public IModule
    {
    public:
        using ConfigType = GraphicsConfig;

        GraphicsModule()
            : IModule(EModuleStage::Render, "GraphicsModule")
        {}

        virtual ~GraphicsModule();
        virtual void Initialize(IConfig* config) override final;
        virtual void Update(double elapsedTime) override final;
        virtual void Shutdown() override final;
    };
}