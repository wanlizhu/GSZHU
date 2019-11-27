#include "Wanlix/Core3D/RHI/ITexture.h"

namespace Wanlix
{
    ITexture::ITexture(const std::string& name) noexcept
        : IResource(name)
    {}
}