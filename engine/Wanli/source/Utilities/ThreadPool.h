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
#include <queue>
#include "Core/BasicTypes.h"

namespace Wanli
{
    class DLLDECL ThreadPool final
    {
    public:
        explicit ThreadPool(int threads = std::thread::hardware_concurrency());
        ~ThreadPool();

        template<typename _Func_, typename... _Args_>
        decltype(auto) Enqueue(_Func_&& func, _Args_&&... args);

        void Wait();

    private:
        std::vector<std::thread> mThreads;
        std::queue<std::function<void()>> mTasks;

        std::mutex mMutex;
        std::condition_variable mCV;
        std::atomic_bool mQuit = false;
    };

    template<typename _Func_, typename... _Args_>
    decltype(auto) ThreadPool::Enqueue(_Func_&& func, _Args_&&... args)
    {
        using ReturnType = typename std::result_of<_Func_(_Args_...)>::type;
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<_Func_>(func),
            std::forward<_Args_>(args)...));
        auto result = task->get_future();

        {
            std::unique_lock<std::mutex> lock(mMutex);
            if (mQuit.load())
            {
                throw std::runtime_error("!!! Enqueue called on a stopped ThreadPool !!!\n");
            }
            mTasks.emplace([task] () {
                (*task)();
            });
        }
        mCV.notify_one();
        return result;
    }
}