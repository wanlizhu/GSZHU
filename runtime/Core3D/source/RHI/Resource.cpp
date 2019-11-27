#include "Wanlix/Core3D/RHI/Resource.h"

namespace Wanlix
{
    Resource::Resource(const std::string& name) noexcept
        : DeviceObject(name)
    {}
}