#pragma once

#include "Core/Common.h"

namespace Wanli
{
    class WANLI_API TickCounter
    {
    public:
        TickCounter();

        void SetInterval(const Duration_SEC& interval);
        int EvalTickCount();
        
        void BeginDelta();
        void EndDelta();
        Duration_SEC GetDeltaTime() const;
        Duration_SEC GetInterval() const;

    private:
        Duration_SEC mInterval;
        TimePoint mLastTick;

        TimePoint mDeltaBegin;
        Duration_SEC mDeltaTime;
    };

    class WANLI_API FrameRateController
    {
    public:
        TickCounter& GetUpdateCounter();
        TickCounter& GetRenderCounter();

        void SetFPSLimit(Optional<float> value);
        void SetUPSLimit(Optional<float> value);

    private:
        TickCounter mUpdateCounter;
        TickCounter mRenderCounter;
        Optional<float> mFPSLimit;
        Optional<float> mUPSLimit;
    };


}