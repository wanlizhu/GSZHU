#pragma once

#include "GraphicTypes.h"
#include <stdint.h>
#include <assert.h>

namespace GSZHU {
    class CTextureFormat {
    public:
        CTextureFormat(ETextureDimension dim, EPixelFormat format,
                       uint32_t width, uint32_t height = 0, uint32_t depth = 0,
                       uint32_t mips = 0, uint32_t samples = 1)
            : mDimension(dim)
            , mWidth(width)
            , mHeight(height)
            , mDepth(depth)
            , mPixelFormat(format)
            , mMipLevels(mips)
            , mSampleCount(samples)
        {}

        ETextureDimension GetDimension() const { return mDimension; }
        uint32_t GetWidth() const { return mWidth; }
        uint32_t GetHeight() const { assert(mHeight != 0); return mHeight; }
        uint32_t GetDepth() const { assert(mDepth != 0);  return mDepth; }
        EPixelFormat GetPixelFormat() const { return mPixelFormat; }
        uint32_t GetMipLevels() const { return mMipLevels; }
        uint32_t GetSampleCount() const { return mSampleCount; }

    private:
        ETextureDimension mDimension = ETextureDimension::Undefined;
        uint32_t mWidth = 0;
        uint32_t mHeight = 0;
        uint32_t mDepth = 0;
        EPixelFormat mPixelFormat = EPixelFormat::Unknown;
        uint32_t mMipLevels = 0; // Specify 0 to generate full mipmap chain
        uint32_t mSampleCount = 1; // Only 2D textures or 2D texture arrays can be multisampled
    };
}