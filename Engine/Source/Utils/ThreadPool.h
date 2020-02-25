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
#include <type_traits>

namespace Wanli
{
    class WANLI_API ThreadPool final
    {
    public:
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

    private:
        explicit ThreadPool(int numThreads)
        {
            for (int i = 0; i < numThreads; i++)
            {
                mThreads.push_back(std::make_shared<std::thread>(&ThreadPool::Main));
            }
        }
        static ThreadPool& Get()
        {
            static ThreadPool _instance(std::thread::hardware_concurrency());
            return _instance;
        }
        static void Main()
        {
            ThreadPool& pool = ThreadPool::Get();
            std::mutex waitMutex;

            while (!pool.mKillFlag.load())
            {
                std::unique_lock<std::mutex> lock(waitMutex);
                pool.mCV.wait(lock,
                    [&]() {
                        std::unique_lock<std::mutex> lock2(pool.mQueueMutex);
                        return pool.mKillFlag.load() || !pool.mTaskQueue.empty();
                    });

                {
                    std::unique_lock<std::mutex> lock2(pool.mQueueMutex);
                    for (auto& func : pool.mTaskQueue)
                    {
                        func();
                    }
                    pool.mTaskQueue.clear();
                }
            }
        }
        
    private:
        std::atomic_bool mKillFlag;
        std::condition_variable mCV;
        mutable std::mutex mQueueMutex;
        std::deque<std::function<void()>> mTaskQueue;
        std::vector<std::shared_ptr<std::thread>> mThreads;
    };
}