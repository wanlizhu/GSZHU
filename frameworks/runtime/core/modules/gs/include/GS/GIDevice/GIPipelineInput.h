#pragma once

#include "GIDeviceObject.h"
#include "GIBuffer.h"
#include "GITexture.h"
#include "GIResourceFormat.h"
#include "shaders/VertexAttributeIndex.h"

namespace GS
{
    struct VertexAttribute
    {
        uint32_t index = 0;
        GIComponentType componentType = GIComponentType::Float;
        uint32_t componentCount = 0;
        bool normalized = false;
        uint32_t stride = 0;
        GIBuffer::Ptr vertexBuffer;
    };

    class GIPipelineInput
    {
    public:

    private:
        std::unordered_map<std::string, uint32_t> mNameIndexMap;
        std::unordered_map<uint32_t, VertexAttribute> mIndexAttribMap;
    };
}