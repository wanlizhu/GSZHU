#pragma once

#include "Graphics/RHI/IBuffer.h"
#include "Graphics/RHI/ITexture.h"
#include "Graphics/RHI/IFence.h"

namespace Wanlix
{
    struct Viewport
    {
        float left = 0.f;
        float top = 0.f;
        float width = 0.f;
        float height = 0.f;
        float minDepth = 0.f;
        float maxDepth = 1.f;
    };

    struct Rect
    {
        int left = 0;
        int top = 0;
        int right = 0;
        int bottom = 0;
    };

    struct CopyTextureAttribs
    {
        ITexture* src = nullptr;
        Uint srcMipLevel = 0;
        Uint srcSlice = 0;
        Optional<Region> region = std::nullopt;

        ITexture* dst;
        Uint dstMipLevel = 0;
        Uint dstSlice = 0;
        Optional<Offset> dstOffset = std::nullopt;
    };

    struct DrawAttribs
    {
        Bool isIndexed = false;
        Uint numVertices = 0;
        Uint numIndices = 0;
        EDataType indexType = EDataType::Int32;
        EDrawFlags drawFlags = EDrawFlags::None;

        Uint numInstances = 0;
        Uint baseVertex = 0;
        Uint startVertexLocation = 0;
        Uint firstIndexLocation = 0;
        Uint firstInstanceLocation = 0;

        IBuffer* indirectDrawAttribs = nullptr;
        Uint indirectDrawArgsOffset = 0;
    };

    struct DispatchComputeAttribs
    {
        Uint groupCountX = 1;
        Uint groupCountY = 1;
        Uint groupCountZ = 1;
        IBuffer* indirectDispatchAttribs = nullptr;
        Uint dispatchArgsOffset = 0;
    };

    class IDeviceContext
    {
    public:
        virtual void SetPipelineState(IPipelineState* pso) = 0;
        virtual void CommitShaderResources(IShaderResourceBinding* resourceBinding) = 0;
        virtual void SetStencilRef(Uint stencil) = 0;
        virtual void SetBlendFactors(const Float4& blendFactors) = 0;
        virtual void SetVertexBuffer(Uint slot, IBuffer* buffer) = 0;
        virtual void SetIndexBuffer(IBuffer* buffer, Uint offset = 0) = 0;
        virtual void SetViewports(Array<Viewport> viewports) = 0;
        virtual void SetScissors(Array<Rect> rects) = 0;
        virtual void SetRenderTarget(Array<ITextureView> targets, ITextureView* depthStencil) = 0;
        virtual void SetSwapChain(ISwapChain* swapChain) = 0;

        virtual void UpdateBuffer(IBuffer* buffer, const Range& range, const void* data) = 0;
        virtual void CopyBuffer(IBuffer* src, const Range& srcRange, IBuffer* dst, Uint dstOffset = 0) = 0;
        virtual void* MapBuffer(IBuffer* buffer, EMapType mapType, EMapFlags mapFlags) = 0;
        virtual void UnmapBuffer(IBuffer* buffer) = 0;

        virtual void UpdateTexture(ITexture* texture, const Subresource& subres, const void* data) = 0;
        virtual void CopyTexture(const CopyTextureAttribs& attribs) = 0;
        virtual void* MapTexture(ITexture* texture, const Subresource& subres, EMapType mapType, EMapFlags mapFlags) = 0;
        virtual void UnmapTexture(ITexture* texture, const Subresource& subres) = 0;

        virtual void Draw(const DrawAttribs& attribs) = 0;
        virtual void DispatchCompute(const DispatchComputeAttribs& attribs) = 0;
        virtual void ClearDepthStencil(ITextureView* view, float depth, Optional<float> stencil = std::nullopt) = 0;
        virtual void ClearRenderTarget(ITextureView* view, const Float4& color) = 0;
        virtual void FinishCommandList(ICommandList* cmdList) = 0;
        virtual void ExecuteCommandList(ICommandList* cmdList) = 0;
        virtual void SignalFence(IFence* fence, Uint64 value) = 0;
        virtual void GenerateMips(ITextureView* view) = 0;
        virtual void Flush() = 0;
        virtual void FinishFrame() = 0;
        virtual void TransitionResourceStates(Array<StateTransitionDesc> barriers) = 0;
    };
}