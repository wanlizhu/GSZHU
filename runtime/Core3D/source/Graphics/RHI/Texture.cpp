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

    size_t Texture::GetDataSize() const
    {
        const Desc& desc = GetDescriptor();
        size_t pixels = (desc.extent.width * desc.extent.height * desc.extent.depth);
        size_t bytes = pixels * GetFormatAttribs(desc.format).bitSize / 32;
        return bytes;
    }

    TextureType Texture::GetType() const
    {
        return mTextureType;
    }

    TextureRegion Texture::GetRegion() const
    {
        TextureRegion region;
        region.offset = Offset();
        region.extent = GetDescriptor().extent;
        return region;
    }
}