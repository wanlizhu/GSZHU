#include "Utilities/ThreadPool.h"

namespace Wanli
{
    void ThreadPool::Main()
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

    ThreadPool::ThreadPool(int numThreads)
    {
        for (int i = 0; i < numThreads; i++)
        {
            mThreads.push_back(std::make_shared<std::thread>(&ThreadPool::Main));
        }
    }

    ThreadPool& ThreadPool::Get()
    {
        static ThreadPool _instance(std::thread::hardware_concurrency());
        return _instance;
    }
}