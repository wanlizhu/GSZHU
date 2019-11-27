#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <type_traits>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
#include "Wanlix/Core3D/Utility/Lockable.h"

namespace Wanlix
{
    class TaskThread : public std::thread
    {
    private:
        static void Main(TaskThread* self);

    public:
        TaskThread() noexcept;
        TaskThread(TaskThread&) = delete;
        TaskThread(TaskThread&& other) = delete;
        TaskThread& operator=(const TaskThread&) = delete;
        TaskThread& operator=(TaskThread&&) = delete;

        void SetName(const std::string& name);
        const std::string& GetName() const;
        void PostTask(std::function<void()>&& func);
        void Quit();
        bool ShouldAwake() const;
        bool IsBlocked() const;

    private:
        std::string mName;
        std::atomic_bool mQuitFlag = false;
        std::atomic_bool mBlocked = false;

        std::condition_variable mCondition;
        Lockable<std::vector<std::function<void()>>> mTaskList;
    };
}