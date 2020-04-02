#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceVk;
    class GICommandBufferVk;
    class GIPipelineVk;
    class GIRenderPassVk;
    class GIFramebufferVk;

    class GIDeviceContextVk 
        : public GINonCopyable
        , public std::enable_shared_from_this<GIDeviceContextVk>
    {
    public:
        static SharedPtr<GIDeviceContextVk> Create(WeakPtr<GIDeviceVk> device);

        virtual ~GIDeviceContextVk();
        void SetCommandBuffer(SharedPtr<GICommandBufferVk> cmdbuf);
        void FlushCommand();
        void FlushCommandIdle();

        SharedPtr<GICommandBufferVk> GetCommandBuffer() const;

    private:
        GIDeviceContextVk();

    private:
        WeakPtr<GIDeviceVk> mDevice;

        SharedPtr<GICommandBufferVk> mCommandBuffer;
        SharedPtr<GIFramebufferVk> mFramebuffer;
        SharedPtr<GIRenderPassVk> mRenderPass;
        SharedPtr<GIPipelineVk> mPipeline;
        
    };
}