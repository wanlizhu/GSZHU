#pragma once

#include <GSZHU/BasicTypes.h>
#include <atomic>

namespace GSZHU {
    // Template switch is used to have distinct counters for unrelated groups of objects
     template<typename GROUP>
     class CUNIQUE_ID {
     public:
         CUNIQUE_ID()
             : mID(0)
             , mInited(false)
         {}
         CUNIQUE_ID(CUNIQUE_ID&& rhs)
             : mID(rhs.mID)
             , mInited(rhs.mInited) {
             rhs.mID = 0;
             rhs.mInited = false;
         }

         const CUNIQUE_ID& operator=(CUNIQUE_ID&& rhs) {
             mID = rhs.mID;
             mInited = rhs.mInited;
             rhs.mID = 0;
             rhs.mInited = false;
             return *this;
         }

         ATOMIC_LONG GetID() const {
             if (!mInited) {
                 static ATOMIC_LONG smGlobalCounter;
                 mID = ++smGlobalCounter;
                 mInited = true;
             }
             return mID;
         }

     private:
         mutable ATOMIC_LONG mID = 0;
         mutable bool mInited = false;
     };
}