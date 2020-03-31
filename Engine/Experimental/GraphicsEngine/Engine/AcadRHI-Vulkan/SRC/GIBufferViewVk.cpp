#include "GIBufferViewVk.h"
#include "GIBufferVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIBufferViewVk> GIBufferViewVk::Create(
        SharedPtr<GIBufferVk> buffer,
        size_t offset,
        size_t size,
        VkFormat format)
    {
        return SharedPtr<GIBufferViewVk>(new GIBufferViewVk(buffer, offset, size, format));
    }

    CACHE_INDEX GIBufferViewVk::ComputeCacheIndex(
        size_t offset,
        size_t size,
        VkFormat format
    )
    {
        return std::hash<size_t>()(offset) 
            ^ std::hash<uint32_t>()(format)
            ^ std::hash<size_t>()(size);
    }

    GIBufferViewVk::GIBufferViewVk(
        SharedPtr<GIBufferVk> buffer,
        size_t offset,
        size_t size,
        VkFormat format
    )
        : GIDeviceObjectVk(buffer->GetDevice())
        , mBuffer(buffer)
        , mOffset(offset)
        , mSize(size)
        , mFormat(format)
    {
        VkBufferViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.buffer = *mBuffer;
        createInfo.format = mFormat;
        createInfo.offset = mOffset;
        createInfo.range = mSize;
        VK_CHECK(vkCreateBufferView(*mDevice, &createInfo, nullptr, &mBufferViewHandle));
    }

    GIBufferViewVk::~GIBufferViewVk()
    {
        if (IsValid())
        {
            vkDestroyBufferView(*mDevice, mBufferViewHandle, nullptr);
            mBufferViewHandle = VK_NULL_HANDLE;
        }
    }

    bool GIBufferViewVk::IsValid() const
    {
        return mBufferViewHandle != VK_NULL_HANDLE;
    }

    void GIBufferViewVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mBufferViewHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT,
            name
        );
    }

    void GIBufferViewVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mBufferViewHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT,
            tag
        );
    }

    GIBufferViewVk::operator const VkBufferView& () const
    {
        return mBufferViewHandle;
    }

    SharedPtr<GIBufferVk> GIBufferViewVk::GetBuffer() const
    {
        return mBuffer;
    }

    size_t GIBufferViewVk::GetOffset() const
    {
        return mOffset;
    }

    size_t GIBufferViewVk::GetSize() const
    {
        return mSize;
    }

    VkFormat GIBufferViewVk::GetFormat() const 
    {
        return mFormat;
    }
}