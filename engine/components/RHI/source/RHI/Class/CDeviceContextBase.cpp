#include "RHI/Class/CDeviceContextBase.h"
#include "Utils/Logger.h"

namespace Wanlix
{
    Bool CDeviceContextBase::IsDeferred() const
    {
        return mIsDeferred;
    }

    void CDeviceContextBase::CommitShaderResources(IShaderResourceBinding* resourceBinding)
    {
        
    }

    void CDeviceContextBase::SetPipeline(IPipeline* pipeline)
    {
        mPipeline = pipeline->GetSharedPtr<IPipeline>();
    }

    void CDeviceContextBase::SetStencilRef(Uint stencil)
    {
        mStencilRef = stencil;
    }

    void CDeviceContextBase::SetBlendFactors(const Float4& blendFactors)
    {
        mBlendFactors = blendFactors;
    }

    void CDeviceContextBase::SetVertexBuffer(Uint slot, IBuffer* buffer)
    {
        assert(slot >= 0 && slot < MAX_NUM_BUFFER_SLOTS);
        mVertexBuffers[slot] = buffer->GetSharedPtr<IBuffer>();
    }

    void CDeviceContextBase::SetIndexBuffer(IBuffer* buffer, Uint offset)
    {
        mIndexBuffer = buffer->GetSharedPtr<IBuffer>();
        mIndexDataOffset = offset;
#ifdef _DEBUG
        if (mIndexBuffer)
        {
            const auto& desc = (const BufferDesc&)mIndexBuffer->GetDesc();
            if (!HasBits(desc.bindFlags, EBindFlags::IndexBuffer))
            {
                LogError("Buffer '%s' being bound as index buffer was not created with EBindFlags::IndexBuffer flag.",
                         mIndexBuffer->GetName().c_str());
            }
        }
#endif
    }

    void CDeviceContextBase::SetViewport(Uint index, const Viewport& viewport)
    {
        mViewports[index] = viewport;
    }

    void CDeviceContextBase::SetScissor(Uint index, const Rect& rect)
    {
        mScissors[index] = rect;
    }

    void CDeviceContextBase::SetRenderTarget(Uint index, ITextureView* target)
    {
        mRenderTargets[index] = target->GetSharedPtr<ITextureView>();
    }

    void CDeviceContextBase::SetDepthStencilTarget(ITextureView* view)
    {
        mDepthStencil = view->GetSharedPtr<ITextureView>();
    }

    void CDeviceContextBase::SetSwapChain(ISwapChain* swapChain)
    {
        mSwapChain = swapChain->GetSharedPtr<ISwapChain>();
    }
}