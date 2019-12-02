#pragma once

#include "Wanlix/Core3D/Graphics/Types.h"
#include "Resource.h"
#include "TextureDescriptor.h"

namespace Wanlix
{
    class Texture : public Resource
    {
    public:
        using Ptr      = std::shared_ptr<Texture>;
        using ConstPtr = std::shared_ptr<const Texture>;
        using Desc     = TextureDescriptor;

        virtual const Desc& GetDescriptor() const = 0;
        virtual Extent      GetMipExtent(int mipLevel) const = 0;

        TextureType GetType() const;
        Region      GetRegion() const;
        
    protected:
        Texture(
            StringCRef name,
            std::shared_ptr<Device> device, 
            TextureType type
        );

    protected:
        TextureType mTextureType;
    };
}