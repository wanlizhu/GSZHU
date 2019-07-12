#pragma once

#include <Common/Config.h>
#include <Common/Utilities/Time.h>
#include <Common/Utilities/NonCopyable.h>
#include <Common/Utilities/BasicTypes.h>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <typeindex>
#include <unordered_map>

namespace ZHU
{
    class ZHU_API Object
    {
    public:
        using SharedPtr = std::shared_ptr<Object>;
        using WeakPtr   = std::weak_ptr<Object>;
        using UniquePtr = std::unique_ptr<Object>;

        void AddRef(Object* owner);
        bool IsReferredBy(Object* owner);
        int  DiscordRef(Object* owner);

        void Wait(Time ms = Time::Milliseconds(0)) noexcept;
        void Notify() noexcept;
        void NotifyAll() noexcept;

    protected:
        ~Object() = default;

    private:
        std::condition_variable mCV;
        std::mutex mMutex;
        mutable std::unordered_map<Object*, std::atomic<int>> mOwnersMap;
    };
}