#pragma once

#include "Graphics/RHI/IDeviceObject.h"

namespace Wanlix
{
    struct StateTransitionDesc
    {
        ITexture* texture = nullptr;
        IBuffer* buffer = nullptr;

        Uint firstMipLevel = 0;
        Uint mipLevelCount = (Uint)-1;
        Uint firstSlice = 0;
        Uint sliceCount = (Uint)-1;

        EResourceState oldState = EResourceState::Unknown;
        EResourceState newState = EResourceState::Unknown;
    };

    struct ResourceDesc : public DeviceObjectDesc
    {};

    class IResource : public IDeviceObject
    {
    public:
        virtual EResourceType GetType() const = 0;
        
        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual const ResourceDesc& GetDesc() const = 0;
    };
}