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
    private:
        struct TimerTask;
        static void Main(); // Timer thread function

    public:
        static const int kForever = -1;
        static const DurationMs kImmediateMs;
        static const TimePoint kInvalidTimePoint;

        static TimerId StartTimer(const DurationMs& interval, int repeat, std::function<void()>&& callback);
        static TimerId StartTimerAt(const TimePoint&  triggerAt, const DurationMs& interval, int repeat, std::function<void()>&& callback);
        static void Cancel(TimerId id);
        static TimePoint NearestTriggerPoint();

    private:
        static std::atomic_bool        mKillFlag;
        static std::atomic_uint64_t    mLastTimerId;
        static std::condition_variable mCV;
        static Lockable<std::priority_queue<TimerTask>> mTaskQueue;
    };
}