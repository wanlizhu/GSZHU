#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceVk;
    class GICommandBufferVk;
    class GIPipelineVk;
    class GIRenderPassVk;
    class GIFramebufferVk;
    class GIImageVk;

    class GIDeviceContextVk 
        : public GINonCopyable
        , public std::enable_shared_from_this<GIDeviceContextVk>
    {
    public:
        static SharedPtr<GIDeviceContextVk> Create(WeakPtr<GIDeviceVk> device);

        virtual ~GIDeviceContextVk();
        bool IsValid() const;
        void FlushCommand(bool waitIdle);
        void SetCommandBuffer(SharedPtr<GICommandBufferVk> cmdbuf);
        void SetFramebuffer(SharedPtr<GIFramebufferVk> framebuffer);
        void SetRenderPass(SharedPtr<GIRenderPassVk> renderPass);
        void SetPipeline(SharedPtr<GIPipelineVk> pipeline);
        void SetDepthStencilImage(SharedPtr<GIImageVk> depthStencil);

        SharedPtr<GICommandBufferVk> GetCommandBuffer() const;

    private:
        GIDeviceContextVk();

    private:
        WeakPtr<GIDeviceVk> mDevice;

        SharedPtr<GICommandBufferVk> mCommandBuffer;
        SharedPtr<GIFramebufferVk> mFramebuffer;
        SharedPtr<GIRenderPassVk> mRenderPass;
        SharedPtr<GIPipelineVk> mPipeline;
        SharedPtr<GIImageVk> mDepthStencilImage;
    };
}