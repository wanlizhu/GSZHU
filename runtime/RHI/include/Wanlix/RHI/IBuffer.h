#pragma once

#include "IResource.h"

namespace Wanlix
{
    class  IBufferView;
    struct BufferViewDesc;

    struct BufferData
    {
        const void* data = nullptr;
        Uint size = 0;
    };

    struct BufferDesc : public DeviceObjectDesc
    {
        Size  sizeInBytes = 0;
        Uint  bindFlags = BindFlags::None;
        Usage usage = Usage::Default;
        Uint  cpuAccessFlags = CpuAccessFlags::None;
        BufferMode mode = BufferMode::Undefined;
        Uint  stride = 0;
        Ui64  commandQueueMask = 1;
    };

    BufferDesc MakeBufferDesc(Uint size, Uint bindFlags);

    class RHI_API IBuffer : public IResource
    {
        CLASSINFO(IBuffer)
        INHERIT_SHARED_FROM_THIS(IResource)
    public:
        virtual ~IBuffer() = default;

        /* interfaces inherited from IDeviceObject */
        virtual Pvoid GetNativeHandle() const override = 0;
        virtual BufferDesc const& GetDesc() const override = 0;
        virtual IDevice* GetDevice() const override = 0;

        /* interfaces inherited from IResource */
        virtual Size GetSizeInBytes() const override = 0;
        virtual Uint GetBindFlags() const override = 0;
        virtual ResourceType  GetType() const override = 0;
        virtual ResourceState GetState() const override = 0;
        virtual void SetState(ResourceState state) override = 0;

        virtual void UpdateData(Range range, BufferData const* data) = 0;
        virtual IBufferView* CreateView(BufferViewDesc const& desc) const = 0;
        virtual IBufferView* GetDefaultView(BufferViewType type) const = 0;
    };
}