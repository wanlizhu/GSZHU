#pragma once

#include "Wanlix/Core3D/Types.h"
#include "IResource.h"
#include "TextureDescriptor.h"

namespace Wanlix
{
    class ITexture : public IResource
    {
    public:
        using Ptr = std::shared_ptr<ITexture>;
        using ConstPtr = std::shared_ptr<const ITexture>;

        virtual TextureType GetTextureType() const = 0;
        virtual TextureDescriptor GetTextureDescriptor() const = 0;
        virtual Extent GetMipExtent(uint32_t mipLevel) const = 0;

    protected:
        ITexture(const std::string& name) noexcept;
    };
}