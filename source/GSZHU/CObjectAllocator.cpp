#include "INC/CObjectAllocator.h"

namespace GSZHU {
    CObjectAllocator::CObjectAllocator(size_t BlockSize, size_t CountInPage, IMemoryAllocator* Allocator)
        : mBlockSize(BlockSize) 
        , mBlockCountInPage(CountInPage) {
        CreatePage(mBlockCountInPage);
    }

    CObjectAllocator::~CObjectAllocator() {
        Clear();
    }

    void* CObjectAllocator::Allocate(size_t Count) {
        if (mFreeBlocks.empty()) {
            CreatePage(Count > mBlockCountInPage ? Count : mBlockCountInPage);
        }
        
    }

    bool CObjectAllocator::Verify(const void* Pointer) const {
        return mUsedBlocks.find((void*)Pointer) != mUsedBlocks.end();
    }

    void CObjectAllocator::Free(void* Pointer) {
        auto Iter = mUsedBlocks.find(Pointer);
        if (Iter != mUsedBlocks.end()) {
            size_t Count = Iter->second;
            char* Mem = (char*)Pointer;
            for (size_t i = 0; i < Count; i++) {
                mFreeBlocks.push_back(Mem + i * mBlockSize);
            }
            mUsedBlocks.erase(Iter);
        }
    }

    void CObjectAllocator::Clear() {
        for (auto Page : mPages) {
            delete[] Page;
        }
        mPages.clear();
        mUsedBlocks.clear();
        mFreeBlocks.clear();
    }

    size_t CObjectAllocator::GetBlockSize() const {
        return mBlockSize;
    }

    size_t CObjectAllocator::GetBlockCountInPage() const {
        return mBlockCountInPage;
    }
}