#pragma once
#include <ZHUENGINE/Common/Config.h>
#include <ZHUENGINE/Common/Time.h>

namespace ZHU
{
    class FPSCounter final
    {
    public:
        FPSCounter() 
            : mLastSwap(Time::Now())
        {}

        inline void AddOne() {
            static const Time onesec = Time::Seconds(1);
            if (Time::Now() - mLastSwap >= onesec) {
                mFPS = mNextFPS;
                mNextFPS = 0;
            }
            mNextFPS++;
        }

        inline int GetPFS() const { return mFPS; }

    public:
        int mFPS = 0;
        int mNextFPS = 0;
        Time mLastSwap;
    };
}