#pragma once

#include "BasicTypes.h"

namespace Wanli
{
    class ChangePerSecond
    {
    public:
        ChangePerSecond()
            : mLastUpdate(Clock::now())
        {}

        void Update()
        {
            mValueCache++;
            auto now = Clock::now();

            if (Second(1) <= (now - mLastUpdate))
            {
                mValue = mValueCache;
                mValueCache = 0;
                mLastUpdate = now;
            }
        }

        inline Uint GetValue() const { return mValue; }

    private:
        TimePoint mLastUpdate;
        Uint mValue = 0;
        Uint mValueCache = 0;
    };
}