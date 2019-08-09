#pragma once

#include "Graphics/Constants.h"

namespace ZHU
{
    struct DepthStencilClearValue
    {
        float Depth = 1.0f;
        uint8_t Stencil = 0;
    };


    struct OptimizedClearValue
    {
        EPixelFormat Format = EPixelFormat::Unknown;
        float Color[4] = { 0.f, 0.f, 0.f, 0.f };
        DepthStencilClearValue DepthStencil;
    };
}