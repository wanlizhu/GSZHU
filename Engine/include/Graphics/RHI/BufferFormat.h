#pragma once

#include "Graphics/Config.h"
#include "Graphics/RHI/Constants.h"

namespace ZHU
{
    struct ZHU_GRAPHICS_API BufferFormat
    {
        EComponent Component = EComponent::Undefined;
        uint8_t NumComponents = 0;
        bool IsNormalized = false;
    };
}