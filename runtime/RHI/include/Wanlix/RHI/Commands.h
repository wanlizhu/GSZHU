#pragma once

#include "Common.h"
#include "IBuffer.h"
#include "ITexture.h"

namespace Wanlix
{
    struct DrawAttribs
    {
        union {
            Uint numVertices = 0;
            Uint numIndices;
        };
        Bool isIndexed = false;
        DataType indexType = DataType::Undefined;
        Uint drawFlags = DrawFalgs::None;

        Uint numInstances = 1;
        Uint baseVertex = 0;
        union {
            Uint firstVertexLocation = 0;
            Uint firstIndexLocation;
        };
        Uint firstInstanceLocation = 0;
        IBuffer* indirectDrawAttribs = nullptr;
        Uint indirectDrawOffset = 0;
        RSTMode indirectDrawSTM = RSTMode::None;
    };

    DrawAttribs MakeDrawAttribs(Uint numVertices,
                                Uint drawFalgs,
                                Uint numInstances = 1,
                                Uint baseVertex = 0,
                                Uint firstVertexLoc = 0,
                                Uint firstInstanceLoc = 0);

    DrawAttribs MakeDrawAttribs(Uint numIndices,
                                DataType indexType,
                                Uint drawFlags,
                                Uint numInstances = 1,
                                Uint baseVertex = 0,
                                Uint firstIndexLoc = 0,
                                Uint firstInstanceLoc = 0);

    DrawAttribs MakeDrawAttribs(IBuffer* indirectDrawAttribs,
                                Uint drawFlags,
                                RSTMode mode,
                                Uint offset = 0);

    DrawAttribs MakeDrawAttribs(IBuffer* indirectDrawAttribs,
                                DataType indexType,
                                Uint drawFlags,
                                RSTMode mode,
                                Uint offset = 0);

    struct ComputeAttribs
    {
        Uint groupX = 1;
        Uint groupY = 1;
        Uint groupZ = 1;
        IBuffer* indirectComputeAttribs = nullptr;
        Uint indirectComputeOffset = 0;
        RSTMode indirectComputeSTM = RSTMode::None;
    };

    ComputeAttribs MakeComputeAttribs(Uint x, Uint y = 1, Uint z = 1);

    ComputeAttribs MakeComputeAttribs(IBuffer* computeAttribs,
                                      RSTMode mode,
                                      Uint offset = 0);

    struct CopyAttribs
    {
        ITexture* srcTexture = nullptr;
        Uint  srcMipLevel = 0;
        Uint  srcSlice = 0;
        Region  srcRegion;
        RSTMode srcTransMode = RSTMode::None;

        ITexture* destTexture = nullptr;
        Uint  destMipLevel = 0;
        Uint  destSlice = 0;
        Offset  destOffset;
        RSTMode destTransMode = RSTMode::None;
    };

    CopyAttribs MakeCopyAttribs(ITexture* src,
                                RSTMode srcMode,
                                ITexture* dest,
                                RSTMode destMode);
}