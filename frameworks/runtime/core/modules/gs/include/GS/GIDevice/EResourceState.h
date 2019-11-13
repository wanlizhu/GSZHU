#pragma once

#include <stdint.h>

namespace GS::GI
{
    enum class EResourceState : uint32_t
    {
        Undefined,
        PreInitialized,
        Common,
        VertexBuffer,
        ConstantBuffer,
        IndexBuffer,
        RenderTarget,
        UnorderedAccess,
        DepthStencil,
        ShaderResource,
        StreamOut,
        IndirectArg,
        CopyDest,
        CopySource,
        ResolveDest,
        ResolveSource,
        Present,
        GenericRead,
        Predication,
        NonPixelShader,
        AccelerationStructure,
    };
}