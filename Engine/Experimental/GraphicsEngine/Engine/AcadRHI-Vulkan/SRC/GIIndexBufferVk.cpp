#include "GIIndexBufferVk.h"
#include "GIBufferVk.h"
#include "GIDeviceVk.h"
#include "GIDeviceQueueVk.h"
#include "GICommandPoolVk.h"
#include "GICommandBufferVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIIndexBufferVk> GIIndexBufferVk::Create(
        SharedPtr<GIDeviceVk> device,
        VkDeviceSize size,
        VkIndexType indexType,
        bool enableStagingBuffer,
        const void* data)
    {
        return SharedPtr<GIIndexBufferVk>(new GIIndexBufferVk(device, size, indexType, enableStagingBuffer, data));
    }

    GIIndexBufferVk::GIIndexBufferVk(
        SharedPtr<GIDeviceVk> device,
        VkDeviceSize size,
        VkIndexType indexType,
        bool enableStagingBuffer,
        const void* data
    )
        : GIBufferVk(
            device,
            size,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            { device->GetGraphicsQueue()->GetFamilyIndex(), device->GetTransferQueue()->GetFamilyIndex() },
            enableStagingBuffer ? nullptr : data
        )
        , mIndexType(indexType)
        , mStagingBuffer(std::nullopt)
    {
        if (enableStagingBuffer)
        {
            mStagingBuffer = GIBufferVk::Create(
                mDevice,
                mSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                { mDevice->GetGraphicsQueue()->GetFamilyIndex(), mDevice->GetTransferQueue()->GetFamilyIndex() },
                data);

            //TODO: merge trivial buffer/image updating commands
            auto cmdbuf = mDevice->GetTransferQueue()->GetCommandPool()->Allocate();

            VkBufferCopy bufferCopy = {};
            bufferCopy.srcOffset = 0;
            bufferCopy.dstOffset = 0;
            bufferCopy.size = mSize;
            vkCmdCopyBuffer(*cmdbuf, *mStagingBuffer.value(), *this, 1, &bufferCopy);
            cmdbuf->Submit();
        }
    }

    VkIndexType const& GIIndexBufferVk::GetIndexType() const
    {
        return mIndexType;
    }

    GIIndexBufferVk::~GIIndexBufferVk()
    {
        if (mStagingBuffer.has_value())
        {
            assert(mStagingBuffer.value().use_count() == 1);
        }
    }

    bool GIIndexBufferVk::IsValid() const
    {
        return GIBufferVk::IsValid() && mIndexType != VK_INDEX_TYPE_MAX_ENUM;
    }
}