#pragma once

#include "Common.h"

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

    struct SemanticName
    {
        std::string name;
        uint32_t semanticIndex = 0;

        SemanticName() = default;
        explicit SemanticName(const std::string& name, uint32_t index = 0)
            : name(name)
            , semanticIndex(index)
        {}
    };

    /* Vertex input/output attribute structure. */
    struct VertexAttribute
    {
        std::variant<std::string, SemanticName> name;

        ResourceFormat format = ResourceFormat::RGBA32Float;
        uint32_t       location = 0;
        uint32_t       instanceDivisor = 0;

        uint32_t       offset = 0;
        uint32_t       stride = 0;
        uint32_t       slot = 0;

        SystemValue    systemValue = SystemValue::Undefined;

        VertexAttribute() = default;
        VertexAttribute(
            const std::string& name,
            ResourceFormat format,
            uint32_t location = 0,
            uint32_t instanceDivisor = 0,
            const SystemValue & systemValue = SystemValue::Undefined
        ) noexcept;
        VertexAttribute(
            const SemanticName& semanticName,
            ResourceFormat format,
            uint32_t location = 0,
            uint32_t instanceDivisor = 0
        ) noexcept;
        VertexAttribute(
            const std::string& name,
            const ResourceFormat& format,
            uint32_t location,
            uint32_t offset,
            uint32_t stride,
            uint32_t slot = 0,
            uint32_t instanceDivisor = 0
        ) noexcept;
        VertexAttribute(
            const SemanticName& semanticName,
            const ResourceFormat& format,
            uint32_t location,
            uint32_t offset,
            uint32_t stride,
            uint32_t slot = 0,
            uint32_t instanceDivisor = 0
        ) noexcept;
    };

    struct VertexShaderAttributes
    {
        std::vector<VertexAttribute> inputAttribs;
        std::vector<VertexAttribute> outputAttribs;
    };
}