#pragma once

#include "IResourceDesc.h"
#include "CTextureFormat.h"

namespace GSZHU {
    class CTextureViewDesc : public IResourceDesc {
    public:
        CTextureViewDesc(ETextureView viewType, ETexture texType, EPixelFormat format)
            : IResourceDesc(typeid(CTextureViewDesc).name())
            , mViewType(viewType)
            , mTextureType(texType)
            , mPixelFormat(format)
        {}

        ETextureView GetViewType() const { return mViewType; }
        ETexture GetTextureType() const { return mTextureType; }
        EPixelFormat GetPixelFormat() const { return mPixelFormat; }
        uint32_t GetNumMipLevels() const { return mNumMipLevels; }
        EAccess GetUAVAccess() const { return mUAVAccess; }

        void SetNumMipLevels(uint32_t levels) { mNumMipLevels = levels; }
        void SetUAVAccess(EAccess access) { mUAVAccess = access; }

    private:
        ETextureView mViewType = ETextureView::Undefined;
        ETexture mTextureType = ETexture::Undefined;
        EPixelFormat mPixelFormat;
        uint32_t mNumMipLevels = 0; // For a render target or a depth stencil view, only one mip level
        EAccess mUAVAccess = EAccess::None;
    };

}