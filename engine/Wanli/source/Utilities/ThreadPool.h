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
#include <deque>
#include "Core/BasicTypes.h"

namespace Wanli
{
    class DLLDECL ThreadPool final
    {
    public:
        static inline void Shutdown()
        {
            Get().mKillFlag.store(true);
            for (auto& thread : Get().mThreads)
            {
                if (thread->joinable())
                    thread->join();
            }
            Get().mThreads.clear();
        }

        template<typename FuncType, typename... Args>
        static inline auto Enqueue(FuncType&& func, Args&&... args)
            -> std::future<std::invoke_result_t<FuncType, Args...>>
        {
            using ReturnType = std::invoke_result_t<FuncType, Args...>;
            auto packagedTask = std::make_shared<std::packaged_task<ReturnType()>>(
                    std::bind(std::forward<FuncType>(func), std::forward<Args>(args)...)
                );
            std::future<ReturnType> future = packagedTask->get_future();
            Get().mTaskQueue.emplace_back([packagedTask] () { (*packagedTask)(); });
            return future;
        }

    private:
        static void Main();
        static ThreadPool& Get();
        explicit ThreadPool(int numThreads);
        
    private:
        std::atomic_bool mKillFlag;
        std::condition_variable mCV;
        mutable std::mutex mQueueMutex;
        std::deque<std::function<void()>> mTaskQueue;
        std::vector<std::shared_ptr<std::thread>> mThreads;
    };
}