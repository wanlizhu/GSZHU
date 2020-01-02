#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "Wanlix/RHI/IDeviceObject.h"

namespace Wanlix
{
    class LogicalDevice;

    template<typename T>
    class VulkanObject final : public IDeviceObject
    {
        CLASSINFO(VulkanObject)
        INHERIT_SHARED_FROM_THIS(IDeviceObject)
    public:
        friend class LogicalDevice;
        using ObjectType = T;

        VulkanObject();
        VulkanObject(SharedPtr<LogicalDevice> device, ObjectType&& object);
        explicit VulkanObject(ObjectType object); // This constructor does not take ownership of the vulkan object
        virtual ~VulkanObject(); 

        operator T() const;
        void Release();

    private:
        SharedPtr<LogicalDevice> mLogicalDevice;
        ObjectType mObject;
    };


    using VkCommandPoolObject = VulkanObject<VkCommandPool>;
    using VkBufferObject = VulkanObject<VkBuffer>;
    using VkBufferViewObject = VulkanObject<VkBufferView>;
    using VkImageObject = VulkanObject<VkImage>;
    using VkImageViewObject = VulkanObject<VkImageView>;
    using VkDeviceMemoryObject = VulkanObject<VkDeviceMemory>;
    using VkFenceObject = VulkanObject<VkFence>;
    using VkRenderPassObject = VulkanObject<VkRenderPass>;
    using VkPipelineObject = VulkanObject<VkPipeline>;
    using VkShaderModuleObject = VulkanObject<VkShaderModule>;
    using VkPipelineLayoutObject = VulkanObject<VkPipelineLayout>;
    using VkSamplerObject = VulkanObject<VkSampler>;
    using VkFramebufferObject = VulkanObject<VkFramebuffer>;
    using VkDescriptorPoolObject = VulkanObject<VkDescriptorPool>;
    using VkDescriptorSetLayoutObject = VulkanObject<VkDescriptorSetLayout>;
    using VkSemaphoreObject = VulkanObject<VkSemaphore>;

}