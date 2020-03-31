#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIResourceStateVk
    {
    public:

    private:
        VkAccessFlags mAccessFlags = VK_ACCESS_FLAG_BITS_MAX_ENUM;
        VkImageLayout mImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    };
}