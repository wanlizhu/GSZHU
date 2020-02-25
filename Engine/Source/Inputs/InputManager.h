#pragma once

#include "Core/IModule.h"

namespace Wanli
{
    class WANLI_API InputManagerImpl
    {
    public:
        InputManagerImpl() = default;
        virtual ~InputManagerImpl() = default;

        virtual void Initialize() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;
    };

    class WANLI_API InputManager 
        : public IModule::Registrar<InputManager, EModuleStage::PreUpdate>
    {
    public:
        InputManager();
        virtual ~InputManager();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void Destroy() override;

    private:
        UniquePtr<InputManagerImpl> mImpl;
    };
}