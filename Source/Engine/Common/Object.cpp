#include "ZHUENGINE_PCH.h"
#include <ZHUENGINE/Common/Object.h>

namespace ZHU
{
    void Object::AddRef(Object* owner)
    {   
        if (mOwnersMap.find(owner) == mOwnersMap.end()) {
            mOwnersMap.emplace(std::pair<Object*, int>(owner, 0));
        }
        mOwnersMap[owner] += 1;
    }

    bool Object::IsReferredBy(Object* owner)
    {
        auto it = mOwnersMap.find(owner);
        if (it == mOwnersMap.end())
            return false;
        return it->second.load() > 0;
    }

    int Object::DiscordRef(Object* object, Object* owner)
    {
        auto it = object->mOwnersMap.find(owner);
        if (it == object->mOwnersMap.end())
            return 0;

        it->second -= 1;
        if (it->second.load() <= 0) {
            object->mOwnersMap.erase(it);
            return 0;
        }
        return it->second.load();
    }

    void Object::Wait(MilliSeconds ms) noexcept
    {
        std::unique_lock<std::mutex> lock(mMutex);
        if (ms == MilliSeconds::zero()) {
            mCV.wait(lock);
        }
        else {
            mCV.wait_for(lock, ms);
        }
    }

    void Object::Notify() noexcept
    {
        mCV.notify_one();
    }

    void Object::NotifyAll() noexcept
    {
        mCV.notify_all();
    }
}