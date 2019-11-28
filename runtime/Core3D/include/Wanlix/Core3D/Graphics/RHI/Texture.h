#pragma once

#include "Wanlix/Core3D/Types.h"
#include "Resource.h"
#include "TextureDescriptor.h"

namespace Wanlix
{
    class Texture : public Resource
    {
    public:
        using Ptr      = std::shared_ptr<Texture>;
        using ConstPtr = std::shared_ptr<const Texture>;

        virtual TextureType GetTextureType() const = 0;
        virtual TextureDescriptor GetTextureDescriptor() const = 0;
        virtual Extent GetMipExtent(uint32_t mipLevel) const = 0;

    protected:
        Texture(const std::string& name) noexcept;
    };
}