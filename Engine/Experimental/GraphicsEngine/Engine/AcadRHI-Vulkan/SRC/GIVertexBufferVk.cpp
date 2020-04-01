#include "GIVertexBufferVk.h"
#include "GIVertexLayoutVk.h"
#include "GIBufferVk.h"
#include "GIDeviceVk.h"
#include "GICommandQueueVk.h"
#include "GICommandPoolVk.h"
#include "GICommandBufferVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIVertexBufferVk> GIVertexBufferVk::Create(
        SharedPtr<GIDeviceVk> device,
        VkDeviceSize size,
        const void* data,
        SharedPtr<GIVertexLayoutVk> vertexLayout,
        const VkVertexInputBindingDescription& bindingPoint,
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
                .AddBufferUsages(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
                .AddMemoryProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
                .AddSharedQueue(device->GetGraphicsQueue()->GetFamilyIndex())
                .AddSharedQueue(device->GetTransferQueue()->GetFamilyIndex())
                .Build();
        }

        return SharedPtr<GIVertexBufferVk>(new GIVertexBufferVk(buffer, vertexLayout, bindingPoint));
    }

    GIVertexBufferVk::GIVertexBufferVk(
        SharedPtr<GIBufferVk> buffer,
        SharedPtr<GIVertexLayoutVk> vertexLayout,
        const VkVertexInputBindingDescription& bindingPoint
    )
        : mBuffer(buffer)
        , mVertexLayout(vertexLayout)
        , mBindingPoint(bindingPoint)
    {}

    GIVertexBufferVk::~GIVertexBufferVk()
    {
        mBuffer = nullptr;
        mVertexLayout = nullptr;
    }

    bool GIVertexBufferVk::IsValid() const
    {
        return mBuffer != nullptr &&
            mBuffer->IsValid() &&
            mVertexLayout != nullptr &&
            mVertexLayout->IsValid() &&
            mBindingPoint.binding != UINT_MAX;
    }

    void GIVertexBufferVk::SetBuffer(SharedPtr<GIBufferVk> buffer) 
    {
        mBuffer = buffer;
    }

    void GIVertexBufferVk::SetVertexLayout(SharedPtr<GIVertexLayoutVk> vertexLayout)
    {
        mVertexLayout = vertexLayout;
    }

    void GIVertexBufferVk::SetBindingPoint(const VkVertexInputBindingDescription& binding)
    {
        mBindingPoint = binding;
    }

    SharedPtr<GIBufferVk> GIVertexBufferVk::GetBuffer() const
    {
        return mBuffer;
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