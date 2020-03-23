#pragma once

#include "GIDeviceObjectVk.h"
#include "GIDescriptorResourceVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIBufferVk 
        : public GIDeviceObjectVk
        , public GIDescriptorResourceVk
    {
        DECL_DEVICE_OBJECT(GIBufferVk)
    public:
        virtual ~GIBufferVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        virtual std::optional<BindingPoint> GetBindingPoint() const override final;
        virtual VkWriteDescriptorSet GetWriteCommand() const override final;
        virtual void SetBindingPoint(uint32_t setId, uint32_t bindingId) override final;

        operator const VkBuffer& () const;
        size_t GetSize() const;

    };
}