#pragma once

#include "RHIDeviceObject.h"

namespace Wanli
{
    class DLLDECL RHICommandQueue : public RHIDeviceObject
    {
    public:
        RHICommandQueue(RHIDevice* device, Uint familyIndex);
        
        virtual void Destroy() override;

        inline Uint GetFamilyIndex() const { return mFamilyIndex; }
        inline const VkQueueFamilyProperties& GetFamilyProperties() const { return mFamilyProperties; }

    private:
        VkQueue mQueue = VK_NULL_HANDLE;
        Uint mFamilyIndex = 0;
        VkQueueFamilyProperties mFamilyProperties;
    };
}