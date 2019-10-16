#pragma once

#include <functional>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <memory>
#include <type_traits>
#include <thread>
#include <mutex>
#include <thread>
#include <any>
#include <array>
#include <list>
#include <condition_variable>

namespace GE2
{
    class NonMutex
    {
    public:
        constexpr bool lock() const { return true; }
        constexpr void unlock() noexcept {}
    };

    template<typename FUNC>
    class Signal;

    template<typename RT, typename... ARGS>
    class Signal<RT(ARGS...)> final
    {
    public:
        using Mutex       = NonMutex;
        using Slot        = std::function<RT(ARGS&&...)>;
        using Accumulater = std::function<void(RT)>;

        size_t connect(const Slot& slot) 
        { 
            static size_t _id = 0;
            mMutex.lock();
            mSlotMap[_id++] = slot;
            mMutex.unlock();
            return _id - 1;
        }

        void disconnect(size_t id) 
        {
            mMutex.lock();
            auto it = mSlotMap.find(id); 
            if (id != mSlotMap.end())
                mSlotMap.erase(it);
            mMutex.unlock();
        }

        bool empty() const 
        {
            mMutex.lock();
            bool v = mSlotMap.empty();
            mMutex.unlock();
            return v;
        }

        void clear() 
        {
            mMutex.lock();
            mSlotMap.clear();
            mMutex.unlock();
        }

        void set_accumulater(const Accumulater& accum)
        {
            mMutex.lock();
            mAccumulater = accum;
            mMutex.unlock();
        }

        void remove_accumulater()
        {
            mMutex.lock();
            mAccumulater = Accumulater();
            mMutex.unlock();
        }

        template<typename... Uref>
        void operator()(Uref&& ... args)
        {
            emit(std::forward<ARGS>(args)...);
        }

        template<typename... Uref>
        void emit(Uref&& ... args)
        {
            mMutex.lock();
            for (auto it = mSlotMap.begin(); it != mSlotMap.end(); ++it)
            {
                Slot& slotFunc = it->second;
                if constexpr (!std::is_same_v<void, RT>)
                {
                    if (mAccumulater)
                    {
                        mAccumulater(slotFunc(std::forward<ARGS>(args)...));
                    }
                }
                else
                {
                    slotFunc(std::forward<ARGS>(args)...);
                }
            }
            mMutex.unlock();
        }

        template<typename... Uref>
        void emit_async(Uref&& ... args)
        {
            std::thread worker(&Signal::emit, this, std::forward<ARGS>(args)...);
            worker.detach();
        }

    private:
        std::unordered_map<size_t, Slot> mSlotMap;
        Accumulater mAccumulater;
        Mutex mMutex;
    };
}

