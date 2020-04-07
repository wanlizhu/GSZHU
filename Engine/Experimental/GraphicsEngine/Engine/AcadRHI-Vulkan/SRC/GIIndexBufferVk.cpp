#include "GIIndexBufferVk.h"
#include "GIBufferVk.h"
#include "GIDeviceVk.h"
#include "GICommandQueueVk.h"
#include "GICommandPoolVk.h"
#include "GICommandBufferVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIIndexBufferVk> GIIndexBufferVk::Create(
        SharedPtr<GIDeviceVk> device,
        VkDeviceSize size,
        const void* data,
        VkIndexType indexType,
        bool useVMA
    )
    {
        SharedPtr<GIBufferVk> buffer;

        if (useVMA)
        {
            buffer = nullptr; // TODO
        }
        else
        {
            buffer = GIBufferBuilderVk(device)
                .SetSize(size)
                .SetInitialData(data)
                .AddBufferUsages(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
                .AddMemoryProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
                .AddSharedQueue(device->GetGraphicsQueue()->GetFamilyIndex())
                .AddSharedQueue(device->GetTransferQueue()->GetFamilyIndex())
                .Build();
        }

        return SharedPtr<GIIndexBufferVk>(new GIIndexBufferVk(buffer, indexType));
    }

    GIIndexBufferVk::GIIndexBufferVk(
        SharedPtr<GIBufferVk> buffer,
        VkIndexType indexType
    )
        : mBuffer(buffer)
        , mIndexType(indexType)
    {}

    GIIndexBufferVk::~GIIndexBufferVk()
    {
        mBuffer = nullptr;
        mIndexType = VK_INDEX_TYPE_MAX_ENUM;
    }
    
    VkIndexType const& GIIndexBufferVk::GetIndexType() const
    {
        return mIndexType;
    }

    GIIndexBufferVk::operator const VkBuffer& () const
    {
        return (const VkBuffer&)(*mBuffer);
    }

    bool GIIndexBufferVk::IsValid() const
    {
        return mBuffer != nullptr && 
            mBuffer->IsValid() && 
            mIndexType != UINT_MAX;
    }

    void GIIndexBufferVk::SetBuffer(SharedPtr<GIBufferVk> buffer)
    {
        mBuffer = buffer;
    }

    void GIIndexBufferVk::SetIndexType(VkIndexType indexType)
    {
        mIndexType = indexType;
    }

    SharedPtr<GIBufferVk> GIIndexBufferVk::GetBuffer() const
    {
        return mBuffer;
    }
}