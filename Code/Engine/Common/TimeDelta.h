#pragma once
#include "Config.h"
#include "Time.h"

namespace ZHU
{
    class TimeDelta final
    {
    public:
        TimeDelta() = default;

        inline void Update() {
            mCurrent = Time::Now();
            mDelta = mCurrent - mPrevious;
            mPrevious = mCurrent;
        }

        inline const Time& GetDelta() const { return mDelta; }

    private:
        Time mCurrent;
        Time mPrevious;
        Time mDelta;
    };
}