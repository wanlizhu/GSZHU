#include "clock.h"

namespace djv
{
    Clock::Clock(time_point now, duration interval)
        : mLastTick(now)
        , mInterval(interval)
    {}

    Clock& Clock::pause()
    {
        mPause = true;
        return *this;
    }

    Clock& Clock::resume()
    {
        mPause = false;
        return *this;
    }

    Clock& Clock::setInterval(duration interval)
    {
        mInterval = interval;
        return *this;
    }

    Clock& Clock::setTime(time_point time)
    {
        mLastTick = time;
        return *this;
    }

    Clock& Clock::tick()
    {
        if (!mPause)
        {
            auto thisTick = std::chrono::system_clock::now();
            mDelta = thisTick - mLastTick;
            mLastTick = thisTick;
            mTickCount += mDelta.count() / mInterval.count();
        }
        return *this;
    }

    uint64_t Clock::getTickCount() const
    {
        return mTickCount;
    }

    Clock::time_point Clock::getLastTickTime() const
    {
        return mLastTick;
    }

    Clock::duration Clock::getDelta() const
    {
        return mDelta;
    }
    
}