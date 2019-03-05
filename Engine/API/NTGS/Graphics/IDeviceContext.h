#pragma once

#include <NTGS/Graphics/IObject.h>
#include <NTGS/Graphics/GraphicsEnums.h>
#include <NTGS/Graphics/DeviceCaps.h>
#include <NTGS/Core/Config.h>
#include <NTGS/Graphics/IBuffer.h>
#include <NTGS/Graphics/InputLayout.h>
#include <NTGS/Graphics/IShader.h>
#include <NTGS/Graphics/ITexture.h>
#include <NTGS/Graphics/ISampler.h>
#include <NTGS/Graphics/IResourceMapping.h>
#include <NTGS/Graphics/ITextureView.h>
#include <NTGS/Graphics/IBufferView.h>
#include <NTGS/Graphics/DepthStencilState.h>
#include <NTGS/Graphics/BlendState.h>
#include <NTGS/Graphics/IPipelineState.h>
#include <NTGS/Graphics/IFence.h>
#include <NTGS/Graphics/ICommandList.h>
#include <NTGS/Graphics/ISwapChain.h>

namespace NTGS {
    struct StateTransitionDesc {
        static constexpr const UINT RemainingMipLevels = static_cast<UINT>(-1);
        static constexpr const UINT RemainingArraySlices = static_cast<UINT>(-1);

        ITexture* Texture = nullptr;
        IBuffer* Buffer = nullptr;
        INT FirstMipLevel = 0;
        INT MipLevelsCount = RemainingMipLevels;
        INT FirstArraySlice = 0;
        INT ArraySliceCount = RemainingArraySlices;
        RESOURCE_STATE OldState = RESOURCE_STATE_UNKNOWN;
        RESOURCE_STATE NewState = RESOURCE_STATE_UNKNOWN;
        STATE_TRANSITION_TYPE TransitionType = STATE_TRANSITION_TYPE_IMMEDIATE;
        bool UpdateResourceState = false;

        StateTransitionDesc() = default;
        StateTransitionDesc(ITexture* _Texture,
                            RESOURCE_STATE _OldState,
                            RESOURCE_STATE _NewState,
                            INT _FirstMipLevel = 0,
                            INT _MipLevelsCount = RemainingMipLevels,
                            INT _FirstArraySlice = 0,
                            INT _ArraySliceCount = RemainingArraySlices,
                            STATE_TRANSITION_TYPE _TransitionType = STATE_TRANSITION_TYPE_IMMEDIATE,
                            bool _UpdateState = false) noexcept
            : Texture(_Texture),
            FirstMipLevel(_FirstMipLevel),
            MipLevelsCount(_MipLevelsCount),
            FirstArraySlice(_FirstArraySlice),
            ArraySliceCount(_ArraySliceCount),
            OldState(_OldState),
            NewState(_NewState),
            TransitionType(_TransitionType),
            UpdateResourceState(_UpdateState) {}

        StateTransitionDesc(ITexture* _Texture,
                            RESOURCE_STATE _OldState,
                            RESOURCE_STATE _NewState,
                            bool _UpdateState) noexcept
            : StateTransitionDesc(_Texture, _OldState, _NewState, 0, RemainingMipLevels, 0, RemainingArraySlices, STATE_TRANSITION_TYPE_IMMEDIATE, _UpdateState)
        {}

        StateTransitionDesc(IBuffer* _Buffer,
                            RESOURCE_STATE _OldState,
                            RESOURCE_STATE _NewState,
                            bool _UpdateState) noexcept
            : Buffer(_Buffer),
            OldState(_OldState),
            NewState(_NewState),
            UpdateResourceState(_UpdateState) {}
    };

    struct DrawAttribs {
        union {
            UINT NumVertices = 0;
            UINT NumIndices;
        };
        bool IsIndexed = false;
        VALUE_TYPE IndexType = VT_UNDEFINED;
        DRAW_FLAGS Flags = DRAW_FLAG_NONE;
        RESOURCE_STATE_TRANSITION_MODE IndirectAttribsBufferStateTransitionMode = RESOURCE_STATE_TRANSITION_MODE_NONE;
        UINT NumInstances = 1;
        UINT BaseVertex = 0;
        UINT IndirectDrawArgsOffset = 0;
        union {
            UINT StartVertexLocation = 0;
            UINT FirstIndexLocation;
        };
        UINT FirstInstanceLocation = 0;
        IBuffer* pIndirectDrawAttribs = nullptr;

