#pragma once

#include "Graphics/Constants.h"
#include "Graphics/IBuffer.h"

namespace ZHU
{
    struct DispatchComputeAttribs
    {
        uint32_t GroupCountX = 1;
        uint32_t GroupCountY = 1;
        uint32_t GroupCountZ = 1;

        IBuffer::Pointer IndirectDispatchAttribs = nullptr;
        uint32_t IndirectDispatchOffset = 0;
    };
}