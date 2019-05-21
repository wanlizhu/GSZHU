#pragma once

#include <GSZHU/IMemoryAllocator.h>

namespace GSZHU {
    class GSZHU_API CMemoryAllocator : public IMemoryAllocator {
    public:
        CMemoryAllocator() = default;
        CMemoryAllocator(const CMemoryAllocator&) = delete;
        CMemoryAllocator(CMemoryAllocator&&) = delete;
        CMemoryAllocator& operator=(const CMemoryAllocator&) = delete;
        CMemoryAllocator& operator=(CMemoryAllocator&&) = delete;

        virtual void* Allocate(size_t Size, size_t Align = 0) override;
        virtual void Free(void* Ptr) override;
    };
}