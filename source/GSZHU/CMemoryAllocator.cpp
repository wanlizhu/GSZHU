#include <GSZHU/CMemoryAllocator.h>

namespace GSZHU {
    void* CMemoryAllocator::Allocate(size_t Size, const char* Desc, const char* File, int Line) {
        return new char[Size];
    }

    void CMemoryAllocator::Free(void* Ptr) {
        if (Ptr) {
            delete[] Ptr;
        }
    }
}