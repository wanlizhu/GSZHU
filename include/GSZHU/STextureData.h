#pragma once

#include <GSZHU/STextureSubResourceData.h>

namespace GSZHU {
    // Describes the initial data to store in the texture
    struct STextureData {
        STextureSubresourceData* pSubResources = nullptr;
        uint32_t NumSubresources = 0;
    };
}