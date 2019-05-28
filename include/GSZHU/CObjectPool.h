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
        }

    private:
        std::weak_ptr<CObjectPool<T, POLICY>::Impl> mPoolImpl;
        ObjectPointer mObjectPointer;
    };
}