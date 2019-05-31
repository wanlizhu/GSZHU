#pragma once

namespace GSZHU {
    class CDisableHeapAllocation {
    public:
        // Disable heap allocation
        static void *operator new(size_t) = delete;
        static void *operator new[](size_t) = delete;
        static void  operator delete(void*) = delete;
        static void  operator delete[](void*) = delete;
    };
}