        DrawAttribs() = default;

        DrawAttribs(UINT _NumVertices,
                    DRAW_FLAGS _Flags,
                    UINT _NumInstances = 1,
                    UINT _BaseVertex = 0,
                    UINT _StartVertexLocation = 0,
                    UINT _FirstInstanceLocation = 0) noexcept
            : NumVertices(_NumVertices),
            Flags(_Flags),
            NumInstances(_NumInstances),
            BaseVertex(_BaseVertex),
            StartVertexLocation(_StartVertexLocation),
            FirstInstanceLocation(_FirstInstanceLocation) {}

        DrawAttribs(UINT _NumIndices,
                    VALUE_TYPE _IndexType,
                    DRAW_FLAGS _Flags,
                    UINT _NumInstances = 1,
                    UINT _BaseVertex = 0,
                    UINT _FirstIndexLocation = 0,
                    UINT _FirstInstanceLocation = 0) noexcept
            : NumIndices(_NumIndices),
            IsIndexed(true),
            IndexType(_IndexType),
            Flags(_Flags),
            NumInstances(_NumInstances),
            BaseVertex(_BaseVertex),
            FirstIndexLocation(_FirstIndexLocation),
            FirstInstanceLocation(_FirstInstanceLocation) {}

        DrawAttribs(IBuffer* _pIndirectDrawAttribs,
                    DRAW_FLAGS _Flags,
                    RESOURCE_STATE_TRANSITION_MODE _IndirectAttribsBufferStateTransitionMode,
                    UINT _IndirectDrawArgsOffset = 0) noexcept
            : Flags(_Flags),
            IndirectAttribsBufferStateTransitionMode(_IndirectAttribsBufferStateTransitionMode),
            IndirectDrawArgsOffset(_IndirectDrawArgsOffset),
            pIndirectDrawAttribs(_pIndirectDrawAttribs) {}

        DrawAttribs(IBuffer* _pIndirectDrawAttribs,
                    VALUE_TYPE _IndexType,
                    DRAW_FLAGS _Flags,
                    RESOURCE_STATE_TRANSITION_MODE _IndirectAttribsBufferStateTransitionMode,
                    UINT _IndirectDrawArgsOffset = 0) noexcept
            : IsIndexed(true),
            IndexType(_IndexType),
            Flags(_Flags),
            IndirectAttribsBufferStateTransitionMode(_IndirectAttribsBufferStateTransitionMode),
            IndirectDrawArgsOffset(_IndirectDrawArgsOffset),
            pIndirectDrawAttribs(_pIndirectDrawAttribs) {}
    };

    struct DispatchComputeAttribs {
        UINT ThreadGroupCountX = 1;
        UINT ThreadGroupCountY = 1;
        UINT ThreadGroupCountZ = 1;
        IBuffer* IndirectDispatchAttribs = nullptr;
        UINT  DispatchArgsByteOffset = 0;
        RESOURCE_STATE_TRANSITION_MODE IndirectAttribsBufferStateTransitionMode = RESOURCE_STATE_TRANSITION_MODE_NONE;

        DispatchComputeAttribs() = default;

        explicit DispatchComputeAttribs(UINT GroupsX, UINT GroupsY = 1, UINT GroupsZ = 1) noexcept
            : ThreadGroupCountX(GroupsX),
            ThreadGroupCountY(GroupsY),
            ThreadGroupCountZ(GroupsZ),
            IndirectDispatchAttribs(nullptr),
            DispatchArgsByteOffset(0) {}

        DispatchComputeAttribs(IBuffer* DispatchAttribs,
                               RESOURCE_STATE_TRANSITION_MODE StateTransitionMode,
                               UINT Offset = 0) :
            ThreadGroupCountX(0),
            ThreadGroupCountY(0),
            ThreadGroupCountZ(0),
            IndirectDispatchAttribs(DispatchAttribs),
            DispatchArgsByteOffset(Offset),
            IndirectAttribsBufferStateTransitionMode(StateTransitionMode) {}
    };

