#include "Utilities/FPSCounter.h"

namespace Wanli
{
    FPSCounter::FPSCounter() 
    {}

    int FPSCounter::GetFPS() const
    {
        return mInstantFPS;
    }

    int FPSCounter::GetAvgFPS() const
    {
        return mAvgFPS;
    }

    double FPSCounter::GetElapsedTimeMs()
    {
        return std::chrono::duration_cast<MilliSecond>(mOneTickDuration).count();
    }

    void FPSCounter::BeginTick()
    {
        mTickBeginTime = Clock::now();
    }

    void FPSCounter::EndTick()
    {
        mTickEndTime = Clock::now();

        mOneTickDuration = std::chrono::duration_cast<MilliSecond>(mTickEndTime - mTickBeginTime);
        mAccumDuration += mOneTickDuration;

        mInstantFPS = (int)(1000 / mOneTickDuration.count());
        if (mAccumDuration >= Second(1))
        {
            mAvgFPS = (int)(mOneSecondTicks / (mAccumDuration.count() / 1000));
            mAccumDuration = MilliSecond(0);
            mOneSecondTicks = 0;
        }

        mOneSecondTicks++;
    }
}