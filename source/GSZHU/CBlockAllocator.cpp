#include <GSZHU/CBlockAllocator.h>
#include <unordered_set>
#include <mutex>
#include "INC/AssertionFailed.h"
#include "INC/CMemoryPage.h"

namespace GSZHU {

    struct CBlockAllocator::SPrivateData {
        std::vector<CMemoryPage> mMemoryPages;
        std::unordered_set<size_t> mFreePages;
        std::unordered_map<void*, size_t> mAddressToPage;
        std::mutex mMutex;
    };

    CBlockAllocator::CBlockAllocator(IMemoryAllocator* _Allocator, size_t _BlockSize, size_t _NumBlocks)
        : mAllocator(_Allocator)
        , mBlockSize(_BlockSize)
        , mNumBlocks(_NumBlocks)
        , mPrivateData(new CBlockAllocator::SPrivateData()) {
        if (mBlockSize > 0) {
            CreatePage();
        }
    }

    CBlockAllocator::~CBlockAllocator() {
#ifdef _DEBUG
        for (size_t p = 0; p < mPrivateData->mMemoryPages.size(); p++) {
            CHECK(mPrivateData->mMemoryPages[p].HasAllocations() == false, "Memory leak detected: memory page has allocated block");
            CHECK(mPrivateData->mFreePages.find(p) != mPrivateData->mFreePages.end(), "Memory page is not in the free page pool");
        }
#endif

        if (mPrivateData) {
            delete mPrivateData;
            mPrivateData = nullptr;
        }
    }
}