#pragma once

#include <GSZHU/CMemoryAllocator.h>

namespace GSZHU {
    class GSZHU_API CBlockAllocator : public IMemoryAllocator {
        struct SPrivateData;
    public:
        CBlockAllocator(IMemoryAllocator* _Allocator, size_t _BlockSize, size_t _NumBlocks);
        CBlockAllocator(const CBlockAllocator&) = delete;
        CBlockAllocator(CBlockAllocator&&) = delete;
        CBlockAllocator& operator=(const CBlockAllocator&) = delete;
        CBlockAllocator& operator=(CBlockAllocator&&) = delete;
        ~CBlockAllocator();

        virtual void* Allocate(size_t Size, const char* Desc, const char* File, int Line) override final;
        virtual void Free(void* Ptr) override final;

    private:
        void CreatePage();

    private:
        IMemoryAllocator* mAllocator = nullptr;
        size_t mBlockSize = 0;
        size_t mNumBlocks = 0;
        SPrivateData* mPrivateData = nullptr;
    };
}