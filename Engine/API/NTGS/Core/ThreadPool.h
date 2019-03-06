#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace NTGS {
    class ThreadPool {
    public:
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator=(ThreadPool&&) = delete;

        inline static ThreadPool* GetInstance() noexcept {
            static ThreadPool smGlobalInstance(std::thread::hardware_concurrency());
            return &smGlobalInstance;
        }

        inline size_t WorkerCount() const noexcept { return mWorkers.size(); }
        inline size_t TaskCount() const noexcept { return mTasks.size(); }
        inline void Stop() {
            {
                std::unique_lock<std::mutex> Lock(mQueueMutex);
                if (mStop)
                    return;
                else
                    mStop = true;
            }

            mCV.notify_all();
            for (std::thread& Worker : mWorkers) {
                if (Worker.joinable())
                    Worker.join();
            }

            mWorkers.clear();
            while (!mTasks.empty())
                mTasks.pop();
        }

        template<typename FuncType, typename... ArgsType>
        inline auto Enqueue(FuncType&& Func, ArgsType&&... Args)
            ->std::future<typename std::result_of<FuncType(ArgsType...)>::type> {

            using ReturnType = typename std::result_of<FuncType(ArgsType...)>::type;
            auto TaskPointer = std::make_shared<std::packaged_task<ReturnType()>>(
                std::bind(std::forward<FuncType>(Func), std::forward<ArgsType>(Args)...));
            std::future<ReturnType> FutureResult = TaskPointer->get_future();
            
            std::unique_lock<std::mutex> Lock(mQueueMutex);
            if (mStop)
                throw std::runtime_error("Enqueue on stopped ThreadPool");
            mTasks.emplace([TaskPointer]() { (*TaskPointer)(); });
            Lock.unlock();

            mCV.notify_one();
            return FutureResult;
        }

        inline virtual ~ThreadPool() {
            Stop();
        }

    private:
        inline ThreadPool(size_t Count) noexcept
            : mStop(false) {
            Initialize(Count);
        }

        inline void Initialize(size_t Count) {
            mStop = false;
            for (size_t i = 0; i < Count; i++) {
                mWorkers.emplace_back([this]() {
                    while (true) {
                        std::function<void()> Task;
                        std::unique_lock<std::mutex> Lock(this->mQueueMutex);

                        this->mCV.wait(Lock, [this]() {
                            return this->mStop || !this->mTasks.empty();
                        });
                        if (this->mStop && this->mTasks.empty())
                            return;
                        Task = std::move(this->mTasks.front());
                        this->mTasks.pop();

                        Lock.unlock();

                        Task();
                    }
                });
            }
        }

    private:
        std::vector<std::thread> mWorkers;
        std::queue<std::function<void()>> mTasks;
        std::mutex mQueueMutex;
        std::condition_variable mCV;
        std::atomic<bool> mStop = false;
    };


}