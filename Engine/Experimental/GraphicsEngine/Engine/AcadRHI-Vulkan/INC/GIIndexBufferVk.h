#pragma once

#include "GIBufferVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIIndexBufferVk : public std::enable_shared_from_this<GIIndexBufferVk>
    {
    public:
        static SharedPtr<GIIndexBufferVk> Create(
            SharedPtr<GIDeviceVk> device,
            VkDeviceSize size,
            const void* data,
            VkIndexType indexType,
            bool useVMA
        );

        virtual ~GIIndexBufferVk();
        operator const VkBuffer& () const;
        bool IsValid() const;

        void SetBuffer(SharedPtr<GIBufferVk> buffer);
        void SetIndexType(VkIndexType indexType);

        SharedPtr<GIBufferVk> GetBuffer() const;
        VkIndexType const& GetIndexType() const;

    protected:
        GIIndexBufferVk(
            SharedPtr<GIBufferVk> buffer,
            VkIndexType indexType
        );

    private:
        SharedPtr<GIBufferVk> mBuffer;
        VkIndexType mIndexType = VK_INDEX_TYPE_UINT32;
    };
}