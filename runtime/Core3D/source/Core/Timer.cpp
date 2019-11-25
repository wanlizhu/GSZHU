#include "Wanlix/Core/Timer.h"
#include <thread>

namespace Wanlix
{
    using namespace std;
    using namespace chrono;

    Timer::Ptr Timer::Create(std::weak_ptr<Context> context)
    {
        return std::shared_ptr<Timer>(new Timer(context));
    }

    Timer::Timer(std::weak_ptr<Context> context)
        : IModule(typeid(Timer), context)
    {
        mTimeBegin = high_resolution_clock::now();
        mTimeFrameBegin = high_resolution_clock::now();
        mTimeFrameEnd = high_resolution_clock::now();
    }

    void Timer::EnableMaxFPS(bool val)
    {
        mEnableMaxFPS = val;
    }

    void Timer::SetMaxFPS(double fps)
    {
        mMaxFPS = fps <= 0.0 ? 60.0 : fps;
    }

    double Timer::GetMaxFPS() const
    {
        return mMaxFPS;
    }

    float Timer::GetTimeMs() const
    {
        return static_cast<float>((double)(high_resolution_clock::now() - mTimeBegin).count());
    }

    float Timer::GetDeltaTimeMs() const
    {
        return static_cast<float>(mDeltaTimeMs);
    }

    void Timer::Tick(float delta)
    {
        mTimeFrameBegin = high_resolution_clock::now();
        auto timeWork = mTimeFrameBegin - mTimeFrameEnd;

        if (mEnableMaxFPS) {
            const double minDelta = 1000.0 / mMaxFPS;
            const double sleepMs = minDelta - timeWork.count();

            if (sleepMs > 0.0) {
                this_thread::sleep_for(milliseconds(static_cast<int64_t>(sleepMs)));
            }
        }

        mTimeFrameEnd = high_resolution_clock::now();
        auto realSlept = mTimeFrameEnd - mTimeFrameBegin;
        mDeltaTimeMs = (timeWork + realSlept).count();
    }
}