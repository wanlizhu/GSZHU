#pragma once

#include "RHIDeviceObject.h"

namespace Wanli
{
    class FencePool;

    class DLLDECL RHIFence : public RHIDeviceObject
    {
        friend class FencePool;
    public:
        RHIFence(
            RHIDevice* device,
            FencePool* pool,
            bool signaled
        );
        virtual ~RHIFence();

        virtual void Destroy() override;
        inline VkFence GetHandle() const { return mFence; }
        inline bool Signaled() const { return mSignaled; }
        inline FencePool* GetFencePool() const { return mFencePool; }

    protected:
        VkFence mFence;
        bool mSignaled = false;
        FencePool* mFencePool = nullptr;
    };
}