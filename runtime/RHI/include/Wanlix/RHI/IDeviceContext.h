#pragma once

#include "Commands.h"
#include "IBuffer.h"

namespace Wanlix
{
    class RHI_API IDeviceContext : public Object
    {
        CLASSINFO(IDeviceContext)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        virtual ~IDeviceContext() = default;

        virtual void SetPipelineState(IPipelineState* pipeline) = 0;
        virtual void TransitionResources(IPipelineState* pipeline, IResourceBinding* binding) = 0;
        virtual void CommitResources(IResourceBinding* binding, RSTMode transitionMode) = 0;
        
        virtual void SetStencilRef(Uint stencilRef) = 0;
        virtual void SetBlendFactors(Color4f const& factors) = 0;
        virtual void SetVertexBuffers(Uint startSlot,
                                      Uint numBuffers,
                                      IBuffer** buffers,
                                      Uint* offsets,
                                      RSTMode transitionMode,
                                      Uint flags) = 0;
        virtual void SetIndexBuffer(IBuffer* buffer, Uint offset, RSTMode transitionMode) = 0;
        virtual void SetViewports(Uint numViewports, Viewport const* viewports) = 0;
        virtual void SetScissors(Uint numScissors, Rect const* scissors) = 0;
        virtual void SetRenderTargets(Uint numTargets,
                                      ITextureView** targets, 
                                      ITexture* depthStencil, 
                                      RSTMode transitionMode) = 0;
        virtual void Draw(DrawAttribs const& drawAttribs) = 0;
        virtual void DispatchCompute(ComputeAttribs const& computeAttribs) = 0;
        virtual void ClearDepthStencil(ITexture* view, 
                                       Uint clearFlags,
                                       float depth,
                                       Uint stencil,
                                       RSTMode transitionMode) = 0;
        virtual void ClearRenderTarget(ITextureView* view, Color4f const& rgba, RSTMode transitionMode) = 0;
        virtual void SetClearValue(ClearValue const& value) = 0;
        virtual void Clear(Uint clearFlags) = 0;
        virtual void EndCommandBuffer(ICommandBuffer* commandBuffer) = 0;
        virtual void ExecuteCommandBuffer(ICommandBuffer* commandBuffer) = 0;
        virtual void SignalFence(IFence* fence, Ui64 value) = 0;
        virtual void Flush() = 0;

        virtual void UpdateBuffer(IBuffer* buffer,
                                  Uint offset, 
                                  Uint size,
                                  const void* data,
                                  RSTMode transitionMode) = 0;
        virtual void CopyBuffer(IBuffer* src,
                                Uint     srcOffset,
                                RSTMode  srcTransMode,
                                IBuffer* dest,
                                IBuffer* destOffset,
                                Uint     size,
                                RSTMode destTransMode) = 0;
        virtual Pvoid MapBuffer(IBuffer* buffer, MapType mapType, Uint mapFlags) = 0;
        virtual void UnMapBuffer(IBuffer* buffer, MapType mapType) = 0;
    
        virtual void UpdateTexture(ITexture* texture,
                                   Uint   mipLevel,
                                   Uint   slice,
                                   Region region,
                                   SubresourceData const& subresourceData,
                                   RSTMode srcTransMode,
                                   RSTMode destTransMode) = 0;
        virtual void CopyTexture(CopyAttribs const& copyAttribs) = 0;
        virtual Pvoid MapTextureSubresource(ITexture* texture,
                                            Uint    mipLevel,
                                            Uint    slice,
                                            MapType mapType,
                                            Uint    mapFlags,
                                            Region  region) = 0;
        virtual void UnMapTextureSubresource(ITexture* texture, Uint mipLevel, Uint slice) = 0;
        virtual void GenerateMips(ITextureView* view) = 0;

        virtual void SetSwapChain(ISwapChain* swapchain) = 0;
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
        virtual void ResourceStateTransition(Uint numBarriers, StateTransitionDesc* barriers) = 0;
    };
}