#pragma once

#include <atomic>

namespace GSZHU {
    // Template switch is used to have distinct counters for unrelated groups of objects
     template<typename GROUP>
     class CUniqueID {
     public:
         CUniqueID()
             : mID(0)
             , mInited(false)
         {}
         CUniqueID(CUniqueID&& rhs)
             : mID(rhs.mID)
             , mInited(rhs.mInited) {
             rhs.mID = 0;
             rhs.mInited = false;
         }

         const CUniqueID& operator=(CUniqueID&& rhs) {
             mID = rhs.mID;
             mInited = rhs.mInited;
             rhs.mID = 0;
             rhs.mInited = false;
             return *this;
         }

         std::atomic<long> GetID() const {
             if (!mInited) {
                 static std::atomic<long> smGlobalCounter;
                 mID = ++smGlobalCounter;
                 mInited = true;
             }
             return mID;
         }

     private:
         mutable std::atomic<long> mID = 0;
         mutable bool mInited = false;
     };
}