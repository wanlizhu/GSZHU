#include "Utilities/ThreadPool.h"

namespace Wanli
{
    ThreadPool::ThreadPool(int threads)
    {
        mThreads.reserve(threads);

        for (int i = 0; i < threads; i++)
        {
            mThreads.emplace_back([this] () {
                while (true)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(mMutex);
                        mCV.wait(lock, [this] () {
                            return mQuit.load() || !mTasks.empty();
                        });

                        if (mQuit.load() && mTasks.empty())
                        {
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
        mQuit.store(true);
        mCV.notify_all();

        for (auto& thread : mThreads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

    void ThreadPool::Wait()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mCV.wait(lock, [this] () {
            return mTasks.empty();
        });
    }
}