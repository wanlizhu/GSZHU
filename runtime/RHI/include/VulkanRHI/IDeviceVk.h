#pragma once

#include "RHI/IDevice.h"
#include "VulkanRHI/VkUtilities.h"

namespace Wanlix
{
    class IDeviceVk : public IDevice
    {
    public:
        virtual VkDevice GetVkDevice() const = 0;
        virtual Uint64 GetNextFenceValue(Uint queueIndex) const = 0;
        virtual Uint64 GetCompletedFenceValue(Uint queueIndex) const = 0;
        virtual Bool IsFenceSignaled(Uint queueIndex, Uint64 fenceValue) = 0;
        virtual SharedPtr<ITexture> CreateTextureFromVkImage(VkImage image, const TextureDesc& desc) = 0;
        virtual SharedPtr<IBuffer> CreateBufferFromVkBuffer(VkBuffer buffer, const BufferDesc& desc) = 0;
    };
}