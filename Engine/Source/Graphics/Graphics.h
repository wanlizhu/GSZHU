#pragma once

#include "Core/IModule.h"
#include "Configuration/Windows/GraphicsConfig.h"

namespace Wanli
{
    class WANLI_API Graphics
        : public IModule::Registrar<Graphics, GraphicsConfig, EModuleStage::Render>
    {
    public:
        Graphics();
        virtual ~Graphics();

        virtual void Update() override;

    };
}