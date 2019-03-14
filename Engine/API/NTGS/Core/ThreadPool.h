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

        inline size_t CountWorker() const noexcept { return mWorkers.size(); }
        inline size_t CountTask() const noexcept { return mTasks.size(); }
        inline void Stop() {
            {
                std::unique_lock<std::mutex> cLock(mQueueMutex);
                if (mStop)
                    return;
                else
                    mStop = true;
            }

            mCV.notify_all();
            for (std::thread& cWorker : mWorkers) {
                if (cWorker.joinable())
                    cWorker.join();
            }

            mWorkers.clear();
            while (!mTasks.empty())
                mTasks.pop();
        }

        // Call with std::ref(x), if you want to enqueue a function which takes argument x as a reference
        template<typename FuncType, typename... ArgsType>
        inline auto Enqueue(FuncType&& Func, ArgsType&&... Args)
            ->std::future<typename std::result_of<FuncType(ArgsType...)>::type> {

            using ReturnType = typename std::result_of<FuncType(ArgsType...)>::type;
            auto pTaskPointer = std::make_shared<std::packaged_task<ReturnType()>>(
                std::bind(std::forward<FuncType>(Func), std::forward<ArgsType>(Args)...));
            std::future<ReturnType> cFutureResult = pTaskPointer->get_future();
            
            std::unique_lock<std::mutex> cLock(mQueueMutex);
            if (mStop)
                throw std::runtime_error("Enqueue on stopped ThreadPool");
            mTasks.emplace([pTaskPointer]() { (*pTaskPointer)(); });
            cLock.unlock();

            mCV.notify_one();
            return cFutureResult;
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
                        std::function<void()> cTask;
                        std::unique_lock<std::mutex> cLock(this->mQueueMutex);

                        this->mCV.wait(cLock, [this]() {
                            return this->mStop || !this->mTasks.empty();
                        });
                        if (this->mStop && this->mTasks.empty())
                            return;
                        cTask = std::move(this->mTasks.front());
                        this->mTasks.pop();

                        cLock.unlock();

                        cTask();
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