#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <functional>
#include <future>
#include <queue>

namespace UT
{
    class ThreadPool final
    {
    public:
        static void Initialize(size_t count);
        static void Initialize(const std::vector<std::string>& names);
        static void Destroy();


        template<typename _Function_, typename... ARGS>
        static auto AddTask(_Function_&& func, ARGS&& ... args) 
            -> std::future<std::result_of_t<_Function_(ARGS...)>>
        {
            return AddTaskTo("*", std::forward<_Function_>(func), 
                                  std::forward<ARGS>(args)...);
        }

        template<typename _Function_, typename... ARGS>
        static auto AddTaskTo(const std::string& threadName, _Function_&& func, ARGS&& ... args)
            -> std::future<std::result_of_t<_Function_(ARGS...)>>
        {
            using ReturnType = std::result_of<_Function_(ARGS...)>;
            auto task = std::make_shared<std::packaged_task<ReturnType()>>(
                    std::bind(std::forward<_Function_>(func), std::forward<ARGS>(args)...)
                );

            std::future<ReturnType> future = task->get_future();
            { // auto-unlock block
                std::unique_lock<std::mutex> lock(smMutex);
                if (smStopFlag.load()) {
                    throw std::runtime_error("adding task to stopped thread pool is not allowed");
                }
                smTaskQueue.emplace([task]() { (*task)(); });
                smNameQueue.emplace(threadName);
            }

            if (threadName == "*") {
                smCondition.notify_one();
            } else {
                smCondition.notify_all();
            }

            return future;
        }

    private:
        static std::vector<std::thread> smThreads;
        static std::queue<std::function<void()>> smTaskQueue;
        static std::unordered_map<std::string, std::thread::id> smThreadNameMap;
        static std::queue<std::string> smNameQueue;

        // Synchronization
        static std::mutex smMutex;
        static std::condition_variable smCondition;
        static std::atomic_bool smStopFlag;
    };
}