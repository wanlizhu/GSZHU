#pragma once

#include <cstdint>
#include "Wanlix/Core3D/Utility/Macro.h"

namespace Wanlix
{
    enum class ResourceType
    {
        Undefined,
        Buffer,
        Texture,
        Sampler,
        Shader,
    };

    struct BindFlags 
    {
        uint32_t value = 0;
        enum 
        {
            VertexBuffer = (1 << 0),
            IndexBuffer = (1 << 1),
            ConstantBuffer = (1 << 2),
            StreamOutputBuffer = (1 << 3),
            IndirectBuffer = (1 << 4),
            Sampled = (1 << 5),
            Storage = (1 << 6),
            ColorAttachment = (1 << 7),
            DepthStencilAttachment = (1 << 8),
            CombinedTextureSampler = (1 << 9),
        };

        BindFlags(uint32_t val = 0) : value(val) {}
        operator uint32_t() const { return value; }
        DEFINE_CMP_OPS(BindFlags, value)
        DEFINE_BIT_OPS(uint32_t, value)
    };

    struct CPUAccessFlags
    {
        uint32_t value = 0;
        enum
        {
            Read = (1 << 0),
            Write = (1 << 1),
            ReadWrite = (Read | Write),
        };

        CPUAccessFlags(uint32_t val = 0) : value(val) {}
        operator uint32_t() const { return value; }
        DEFINE_CMP_OPS(CPUAccessFlags, value)
        DEFINE_BIT_OPS(uint32_t, value)
    };

    struct MiscFlags
    {
        uint32_t value = 0;
        enum
        {
            DynamicUsage = (1 << 0),
            FixedSamples = (1 << 1),
            GenerateMips = (1 << 2),
            NoInitialData = (1 << 3),
            Append = (1 << 4),
            Counter = (1 << 5),
        };

        MiscFlags(uint32_t val = 0) : value(val) {}
        operator uint32_t() const { return value; }
        DEFINE_CMP_OPS(MiscFlags, value)
        DEFINE_BIT_OPS(uint32_t, value)
    };

    struct ResourceDescriptor
    {
        ResourceType type = ResourceType::Undefined;
        BindFlags bindFlags;
        CPUAccessFlags cpuAccess;
        MiscFlags misc;

        bool operator==(const ResourceDescriptor& rhs) const;
        bool operator!=(const ResourceDescriptor& rhs) const;
    };
}