#pragma once

#include "CObjectPool.inc.h"

namespace GSZHU {
    // Use std::enable_shared_from_this<...> to make sure we can pass a raw pointer to the pool.
    // We need objects to be able to add themselves back into the pool once they go out of scope.
    template<typename T, typename POLICY>
    struct CObjectPool<T, POLICY>::Impl : public std::enable_shared_from_this<CObjectPool<T, POLICY>::Impl> {
        Impl(Allocator _Allocator)
            : mAllocator(std::move(_Allocator)) {
            assert(mAllocator);
        }

        Impl(Allocator _Allocator, Recycler _Recycler)
            : mAllocator(std::move(_Allocator))
            , mRecycler(std::move(_Recycler)) {
            assert(mAllocator);
            assert(mRecycler);
        }

        Impl(const Impl& Other)
            : std::enable_shared_from_this<Impl>(Other)
            , mAllocator(Other.mAllocator)
            , mRecycler(Other.mRecycler) {
            uint32_t Size = Other.GetFreeObjectCount();
            for (uint32_t i = 0; i < Size; i++) {
                mFreeList.push_back(mAllocator());
            }
        }

        Impl(Impl&& Other)
            : std::enable_shared_from_this<Impl>(Other)
            , mAllocator(std::move(Other.mAllocator))
            , mRecycler(std::move(Other.mRecycler))
            , mFreeList(std::move(Other.mFreeList))
        {}

        Impl& operator=(const Impl& Other) {
            Impl Tmp(Other);
            std::swap(*this, Tmp);
            return *this;
        }

        Impl& operator=(Impl&& Other) {
            mAllocator = std::move(Other.mAllocator);
            mRecycler = std::move(Other.mRecycler);
            mFreeList = std::move(Other.mFreeList);
            return *this;
        }

        ObjectPointer Allocate() {
            ObjectPointer Ptr;

            {
                LockType Locker(mMutex);
                if (mFreeList.size() > 0) {
                    Ptr = mFreeList.back();
                    mFreeList.pop_back();
                }
            }

            if (!Ptr) {
                assert(mAllocator);
                Ptr = mAllocator();
            }

            // Create a std::shared_ptr<T> with a naked pointer to the resource and a custom deleter object
            auto Me = Impl::shared_from_this();
            return ObjectPointer(Ptr.get(), Deleter(Me, Ptr));
        }

        void Shrink() {
            LockType Locker(mMutex);
            mFreeList.clear();
        }

        uint32_t GetFreeObjectCount() const {
            LockType Locker(mMutex);
            return static_cast<uint32_t>(mFreeList.size());
        }

        // This function called when a resource should be added back into the pool
        void Recycle(const ObjectPointer& Ptr) {
            if (mRecycler) {
                mRecycler(Ptr);
            }

            LockType Locker(mMutex);
            mFreeList.push_back(Ptr);
        }

    private:
        Allocator mAllocator;
        Recycler mRecycler;
        std::list<ObjectPointer> mFreeList;
        mutable MutexType mMutex;
    };


    // Used by the std::shared_ptr<T> to de-allocate the object if the pool goes out of scope.
    // When a std::shared_ptr wants to de-allocate the object contained it will call the operator() define here.
    template<typename T, typename POLICY>
    struct CObjectPool<T, POLICY>::Deleter {
        Deleter(const std::weak_ptr<CObjectPool<T, POLICY>::Impl>& _Impl, const ObjectPointer& Ptr) 
            : mPoolImpl(_Impl)
            , mObjectPointer(Ptr) {
            assert(!mPoolImpl.expired());
            assert(mObjectPointer);
        }

        // Call operator called by std::shared_ptr<T> when de-allocating the object.
        void operator()(Object*) {
            auto PoolImpl = mPoolImpl.lock();
            if (PoolImpl) {
                PoolImpl->Recycle(mObjectPointer);
            }
            
            mObjectPointer.reset();
            // This reset() is needed because otherwise a circular
            // dependency can arise here in special situations.
            //
            // One example of such a situation is when the value_type
            // derives from std::enable_shared_from_this in that case,
            // the following will happen:
            //
            // The std::enable_shared_from_this implementation works by
            // storing a std::weak_ptr to itself. This std::weak_ptr
            // internally points to an "counted" object keeping track
            // of the reference count managing the raw pointer's release
            // policy (e.g. storing the custom deleter etc.) for all
            // the shared_ptr's. The "counted" object is both kept
            // alive by all std::shared_ptr and std::weak_ptr objects.
            //
            // In this specific case of std::enable_shared_from_this,
            // the custom deleter is not destroyed because the internal
            // std::weak_ptr still points to the "counted" object and
            // inside the custom deleter we are keeping the managed
            // object alive because we have a std::shared_ptr to it.
            //
            // The following diagram show the circular dependency where
            // the arrows indicate what is keeping what alive:
            //
            //  +----------------+                   +--------------+
            //  | custom deleter +--------------+    | real deleter |
            //  +----------------+              |    +--------------+
            //         ^                        |            ^
            //         |                        |            |
            //         |                        |            |
            //   +-----+--------+               |    +-------+------+
            //   | shared_count |               |    | shared_count |
            //   +--------------+               |    +--------------+
            //      ^    ^                      |            ^
            //      |    |                      |            |
            //      |    |                      |            |
            //      |    |                      v            |
            //      |    |  +------------+    +------------+ |
            //      |    +--+ shared_ptr |    | shared_ptr +-+
            //      |       +------------+    +----+-------+
            //      |                              |
            //      |                              |
            // +----+-----+            +--------+  |
            // | weak_ptr |<-----------+ object |<-+
            // +----------+            +--------+
            //
            // The std::shared_ptr on the right is the one managed by the
            // shared pool, it is the one actually deleting the
            // object when it goes out of scope. The shared_ptr on the
            // left is the one which contains the custom
            // deleter that will return the object into the resource
            // pool when it goes out of scope.
            //
            // By calling reset on the shared_ptr in the custom deleter
            // we break the cyclic dependency.
        }

    private:
        std::weak_ptr<CObjectPool<T, POLICY>::Impl> mPoolImpl;
        ObjectPointer mObjectPointer;
    };
}