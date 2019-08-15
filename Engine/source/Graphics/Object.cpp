#include "Graphics/RHI/Object.h"
#include <cassert>

namespace ZHU
{
    Object::Object(const std::string& name)
        : mObjectName(name)
    {
        assert(!weak_from_this().expired());
        static uint64_t _sUniqueID = -1;
        _sUniqueID++;

        if (mObjectName == "") {
            mObjectName = "Object" + std::to_string(_sUniqueID);
        }
    }

    Object::~Object()
    {
        auto it = smObjects.find(mObjectName);
        if (it != smObjects.end()) {
            smObjects.erase(it);
        }
    }

    bool Object::SetObjectName(const std::string& name)
    {
        auto it = smObjects.find(name);
        if (it != smObjects.end()) {
            return false;
        }

        smObjects.erase(mObjectName);
        mObjectName = name;
        smObjects[mObjectName] = weak_from_this();

        return true;
    }

    const std::string& Object::GetObjectName() const
    {
        return mObjectName;
    }

    void Object::LockObject() const
    {
#ifdef ZHU_MODE_THREAD_SAFE
        mObjectMutex.lock();
#endif
    }

    bool Object::TryLockObject(uint64_t ms) const
    {
#ifdef ZHU_MODE_THREAD_SAFE
        if (ms == 0)
            return mObjectMutex.try_lock();
        else
            return mObjectMutex.try_lock_for(
                std::chrono::duration<unsigned long long, std::milli>(ms));
#else
        return true;
#endif
    }

    void Object::UnlockObject() const
    {
#ifdef ZHU_MODE_THREAD_SAFE
        mObjectMutex.unlock();
#endif
    }

    void Object::SetObjectProperty(const std::string& key, const std::string& value)
    {
        mObjectProperties[key] = value;
    }

    bool Object::GetObjectProperty(const std::string& key, std::string* value) const
    {
        auto it = mObjectProperties.find(key);
        if (it == mObjectProperties.end()) {
            return false;
        }

        *value = it->second;
        return true;
    }

    std::shared_ptr<Object> Object::FindObject(const std::string& name)
    {
        auto it = smObjects.find(name);
        if (it != smObjects.end()) {
            auto shared = it->second.lock();
            if (shared) {
                return shared;
            }
            smObjects.erase(it);
        }

        return std::shared_ptr<Object>(nullptr);
    }

    size_t Object::GetObjectCount()
    {
        return smObjects.size();
    }

    std::weak_ptr<Object> Object::GetObject(size_t index)
    {
        auto it = smObjects.begin();
        while (index--) {
            it++;
        }

        return it == smObjects.end() ? std::weak_ptr<Object>() : it->second;
    }
}