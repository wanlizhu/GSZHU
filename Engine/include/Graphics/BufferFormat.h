#pragma once

#include "Graphics/Constants.h"

namespace ZHU
{
    struct BufferFormat
    {
        EComponent Component = EComponent::Undefined;
        uint8_t NumComponents = 0;
        bool IsNormalized = false;
    };
}