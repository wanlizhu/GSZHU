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
#include "Lockable.h"

namespace Wanlix
{
    class ThreadPool final
    {
    private:
        static void Main();

    public:
        static void Initialize(int numThreads);
        static void Destroy();

        template<typename _Func_, typename... _Args_>
        static auto AddTask(_Func_&& func,
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
        static std::atomic_bool mKillFlag;
        static std::condition_variable mCV;
        static Lockable<std::deque<std::function<void()>>> mTaskQueue;
        static std::vector<std::shared_ptr<std::thread>> mThreads;
    };
}