#pragma once

#include "ResourceDescriptor.h"
#include "ShaderDescriptor.h"

namespace Wanlix
{
    struct BufferDescriptor : public Resource::Desc
    {
        size_t         size = 0;
        uint32_t       stride = 0;
        ResourceFormat format = ResourceFormat::Undefined;

        std::vector<VertexAttribute> vertexAttribs;
    };

    struct BufferViewDescriptor : public ResourceViewDescriptor
    {

    };
}