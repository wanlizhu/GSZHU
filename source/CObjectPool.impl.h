#pragma once

#include "CObjectPool.h"

namespace GSZHU {
    // Use std::enable_shared_from_this<...> to make sure we can pass a raw pointer to the pool.
    // We need objects to be able to add themselves back into the pool once they go out of scope.
    template<typename T, typename POLICY>
    struct CObjectPool<T, POLICY>::Impl : public std::enable_shared_from_this<CObjectPool<T, POLICY>::Impl> {
        Impl(Allocator alloc)
            : mAllocator(std::move(alloc)) {
            assert(mAllocator);
        }

        Impl(Allocator alloc, Recycler recycler)
            : mAllocator(std::move(alloc))
            , mRecycler(std::move(recycler)) {
            assert(mAllocator);
            assert(mRecycler);
        }

        Impl(const Impl& other)
            : std::enable_shared_from_this<Impl>(other)
            , mAllocator(other.mAllocator)
            , mRecycler(other.mRecycler) {
            uint32_t Size = other.GetFreeObjectCount();
            for (uint32_t i = 0; i < Size; i++) {
                mFreeList.push_back(mAllocator());
            }
        }

        Impl(Impl&& other)
            : std::enable_shared_from_this<Impl>(other)
            , mAllocator(std::move(other.mAllocator))
            , mRecycler(std::move(other.mRecycler))
            , mFreeList(std::move(other.mFreeList))
        {}

        Impl& operator=(const Impl& other) {
            Impl Tmp(other);
            std::swap(*this, Tmp);
            return *this;
        }

        Impl& operator=(Impl&& other) {
            mAllocator = std::move(other.mAllocator);
            mRecycler = std::move(other.mRecycler);
            mFreeList = std::move(other.mFreeList);
            return *this;
        }

        ObjectPointer Allocate() {
            ObjectPointer ptr;

            {
                LockType Locker(mMutex);
                if (mFreeList.size() > 0) {
                    ptr = mFreeList.back();
                    mFreeList.pop_back();
                }
            }

            if (!ptr) {
                assert(mAllocator);
                ptr = mAllocator();
            }

            // Create a std::shared_ptr<T> with a naked pointer to the resource and a custom deleter object
            auto Me = Impl::shared_from_this();
            return ObjectPointer(ptr.get(), Deleter(Me, ptr));
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
        void Recycle(const ObjectPointer& ptr) {
            if (mRecycler) {
                mRecycler(ptr);
            }

            LockType Locker(mMutex);
            mFreeList.push_back(ptr);
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
        Deleter(const std::weak_ptr<CObjectPool<T, POLICY>::Impl>& _impl, const ObjectPointer& ptr) 
            : mPoolImpl(_impl)
            , mObjectPointer(ptr) {
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
        }

    private:
        std::weak_ptr<CObjectPool<T, POLICY>::Impl> mPoolImpl;
        ObjectPointer mObjectPointer;
    };
}