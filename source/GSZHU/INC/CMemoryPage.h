#pragma once

#include <GSZHU/BasicTools.h>

namespace GSZHU {
    class CBlockAllocator;

    class CMemoryPage {
        static constexpr uint8_t NewPagePattern = 0xAA;
        static constexpr uint8_t AllocatedBlockPattern = 0xAB;
        static constexpr uint8_t FreedBlockPattern = 0xDE;
        static constexpr uint8_t InitializedBlockPattern = 0xCF;

    public:
        CMemoryPage(CBlockAllocator* Allocator);
        CMemoryPage(const CMemoryPage&) = delete;
        CMemoryPage(CMemoryPage&& Page);
        CMemoryPage& operator=(const CMemoryPage&) = delete;
        CMemoryPage& operator=(CMemoryPage&&) = delete;
        ~CMemoryPage();

        void* GetBlockAddress(uint32_t Index) const;
        void VerifyAddress(const void* BlockAddress) const;
        void* Allocate();
        void Free(void* Ptr);
        bool HasSpace() const;
        bool HasAllocations() const;

    private:
        uint32_t mNumFreeBlocks = 0;
        uint32_t mNumInitedBlocks = 0;
        void* mStartAddress = nullptr;
        void* mNextFreeBlock = nullptr;
        CBlockAllocator* mBlockAllocator = nullptr;
    };
}