#include "FrameRateController.h"
#include <algorithm>

namespace Wanli
{
    TickCounter::TickCounter()
        : mInterval(1)
        , mDeltaTime(0)
        , mLastTick(Clock::now())
        , mDeltaBegin(Clock::now())
    {}

    void TickCounter::SetInterval(const Duration_SEC& interval)
    {
        mInterval = interval;
    }

    int TickCounter::EvalTickCount()
    {
        auto now = Clock::now();
        if ((now - mLastTick) >= mInterval)
        {
            mLastTick = now;
            return std::max(1, (int)((now - mLastTick).count() / mInterval.count()));
        }

        return 0;
    }

    void TickCounter::BeginDelta()
    {
        mDeltaBegin = Clock::now();
    }

    void TickCounter::EndDelta()
    {
        mDeltaTime = Clock::now() - mDeltaBegin;
    }

    Duration_SEC TickCounter::GetDeltaTime() const
    {
        return mDeltaTime;
    }

    Duration_SEC TickCounter::GetInterval() const
    {
        return mInterval;
    }

    TickCounter& FrameRateController::GetUpdateCounter()
    {
        return mUpdateCounter;
    }

    TickCounter& FrameRateController::GetRenderCounter()
    {
        return mRenderCounter;
    }

    void FrameRateController::SetFPSLimit(Optional<float> value)
    {
        if (value.has_value() && value.value() > 0)
        {
            mRenderCounter.SetInterval(SECOND(1.0 / value.value()));
        }
        else
        {
            mRenderCounter.SetInterval(SECOND(1.0 / 60));
        }
    }

    void FrameRateController::SetUPSLimit(Optional<float> value)
    {
        if (value.has_value() && value.value() > 0)
        {
            mUpdateCounter.SetInterval(SECOND(1.0 / value.value()));
        }
        else
        {
            mUpdateCounter.SetInterval(SECOND(1.0 / 60));
        }
    }
}