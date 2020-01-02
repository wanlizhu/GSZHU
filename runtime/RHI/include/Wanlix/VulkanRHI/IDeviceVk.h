#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "Wanlix/RHI/IDevice.h"

namespace Wanlix
{
    class RHI_API IDeviceVk : public IDevice
    {
        CLASSINFO(IDeviceVk)
        INHERIT_SHARED_FROM_THIS(IDevice)
    public:
        virtual VkDevice GetVkDevice() = 0;
        virtual Uint64 GetNextFenceValue(Uint queueIndex) = 0;
        virtual Uint64 GetCompletedFenceValue(Uint queueIndex) = 0;
        virtual Bool IsFenceSignaled(Uint queueIndex, Uint64 fenceValue) = 0;
        virtual ITexture* CreateTextureFromVulkanImage(VkImage image,
                                                       TextureDesc const& desc,
                                                       ResourceState state) = 0;
        virtual IBuffer* CreateBufferFromVulkanResource(VkBuffer buffer,
                                                        BufferDesc const& desc,
                                                        ResourceState state) = 0;
    };
}