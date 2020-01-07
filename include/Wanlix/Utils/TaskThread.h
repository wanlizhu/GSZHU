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
#include "Lockable.h"

namespace Wanlix
{
    class TaskThread : public std::thread
    {
    private:
        static void Main(TaskThread* self);

    public:
        enum Tag
        {
            TagPushBack,
            TagPushFront,
        };

        TaskThread() noexcept;
        TaskThread(TaskThread&) = delete;
        TaskThread(TaskThread&& other) = delete;
        TaskThread& operator=(const TaskThread&) = delete;
        TaskThread& operator=(TaskThread&&) = delete;

        bool AddTask(std::function<void()>&& func, Tag tag = TagPushBack);
        
        void Kill();
        void Clear();
        bool IsFree() const;
        bool IsKilled() const;
        bool HasTask() const;
        
    private:
        std::atomic_bool mKillFlag = false;
        std::atomic_bool mFreeFlag = false;

        std::condition_variable mCV;
        Lockable<std::deque<std::function<void()>>> mTaskQueue;
    };
}