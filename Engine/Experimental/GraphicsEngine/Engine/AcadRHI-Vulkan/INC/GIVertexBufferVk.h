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
            VkBufferUsageFlagBits usage,
            std::vector<uint32_t> sharingQueues,
            VkMemoryPropertyFlags properties,
            const void* initialData,
            EResourceState initialState,
            SharedPtr<GIVertexLayoutVk> vertexLayout,
            const VkVertexInputBindingDescription& bindingPoint
        );

        virtual ~GIVertexBufferVk();
        bool IsValid() const;

        void SetVertexLayout(SharedPtr<GIVertexLayoutVk> vertexLayout);
        void SetBindingPoint(const VkVertexInputBindingDescription& binding);
        
        SharedPtr<GIVertexLayoutVk> GetVertexLayout() const;
        VkVertexInputBindingDescription const& GetBindingPoint() const;

    protected:
        GIVertexBufferVk(SharedPtr<GIDeviceVk> device);
        
    private:
        SharedPtr<GIVertexLayoutVk> mVertexLayout;
        VkVertexInputBindingDescription mBindingPoint = {};
    };
}