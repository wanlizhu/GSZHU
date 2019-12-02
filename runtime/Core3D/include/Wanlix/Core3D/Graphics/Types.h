#pragma once

#include "Wanlix/Core3D/Types.h"
#include "Config.h"
#include "Color.h"
#include "Format.h"
#include "Limits.h"

namespace Wanlix
{
    using ByteBuffer = std::unique_ptr<char[]>;

    struct ClearValue
    {

    };

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
}