#pragma once

#include "Common/Config.h"
#include <memory>
#include <limits>

namespace ZHU
{
    template<typename T>
    typename std::enable_if_t<std::is_destructible_v<T>, void> 
        Destruct(T* pointer)
    {
        pointer->~T();
    }

    template<typename T>
    typename std::enable_if_t<!std::is_constructible_v<T>, void>
        Destruct(T* pointer)
    {}


    class ZHU_API IMemory
    {
    public:
        virtual void* Allocate(size_t size) = 0;
        virtual void Free(void* ptr) = 0;
    };
}