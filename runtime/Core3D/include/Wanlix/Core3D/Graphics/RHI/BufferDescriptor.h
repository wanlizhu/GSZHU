#pragma once

#include "ResourceDescriptor.h"
#include "Wanlix/Core3D/Graphics/VertexAttribute.h"

namespace Wanlix
{
    struct BufferDescriptor : public ResourceDescriptor
    {
        uint64_t size = 0;
        uint32_t stride = 0;
        Format   format = Format::Undefined;
        std::vector<VertexAttribute> vertexAttribs;
    };

}