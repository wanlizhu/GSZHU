#pragma once

#include "GIBufferVk.h"
#include "GIVertexLayoutVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIVertexBufferVk : public GIBufferVk
    {
    public:
        static SharedPtr<GIVertexBufferVk> Create(
            VkDeviceSize size, 
            SharedPtr<GIVertexLayoutVk> vertexLayout,
            const void* data = nullptr);

        virtual ~GIVertexBufferVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        void SetBindingPoint(uint32_t binding);
        SharedPtr<GIVertexLayoutVk> GetVertexLayout() const;
        VkVertexInputBindingDescription const& GetBindingPoint() const;
        
    private:
        VkVertexInputBindingDescription mBindingPoint = {};
        SharedPtr<GIVertexLayoutVk> mVertexLayout;
    };
}