    class IDeviceContext : public IObject {
    public:
        virtual void SetPipelineState(IPipelineState* pPipelineState) = 0;
        virtual void TransitionShaderResources(IPipelineState* pPipelineState, IShaderResourceBinding* pShaderResourceBinding) = 0;
        virtual void CommitShaderResources(IShaderResourceBinding* pShaderResourceBinding, RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;
        virtual void SetStencilRef(UINT StencilRef) = 0;
        virtual void SetBlendFactors(const float* pBlendFactors = nullptr) = 0;
        virtual void SetVertexBuffers(UINT StartSlot, UINT NumBuffersSet, IBuffer** ppBuffers, UINT* pOffsets, RESOURCE_STATE_TRANSITION_MODE StateTransitionMode, SET_VERTEX_BUFFERS_FLAGS Flags) = 0;
        virtual void InvalidateState() = 0;
        virtual void SetIndexBuffer(IBuffer* pIndexBuffer, UINT ByteOffset, RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;
        virtual void SetViewports(UINT NumViewports, const Viewport* pViewports, UINT RTWidth, UINT RTHeight) = 0;
        virtual void SetScissorRects(UINT NumRects, const Rect* pRects, UINT RTWidth, UINT RTHeight) = 0;
        virtual void SetRenderTargets(UINT NumRenderTargets, ITextureView* ppRenderTargets[], ITextureView* pDepthStencil, RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;
        virtual void Draw(DrawAttribs &DrawAttribs) = 0;
        virtual void DispatchCompute(const DispatchComputeAttribs &DispatchAttrs) = 0;
        virtual void ClearDepthStencil(ITextureView* pView, CLEAR_DEPTH_STENCIL_FLAGS ClearFlags, FLOAT fDepth, UINT8 Stencil, RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;
        virtual void ClearRenderTarget(ITextureView* pView, const FLOAT* RGBA, RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;
        virtual void FinishCommandList(ICommandList **ppCommandList) = 0;
        virtual void ExecuteCommandList(ICommandList* pCommandList) = 0;
        virtual void SignalFence(IFence* pFence, UINT64 Value) = 0;
        virtual void Flush() = 0;
        virtual void UpdateBuffer(IBuffer* pBuffer, UINT Offset, UINT Size, const void* DataPtr, RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;
        virtual void CopyBuffer(IBuffer* SrcBufferPtr, UINT SrcOffset, RESOURCE_STATE_TRANSITION_MODE SrcBufferTransitionMode, IBuffer* pDstBuffer, UINT DstOffset, UINT  Size, RESOURCE_STATE_TRANSITION_MODE DstBufferTransitionMode) = 0;
        virtual void* MapBuffer(IBuffer* pBuffer, MAP_TYPE MapType, MAP_FLAGS MapFlags) = 0;
        virtual void UnmapBuffer(IBuffer* pBuffer, MAP_TYPE MapType) = 0;
        virtual void UpdateTexture(ITexture* pTexture, UINT MipLevel, UINT Slice, const Box& DstBox, const TextureSubResData& SubresData, RESOURCE_STATE_TRANSITION_MODE SrcBufferTransitionMode, RESOURCE_STATE_TRANSITION_MODE TextureTransitionMode) = 0;
        virtual void CopyTexture(const CopyTextureAttribs& CopyAttribs) = 0;
        virtual void MapTextureSubresource(ITexture* pTexture, UINT MipLevel, UINT ArraySlice, MAP_TYPE MapType, MAP_FLAGS MapFlags, const Box* pMapRegion, MappedTextureSubResource& MappedData) = 0;
        virtual void UnmapTextureSubresource(ITexture* pTexture, UINT MipLevel, UINT ArraySlice) = 0;
        virtual void GenerateMips(ITextureView* pTextureView) = 0;
        virtual void SetSwapChain(ISwapChain* pSwapChain) = 0;
        virtual void FinishFrame() = 0;
        virtual void TransitionResourceStates(UINT BarrierCount, StateTransitionDesc* pResourceBarriers) = 0;
    };
}