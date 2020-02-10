#include "Graphics/VulkanRHI/RHIDeviceObject.h"

namespace Wanli
{
    static Uint lastUniqueID = 0;

    RHIDeviceObject::RHIDeviceObject(RHIDevice* device, const char* name)
        : mDevice(device->shared_from_this())
        , mUniqueID(++lastUniqueID)
        , mName(name)
    {}

    RHIDeviceObject::~RHIDeviceObject()
    {
        Destroy();
    }
}