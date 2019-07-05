#pragma once
#include "Config.h"
#include <optional>
#include "Time.h"
#include "Delegate.h"
#include <atomic>

namespace ZHU
{
    class ZHU_API Timer
    {
    public:
        friend class Timers;
        Timer(const Time& interval, const std::optional<int>& repeat)
            : mInterval(interval)
            , mNext(Time::Now() + mInterval)
            , mRepeat(repeat)
            , mStopped(true)
        {}

        inline const Time&               GetInterval() const { return mInterval; }
        inline const std::optional<int>& GetRepeat() const { return mRepeat; }
        inline bool                      IsStopped() const { return mStopped.load(); }
        inline void                      Stop() { mStopped.store(true); }
        inline Delegate<void()>&         OnTick() { return mOnTick; }

    private:
        Time mInterval;
        Time mNext;
        std::optional<int> mRepeat;
        std::atomic<bool> mStopped = true;
        Delegate<void()> mOnTick;
    };
}