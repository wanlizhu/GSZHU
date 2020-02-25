#pragma once

#include "Core/IModule.h"

namespace Wanli
{
    class WANLI_API Files
        : public IModule::Registrar<Files, EModuleStage::Render>
    {
    public:
        Files() = default;
        virtual ~Files();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void Destroy() override;
    };
}