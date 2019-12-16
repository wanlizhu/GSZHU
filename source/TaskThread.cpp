#include "Wanlix/TaskThread.h"

namespace Wanlix
{
    void TaskThread::Main(TaskThread* self)
    {
        std::mutex mutex;
        while (!self->mKillFlag.load()) 
        {
            std::unique_lock<std::mutex> lock(mutex);
            self->mFreeFlag.store(true);
            self->mCV.wait(lock, 
                           [self] () {
                               return self->HasTask() || self->IsKilled();
                           });
            self->mFreeFlag.store(false);

            {
                std::unique_lock<std::mutex> lockData(self->mTaskQueue.GetMutex());
                for (auto& func : self->mTaskQueue.GetData()) {
                    func();
                }
                self->mTaskQueue.GetData().clear();
            }
        }
    }

    TaskThread::TaskThread() noexcept
        : std::thread(std::bind(&TaskThread::Main, this))
    {}

    bool TaskThread::AddTask(std::function<void()>&& func, Tag tag)
    {
        std::unique_lock<std::mutex> lockData(mTaskQueue.GetMutex());
        if (tag == TagPushFront) {
            mTaskQueue.GetData().emplace_front(std::move(func));
        }
        else if (tag == TagPushBack){
            mTaskQueue.GetData().emplace_back(std::move(func));
        }
        else {
            return false;
        }
        return true;
    }

    void TaskThread::Kill()
    {
        mKillFlag.store(true);
        mCV.notify_all();
    }

    void TaskThread::Clear()
    {
        std::unique_lock<std::mutex> lockData(mTaskQueue.GetMutex());
        mTaskQueue.GetData().clear();
    }

    bool TaskThread::IsFree() const
    {
        return mFreeFlag.load();
    }

    bool TaskThread::IsKilled() const
    {
        return mKillFlag.load();
    }

    bool TaskThread::HasTask() const
    {
        std::unique_lock<std::mutex> lockData(mTaskQueue.GetMutex());
        return !mTaskQueue.GetData().empty();
    }
}