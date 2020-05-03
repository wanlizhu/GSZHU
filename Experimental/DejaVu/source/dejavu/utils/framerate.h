#pragma once

#include <vector>
#include <algorithm>
#include "utils/clock.h"
#include "utils/preprocess.h"

namespace djv
{
    class DJV_API Framerate
    {
    public:
        Framerate() = default;

        void reset();
        void tick();
        void setFrameWindow(uint64_t count);
        uint64_t getFrameCount() const;
        Clock::duration getAverageFrameTime() const;
        Clock::duration getLastFrameTime() const;

    private:
        Clock mClock;
        uint64_t mFrameCount = 0;
        uint64_t mFrameWindow = 60;
        std::vector<Clock::duration> mFrameTimes;
    };
}