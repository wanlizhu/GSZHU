#include "VulkanRHI/RHICommandQueue.h"

namespace Wanli
{
    RHICommandQueue::RHICommandQueue(RHIDevice* device, Uint familyIndex)
        : RHIDeviceObject(device, "VkQueue")
        , mFamilyIndex(familyIndex)
    {
        vkGetDeviceQueue(device->GetHandle(), mFamilyIndex, 0, &mQueue);
        if (mQueue == VK_NULL_HANDLE)
        {

        }
    }

    void RHICommandQueue::Destroy()
    {}
}