#include "Wanlix/Core3D/RHI/IResource.h"

namespace Wanlix
{
    IResource::IResource(const std::string& name) noexcept
        : IDeviceObject(name)
    {}
}