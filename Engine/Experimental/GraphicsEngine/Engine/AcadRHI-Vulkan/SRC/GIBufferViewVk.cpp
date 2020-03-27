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
        VK_CHECK(vkCreateBufferView(*mDevice, &createInfo, nullptr, &mBufferView));

        mBufferInfo.buffer = *mBuffer;
        mBufferInfo.offset = offset;
        mBufferInfo.range = mSize;
    }

    GIBufferViewVk::~GIBufferViewVk()
    {
        if (IsValid())
        {
            vkDestroyBufferView(*mDevice, mBufferView, nullptr);
            mBufferView = VK_NULL_HANDLE;
        }
    }

    bool GIBufferViewVk::IsValid() const
    {
        return mBufferView != VK_NULL_HANDLE;
    }

    void GIBufferViewVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mBufferView,
            VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT,
            name);
    }

    void GIBufferViewVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mBufferView,
            VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT,
            tag);
    }

    size_t GIBufferViewVk::GetResourceSize() const
    {
        return mSize;
    }

    uint32_t GIBufferViewVk::GetUnderlyingCopyCount() const
    {
        return 1;
    }

    VkAccessFlags GIBufferViewVk::GetResourceState() const
    {
        return mResourceState;
    }

    VkWriteDescriptorSet GIBufferViewVk::GetWriteCommand() const
    {
        VkWriteDescriptorSet writeCmd = {};
        writeCmd.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeCmd.pNext = nullptr;
        writeCmd.dstSet = 0; //TODO
        writeCmd.dstBinding = 0; //TODO
        writeCmd.dstArrayElement = 0;
        writeCmd.descriptorCount = 1;
        writeCmd.descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM; //TODO
        writeCmd.pBufferInfo = &mBufferInfo;

        return writeCmd;
    }

    GIBufferViewVk::operator const VkBufferView& () const
    {
        return mBufferView;
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