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
        static SharedPtr<LogicalDevice> Create(VkPhysicalDevice device,
                                               VkDeviceCreateInfo const& deviceCi,
                                               VkAllocationCallbacks* allocator);

        virtual ~LogicalDevice();
        VkQueue GetQueue(Uint32 queueFamily, Uint32 queueIndex) const;
        VkDevice GetVkDevice() const;
        VkAllocationCallbacks* GetVkAllocator() const;
        VkPipelineStageFlags GetEnabledShaderStages() const;
        
    private:
        VkDevice mVkDevice = VK_NULL_HANDLE;
        VkAllocationCallbacks* mVkAllocator = nullptr;
        VkPipelineStageFlags mEnabledShaderStages = 0;
    };

}