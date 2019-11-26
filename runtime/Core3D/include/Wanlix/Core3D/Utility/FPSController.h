#pragma once

#include "Wanlix/Core3D/IModule.h"
#include <chrono>
#include <deque>

namespace Wanlix
{
    class FPSController final : public IModule
    {
        using TimePoint = std::chrono::high_resolution_clock::time_point;
    public:
        using Ptr = std::shared_ptr<FPSController>;
        using ConstPtr = std::shared_ptr<const FPSController>;

        static Ptr Create(std::weak_ptr<Context> context);

        virtual ~FPSController() = default;
        virtual bool Tick(float delta) override;

        void EnableMaxFPS(bool val);
        void SetMaxFPS(double fps);
        double GetMaxFPS() const;

        float GetTimeMs() const;
        float GetDeltaTimeMs() const;

    protected:
        FPSController(std::weak_ptr<Context> context);

    private:
        // Internal
        TimePoint mTimeBegin;
        TimePoint mTimeFrameBegin;
        TimePoint mTimeFrameEnd;

        // Frame time
        double mDeltaTimeMs = 0.0;

        // FPS
        double mMaxFPS = 200.0;
        bool mEnableMaxFPS = true;
    };
}