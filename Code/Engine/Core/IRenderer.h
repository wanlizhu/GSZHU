#pragma once
#include "Common/Config.h"

namespace ZHU
{
    class ZHU_API IRenderer
    {
    public:
        friend class Engine;

        virtual ~IRenderer() = default;
        virtual void Update() = 0;
        virtual void Render() = 0;
    };
}