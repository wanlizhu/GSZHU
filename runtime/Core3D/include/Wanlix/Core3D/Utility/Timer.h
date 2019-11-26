#pragma once

#include <functional>
#include <type_traits>
#include <queue>
#include <deque>
#include <unordered_map>
#include <chrono>
#include <atomic>
#include <mutex>
#include <thread>
#include "TaskThread.h"

namespace Wanlix
{
    using DurationMs = std::chrono::milliseconds;
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::high_resolution_clock::time_point;
    using TimerId = uint64_t;

    class Timer final
    {
        struct Task;
    public:
        static const int kForever = -1;
        static const DurationMs kImmediate;

        static TimerId StartTimer(
            const DurationMs& interval,
            int repeat,
            std::function<void()>&& callback
            ) noexcept;

        static TimerId StartTimerAt(
            const TimePoint& triggerAt,
            const DurationMs& interval,
            int repeat,
            std::function<void()>&& callback
        ) noexcept;

        static bool Query(
            TimerId id,
            DurationMs* interval,
            int* repeat,
            DurationMs* nextTrigger
        ) noexcept;

        static void Cancel(TimerId id) noexcept;
        static DurationMs DurationToNearestTimer() noexcept;
        
    private:
        static std::priority_queue<Task> mTaskQueue;
        static std::atomic_uint64_t mTimerIdGenerator;
        static std::mutex mMutex;
        static TaskThread mTimerThread;
    };
}