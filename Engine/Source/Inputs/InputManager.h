#pragma once

#include "Core/IModule.h"

namespace Wanli
{
    class WANLI_API InputManager : public IModule
    {
    public:
        InputManager() = default;
        virtual ~InputManager() = default;

        virtual void Create() override;
        virtual void Update() override;
        virtual void Destroy() override;
    };
}