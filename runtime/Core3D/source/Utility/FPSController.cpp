#include "Wanlix/Core3D/Utility/FPSController.h"
#include <thread>

namespace Wanlix
{
    using namespace std;
    using namespace chrono;

    FPSController::Ptr FPSController::Create(std::weak_ptr<Context> context)
    {
        return std::shared_ptr<FPSController>(new FPSController(context));
    }

    FPSController::FPSController(std::weak_ptr<Context> context)
        : IModule(typeid(FPSController), context)
    {
        mTimeBegin = high_resolution_clock::now();
        mTimeFrameBegin = high_resolution_clock::now();
        mTimeFrameEnd = high_resolution_clock::now();
    }

    void FPSController::EnableMaxFPS(bool val)
    {
        mEnableMaxFPS = val;
    }

    void FPSController::SetMaxFPS(double fps)
    {
        mMaxFPS = fps <= 0.0 ? 60.0 : fps;
    }

    double FPSController::GetMaxFPS() const
    {
        return mMaxFPS;
    }

    float FPSController::GetTimeMs() const
    {
        return static_cast<float>((double)(high_resolution_clock::now() - mTimeBegin).count());
    }

    float FPSController::GetDeltaTimeMs() const
    {
        return static_cast<float>(mDeltaTimeMs);
    }

    bool FPSController::Tick(float delta)
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