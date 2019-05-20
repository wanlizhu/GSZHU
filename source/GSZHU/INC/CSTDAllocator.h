#pragma once

#include <limits>
#include <GSZHU/BasicTools.h>
#include <GSZHU/IMemoryAllocator.h>

namespace GSZHU {
    template<typename T, typename ALLOCATOR = IMemoryAllocator> 
    class CSTDAllocator {
    public:
        using value_type = T;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type & ;
        using const_reference = const value_type&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        template<typename U>
        struct rebind {
            typedef CSTDAllocator<U, ALLOCATOR> other;
        };

        CSTDAllocator(ALLOCATOR* Allocator, const char* Desc, const char* File, int Line) noexcept
            : mAllocator(Allocator)
            , mDesc(Desc)
            , mFile(File)
            , mLine(Line)
        {}

        template<typename U>
        CSTDAllocator(const CSTDAllocator<U, ALLOCATOR>& Other) noexcept
            : mAllocator(Other.mAllocator)
            , mDesc(Other.mDesc)
            , mFile(Other.mFile)
            , mLine(Other.mLine)
        {}

        T* allocate(std::size_t count) {
            return reinterpret_cast<T*>(mAllocator->Allocate(count * sizeof(T), mDesc.c_str(), mFile.c_str(), mLine));
        }

        pointer address(reference ref) { return &ref; }
        const_pointer address(const_reference ref) { return &ref; };
        void deallocate(T* ptr, std::size_t count) { mAllocator->Free(ptr); }
        inline size_type max_size() const { return std::numeric_limits<size_type>::max() / sizeof(T); }
        
        template<typename U, typename... ARGS>
        void construct(U* ptr, ARGS&&... args) {
            ::new(ptr) U(std::forward<ARGS>(args)...);
        }

        inline void destroy(pointer ptr) {
            ptr->~T();
        }

    public:
        ALLOCATOR* mAllocator = nullptr;
        const std::string mDesc;
        const std::string mFile;
        const int mLine;
    };

#define STD_ALLOCATOR(Type, Allocator, Desc) CSTDAllocator<Type, IMemoryAllocator>(Allocator, Desc, __FILE__, __LINE__)


    template <class T, class U, class A>
    bool operator==(const CSTDAllocator<T, A>&left, const CSTDAllocator<U, A>&right) {
        return &left.m_Allocator == &right.m_Allocator;
    }

    template <class T, class U, class A>
    bool operator!=(const CSTDAllocator<T, A> &left, const CSTDAllocator<U, A> &right) {
        return !(left == right);
    }
}