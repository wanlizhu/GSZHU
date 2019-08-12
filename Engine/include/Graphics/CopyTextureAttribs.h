#pragma once

#include "Graphics/ITexture.h"
#include "Graphics/Tools.h"

namespace ZHU
{
    struct CopyTextureAttribs
    {
        ITexture::Pointer SourceTexture = nullptr;
        uint32_t SourceMipLevel = 0;
        uint32_t SourceSlice = 0;
        Box<int> SourceBox;

        ITexture::Pointer DestTexture = nullptr;
        uint32_t DestMipLevel = 0;
        uint32_t DestSlice = 0;
        uint32_t DestOffsets[3] = { 0 };
    };
}