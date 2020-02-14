#pragma once

#include "BasicTypes.h"

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
}