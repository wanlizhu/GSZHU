#pragma once

#include "GIBufferVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIIndexBufferVk : public GIBufferVk
    {
    public:
        static SharedPtr<GIIndexBufferVk> Create(
            SharedPtr<GIDeviceVk> device,
            VkDeviceSize size,
            VkBufferUsageFlagBits usage,
            std::vector<uint32_t> sharingQueues,
            VkMemoryPropertyFlags properties,
            const void* initialData,
            EResourceState initialState,
            VkIndexType indexType
        );

        virtual ~GIIndexBufferVk();
        bool IsValid() const;

        void SetIndexType(VkIndexType indexType);
        VkIndexType const& GetIndexType() const;

    protected:
        GIIndexBufferVk(SharedPtr<GIDeviceVk> device);

    private:
        VkIndexType mIndexType = VK_INDEX_TYPE_UINT32;
    };
}