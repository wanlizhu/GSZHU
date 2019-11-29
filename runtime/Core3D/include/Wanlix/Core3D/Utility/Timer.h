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
#include "Lockable.h"
#include "IterablePriorityQueue.h"

namespace Wanlix
{
    using DurationMs = std::chrono::milliseconds;
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::high_resolution_clock::time_point;
    using TimerId = uint64_t;

    class Timer final
    {
        struct Task;
        struct Comparator;
        using VoidFunction = std::function<void()>;
        using TaskPtr = std::shared_ptr<Task>;
        using TaskPriorityQueue = IterablePriorityQueue<TaskPtr, std::vector<TaskPtr>, Comparator>;
    public:
        static const int kForever = -1;
        static const DurationMs kImmediateMs;
        static const TimePoint kInvalidTimePoint;

        static TimerId StartTimer(
            const DurationMs& interval,
            int               repeat,
            VoidFunction&&    callback
        ) noexcept;

        static TimerId StartTimerAt(
            const TimePoint&  triggerAt,
            const DurationMs& interval,
            int               repeat,
            VoidFunction&&    callback
        ) noexcept;

        static bool Query(
            TimerId     id,
            DurationMs* interval,
            int*        repeat,
            TimePoint*  nextTrigger
        ) noexcept;

        static void Cancel(TimerId id) noexcept;
        static TimePoint NearestTriggerPoint() noexcept;

    private:
        static void Main(); // Timer thread function
        
    private:
        static Lockable<TaskPriorityQueue> mTaskQueue;
        static std::condition_variable     mCondition;
        static std::atomic_uint64_t        mTimerIdGen;
        static std::atomic_bool            mQuitFlag;
    };
}