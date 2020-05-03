#pragma once

#include <chrono>
#include <string>
#include <vector>
#include "utils/preprocess.h"

namespace djv
{
    class DJV_API Clock
    {
    public:
        using time_point = std::chrono::system_clock::time_point;
        using duration = std::chrono::system_clock::duration;

        Clock() = default;
        Clock(time_point now, duration interval);

        Clock& pause();
        Clock& resume();
        Clock& setInterval(duration interval);
        Clock& setTime(time_point time);
        Clock& tick(); /* mTickCount += 1 only if delta >= interval */

        uint64_t   getTickCount() const;
        time_point getLastTickTime()  const; /* Get the time of the last `tick()` call */
        duration   getDelta() const; /* Get the time delta between the 2 previous ticks */
    
    private:
        duration   mInterval = duration(1);
        duration   mDelta = duration(0);
        time_point mLastTick = std::chrono::system_clock::now();
        uint64_t   mTickCount = 0;
        bool       mPause = false;
    };
}