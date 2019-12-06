#pragma once

#include "DeviceObject.h"
#include "ResourceHeapDescriptor.h"

namespace Wanlix
{
    /*
      An instance of this interface provides a descriptor set (as called in Vulkan)
      or descriptor heap (as called in Direct3D 12) for graphics and compute pipelines.
    */
    class ResourceHeap : public DeviceObject
    {
    public:
        using SharedPtr = std::shared_ptr<ResourceHeap>;
        using UniquePtr = std::unique_ptr<ResourceHeap>;
        using WeakPtr   = std::weak_ptr<ResourceHeap>;
        using Desc      = ResourceHeapDescriptor;

    protected:
        ResourceHeap(
            const std::string& name,
            std::shared_ptr<Device> device
        ) noexcept;
    };
}