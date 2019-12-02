#include "Wanlix/Core3D/Graphics/RHI/Buffer.h"

namespace Wanlix
{
    Range Buffer::GetRange() const
    {
        return Range(0, GetDescriptor().size);
    }

    Buffer::Buffer(
        StringCRef name,
        std::shared_ptr<Device> device,
        long bindFlags
    ) noexcept
        : Resource(name, device)
        , mBindFlags(bindFlags)
    {}
}