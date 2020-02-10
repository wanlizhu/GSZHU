#pragma once

#include "Graphics/VulkanRHI/RHIUtilities.h"
#include "Graphics/VulkanRHI/RHIDeviceObject.h"
#include "BasicTypes.h"

namespace Wanli
{
    class RHIDevice;
    class RHIQueue;
    class RHIQueue;
    class RHIDescriptorSet;

    class DLLDECL RHICommandBuffer : public RHIDeviceObject
    {
    public:
        RHICommandBuffer(
            RHIDevice* device,
            VkCommandPool commandPool,
            VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            RHIQueue* queue = nullptr
        );
        virtual ~RHICommandBuffer();

        virtual void Destroy() override;

        void Begin();
        void End();
        void Submit(VkSemaphore* semaphore = nullptr);

        void BindDescriptorSet(RHIDescriptorSet* descriptorSet);


    protected:
        SharedPtr<RHIQueue> mQueue;

        VkCommandBuffer mCommandBuffer = VK_NULL_HANDLE;
        VkFence mFence = VK_NULL_HANDLE;
        VkCommandPool mCommandPool = VK_NULL_HANDLE;
        Array<VkPipelineStageFlags> mWaitFlags;
        Array<VkSemaphore> mWaitSemaphores;

        bool mIsBegun = false;
    };
}