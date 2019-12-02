#include "Wanlix/Core3D/Graphics/RHI/Buffer.h"

namespace Wanlix
{
    Buffer::Buffer(
        StringCRef name,
        std::shared_ptr<Device> device,
        long bindFlags
    )
        : Resource(name, device)
        , mBindFlags(bindFlags)
    {}

    size_t Buffer::GetSize() const
    {
        return GetDescriptor().size;
    }

    Range Buffer::GetRange() const
    {
        return Range(0, GetDescriptor().size);
    }
}