#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <deque>
#include <condition_variable>
#include <type_traits>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>

namespace Wanlix
{
    class TaskThread : public std::thread
    {
    private:
        static void Main(TaskThread* self);

    public:
        TaskThread() noexcept
            : std::thread(std::bind(&TaskThread::Main, this))
        {}
        TaskThread(TaskThread&) = delete;
        TaskThread(TaskThread&& other) = delete;
        TaskThread& operator=(const TaskThread&) = delete;
        TaskThread& operator=(TaskThread&&) = delete;

        inline void AddTask(std::function<void()>&& func)
        {
            std::unique_lock<std::mutex> lock(mQueueMutex);
            mTaskQueue.emplace_back(std::move(func));
        }
        
        inline void Kill()
        {
            mKillFlag.store(true);
            mCV.notify_all();
        }

        inline void Clear()
        {
            std::unique_lock<std::mutex> lock(mQueueMutex);
            mTaskQueue.clear();
        }

        inline bool HasTask() const
        {
            std::unique_lock<std::mutex> lock(mQueueMutex);
            return !mTaskQueue.empty();
        }

        inline bool IsFree() const { return mFreeFlag.load(); }
        inline bool IsKilled() const { return mKillFlag.load(); }
        
    private:
        std::atomic_bool mKillFlag = false;
        std::atomic_bool mFreeFlag = false;

        std::condition_variable mCV;
        std::mutex mQueueMutex;
        std::deque<std::function<void()>> mTaskQueue;
    };
}