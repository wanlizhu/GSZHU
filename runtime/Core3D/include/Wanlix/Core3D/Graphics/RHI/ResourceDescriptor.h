#pragma once

#include <cstdint>
#include "Wanlix/Core3D/Utility/Macro.h"
#include "Wanlix/Core3D/Utility/NonInstantiable.h"

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

    struct BindFlags : public NonInstantiable
    {
        using UnderlyingType = uint32_t;
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
    };

    struct CPUAccessFlags : public NonInstantiable
    {
        using UnderlyingType = uint32_t;
        enum
        {
            Read = (1 << 0),
            Write = (1 << 1),
            ReadWrite = (Read | Write),
        };
    };

    struct MiscFlags : public NonInstantiable
    {
        using UnderlyingType = uint32_t;
        enum
        {
            DynamicUsage = (1 << 0),
            FixedSamples = (1 << 1),
            GenerateMips = (1 << 2),
            NoInitialData = (1 << 3),
            Append = (1 << 4),
            Counter = (1 << 5),
        };
    };

    struct ResourceDescriptor
    {
        ResourceType                   type = ResourceType::Undefined;
        BindFlags::UnderlyingType      bindFlags = 0;
        CPUAccessFlags::UnderlyingType cpuAccess = 0;
        MiscFlags::UnderlyingType      misc = 0;

        bool operator==(const ResourceDescriptor& rhs) const;
        bool operator!=(const ResourceDescriptor& rhs) const;
    };
}