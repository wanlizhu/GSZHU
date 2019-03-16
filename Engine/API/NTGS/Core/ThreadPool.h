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

        inline size_t CountWorker() const noexcept { return mWorkerList.size(); }
        inline size_t CountTask() const noexcept { return mTaskQueue.size(); }
        inline void Stop() {
            std::unique_lock<std::mutex> UniqueLock(mQueueMutex);
            if (mStopFlag)
                return;
            else
                mStopFlag = true;
            UniqueLock.unlock();

            mCV.notify_all();
            for (std::thread& CurrentWorker : mWorkerList) {
                if (CurrentWorker.joinable())
                    CurrentWorker.join();
            }

            mWorkerList.clear();
            while (!mTaskQueue.empty())
                mTaskQueue.pop();
        }

        // Call with std::ref(x), if you want to enqueue a function which takes argument x as a reference
        template<typename FuncType, typename... ArgsType>
        inline auto Enqueue(FuncType&& Func, ArgsType&&... Args)
            ->std::future<typename std::result_of<FuncType(ArgsType...)>::type> {
            using ReturnType = typename std::result_of<FuncType(ArgsType...)>::type;

            auto TaskPointer = std::make_shared<std::packaged_task<ReturnType()>>(
                std::bind(std::forward<FuncType>(Func), std::forward<ArgsType>(Args)...));
            auto TaskFuture = TaskPointer->get_future();
            
            std::unique_lock<std::mutex> UniqueLock(mQueueMutex);
            if (mStopFlag)
                throw std::runtime_error("Enqueue on stopped ThreadPool");

            mTaskQueue.emplace([TaskPointer]() { (*TaskPointer)(); });
            UniqueLock.unlock();

            mCV.notify_one();
            return TaskFuture;
        }

        inline virtual ~ThreadPool() {
            Stop();
        }

    private:
        inline ThreadPool(size_t Count) noexcept
            : mStopFlag(false) {
            Initialize(Count);
        }

        inline void Initialize(size_t Count) {
            mStopFlag = false;
            for (size_t i = 0; i < Count; i++) {
                mWorkerList.emplace_back([this]() {
                    while (true) {
                        std::function<void()> TaskFunc;
                        std::unique_lock<std::mutex> UniqueLock(this->mQueueMutex);

                        this->mCV.wait(UniqueLock, [this]() {
                            return this->mStopFlag || !this->mTaskQueue.empty();
                        });
                        if (this->mStopFlag && this->mTaskQueue.empty())
                            return;
                        TaskFunc = std::move(this->mTaskQueue.front());
                        this->mTaskQueue.pop();

                        UniqueLock.unlock();

                        TaskFunc();
                    }
                });
            }
        }

    private:
        std::vector<std::thread> mWorkerList;
        std::queue<std::function<void()>> mTaskQueue;
        std::mutex mQueueMutex;
        std::condition_variable mCV;
        std::atomic<bool> mStopFlag = false;
    };


}