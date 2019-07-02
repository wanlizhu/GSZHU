#pragma once
#include "Config.h"
#include <thread>
#include <queue>
#include <future>
#include <mutex>
#include <functional>
#include <condition_variable>

namespace ZHU
{
    class ZHU_API ThreadPool final
    {
    public:
        explicit ThreadPool(int threads = std::thread::hardware_concurrency());
        ~ThreadPool();

        template<typename F, typename... ARGS>
        decltype(auto) Commit(F&& func, ARGS&& ... args);

        void Wait();
        inline const std::vector<std::thread>& GetWorkers() const { return mWorkers; }

    private:
        std::vector<std::thread> mWorkers;
        std::queue<std::function<void()>> mTasks;

        std::mutex mMutex;
        std::condition_variable mCV;
        bool mQuit = false;
    };




    template<typename F, typename... ARGS>
    decltype(auto) ThreadPool::Commit(F&& func, ARGS&& ... args) 
    {
        using return_type = typename std::result_of<F(ARGS...)>::type;
        auto task{ std::make_shared<std::packaged_task<return_type()>>(
                                    std::bind(std::forward<F>(func), std::forward<ARGS>(args)...)) };
        auto result{ task->get_future() };

        {
            std::unique_lock<std::mutex> lock{ mMutex };
            if (mQuit) {
                throw std::runtime_error("Commit() called on a stopped ThreadPool");
            }

            mTasks.emplace([task]() {
                (*task)();
            });
        }

        mCV.notify_one();
        return result;
    }
}