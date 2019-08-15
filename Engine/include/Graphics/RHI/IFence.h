#pragma once

#include "Graphics/RHI/IDeviceObject.h"

namespace ZHU
{
    struct ZHU_GS_API FenceDesc : public IDeviceObjectDesc
    {};


    class ZHU_GS_API IFence : public IDeviceObject
    {
    public:
        using Pointer = std::shared_ptr<IFence>;
		using Desc = FenceDesc;

        virtual const FenceDesc& GetDesc() const = 0;
        virtual uint64_t GetCompletedValue() const = 0;
        virtual void Reset(uint64_t value) = 0;
    };
}