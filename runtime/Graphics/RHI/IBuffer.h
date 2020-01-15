#pragma once

#include "RHI/IResource.h"

namespace Wanlix
{
    class IBuffer : public IResource
    {
    public:
        virtual bool UpdateData(const void* data, const Range& range) = 0;
        virtual ECPUAccessFlags GetCpuAccess() const = 0;
        virtual Uint64 GetGPUAddress() const = 0;
        virtual void* Map(EMapType type, EMapFlags flags = EMapFlags::None) = 0;
        virtual void Unmap() = 0;
        virtual SharedPtr<IResourceView> GetSRV(const Range& range) = 0;
        virtual SharedPtr<IResourceView> GetUAV(const Range& range) = 0;
        
        // Interfaces inherited from IResource
        virtual EResourceType GetType() const = 0;
        virtual EBindFlags GetBindFlags() const = 0;
        virtual size_t GetSizeInBytes() const = 0;
        virtual Bool HasGlobalState() const = 0;
        virtual EResourceState GetGlobalState() const = 0;
        virtual EResourceState GetSubresourceState(Subresource subres) const = 0;

        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
    };
}