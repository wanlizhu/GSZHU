#include "Wanlix/Core3D/Utility/Timer.h"
#include "Wanlix/Core3D/Utility/NonCopyable.h"

using namespace std;
using namespace chrono;

namespace Wanlix
{
    const DurationMs Timer::kImmediate = DurationMs::min();
    std::atomic_uint64_t Timer::mTimerIdGenerator(0);
    std::mutex Timer::mMutex;
    TaskThread Timer::mTimerThread;

    struct Timer::Task
    {
        TimePoint triggerAt;
        DurationMs interval;
        int repeat;
        std::function<void()> callback;

        inline Task(
            const TimePoint& triggerAt,
            const DurationMs& interval,
            int repeat,
            std::function<void()>&& func
        ) noexcept
            : triggerAt(triggerAt)
            , interval(interval)
            , repeat(repeat)
            , callback(callback)
        {}

        inline bool operator<(const Task& other) const {
            return triggerAt < other.triggerAt;
        }
    };
    
    TimerId Timer::StartTimer(
        const DurationMs& interval,
        int repeat,
        std::function<void()>&& callback
    ) noexcept 
    {
        return StartTimerAt(Clock::now(), 
                            interval,
                            repeat,
                            std::move(callback));
    }

    TimerId Timer::StartTimerAt(
        const TimePoint& triggerAt,
        const DurationMs& interval,
        int repeat,
        std::function<void()>&& callback
    ) noexcept
    {
        mTaskQueue.emplace(triggerAt, 
                           interval,
                           repeat,
                           std::move(callback));

    }
}