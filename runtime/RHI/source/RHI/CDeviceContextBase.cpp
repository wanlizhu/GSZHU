#include "RHI/CDeviceContextBase.h"

namespace Wanlix
{
    Bool CDeviceContextBase::IsDeferred() const
    {
        return mIsDeferred;
    }

    void CDeviceContextBase::CommitShaderResources(IShaderResourceBinding* resourceBinding)
    {
        
    }

    void CDeviceContextBase::SetPipelineState(IPipelineState* pso)
    {
        mPipelineState = ToPtr<IPipelineState>(pso->GetSharedPtr());
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
        mVertexBuffers[slot] = ToPtr<IBuffer>(buffer->GetSharedPtr());
    }

    void CDeviceContextBase::SetIndexBuffer(IBuffer* buffer, Uint offset)
    {
        mIndexBuffer = ToPtr<IBuffer>(buffer->GetSharedPtr());
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

    void CDeviceContextBase::SetViewports(const Array<Viewport>& viewports)
    {

    }

    void CDeviceContextBase::SetScissors(const Array<Rect>& rects)
    {

    }

    void CDeviceContextBase::SetRenderTarget(const Array<ITextureView>& targets, ITextureView* depthStencil)
    {

    }

    void CDeviceContextBase::SetSwapChain(ISwapChain* swapChain)
    {
        mSwapChain = ToPtr<ISwapChain>(swapChain->GetSharedPtr());
    }
}