#pragma once

#include "Common.h"

namespace Wanlix
{
    struct LayoutElement
    {
        Uint inputIndex = 0;
        Uint bufferSlot = 0;
        Uint components = 0;
        DataType dataType = DataType::Float32;
        Bool isNormalized = true;
        Uint offset = 0;
        Uint stride = 0;
        InputFrequency frequency = InputFrequency::PerVertex;
        Uint instanceStep = 1;
    };

    struct InputLayoutDesc
    {
        Array<LayoutElement> layoutElements;
    };
}