#pragma once

#include <GSZHU/BasicTools.h>
#include <GSZHU/IMemoryAllocator.h>

namespace GSZHU {
    template<typename T, typename ALLOCATOR = IMemoryAllocator>
    class CSTDDeleter {
        typename std::enable_if<!std::is_destructible<T>::value, void>::type Destruct(T* ptr) {}
        typename std::enable_if<std::is_destructible<T>::value, void>::type Destruct(T* ptr) {
            ptr->~T();
        }

    public:
        CSTDDeleter() noexcept {}
        CSTDDeleter(ALLOCATOR* Allocator) noexcept
            : mAllocator(Allocator)
        {}
        CSTDDeleter(const CSTDDeleter&) = delete;
        CSTDDeleter& operator=(const CSTDDeleter&) = delete;

        void operator()(T* ptr) noexcept {
            CHECK(mAllocator != nullptr, "The deleter has been moved away or never initialized, and can't be used");
            Destruct(ptr);
            mAllocator->Free(ptr);
        }

    private:
        ALLOCATOR* mAllocator = nullptr;
    };
}