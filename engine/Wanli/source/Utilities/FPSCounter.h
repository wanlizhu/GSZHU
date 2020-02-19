#pragma once

#include "Core/BasicTypes.h"

namespace Wanli
{
    class DLLDECL FPSCounter
    {
    public:
        FPSCounter();
        virtual ~FPSCounter() = default;

        int GetFPS() const;
        int GetAvgFPS() const;
        double GetElapsedTimeMs();

        void BeginTick();
        void EndTick();
        
    protected:
        int mInstantFPS = 0;
        int mAvgFPS = 0;
        int mOneSecondTicks = 0;

        TimePoint mTickBeginTime = Clock::now();
        TimePoint mTickEndTime = Clock::now();
        MilliSecond mOneTickDuration = MilliSecond(0);
        MilliSecond mAccumDuration = MilliSecond(0);
    };
}