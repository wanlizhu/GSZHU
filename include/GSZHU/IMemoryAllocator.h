#pragma once

#include <GSZHU/BasicTools.h>

namespace GSZHU {
    class GSZHU_API IMemoryAllocator {
    public:
        virtual void* Allocate(size_t Size, const char* Desc, const char* File, int Line) = 0;
        virtual void Free(void* Ptr) = 0;
    };
}