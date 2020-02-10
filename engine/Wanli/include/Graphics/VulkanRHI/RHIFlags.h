#pragma once

#include "RHIPixelFormat.h"

namespace Wanli
{
    enum EResourceLimit
    {
        MaxTextureMipCount = 14,
        MaxImmutableSamplers = 2,
        MaxVertexElementCount = 16,
        MaxVertexElementCount_NumBits = 4,
        MaxSimultaneousRenderTargets = 8,
        MaxSimultaneousRenderTargets_NumBits = 3,
        ShaderArrayElementAlignBytes = 16,
        MaxSimultaneousUAVs = 8
    };

    enum class ImageLayoutBarrier
    {
        Undefined,
        TransferDest,
        ColorAttachment,
        DepthStencilAttachment,
        TransferSource,
        Present,
        PixelShaderRead,
        PixelDepthStencilRead,
        ComputeGeneralRW,
        PixelGeneralRW,
    };

    enum EVertexAttribute
    {
        VA_None = 0,
        VA_Position,
        VA_UV0,
        VA_UV1,
        VA_Normal,
        VA_Tangent,
        VA_Color,
        VA_SkinWeight,
        VA_SkinIndex,
        VA_SkinPack,
        VA_InstanceFloat1,
        VA_InstanceFloat2,
        VA_InstanceFloat3,
        VA_InstanceFloat4,
        VA_Custom0,
        VA_Custom1,
        VA_Custom2,
        VA_Custom3,
        VA_Count,
    };

    enum EVertexElementType
    {
        VET_None,
        VET_Float1,
        VET_Float2,
        VET_Float3,
        VET_Float4,
        VET_PackedNormal,
        VET_UByte4,
        VET_UByte4N,
        VET_Color,
        VET_Short2,
        VET_Short4,
        VET_Short2N,
        VET_Half2,
        VET_Half4,
        VET_Short4N,
        VET_UShort2,
        VET_UShort4,
        VET_UShort2N,
        VET_UShort4N,
        VET_URGB10A2N,
        VET_MAX,
    };

    enum ECubeFace
    {
        CubeFace_PosX = 0,
        CubeFace_NegX,
        CubeFace_PosY,
        CubeFace_NegY,
        CubeFace_PosZ,
        CubeFace_NegZ,

        CubeFace_MAX,
    };
}