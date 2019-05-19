#pragma once

#include <GSZHU/IObject.h>
#include <GSZHU/SDeviceCaps.h>
#include <GSZHU/Constants.h>
#include <GSZHU/IBuffer.h>
#include <GSZHU/DESC/SInputLayoutDesc.h>
#include <GSZHU/IShader.h>
#include <GSZHU/ITexture.h>
#include <GSZHU/ITextureView.h>
#include <GSZHU/ISampler.h>
#include <GSZHU/IResourceTable.h>
#include <GSZHU/IBufferView.h>
#include <GSZHU/IPipelineState.h>
#include <GSZHU/IFence.h>
#include <GSZHU/DESC/SDepthStencilStateDesc.h>
#include <GSZHU/DESC/SRasterizerStateDesc.h>
#include <GSZHU/DESC/SBlendStateDesc.h>
#include <GSZHU/SBufferData.h>
#include <GSZHU/SShaderCreationAttribs.h>
#include <GSZHU/STextureFormatInfo.h>
#include <GSZHU/ENUM/EResourceStateTransitionMode.h>
#include <GSZHU/ENUM/ESetVertexBufferFlags.h>
#include <GSZHU/SViewport.h>
#include <GSZHU/SDrawAttribs.h>
#include <GSZHU/SDisplayModeAttribs.h>
#include <GSZHU/SDispatchComputeAttribs.h>
#include <GSZHU/ENUM/EClearDepthStencilFlags.h>
#include <GSZHU/ICommandList.h>
#include <GSZHU/SCopyTextureAttribs.h>
#include <GSZHU/ISwapChain.h>
#include <GSZHU/ENUM/EMapType.h>
#include <GSZHU/ENUM/EMapFlags.h>
#include <GSZHU/SMappedTextureSubresource.h>
#include <GSZHU/DESC/SStateTransitionDesc.h>

namespace GSZHU {
    // Device context keeps strong references to all objects currently bound to the pipeline
    // The context also keeps strong reference to the device and the swap chain.
    class IDeviceContext : public IObject {
    public:
        virtual void SetPipelineState(IPipelineState* PipelineState) = 0;
        virtual void TransitionShaderResources(IPipelineState* PipelineState, IShaderResourceBinding* ShaderResourceBinding) = 0;
        virtual void CommitShaderResources(IShaderResourceBinding* ShaderResourceBinding, ERESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;
        virtual void SetStencilRef(uint32_t StencilRef) = 0;
        virtual void SetBlendFactors(const float* BlendFactors = nullptr) = 0;
        virtual void SetVertexBuffers(uint32_t StartSlot, uint32_t NumBuffersSet, IBuffer** Buffers, uint32_t* Offsets, ERESOURCE_STATE_TRANSITION_MODE StateTransitionMode, ESET_VERTEX_BUFFERS_FLAGS Flags) = 0;
        virtual void InvalidateState() = 0;
        virtual void SetIndexBuffer(IBuffer* IndexBuffer, uint32_t ByteOffset, ERESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;
        virtual void SetViewports(uint32_t NumViewports, const SViewport* Viewports, uint32_t RTWidth, uint32_t RTHeight) = 0;
        virtual void SetScissorRects(uint32_t NumRects, const RECT* Rects, uint32_t RTWidth, uint32_t RTHeight) = 0;
        virtual void SetRenderTargets(uint32_t NumRenderTargets, ITextureView** RenderTargets, ITextureView* DepthStencil, ERESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;
        virtual void Draw(const SDrawAttribs& DrawAttribs) = 0;
        virtual void DispatchCompute(const SDispatchComputeAttribs& DispatchAttrs) = 0;
        virtual void ClearDepthStencil(ITextureView* View, ECLEAR_DEPTH_STENCIL_FLAGS ClearFlags, float Depth, uint8_t Stencil, ERESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;
        virtual void ClearRenderTarget(ITextureView* View, const float* RGBA, ERESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;
        virtual void FinishCommandList(ICommandList** CommandLists) = 0;
        virtual void ExecuteCommandList(ICommandList* CommandList) = 0;
        virtual void SignalFence(IFence* Fence, uint64_t Value) = 0;
        virtual void Flush() = 0;

        virtual void UpdateBuffer(IBuffer* Buffer, uint32_t Offset, uint32_t Size, const void* Data, ERESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;
        virtual void CopyBuffer(IBuffer* SrcBuffer, uint32_t SrcOffset, ERESOURCE_STATE_TRANSITION_MODE SrcBufferTransitionMode,
                                IBuffer* DstBuffer, uint32_t DstOffset, uint32_t Size, ERESOURCE_STATE_TRANSITION_MODE DstBufferTransitionMode) = 0;
        virtual void* MapBuffer(IBuffer* Buffer, EMAP_TYPE MapType, EMAP_FLAGS MapFlags) = 0;
        virtual void UnmapBuffer(IBuffer* Buffer, EMAP_TYPE MapType) = 0;
        virtual void UpdateTexture(ITexture* Texture, uint32_t MipLevel, uint32_t Slice, const Box3D& DstBox, const STextureSubresourceData& SubresData,
                                   ERESOURCE_STATE_TRANSITION_MODE SrcBufferTransitionMode, ERESOURCE_STATE_TRANSITION_MODE TextureTransitionMode) = 0;
        virtual void CopyTexture(const SCopyTextureAttribs& CopyAttribs) = 0;
        virtual void MapTextureSubresource(ITexture* Texture, uint32_t MipLevel, uint32_t ArraySlice, EMAP_TYPE MapType, EMAP_FLAGS MapFlags, const Box3D* MapRegion, SMappedTextureSubresource& MappedData) = 0;
        virtual void UnmapTextureSubresource(ITexture* Texture, uint32_t MipLevel, uint32_t ArraySlice) = 0;
        virtual void GenerateMips(ITextureView* TextureView) = 0;
        virtual void SetSwapChain(ISwapChain* SwapChain) = 0;
        virtual void FinishFrame() = 0;
        virtual void TransitionResourceStates(uint32_t BarrierCount, SStateTransitionDesc* ResourceBarriers) = 0;
    };
}