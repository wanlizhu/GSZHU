#include "Wanlix/Core3D/Graphics/RHI/Resource.h"

namespace Wanlix
{
    Resource::Resource(
        StringCRef name,
        std::shared_ptr<Device> device
    ) noexcept
        : GObject(name, device)
    {}
}