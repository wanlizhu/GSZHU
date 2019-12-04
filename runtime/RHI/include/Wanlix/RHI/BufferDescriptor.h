#pragma once

#include "ResourceDescriptor.h"

namespace Wanlix
{
    /* This is only used for shader code reflection. */
    enum class SystemValue
    {
        Undefined,
        ClipDistance,
        Color,
        CullDistance,
        Depth,
        DepthGreater,
        DepthLess,
        FrontFacing,
        InstanceID,
        Position,
        PrimitiveID,
        RenderTargetIndex,
        SampleMask,
        SampleID,
        Stencil,
        VertexID,
        ViewportIndex,
    };


    struct VertexAttribute
    {
        std::string    name;

        ResourceFormat format = ResourceFormat::RGBA32Float;
        uint32_t       location = 0;
        uint32_t       semanticIndex = 0;

        SystemValue    systemValue = SystemValue::Undefined;
        uint32_t       slot = 0;
        uint32_t       offset = 0;
        uint32_t       stride = 0;
        uint32_t       instanceDivisor = 0;
    };

    struct BufferDescriptor : public ResourceDescriptor
    {
        size_t         size = 0;
        uint32_t       stride = 0;
        ResourceFormat format = ResourceFormat::Undefined;
    };

    struct VertexBufferDescriptor : public BufferDescriptor
    {
        std::vector<VertexAttribute> vertexAttribs;
    };
}