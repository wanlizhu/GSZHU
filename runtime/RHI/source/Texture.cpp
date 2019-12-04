#include "Wanlix/RHI/Texture.h"

namespace Wanlix
{
    Texture::Texture(
        const std::string& name,
        std::shared_ptr<Device> device
    ) noexcept
        : Resource(name, device)
    {

    }
}