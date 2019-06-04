#pragma once

#include <memory>
#include <mutex>
#include <chrono>

// This class will use std::mutex for the internal locking mechanism by default.
// Other classes which are useful for the mutex type are
// std::recursive_mutex, std::timed_mutex, and std::recursive_timed_mutex.

// The handle returned by the various lock methods is moveable but not copyable.

namespace GSZHU {
    template<typename T, typename MUTEX = std::mutex>
    class CGuarded {
        class CDeleter;
    public:
        using Handle = std::unique_ptr<T, CDeleter>;

        template<typename... ARGS>
        CGuarded(ARGS&&... args);

        Handle Lock();
        Handle TryLock(); // Returns a null handle if the object is already locked
        template<typename DURATION>
        Handle TryLockFor(DURATION const& duration);
        template<typename TIME_POINT>
        Handle TryLockUntil(TIME_POINT const& time);

    private:
        T mObject;
        MUTEX mMutex;
    };

    // Partial specialization for single thread policy
    template<typename T>
    class CGuarded<T, void> {
    public:
        using Handle = T*;

        template<typename... ARGS>
        CGuarded(ARGS&& ... args) 
            : mObject(std::forward<ARGS>(args)...)
        {}

        Handle Lock() { return &mObject; }
        Handle TryLock() { return &mObject; }
        template<typename DURATION>
        Handle TryLockFor(DURATION const& duration) { return &mObject; }
        template<typename TIME_POINT>
        Handle TryLockUntil(TIME_POINT const& time) { return &mObject; }

    private:
        T mObject;
    };








///////////////////////////////////////////////////////////
// DEFINITION
///////////////////////////////////////////////////////////

    template<typename T, typename MUTEX>
    class CGuarded<T, MUTEX>::CDeleter {
    public:
        CDeleter(std::unique_lock<MUTEX> lock)
            : mMutex(std::move(lock))
        {}

        void operator()(T* ptr) {
            if (mLock.owns_lock())
                mLock.unlock();
        }

    private:
        std::unique_lock<MUTEX> mLock;
    };

    template<typename T, typename MUTEX>
    template<typename... ARGS>
    CGuarded<T, MUTEX>::CGuarded(ARGS&&... args)
        : mObject(std::forward<ARGS>(args)...)
    {}

    template<typename T, typename MUTEX>
    auto CGuarded<T, MUTEX>::Lock() -> Handle {
        std::unique_lock<MUTEX> lock(mMutex);
        return Handle(&mObject, CDeleter(std::move(lock)));
    }

    template<typename T, typename MUTEX>
    auto CGuarded<T, MUTEX>::TryLock() -> Handle {
        std::unique_lock<MUTEX> lock(mMutex, std::try_to_lock);
        if (lock.owns_lock())
            return Handle(&mObject, CDeleter(std::move(lock)));
        else
            return Handle(nullptr, CDeleter(std::move(lock)));
    }

    template<typename T, typename MUTEX>
    template<typename DURATION>
    auto CGuarded<T, MUTEX>::TryLockFor(DURATION const& duration) -> Handle {
        std::unique_lock<MUTEX> lock(mMutex, duration);
        if (lock.owns_lock())
            return Handle(&mObject, CDeleter(std::move(lock)));
        else
            return Handle(nullptr, CDeleter(std::move(lock)));
    }

    template<typename T, typename MUTEX>
    template<typename TIME_POINT>
    auto CGuarded<T, MUTEX>::TryLockUntil(TIME_POINT const& time) -> Handle {
        std::unique_lock<MUTEX> lock(mMutex, time);
        if (lock.owns_lock())
            return Handle(&mObject, CDeleter(std::move(lock)));
        else
            return Handle(nullptr, CDeleter(std::move(lock)));
    }
}