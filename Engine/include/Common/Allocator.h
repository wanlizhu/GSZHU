#pragma once

#include "Common/IMemory.h"
#include <type_traits>
#include <cassert>

namespace ZHU
{
    template<typename T>
    class Allocator
    {
    public:
        using value_type      = T;
        using pointer         = value_type *;
        using const_pointer   = const value_type*;
        using reference       = value_type &;
        using const_reference = const value_type &;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;

        template<typename U>
        struct rebind { using other = Allocator<U>; };

        Allocator() = default;
        Allocator(IMemory* mem) : mMemory(mem) {}
        Allocator(const Allocator&) = default;
        Allocator(Allocator&&) = default;
        Allocator& operator=(Allocator&&) = default;

        T* allocate(std::size_t count) {
            if (mMemory)
                return reinterpret_cast<T*>(mMemory->Allocate(count * sizeof(T)));
            else
                return reinterpret_cast<T*>(new char[count * sizeof(T)]);
        }
        pointer address(reference ref) { return &ref; }
        const_reference address(const_reference ref) { return &ref; }
        size_type max_size() const { return std::numeric_limits<size_type>::max() / sizeof(T); }

        template<typename U, typename... ARGS>
        void construct(U* p, ARGS&& ... args) {
            ::new(p) U(std::forward<ARGS>(args)...);
        }
        void destroy(pointer p) { p->~T(); }

    private:
        IMemory* mMemory = nullptr;
    };
}