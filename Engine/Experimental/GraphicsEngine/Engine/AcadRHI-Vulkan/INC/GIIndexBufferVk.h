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
            VkIndexType indexType,
            bool enableStagingBuffer = true,
            const void* data = nullptr);

        virtual ~GIIndexBufferVk();
        virtual bool IsValid() const override final;
        VkIndexType const& GetIndexType() const;

    protected:
        GIIndexBufferVk(
            SharedPtr<GIDeviceVk> device,
            VkDeviceSize size,
            VkIndexType indexType,
            bool enableStagingBuffer = true,
            const void* data = nullptr);

    private:
        VkIndexType mIndexType = VK_INDEX_TYPE_UINT32;
        std::optional<SharedPtr<GIBufferVk>> mStagingBuffer;
    };
}