#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "VulkanObject.h"

namespace Wanlix
{
    class LogicalDevice final : public Object
    {
        CLASSINFO(LogicalDevice)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        using SharedPtr = std::shared_ptr<LogicalDevice>;

        static SharedPtr Create(VkPhysicalDevice device, 
                                VkDeviceCreateInfo const& deviceCi,
                                VkAllocationCallbacks const* allocator);

        VkQueue GetQueue(Uint32 queueFamily, Uint32 queueIndex) const;
        VkDevice GetVkDevice() const;
        void WaitIdle() const;

        VkCommandPoolObject CreateCommandPool(VkCommandPoolCreateInfo const& ci, const char* debugName = "") const;
        VkBufferObject CreateBuffer(VkBufferCreateInfo const& ci, const char* debugName) const;
        VkBufferViewObject CreateBufferView(VkBufferViewCreateInfo const& ci, const char* debugName) const;
        VkImageObject CreateImage(VkImageCreateInfo const& ci, const char* debugName) const;
        VkImageViewObject CreateImageView(VkImageViewCreateInfo const& ci, const char* debugName) const;
        VkSamplerObject CreateSampler(VkSamplerCreateInfo const& ci, const char* debugName) const;
        VkFenceObject CreateFence(VkFenceCreateInfo const& ci, const char* debugName) const;
        VkRenderPassObject CreateRenderPass(VkRenderPassCreateInfo const& ci, const char* debugName) const;
        VkDeviceMemoryObject AllocateDeviceMemory(VkMemoryAllocateInfo const& info, const char* debugName) const;
        VkPipelineObject CreatePipeline(VkComputePipelineCreateInfo const& ci, VkPipelineCache cache, const char* debugName) const;
        VkPipelineObject CreatePipeline(VkGraphicsPipelineCreateInfo const& ci, VkPipelineCache cache, const char* debugName) const;
        VkShaderModuleObject CreateShaderModule(VkShaderModuleCreateInfo const& ci, const char* debugName) const;
        VkPipelineLayoutObject CreatePipelineLayout(VkPipelineLayoutCreateInfo const& ci, const char* debugName) const;
        VkFramebufferObject CreateFramebuffer(VkFramebufferCreateInfo const& ci, const char* debugName) const;
        VkDescriptorPoolObject CreateDescriptorPool(VkDescriptorPoolCreateInfo const& ci, const char* debugName) const;
        VkDescriptorSetLayoutObject CreateDescriptorSetLayout(VkDescriptorSetLayoutCreateInfo const& ci, const char* debugName) const;
        VkSemaphoreObject CreateSemaphore(VkSemaphoreCreateInfo const& ci, const char* debugName) const;
        VkCommandBuffer AllocateCommandBuffer(VkCommandBufferAllocateInfo const& info, const char* debugName) const;
        VkDescriptorSet AllocateDescriptorSet(VkDescriptorSetAllocateInfo const& info, const char* debugName) const;

    };


}