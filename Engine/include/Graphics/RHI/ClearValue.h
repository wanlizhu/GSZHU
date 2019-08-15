#pragma once

#include "Graphics/Config.h"
#include "Graphics/RHI/Constants.h"

namespace ZHU
{
    struct ZHU_GRAPHICS_API DepthStencilClearValue
    {
        float Depth = 1.0f;
        uint8_t Stencil = 0;
    };


    struct ZHU_GRAPHICS_API OptimizedClearValue
    {
        EPixelFormat Format = EPixelFormat::Unknown;
        float Color[4] = { 0.f, 0.f, 0.f, 0.f };
        DepthStencilClearValue DepthStencil;
    };
}