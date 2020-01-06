#pragma once

#include "Wanlix/IResource.h"

namespace Wanlix
{
    struct BufferDesc
    {
        size_t sizeInBytes = 0;
        BitSet cpuAccessFlags = 0;
    };

    class IBuffer : public IResource
    {
        CLASSINFO(IBuffer)
        INHERIT_SHARED_FROM_THIS(IResource)
    public:
        virtual ~IBuffer() = 0;
        virtual void UpdateData(size_t offset, size_t size, const void* data) = 0;
        virtual size_t GetSizeInBytes() const = 0;
        virtual CpuAccessFlags GetCpuAccessFlags() const = 0;
        virtual void* Map(MapType type, BitSet flags = 0) = 0;
        virtual void UnMap() = 0;
    };
}