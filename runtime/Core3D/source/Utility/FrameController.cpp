#include "Wanlix/Core3D/Utility/FrameController.h"
#include <thread>

namespace Wanlix
{
    using namespace std;
    using namespace chrono;

    FrameController::Ptr FrameController::Create(std::weak_ptr<Context> context)
    {
        return std::shared_ptr<FrameController>(new FrameController(context));
    }

    FrameController::FrameController(std::weak_ptr<Context> context)
        : IModule(typeid(FrameController), context)
    {
        mTimeBegin = high_resolution_clock::now();
        mTimeFrameBegin = high_resolution_clock::now();
        mTimeFrameEnd = high_resolution_clock::now();
    }

    void FrameController::EnableMaxFPS(bool val)
    {
        mEnableMaxFPS = val;
    }

    void FrameController::SetMaxFPS(double fps)
    {
        mMaxFPS = fps <= 0.0 ? 60.0 : fps;
    }

    double FrameController::GetMaxFPS() const
    {
        return mMaxFPS;
    }

    float FrameController::GetTimeMs() const
    {
        return static_cast<float>((double)(high_resolution_clock::now() - mTimeBegin).count());
    }

    float FrameController::GetDeltaTimeMs() const
    {
        return static_cast<float>(mDeltaTimeMs);
    }

    bool FrameController::Tick(float delta)
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
        mDeltaTimeMs = static_cast<double>((timeWork + realSlept).count());
        return true;
    }
}