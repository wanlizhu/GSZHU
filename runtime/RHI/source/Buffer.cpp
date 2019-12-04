#include "Wanlix/RHI/Buffer.h"

namespace Wanlix
{
    Buffer::Buffer(
        const std::string& name,
        std::shared_ptr<Device> device
    ) noexcept
        : Resource(name, device)
    {

    }
}