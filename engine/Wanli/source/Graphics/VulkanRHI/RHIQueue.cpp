#include "Graphics/VulkanRHI/RHIQueue.h"

namespace Wanli
{
    RHIQueue::RHIQueue(RHIDevice* device, Uint familyIndex)
        : RHIDeviceObject(device, "VkQueue")
        , mFamilyIndex(familyIndex)
    {
        vkGetDeviceQueue(device->GetHandle(), mFamilyIndex, 0, &mQueue);
        if (mQueue == VK_NULL_HANDLE)
        {

        }
    }

    void RHIQueue::Destroy()
    {}
}