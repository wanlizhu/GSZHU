#include "Wanlix/Utils/ThreadPool.h"

namespace Wanlix
{
    std::atomic_bool ThreadPool::mKillFlag;
    std::condition_variable ThreadPool::mCV;
    Lockable<std::deque<std::function<void()>>> ThreadPool::mTaskQueue;
    std::vector<std::shared_ptr<std::thread>> ThreadPool::mThreads;

    void ThreadPool::Initialize(int numThreads)
    {
        Destroy();
        for (int i = 0; i < numThreads; i++) {
            mThreads.push_back(std::make_shared<std::thread>(&ThreadPool::Main));
        }
    }

    void ThreadPool::Destroy()
    {
        mKillFlag.store(true);
        for (auto& thread : mThreads) {
            if (thread->joinable()) {
                thread->join();
            }
        }
        mThreads.clear();
    }

    void ThreadPool::Main()
    {
        std::mutex mutex;
        while (!mKillFlag.load())
        {
            std::unique_lock<std::mutex> lock(mutex);
            mCV.wait(lock,
                     [] () {
                         std::unique_lock<std::mutex> lockData(mTaskQueue.GetMutex());
                         return mKillFlag.load() || !mTaskQueue.GetData().empty();
                     });

            {
                std::unique_lock<std::mutex> lockData(mTaskQueue.GetMutex());
                for (auto& func : mTaskQueue.GetData()) {
                    func();
                }
                mTaskQueue.GetData().clear();
            }
        }
    }
}