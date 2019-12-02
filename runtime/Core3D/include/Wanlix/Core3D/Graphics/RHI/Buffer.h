#pragma once

#include "Resource.h"
#include "BufferDescriptor.h"

namespace Wanlix
{
    class Buffer : public Resource
    {
    public:
        using Ptr      = std::shared_ptr<Buffer>;
        using ConstPtr = std::shared_ptr<const Buffer>;
        using Desc     = BufferDescriptor;

        virtual const Desc& GetDescriptor() const = 0;
        virtual void* Map(const Range& range, CPUAccessFlags cpuAccess) = 0;
        virtual void  Unmap(void* data) = 0;
        virtual bool  IsMapped(const Range& range) const = 0;

        size_t GetSize() const;
        Range  GetRange() const;

    protected:
        Buffer(
            StringCRef name, 
            std::shared_ptr<Device> device,
            long bindFlags
        );

    protected:
        long mBindFlags = 0;
    };
}