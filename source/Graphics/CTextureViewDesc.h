#pragma once

#include "IResourceDesc.h"
#include "CTextureFormat.h"

namespace GSZHU {
    class CTextureViewDesc : public IResourceDesc {
    public:
        CTextureViewDesc(ETextureViewType type, ETextureDimension dim, EPixelFormat format, 
                         uint32_t mips = 0, EAccess access = EAccess::None)
            : IResourceDesc(typeid(CTextureViewDesc).name())
            , mType(type)
            , mDimension(dim)
            , mPixelFormat(format)
            , mNumMipLevels(mips)
            , mUAVAccess(access)
        {}

        ETextureViewType GetViewType() const { return mType; }
        ETextureDimension GetDimension() const { return mDimension; }
        EPixelFormat GetPixelFormat() const { return mPixelFormat; }
        uint32_t GetNumMipLevels() const { return mNumMipLevels; }
        EAccess GetUAVAccess() const { return mUAVAccess; }

    private:
        ETextureViewType mType = ETextureViewType::Undefined;
        ETextureDimension mDimension = ETextureDimension::Undefined;
        EPixelFormat mPixelFormat;
        uint32_t mNumMipLevels = 0; // For a render target or a depth stencil view, only one mip level
        EAccess mUAVAccess = EAccess::None;
    };

}