#include "GIDeviceContextVk.h"
#include "GIDeviceVk.h"
#include "GITextureVk.h"
#include "GIFramebufferVk.h"
#include "GIRenderPassVk.h"
#include "GIPipelineVk.h"
#include "GICommandBufferVk.h"
#include "GICommandPoolVk.h"
#include "GICommandQueueVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIDeviceContextVk> GIDeviceContextVk::Create(WeakPtr<GIDeviceVk> device)
    {
        return SharedPtr<GIDeviceContextVk>(new GIDeviceContextVk());
    }

    GIDeviceContextVk::GIDeviceContextVk()
    {}

    GIDeviceContextVk::~GIDeviceContextVk()
    {}

    bool GIDeviceContextVk::IsValid() const
    {
        return !mDevice.expired()
            && mCommandBuffer
            && mCommandBuffer->IsValid()
            && mFramebuffer
            && mFramebuffer->IsValid()
            && mRenderPass
            && mRenderPass->IsValid()
            && mPipeline
            && mPipeline->IsValid()
            && mDepthStencilImage
            && mDepthStencilImage->IsValid();
    }

    void GIDeviceContextVk::FlushCommand(bool waitIdle)
    {
        //TODO
    }

    void GIDeviceContextVk::SetCommandBuffer(SharedPtr<GICommandBufferVk> cmdbuf)
    {
        mCommandBuffer = cmdbuf;
    }

    void GIDeviceContextVk::SetFramebuffer(SharedPtr<GIFramebufferVk> framebuffer)
    {
        mFramebuffer = framebuffer;
    }

    void GIDeviceContextVk::SetRenderPass(SharedPtr<GIRenderPassVk> renderPass)
    {
        mRenderPass = renderPass;
    }

    void GIDeviceContextVk::SetPipeline(SharedPtr<GIPipelineVk> pipeline)
    {
        mPipeline = pipeline;
    }

    void GIDeviceContextVk::SetDepthStencilImage(SharedPtr<GITextureVk> depthStencil)
    {
        mDepthStencilImage = depthStencil;
    }

    SharedPtr<GICommandBufferVk> GIDeviceContextVk::GetCommandBuffer() const
    {
        return mCommandBuffer;
    }
}