#pragma once

#include "GIDeviceVk.h"
#include "GIDeviceQueueVk.h"
#include "GIIDeviceMemoryVk.h"
#include "GISwapchainVk.h"
#include "GIFramebufferVk.h"
#include "GIRenderPassVk.h"
#include "GIGraphicsPipelineVk.h"
#include "GIComputePipelineVk.h"
#include "GIRenderStatesVk.h"
#include "GIShaderModuleVk.h"
#include "GIShaderProgramVk.h"
#include "GIBufferVk.h"
#include "GIBufferViewVk.h"
#include "GITextureVk.h"
#include "GITextureViewVk.h"
#include "GISamplerVk.h"
#include "GICommandBufferVk.h"
#include "GICommandPoolVk.h"
#include "GIIDescriptorVk.h"
#include "GIDescriptorSetVk.h"
#include "GIDescriptorPoolVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceContextVk : public std::enable_shared_from_this<GIDeviceContextVk>
    {
    public:
        struct FrameSyncInfo
        {
            VkSemaphore presentCompleted = VK_NULL_HANDLE; // Signaled by vkAcquireNextImage(...)
            VkSemaphore renderCompleted = VK_NULL_HANDLE;  // Signaled by the last call to vkQueueSubmit(...) and waited by vkQueuePresent(...)
            VkFence submittedFence = VK_NULL_HANDLE;       // Signaled by each vkQueueSubmit(...) and waited by vkAcquireNextImage(...)
        };

        static SharedPtr<GIDeviceContextVk> Create(SharedPtr<GIDeviceVk> device);
        virtual ~GIDeviceContextVk();

        void Flush();

    protected:
        GIDeviceContextVk(SharedPtr<GIDeviceVk> device);
        GIDeviceContextVk(const GIDeviceContextVk&) = delete;
        GIDeviceContextVk(GIDeviceContextVk&&) = default;
        GIDeviceContextVk& operator=(const GIDeviceContextVk&) = delete;
        GIDeviceContextVk& operator=(GIDeviceContextVk&&) = default;

        
    protected:
        SharedPtr<GIDeviceVk> mDevice;
        SharedPtr<GISwapchainVk> mSwapchain;
        SharedPtr<GIDescriptorPoolVk> mDescriptorPool;
        
        uint32_t mCurrentFrame = 0;
        std::vector<FrameSyncInfo> mFrameSyncInfos;
    };
}