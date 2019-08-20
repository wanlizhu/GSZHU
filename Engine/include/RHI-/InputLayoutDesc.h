#pragma once

#include "Graphics/RHI/Constants.h"
#include <vector>

namespace ZHU
{
    struct ZHU_GS_API InputLayoutDesc
    {
		struct ZHU_GS_API LayoutElement
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

        std::vector<LayoutElement> LayoutElements;
    };
}