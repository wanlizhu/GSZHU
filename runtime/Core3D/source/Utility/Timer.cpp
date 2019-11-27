#include "Wanlix/Core3D/Utility/Timer.h"
#include "Wanlix/Core3D/Utility/NonCopyable.h"

using namespace std;
using namespace chrono;

namespace Wanlix
{
    const DurationMs Timer::kImmediateMs = DurationMs::min();
    const TimePoint Timer::kInvalidTimePoint = TimePoint::max();
    std::condition_variable Timer::mCondition;
    std::atomic_uint64_t Timer::mTimerIdGen(0);
    std::atomic_bool Timer::mQuitFlag = false;

    struct Timer::Task
    {
        TimerId id;
        TimePoint triggerAt;
        DurationMs interval;
        int repeat;
        std::function<void()> callback;

        inline Task(
            TimerId id,
            const TimePoint& triggerAt,
            const DurationMs& interval,
            int repeat,
            std::function<void()>&& func
        ) noexcept
            : id(id)
            ,triggerAt(triggerAt)
            , interval(interval)
            , repeat(repeat)
            , callback(callback)
        {}

        inline bool operator<(const Task& other) const {
            return triggerAt < other.triggerAt;
        }
    };

    struct Timer::Comparator
    {
        bool operator()(
            const Timer::TaskPtr& task1,
            const Timer::TaskPtr& task2
            ) const
        {
            return task1->triggerAt < task2->triggerAt;
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
        std::unique_lock<std::mutex> queueLock(mTaskQueue.GetMutex());
        TimerId id = ++mTimerIdGen;
        mTaskQueue.GetData().emplace(
            std::make_shared<Task>(
            id,
            triggerAt,
            interval,
            repeat,
            std::move(callback)));
        mCondition.notify_one();
        return id;
    }

    bool Timer::Query(
        TimerId id,
        DurationMs* interval,
        int* repeat,
        TimePoint* nextTrigger
    ) noexcept
    {
        std::unique_lock<std::mutex> queueLock(mTaskQueue.GetMutex());
        auto it = std::find_if(mTaskQueue.GetData().cbegin(), 
                               mTaskQueue.GetData().cend(),
                               [&](const TaskPtr& task) {
                                   return task->id == id;
                               });
        if (it == mTaskQueue.GetData().cend()) {
            return false;
        }

        if (interval) {
            *interval = (*it)->interval;
        }
        if (repeat) {
            *repeat = (*it)->repeat;
        }
        if (nextTrigger) {
            *nextTrigger = (*it)->triggerAt;
        }
        return true;
    }

    void Timer::Cancel(TimerId id) noexcept {
        std::unique_lock<std::mutex> queueLock(mTaskQueue.GetMutex());
        auto it = std::find_if(mTaskQueue.GetData().cbegin(),
                               mTaskQueue.GetData().cend(),
                               [&](const TaskPtr& task) {
                                   return task->id == id;
                               });
        if (it != mTaskQueue.GetData().cend()) {
            mTaskQueue.GetData().erase(it);
        }
    }

    TimePoint Timer::NearestTriggerPoint() noexcept {
        std::unique_lock<std::mutex> queueLock(mTaskQueue.GetMutex());
        if (mTaskQueue.GetData().empty()) {
            return kInvalidTimePoint;
        }
        return mTaskQueue.GetData().top()->triggerAt;
    }

    void Timer::Main()
    {
        std::mutex mutex;
        while (!mQuitFlag.load())
        {
            std::unique_lock<std::mutex> lock(mutex);
            mCondition.wait(lock, 
                            [&]() {
                                return mQuitFlag.load() || !mTaskQueue.GetData().empty();
                            });

            auto nextTrigger = NearestTriggerPoint();
            if (nextTrigger != kInvalidTimePoint) {
                if (nextTrigger > Clock::now()) {
                    std::this_thread::sleep_until(nextTrigger);
                }
            } else {
                continue;
            }

            std::unique_lock<std::mutex> queueLock(mTaskQueue.GetMutex());
            while (mTaskQueue.GetData().top()->triggerAt <= Clock::now()) {
                auto task = mTaskQueue.GetData().top();
                mTaskQueue.GetData().pop();
                std::thread(task->callback).detach();

                if (task->repeat > 0) {
                    task->repeat -= 1;
                    task->triggerAt += task->interval;
                    mTaskQueue.GetData().emplace(task);
                }
            }
        }
    }
}