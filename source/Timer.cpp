#include "Wanlix/Timer.h"

using namespace std;
using namespace chrono;

namespace Wanlix
{
    const DurationMs Timer::kImmediateMs = DurationMs(0);
    const TimePoint Timer::kMaxTimePoint = Clock::time_point(DurationMs(std::numeric_limits<long long>::max()));

    std::atomic_bool Timer::mKillFlag;
    std::atomic_uint64_t Timer::mLastTimerId;
    std::condition_variable Timer::mCV;
    Lockable<OrderedList<Timer::TimerTask>> Timer::mTaskQueue;

    struct Timer::TimerTask
    {
        using CallbackPtr = std::shared_ptr<std::function<void()>>;

        TimerId id;
        TimePoint triggerAt;
        DurationMs interval;
        int repeat;
        CallbackPtr callback;

        TimerTask::TimerTask(TimerId id,
                             TimePoint triggerAt,
                             DurationMs interval,
                             int repeat,
                             CallbackPtr callback)
            : id(id)
            , triggerAt(triggerAt)
            , interval(interval)
            , repeat(repeat)
            , callback(callback)
        {}

        bool operator<(const TimerTask& rhs) const {
            return triggerAt < rhs.triggerAt;
        }
    };

    void Timer::Main()
    {
        std::mutex mutex;
        TimePoint nextTrigger = kMaxTimePoint;
        while (!mKillFlag.load())
        {
            std::unique_lock<std::mutex> lock(mutex);
            mCV.wait_until(lock,
                           nextTrigger,
                           [&] () {
                               std::unique_lock<std::mutex> lockData(mTaskQueue.GetMutex());
                               return mKillFlag.load() || !mTaskQueue.GetData().empty();
                           });

            if (mKillFlag.load()) {
                return;
            }

            nextTrigger = NearestTriggerPoint();
            if (nextTrigger == kMaxTimePoint) {
                continue;
            }

            if (nextTrigger > Clock::now()) {
                nextTrigger = Clock::now();
                continue;
            }

            if (nextTrigger <= Clock::now()) {
                std::unique_lock<std::mutex> lockData(mTaskQueue.GetMutex());
                while (mTaskQueue.GetData().top().triggerAt <= Clock::now()) {
                    auto task = mTaskQueue.GetData().top();
                    mTaskQueue.GetData().pop();
                    std::thread(*(task.callback)).detach();

                    if (task.repeat > 0) {
                        task.repeat -= 1;
                        task.triggerAt += task.interval;
                        mTaskQueue.GetData().emplace_back(task);
                    }
                }
            }
        }
    }

    TimerId Timer::StartTimer(const DurationMs& interval, 
                              int repeat,
                              std::function<void()>&& callback)
    {
        return StartTimerAt(Clock::now(),
                            interval,
                            repeat,
                            std::move(callback));
    }

    TimerId Timer::StartTimerAt(const TimePoint& triggerAt,
                                const DurationMs& interval,
                                int repeat,
                                std::function<void()>&& callback)
    {
        TimerId thisId = mLastTimerId++;
        {
            std::unique_lock<std::mutex> lockData(mTaskQueue.GetMutex());
            mTaskQueue.GetData().emplace_back(thisId,
                                              triggerAt,
                                              interval,
                                              repeat,
                                              std::make_shared<std::function<void()>>(std::move(callback)));
        }
        mCV.notify_one();
        return thisId;
    }

    void Timer::Cancel(TimerId id)
    {
        std::unique_lock<std::mutex> lock(mTaskQueue.GetMutex());
        for (auto& it = mTaskQueue.GetData().begin(); 
             it != mTaskQueue.GetData().end();
             it++) {
            if (it->id == id) {
                mTaskQueue.GetData().erase(it);
                return;
            }
        }
    }

    TimePoint Timer::NearestTriggerPoint()
    {
        std::unique_lock<std::mutex> lock(mTaskQueue.GetMutex());
        if (mTaskQueue.GetData().empty()) {
            return kInvalidTimePoint;
        }
        return mTaskQueue.GetData().top().triggerAt;
    }
}