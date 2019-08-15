#include "Graphics/RHI/Object.h"
#include <cassert>

namespace ZHU
{
    Object::Object(const std::string& name)
        : mObjectName(name)
        , mObjectTypeIndex(typeid(Object))
    {
        assert(!weak_from_this().expired());
        static uint64_t _sUniqueID = -1;
        _sUniqueID++;

        if (mObjectName == "") {
            mObjectName = std::string(mObjectTypeIndex.name())
                + " (" + std::to_string(_sUniqueID) + ")";
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

    void Object::SetObjectParent(std::weak_ptr<Object> parent)
    {
        mObjectParent = parent;
    }

    std::weak_ptr<Object> Object::GetObjectParent() const
    {
        return mObjectParent;
    }

    void Object::AddObjectChild(std::weak_ptr<Object> child)
    {
        mObjectChildren.push_back(child);
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

    size_t Object::FindObjectChild(const std::string& name) const
    {
        LockObject();
        auto it = std::find_if(mObjectChildren.begin(), mObjectChildren.end(), 
                               [&](std::weak_ptr<Object> child) {
                                   auto shared = child.lock();
                                   if (shared) { 
                                       return shared->GetObjectName() == name;
                                   }
                                   return false;
                               });
        int index = it == mObjectChildren.end() ? -1 : 0;
        UnlockObject();
        return index;
    }

    size_t Object::GetObjectChildCount() const
    {
        return mObjectChildren.size();
    }

    std::weak_ptr<Object> Object::GetObjectChild(int index)
    {
        return mObjectChildren[index];
    }

    void Object::SetObjectTypeIndex(const std::type_index& index)
    {
        mObjectTypeIndex = index;
    }
}