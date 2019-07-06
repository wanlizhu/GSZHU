#pragma once
#include "Common/Config.h"
#include "Common/Time.h"
#include "Common/Delegate.h"
#include <optional>
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