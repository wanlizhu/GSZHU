#pragma once

#include "RHI/IResource.h"

namespace Wanlix
{
    struct BufferViewDesc;
    class IBufferView;

    struct BufferDesc : public DeviceObjectDesc
    {
        size_t sizeInBytes = 0;
        EBindFlags bindFlags = EBindFlags::None;
        EUsage usage = EUsage::Default;
        ECPUAccessFlags cpuAccessFlags = ECPUAccessFlags::None;
        EBufferMode bufferMode = EBufferMode::Undefined;
        Uint stride = 0;
        Uint64 queueMask = 0;
    };

    class IBuffer : public IResource
    {
    public:
        using Desc = BufferDesc;
        using View = IBufferView;

        virtual bool UpdateData(const Range& range, const void* data) = 0;
        virtual void* Map(EMapType type, EMapFlags flags = EMapFlags::None) = 0;
        virtual void Unmap() = 0;
        virtual SharedPtr<IBufferView> CreateView(const BufferViewDesc& desc) = 0;
        virtual SharedPtr<IBufferView> GetDefaultView(EBufferViewType viewType) = 0;
        virtual void SetState(EResourceState state) = 0;
        virtual EResourceState GetState() const = 0;
    };
}