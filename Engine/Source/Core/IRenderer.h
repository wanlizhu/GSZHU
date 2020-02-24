#pragma once

#include "Common.h"

namespace Wanli
{
    class IRenderer
    {
    public:
        using PTR = std::shared_ptr<IRenderer>;

        virtual ~IRenderer() = default;
        virtual void OnInitialize() {}
        virtual void OnUpdate() {}
        virtual void OnRender() {}
        virtual void OnDestroy() {}
    };
}