#pragma once

#include "Common.h"
#include "EDeviceFeature.h"

namespace GS::GI
{
    class DeviceObject;
    class ResourcePool;

    class Device : public std::enable_shared_from_this<Device>
    {
    public:
        using Ptr = std::shared_ptr<Device>;

        

    private:
        void* mHandle = nullptr;
        EDeviceFeature mSupportedFeatures = EDeviceFeature::None;
        std::unique_ptr<ResourcePool> mResourcePool;
    };
}