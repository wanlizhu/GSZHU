#pragma once

#include "Common.h"
#include "EDeviceFeature.h"

namespace GS2::GI
{
    class DeviceObject;
    class ResourcePool;

    class Device : public std::enable_shared_from_this<Device>
    {
    public:
        using SharedPtr = std::shared_ptr<Device>;
        using WeakPtr   = std::weak_ptr<Device>;
        using UniquePtr = std::unique_ptr<Device>;

        

    private:
        void* _handle = nullptr;
        EDeviceFeature _supportedFeatures = EDeviceFeature::None;
        std::unique_ptr<ResourcePool> _resourcePool;
    };
}