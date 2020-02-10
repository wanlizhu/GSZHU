#pragma once

#include "RHIDevice.h"
#include <vulkan/vulkan.h>

namespace Wanli
{
    class RHIDevice;

    class DLLDECL RHIDeviceObject : public std::enable_shared_from_this<RHIDeviceObject>
    {
    public:
        RHIDeviceObject(RHIDevice* device, const char* name);
        RHIDeviceObject(const RHIDeviceObject&) = delete;
        RHIDeviceObject& operator=(const RHIDeviceObject&) = delete;
        virtual ~RHIDeviceObject();

        virtual void Destroy() = 0;

        inline RHIDevice* GetDevice() const { return mDevice.get(); }
        inline const String& GetName() const { return mName; }
        inline Uint GetUniqueID() const { return mUniqueID; }

    protected:
        String mName;
        Uint mUniqueID = 0;
        SharedPtr<RHIDevice> mDevice;
    };
}