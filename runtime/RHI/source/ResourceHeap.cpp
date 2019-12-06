#include "Wanlix/RHI/ResourceHeap.h"

namespace Wanlix
{
    ResourceHeap::ResourceHeap(
        const std::string& name,
        std::shared_ptr<Device> device
    ) noexcept
        : DeviceObject(name, device)
    {

    }
}