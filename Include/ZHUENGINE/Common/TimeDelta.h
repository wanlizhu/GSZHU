#pragma once
#include <ZHUENGINE/Common/Config.h>
#include <ZHUENGINE/Common/Time.h>

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