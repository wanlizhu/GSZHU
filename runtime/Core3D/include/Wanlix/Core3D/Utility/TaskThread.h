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
        static void Main(TaskThread* self)
        {
            std::mutex mMutex;
            while (!self->mQuitFlag.load())
            {
                std::unique_lock<std::mutex> lock(mMutex);
                self->mBlocked.store(true);
                self->mCondition.wait(lock, 
                                      [self]() { 
                                          return self->ShouldAwake(); 
                                      });
                self->mBlocked.store(false);
                {
                    std::unique_lock<std::mutex> lockTaskList(self->mTaskList.GetMutexRef());
                    for (auto& function : self->mTaskList.GetData()) {
                        function();
                    }
                    self->mTaskList.GetData().clear();
                }
            }
        }

    public:
        TaskThread() noexcept 
            : std::thread(std::bind(&TaskThread::Main, this)) 
        {}

        TaskThread(TaskThread&) = delete;
        TaskThread(TaskThread&& other) = delete;
        TaskThread& operator=(const TaskThread&) = delete;
        TaskThread& operator=(TaskThread&&) = delete;

        inline void SetName(const std::string& name) {
            mName = name;
        }

        inline const std::string& GetName() const {
            return mName; 
        }

        inline void PostTask(std::function<void()>&& func) {
            {
                std::unique_lock<std::mutex> lock(mTaskList.GetMutexRef());
                mTaskList.GetData().emplace_back(std::move(func));
            }
            mCondition.notify_one();
        }

        inline void Quit() {
            mQuitFlag.store(true);
            mCondition.notify_one();
        }

        inline bool ShouldAwake() const {
            return mQuitFlag.load() || !mTaskList.GetData().empty();
        }

        inline bool IsBlocked() const {
            return mBlocked.load();
        }

    private:
        std::string mName;
        std::atomic_bool mQuitFlag = false;
        std::atomic_bool mBlocked = false;

        std::condition_variable mCondition;
        Lockable<std::vector<std::function<void()>>> mTaskList;
    };
}