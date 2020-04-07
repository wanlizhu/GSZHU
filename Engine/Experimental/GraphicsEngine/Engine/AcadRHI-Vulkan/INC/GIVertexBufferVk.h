#pragma once

#include "GIBufferVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceVk;
    class GIVertexLayoutVk;

    class GIVertexBufferVk : public std::enable_shared_from_this<GIVertexBufferVk>
    {
    public:
        static SharedPtr<GIVertexBufferVk> Create(
            SharedPtr<GIDeviceVk> device,
            VkDeviceSize size, 
            const void* data,
            SharedPtr<GIVertexLayoutVk> vertexLayout,
            const VkVertexInputBindingDescription& bindingPoint
        );

        virtual ~GIVertexBufferVk();
        bool IsValid() const;

        void SetBuffer(SharedPtr<GIBufferVk> buffer);
        void SetVertexLayout(SharedPtr<GIVertexLayoutVk> vertexLayout);
        void SetBindingPoint(const VkVertexInputBindingDescription& binding);
        
        SharedPtr<GIBufferVk> GetBuffer() const;
        SharedPtr<GIVertexLayoutVk> GetVertexLayout() const;
        VkVertexInputBindingDescription const& GetBindingPoint() const;

    protected:
        GIVertexBufferVk(
            SharedPtr<GIBufferVk> buffer,
            SharedPtr<GIVertexLayoutVk> vertexLayout,
            const VkVertexInputBindingDescription& bindingPoint
        );
        
    private:
        SharedPtr<GIBufferVk> mBuffer;
        SharedPtr<GIVertexLayoutVk> mVertexLayout;
        VkVertexInputBindingDescription mBindingPoint = {};
    };
}