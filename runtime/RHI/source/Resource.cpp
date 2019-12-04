#include "Wanlix/RHI/Resource.h"

namespace Wanlix
{
    Resource::Resource(
        const std::string& name,
        std::shared_ptr<Device> device
    ) noexcept
        : DeviceObject(name, device)
    {

    }
}