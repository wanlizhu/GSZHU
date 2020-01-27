#pragma once

#include "VulkanRHI/IBufferVk.h"

namespace Wanlix
{
    class CBufferViewVk;

    class CBufferVk : public IBufferVk
    {
    public:
        CBufferVk(IDevice* device,
                  const BufferDesc& desc,
                  const char* data,
                  const String& name);
        CBufferVk(IDevice* device,
                  const BufferDesc& desc,
                  VkBuffer vkBuffer,
                  const String& name);
        virtual ~CBufferVk();

        virtual VkBuffer GetVkBuffer() const override final;
        virtual void SetAccessFlags(VkAccessFlags flags) override final;
        virtual VkAccessFlags GetAccessFlags() const override final;

        virtual bool UpdateData(const Range& range, const void* data) override final;
        virtual void* Map(EMapType type, EMapFlags flags = EMapFlags::None) override final;
        virtual void Unmap() override final;
        virtual SharedPtr<IBufferView> CreateView(const BufferViewDesc& desc) override final;
        virtual SharedPtr<IBufferView> GetDefaultView(EBufferViewType viewType) override final;
        virtual void SetState(EResourceState state) override final;
        virtual EResourceState GetState() const override final;

        virtual EResourceType GetResourceType() const override final;
        virtual IDevice* GetDevice() const override final;
        virtual const DeviceObjectDesc& GetDesc() const override final;

    protected:
        VkBuffer mVkBuffer = VK_NULL_HANDLE;
        VkDeviceMemory mVkMemory = VK_NULL_HANDLE;
        EResourceState mResourceState = EResourceState::Undefined;

        SharedPtr<CBufferViewVk> mDefaultUAV;
        SharedPtr<CBufferViewVk> mDefaultSRV;
    };
}