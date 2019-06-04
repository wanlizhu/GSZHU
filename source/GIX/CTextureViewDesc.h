#pragma once

#include "IResourceDesc.h"
#include "CTextureFormat.h"

namespace GSZHU {
    class CTextureViewDesc : public IResourceDesc {
    public:
        CTextureViewDesc(ETextureViewType viewType, ETextureType texType, EPixelFormat format)
            : IResourceDesc(typeid(CTextureViewDesc).name())
            , mViewType(viewType)
            , mTextureType(texType)
            , mPixelFormat(format)
        {}

        ETextureViewType GetViewType() const { return mViewType; }
        ETextureType GetTextureType() const { return mTextureType; }
        EPixelFormat GetPixelFormat() const { return mPixelFormat; }
        uint32_t GetNumMipLevels() const { return mNumMipLevels; }
        EAccess GetUAVAccess() const { return mUAVAccess; }

        void SetNumMipLevels(uint32_t levels) { mNumMipLevels = levels; }
        void SetUAVAccess(EAccess access) { mUAVAccess = access; }

    private:
        ETextureViewType mViewType = ETextureViewType::Undefined;
        ETextureType mTextureType = ETextureType::Undefined;
        EPixelFormat mPixelFormat;
        uint32_t mNumMipLevels = 0; // For a render target or a depth stencil view, only one mip level
        EAccess mUAVAccess = EAccess::None;
    };

}