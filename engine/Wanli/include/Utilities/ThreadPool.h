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

namespace Wanli
{
    class ThreadPool final
    {
    private:
        static void Main()
        {
            ThreadPool& pool = ThreadPool::Get();
            std::mutex waitMutex;
            while (!pool.mKillFlag.load())
            {
                std::unique_lock<std::mutex> lock(waitMutex);
                pool.mCV.wait(lock,
                              [&] () {
                                  std::unique_lock<std::mutex> lock2(pool.mQueueMutex);
                                  return pool.mKillFlag.load() || !pool.mTaskQueue.empty();
                              });

                {
                    std::unique_lock<std::mutex> lock2(pool.mQueueMutex);
                    for (auto& func : pool.mTaskQueue) {
                        func();
                    }
                    pool.mTaskQueue.clear();
                }
            }
        }

    public:
        static ThreadPool& Get()
        {
            static ThreadPool _inst;
            return _inst;
        }

        inline void Initialize(int numThreads)
        {
            Destroy();
            for (int i = 0; i < numThreads; i++) {
                mThreads.push_back(std::make_shared<std::thread>(&ThreadPool::Main));
            }
        }

        inline void Destroy()
        {
            mKillFlag.store(true);
            for (auto& thread : mThreads) {
                if (thread->joinable()) {
                    thread->join();
                }
            }
            mThreads.clear();
        }

        template<typename _Func_, typename... _Args_>
        inline auto AddTask(_Func_&& func,
                            _Args_&&... args)
            -> std::future<std::invoke_result_t<_Func_, _Args_...>>
        {
            using ReturnType = std::invoke_result_t<_Func_, _Args_...>;
            auto packagedTask = std::make_shared<std::packaged_task<ReturnType()>>(
                std::bind(std::forward<_Func_>(func), std::forward<_Args_>(args)...)
                );
            std::future<ReturnType> future = packagedTask->get_future();
            mTaskQueue.emplace_back([packagedTask] () { (*packagedTask)(); });
            return future;
        }

    private:
        std::atomic_bool mKillFlag;
        std::condition_variable mCV;
        mutable std::mutex mQueueMutex;
        std::deque<std::function<void()>> mTaskQueue;
        std::vector<std::shared_ptr<std::thread>> mThreads;
    };
}