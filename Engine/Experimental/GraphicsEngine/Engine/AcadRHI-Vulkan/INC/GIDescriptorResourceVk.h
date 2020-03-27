#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDescriptorResourceVk
    {
    public:
        virtual ~GIDescriptorResourceVk() = default;
        virtual size_t GetResourceSize() const = 0;
        virtual uint32_t GetUnderlyingCopyCount() const = 0;
        virtual VkAccessFlags GetResourceState() const = 0;
        virtual VkWriteDescriptorSet GetWriteCommand() const = 0;
    };
}