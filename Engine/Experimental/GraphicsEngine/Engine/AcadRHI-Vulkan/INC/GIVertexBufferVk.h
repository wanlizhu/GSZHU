#pragma once

#include "GIBufferVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceVk;
    class GIVertexLayoutVk;

    class GIVertexBufferVk : public GIBufferVk
    {
    public:
        static SharedPtr<GIVertexBufferVk> Create(
            SharedPtr<GIDeviceVk> device,
            VkDeviceSize size, 
            SharedPtr<GIVertexLayoutVk> vertexLayout,
            const void* data
        );

        virtual ~GIVertexBufferVk();
        virtual bool IsValid() const override final;

        void SetBindingPoint(const VkVertexInputBindingDescription& binding);
        SharedPtr<GIVertexLayoutVk> GetVertexLayout() const;
        VkVertexInputBindingDescription const& GetBindingPoint() const;

    protected:
        GIVertexBufferVk(
            SharedPtr<GIDeviceVk> device,
            VkDeviceSize size,
            SharedPtr<GIVertexLayoutVk> vertexLayout,
            const void* data
        );
        
    private:
        VkVertexInputBindingDescription mBindingPoint = {};
        SharedPtr<GIVertexLayoutVk> mVertexLayout;
    };
}