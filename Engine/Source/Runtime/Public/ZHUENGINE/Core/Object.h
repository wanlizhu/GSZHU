#pragma once

#include <ZHUENGINE/Core/Config.h>
#include <ZHUENGINE/Core/NonCopyable.h>
#include <ZHUENGINE/Core/BasicTypes.h>
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
        using WeakPtr = std::weak_ptr<Object>;
        using UniquePtr = std::unique_ptr<Object>;

        explicit Object(const std::type_index& type, Object* parent = nullptr)
            : mType(type)
            , mParent(parent)
        {}

        inline std::string_view GetTypeName() const { return mType.name(); }
        inline size_t GetHashCode() const { return mType.hash_code(); }
        
        void AddRef(Object* owner);
        bool IsReferredBy(Object* owner);
        static int DiscordRef(Object* object, Object* owner);

        void Wait(MilliSeconds ms = MilliSeconds::zero()) noexcept;
        void Notify() noexcept;
        void NotifyAll() noexcept;

    protected:
        ~Object() = default;
        inline void UpdateType(const std::type_index& type) { mType = type; }

    private:
        Object* mParent = nullptr;
        std::type_index mType;
        std::condition_variable mCV;
        std::mutex mMutex;
        mutable std::unordered_map<Object*, std::atomic<int>> mOwnersMap;
    };
}