#pragma once

#include <GSZHU/STextureSubResourceData.h>

namespace GSZHU {
    // Describes the initial data to store in the texture
    struct GSZHU_API STextureData {
        STextureSubresourceData* Subresources = nullptr;
        uint32_t NumSubresources = 0;

        STextureData() noexcept;
        STextureData(STextureSubresourceData* _Subres, uint32_t _NumSubres) noexcept;
    };
}