#include "ZHUENGINE_PCH.h"
#include "Core/ThreadPool.h"

namespace ZHU
{
    ThreadPool::ThreadPool(int threads)
    {
        mWorkers.reserve(threads);

        for (int i = 0; i < threads; i++) {
            mWorkers.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock{ mMutex };
                        mCV.wait(lock, [this]() {
                            return mQuit || !mTasks.empty();
                        });

                        if (mQuit && mTasks.empty()) {
                            return;
                        }

                        task = std::move(mTasks.front());
                        mTasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ThreadPool::~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock{ mMutex };
            mQuit = true;
        }

        mCV.notify_all();

        for (auto& worker : mWorkers) {
            worker.join();
        }
    }

    void ThreadPool::Wait()
    {
        std::unique_lock<std::mutex> lock{ mMutex };

        mCV.wait(lock, [this]() {
            return mTasks.empty();
        });
    }
}