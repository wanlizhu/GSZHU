#pragma once

#include <GSZHU/IMemoryAllocator.h>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <type_traits>
#include <utility>
#include <list>

namespace GSZHU {
    struct SPeudoLock {
        struct NoMutex {};
        struct NoLock { NoLock(NoMutex&) {} };

        using Mutex = NoMutex;
        using Lock = NoLock;
    };

    template<typename T, typename POLICY = SPeudoLock>
    class CObjectPool {
        struct Impl;
        struct Deleter;
    public:
        using Object = T;
        using ObjectPointer = std::shared_ptr<Object>;
        using Allocator = std::function<ObjectPointer()>; 
        using Recycler = std::function<void(ObjectPointer)>;
        using MutexType = typename POLICY::Mutex;
        using LockType = typename POLICY::Lock;

        // Use SFINAE to disable the default constructor of CObjectPool for non-default-constructible types.
        template<typename U = T, 
                 typename std::enable_if<std::is_default_constructible<U>::value, int>::type = 0>
        CObjectPool()
            : mImpl(std::make_shared<Impl>(Allocator(std::make_shared<Object>))) // Cast function "std::make_unique<Object>" to type "Allocator"
        {}

        CObjectPool(Allocator _Allocator)
            : mImpl(std::make_shared<Impl>(std::move(_Allocator)))
        {}

        CObjectPool(Allocator _Allocator, Recycler _Recycler)
            : mImpl(std::make_shared<Impl>(std::move(_Allocator), std::move(_Recycler)))
        {}

        CObjectPool(const CObjectPool& Other)
            : mImpl(std::make_shared<Impl>(*Other.mImpl))
        {}

        CObjectPool(CObjectPool&& Other)
            : mImpl(std::move(Other.mImpl)) {
            assert(mImpl);
        }

        CObjectPool& operator=(const CObjectPool& Other) {
            CObjectPool Tmp(Other);
            std::swap(*this, Tmp);
            return *this;
        }

        CObjectPool& operator=(CObjectPool&& Other) {
            mImpl = std::move(Other.mImpl);
            return *this;
        }

        uint32_t GetFreeObjectCount() const {
            assert(mImpl);
            return mImpl->GetFreeObjectCount();
        }

        void Shrink() {
            assert(mImpl);
            mImpl->Shrink();
        }

        ObjectPointer Allocate() {
            assert(mImpl);
            return mImpl->Allocate();
        }

    private:
        std::shared_ptr<Impl> mImpl;
    };
}