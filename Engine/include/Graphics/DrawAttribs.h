#pragma once

#include "Graphics/Constants.h"
#include "Graphics/IBuffer.h"

namespace ZHU
{
    struct DrawAttribs
    {
        union {
            uint32_t NumVertices = 0;
            uint32_t NumIndices;
        };
        bool IsIndexed = false;
        EComponent IndexType = EComponent::Undefined;
        bool VerifyStates = false;

        uint32_t NumInstances = 1;
        uint32_t BaseVertex = 0;
        union {
            uint32_t StartVertexLocation = 0;
            uint32_t FirstIndexLocation;
        };
        uint32_t FirstInstanceLocation = 0;

        // Indirect draw
        IBuffer::Pointer IndirectDrawAttribs = nullptr;
        uint32_t IndirectDrawOffset = 0;
    };
}