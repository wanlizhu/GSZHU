#include "Wanlix/Core3D/Graphics/RHI/Resource.h"

namespace Wanlix
{
    Resource::Resource(StringCRef name) noexcept
        : GObject(name)
    {}
}