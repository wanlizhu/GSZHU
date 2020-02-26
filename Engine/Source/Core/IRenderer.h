#pragma once

#include "Common.h"

namespace Wanli
{
    class WANLI_API IRenderer
    {
    public:
        using PTR = std::shared_ptr<IRenderer>;

        IRenderer() = default;
        virtual ~IRenderer() = default;

        virtual void OnUpdate() {}
        virtual void OnRender() {}
    };
}