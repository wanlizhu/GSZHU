#pragma once

#include "Graphics/Constants.h"
#include <vector>

namespace ZHU
{
    struct LayoutElement
    {
        uint32_t InputIndex = 0;
        uint32_t BufferSlot = 0;

        EComponent ComponentType = EComponent::FLOAT32;
        uint32_t NumComponents = 0;
        bool IsNormalized = true;
        uint32_t Offset = 0;
        uint32_t Stride = 0;

        EFrequency Frequency = EFrequency::Vertex;
        uint32_t InstanceStep = 1;
    };


    struct InputLayoutDesc
    {
        std::vector<LayoutElement> LayoutElements;
    };
}