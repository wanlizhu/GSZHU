#include "GIVertexBufferVk.h"
#include "GIVertexLayoutVk.h"
#include "GIBufferVk.h"
#include "GIDeviceVk.h"
#include "GIDeviceQueueVk.h"
#include "GICommandPoolVk.h"
#include "GICommandBufferVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIVertexBufferVk> GIVertexBufferVk::Create(
        SharedPtr<GIDeviceVk> device,
        VkDeviceSize size,
        SharedPtr<GIVertexLayoutVk> vertexLayout,
        bool enableStagingBuffer,
        const void* data)
    {
        return SharedPtr<GIVertexBufferVk>(new GIVertexBufferVk(device, size, vertexLayout, enableStagingBuffer, data));
    }

    GIVertexBufferVk::GIVertexBufferVk(
        SharedPtr<GIDeviceVk> device,
        VkDeviceSize size,
        SharedPtr<GIVertexLayoutVk> vertexLayout,
        bool enableStagingBuffer,
        const void* data
    )
        : GIBufferVk(
            device,
            size,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            {},
            enableStagingBuffer ? nullptr : data
        )
        , mVertexLayout(vertexLayout)
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

    GIVertexBufferVk::~GIVertexBufferVk()
    {
        if (mStagingBuffer.has_value())
        {
            assert(mStagingBuffer.value().use_count() == 1);
        }
    }

    bool GIVertexBufferVk::IsValid() const
    {
        return GIBufferVk::IsValid() && mVertexLayout->IsValid();
    }

    void GIVertexBufferVk::SetBindingPoint(const VkVertexInputBindingDescription& binding)
    {
        mBindingPoint = binding;
    }

    SharedPtr<GIVertexLayoutVk> GIVertexBufferVk::GetVertexLayout() const
    {
        return mVertexLayout;
    }

    VkVertexInputBindingDescription const& GIVertexBufferVk::GetBindingPoint() const
    {
        return mBindingPoint;
    }
}