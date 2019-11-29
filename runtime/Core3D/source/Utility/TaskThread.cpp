#include "Wanlix/Core3D/Utility/TaskThread.h"
#include "Wanlix/Core3D/Types.h"

namespace Wanlix
{
    void TaskThread::Main(TaskThread* self)
    {
        std::mutex mMutex;
        while (!self->mQuitFlag.load())
        {
            std::unique_lock<std::mutex> lock(mMutex);
            self->mBlocked.store(true);
            self->mCondition.wait(lock,
                                  [self]() {
                                      return self->ShouldAwake();
                                  });
            self->mBlocked.store(false);
            {
                std::unique_lock<std::mutex> lockTaskList(self->mTaskList.GetMutex());
                for (auto& function : self->mTaskList.GetData()) {
                    function();
                }
                self->mTaskList.GetData().clear();
            }
        }
    }

    TaskThread::TaskThread() noexcept
        : std::thread(std::bind(&TaskThread::Main, this))
    {}

    void TaskThread::SetName(StringCRef name) {
        mName = name;
    }

    StringCRef TaskThread::GetName() const {
        return mName;
    }

    void TaskThread::PostTask(std::function<void()>&& func) {
        {
            std::unique_lock<std::mutex> lock(mTaskList.GetMutex());
            mTaskList.GetData().emplace_back(std::move(func));
        }
        mCondition.notify_one();
    }

    void TaskThread::Quit() {
        mQuitFlag.store(true);
        mCondition.notify_one();
    }

    bool TaskThread::ShouldAwake() const {
        return mQuitFlag.load() || !mTaskList.GetData().empty();
    }

    bool TaskThread::IsBlocked() const {
        return mBlocked.load();
    }
}