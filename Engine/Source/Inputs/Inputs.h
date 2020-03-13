#pragma once

#include "Core/IModule.h"
#include "Configuration/Windows/InputConfig.h"

namespace Wanli
{
    class WANLI_API InputManagerImpl
    {
    public:
        InputManagerImpl() = default;
        virtual ~InputManagerImpl() = default;

        virtual void Update() = 0;
    };

    class WANLI_API Inputs 
        : public IModule::Registrar<Inputs, InputConfig, EModuleStage::PreUpdate>
    {
    public:
        Inputs();
        virtual ~Inputs();

        virtual void Update() override;

    private:
        UniquePtr<InputManagerImpl> mImpl;
    };
}