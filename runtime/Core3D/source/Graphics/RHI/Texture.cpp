#include "Wanlix/Core3D/Graphics/RHI/Texture.h"
#include "Wanlix/Core3D/Graphics/Config.h"
#include <cmath>

namespace Wanlix
{
    Texture::Texture(
        StringCRef name,
        std::shared_ptr<Device> device,
        TextureType type
    ) 
        : Resource(name, device)
        , mTextureType(type)
    {}

    Region Texture::GetRegion() const
    {
        return Region(Offset(), GetDescriptor().extent);
    }
}