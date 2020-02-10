#pragma once

#include "BasicTypes.h"
#include "Graphics/VulkanRHI/RHIUtilities.h"
#include "Graphics/VulkanRHI/RHIDeviceObject.h"

namespace Wanli
{
    class RHIDevice;

    class DLLDECL RHIBuffer : public RHIDeviceObject
    {
    public:
        RHIBuffer(
            RHIDevice* device,
            VkBufferUsageFlags usageFlags,
            VkMemoryPropertyFlags memoryPropertyFlags,
            VkDeviceSize size,
            void* data = nullptr
        );
        virtual ~RHIBuffer();

        virtual void Destroy() override;
        VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void UnMap();
        VkResult Bind(VkDeviceSize offset = 0);
        void SetDescriptor(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void CopyFrom(void* data, VkDeviceSize size);
        VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

    protected:
        SharedPtr<RHIDevice> mDevice;

        VkBuffer mBuffer = VK_NULL_HANDLE;
        VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE;

        VkDescriptorBufferInfo mDescriptor;

        VkDeviceSize mSize = 0;
        VkDeviceSize mAlignment = 0;

        void* mMapped = nullptr;

        VkBufferUsageFlags mUsageFlags;
        VkMemoryPropertyFlags mMemoryPropertyFlags;
    };
}