#pragma once

#include "Common.h"
#include "DeviceObject.h"

namespace Wanlix
{
    struct BindFlags
    {
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

    struct CPUAccessFlags
    {
        enum
        {
            Read = (1 << 0),
            Write = (1 << 1),
            ReadWrite = (Read | Write),
        };
    };

    struct MiscFlags
    {
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

    struct ResourceDescriptor : public DeviceObject::Desc
    {
        long bindFlags = 0;
        long cpuAccessFlags = 0;
        long miscFlags = 0;
    };

    struct ResourceViewDescriptor
    {};
}