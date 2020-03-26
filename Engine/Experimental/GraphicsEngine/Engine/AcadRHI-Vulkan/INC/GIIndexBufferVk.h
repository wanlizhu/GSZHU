#pragma once

#include "GIBufferVk.h"
#include "GIVertexLayoutVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIIndexBufferVk : public GIBufferVk
    {
    public:
        static SharedPtr<GIIndexBufferVk> Create(
            VkDeviceSize size,
            VkIndexType indexType,
            const void* data = nullptr);

        VkIndexType const& GetIndexType() const;

    private:
        VkIndexType mIndexType = VK_INDEX_TYPE_UINT32;
    };
}