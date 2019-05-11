#pragma once

#include <GSZHU/STextureSubResourceData.h>

namespace GSZHU {
    // Describes the initial data to store in the texture
    struct STextureData {
        STextureSubresourceData* pSubResources = nullptr;
        UINT NumSubresources = 0;
    };
}