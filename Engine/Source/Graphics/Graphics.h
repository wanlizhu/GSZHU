#pragma once

#include "Core/IModule.h"

namespace Wanli
{
    class WANLI_API Graphics
        : public IModule::Registrar<Graphics, EModuleStage::Render>
    {
    public:
        Graphics() = default;
        virtual ~Graphics();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void Destroy() override;
    };
}