#pragma once

#include "Common/IMemory.h"

namespace ZHU
{
    class ZHU_API MemoryPool : public IMemory
    {
    public:
        virtual void* Allocate(size_t size) override;
        virtual void  Free(void* ptr) override;
    };
}