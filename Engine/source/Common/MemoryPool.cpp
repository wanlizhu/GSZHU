#include "Common/MemoryPool.h"

namespace ZHU
{
    void* MemoryPool::Allocate(size_t size)
    {
        return new char[size];
    }

    void MemoryPool::Free(void* ptr)
    {
        delete ptr;
    }
}