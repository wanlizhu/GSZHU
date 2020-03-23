#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDescriptorResourceVk
    {
    public:
        struct BindingPoint 
        {
            uint32_t setId = 0;
            uint32_t binding = 0;
        };

        virtual ~GIDescriptorResourceVk() = default;
        virtual std::optional<BindingPoint> GetBindingPoint() const = 0;
        virtual VkWriteDescriptorSet GetWriteCommand() const = 0;
        virtual void SetBindingPoint(uint32_t setId, uint32_t bindingId) = 0;
        //virtual void GetResourceState() = 0;
        //virtual void SetResourceState() = 0;
        //virtual bool IsHostReadable() const = 0;
        //virtual bool IsHostWritable() const = 0;
        //virtual uint32_t GetUnderlyingCopiesCount() const = 0;
    };
}