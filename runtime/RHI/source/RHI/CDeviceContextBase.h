#pragma once

#include "RHI/IDeviceContext.h"
#include "RHI/CDeviceObjectBase.h"
#include "RHI/ISwapChain.h"
#include "RHI/IPipelineState.h"

namespace Wanlix
{
    class CDeviceContextBase : public IDeviceContext
    {
    public:
        virtual Bool IsDeferred() const override final;
        virtual void CommitShaderResources(IShaderResourceBinding* resourceBinding) override final;
        virtual IPipelineState* GetPipelineState() const override final;
        virtual Uint GetStencilRef() const override final;
        virtual const Float4& GetBlendFactor() const override final;
        virtual IBuffer* GetVertexBuffer(Uint slot) const override final;
        virtual IBuffer* GetIndexBuffer(Uint* offset = nullptr) const override final;
        virtual const Viewport& GetViewport(Uint index) const override final;
        virtual const Rect& GetScissor(Uint index) const override final;
        virtual ITextureView* GetRenderTarget(Uint index) const override final;
        virtual ISwapChain* GetSwapChain() const override final;

        virtual void SetPipelineState(IPipelineState* pso) override final;
        virtual void SetStencilRef(Uint stencil) override final;
        virtual void SetBlendFactors(const Float4& blendFactors) override final;
        virtual void SetVertexBuffer(Uint slot, IBuffer* buffer) override final;
        virtual void SetIndexBuffer(IBuffer* buffer, Uint offset = 0) override final;
        virtual void SetViewports(const Array<Viewport>& viewports) override final;
        virtual void SetScissors(const Array<Rect>& rects) override final;
        virtual void SetRenderTarget(const Array<ITextureView>& targets, ITextureView* depthStencil) override final;
        virtual void SetSwapChain(ISwapChain* swapChain) override final;

    protected:
        CDeviceContextBase(IDevice* device, Bool isDeferred)
            : mDevice(device->GetSharedPtr())
            , mIsDeferred(isDeferred)
            , mVertexBuffers(MAX_NUM_BUFFER_SLOTS)
            , mViewports(MAX_NUM_VIEWPORTS)
            , mScissors(MAX_NUM_VIEWPORTS)
            , mRenderTargets(MAX_NUM_RENDER_TARGETS)
        {}

    protected:
        Bool mIsDeferred = false;
        SharedPtr<IDevice> mDevice;
        SharedPtr<ISwapChain> mSwapChain;
        Array<SharedPtr<IBuffer>> mVertexBuffers;
        SharedPtr<IBuffer> mIndexBuffer;
        SharedPtr<IPipelineState> mPipelineState;
        Uint mIndexDataOffset = 0;
        Uint mStencilRef = 0;
        Float4 mBlendFactors = Float4(0, 0, 0, 0);
        Array<Viewport> mViewports;
        Array<Rect> mScissors;
        Array<SharedPtr<ITextureView>> mRenderTargets;
        SharedPtr<ITextureView> mDepthStencil;
        Uint mFramebufferWidth = 0;
        Uint mFramebufferHeight = 0;
        Uint mFramebufferSlices = 0;
    };
}