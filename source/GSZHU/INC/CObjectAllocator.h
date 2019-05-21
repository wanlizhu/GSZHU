#pragma once

#include <GSZHU/IMemoryAllocator.h>
#include <unordered_map>
#include <list>

namespace GSZHU {
    class CObjectAllocator {
    public:
        CObjectAllocator(size_t BlockSize, size_t CountInPage, IMemoryAllocator* Allocator = nullptr);
        CObjectAllocator(const CObjectAllocator&) = delete;
        CObjectAllocator& operator=(const CObjectAllocator&) = delete;
        virtual ~CObjectAllocator();

        void* Allocate(size_t Count = 1);
        bool Verify(const void* Pointer) const;
        void Free(void* Pointer);
        void Clear();
        size_t GetBlockSize() const;
        size_t GetBlockCountInPage() const;

    private:
        void CreatePage(size_t Count);

    private:
        size_t mBlockSize = 0;
        size_t mBlockCountInPage = 0;
        std::unordered_map<void*, size_t> mUsedBlocks;
        std::list<void*> mFreeBlocks;
        std::list<char*> mPages;
        IMemoryAllocator* mAllocator = nullptr;
    };
}