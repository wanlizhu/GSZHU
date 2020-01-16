#pragma once

#include "Graphics/RHI/IResource.h"

namespace Wanlix
{
    struct BufferDesc : public ResourceDesc
    {
        size_t sizeInBytes = 0;
        EBindFlags bindFlags = EBindFlags::None;
        EUsage usage = EUsage::Default;
        ECPUAccessFlags cpuAccessFlags = ECPUAccessFlags::None;
        EBufferMode bufferMode = EBufferMode::Undefined;
        Uint stride = 0;
        Uint64 queueMask = 0;
    };

    class IBufferView;
    struct BufferViewDesc;

    class IBuffer : public IResource
    {
    public:
        virtual bool UpdateData(const void* data, const Range& range) = 0;
        virtual void* Map(EMapType type, EMapFlags flags = EMapFlags::None) = 0;
        virtual void Unmap() = 0;
        virtual SharedPtr<IBufferView> CreateView(const BufferViewDesc& desc) = 0;
        virtual Optional<EResourceState> GetState() const = 0;

        // Interfaces inherited from IResource
        virtual EResourceType GetType() const override { return EResourceType::Sampler; }
        
        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual const BufferDesc& GetDesc() const = 0;
    };
}