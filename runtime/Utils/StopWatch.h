#pragma once

#include <chrono>

namespace Wanlix
{
    class StopWatch
    {
    public:
        using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

        inline static TimePoint Now() {
            return std::chrono::high_resolution_clock::now();
        }

        inline TimePoint Update() {
            auto now = Now();
            mElapsedTime = now - mLastTime;
            mLastTime = now;
            return mLastTime;
        }

        inline float GetElapsedTime() const {
            return float(mElapsedTime.count());
        }

    protected:
        TimePoint mLastTime;
        std::chrono::duration<double, std::milli> mElapsedTime;
    };
}