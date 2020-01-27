#pragma once

#include "VulkanRHI/IBufferViewVk.h"
#include "VulkanRHI/Class/CBufferVk.h"

namespace Wanlix
{
    class CBufferViewVk : public IBufferViewVk
    {
    public:
        CBufferViewVk(IDeviceVk* device,
                      const BufferViewDesc& viewDesc,
                      IBufferVk* buffer,
                      Bool isDefaultView);
        virtual ~CBufferViewVk();

        virtual VkBufferView GetVkBufferView() const override final;
        virtual IBufferVk* GetBuffer() const override final;

    protected:
        VkBufferView mVkBufferView = VK_NULL_HANDLE;
        CBufferVk* mBuffer = nullptr;

    private:
        SharedPtr<CBufferVk> mBufferStrongRef;
    };
}