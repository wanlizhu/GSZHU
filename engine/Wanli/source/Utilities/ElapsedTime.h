#pragma once

#include "Core/BasicTypes.h"

namespace Wanli
{
    class DLLDECL ElapsedTime
    {
    public:
        explicit ElapsedTime(const Duration& interval = Duration(1))
            : mLastUpdate(Clock::now())
            , mInterval(interval)
        {}

        inline int GetElapsed()
        {
            auto now = Clock::now();
            int elapsedIntervalCount = (int)std::floor((now - mLastUpdate) / mInterval);

            if (elapsedIntervalCount != 0)
            {
                mLastUpdate = now;
            }

            return elapsedIntervalCount;
        }

        inline TimePoint GetLastUpdate() const { return mLastUpdate; }
        inline Duration GetInterval() const { return mInterval; }
        inline void SetInterval(const Duration& interval) { mInterval = interval; }
        
    private:
        TimePoint mLastUpdate;
        Duration mInterval;
    };

    class DLLDECL DeltaTime
    {
    public:
        inline void Update()
        {
            mCurrent = Clock::now();
            mDelta = mCurrent - mLast;
            mLast = mCurrent;
        }

        template<typename T>
        inline typename T::rep Get() const 
        {
            return std::chrono::duration_cast<T>(mDelta).count();
        }

    private:
        TimePoint mCurrent = Clock::now();
        TimePoint mLast = Clock::now();
        Duration mDelta;
    };
}