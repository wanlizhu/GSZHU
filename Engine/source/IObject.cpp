#include "Graphics/IObject.h"
#include <cassert>

namespace ZHU
{
    IObject::IObject(const std::string& name)
        : mObjectName(name)
        , mObjectTypeIndex(typeid(IObject))
    {
        assert(!weak_from_this().expired());
        static uint64_t _sUniqueID = -1;
        _sUniqueID++;

        if (mObjectName == "") {
            mObjectName = std::string(mObjectTypeIndex.name())
                + " (" + std::to_string(_sUniqueID) + ")";
        }
    }

    IObject::~IObject()
    {
        LockObject();
        auto it = smObjects.find(mObjectName);
        if (it != smObjects.end()) {
            smObjects.erase(it);
        }
        UnlockObject();
    }

    bool IObject::SetObjectName(const std::string& name)
    {
        LockObject();
        auto it = smObjects.find(name);
        if (it != smObjects.end()) {
            UnlockObject();
            return false;
        }

        smObjects.erase(mObjectName);
        mObjectName = name;
        smObjects[mObjectName] = weak_from_this();

        UnlockObject();
        return true;
    }

    const std::string& IObject::GetObjectName() const
    {
        return mObjectName;
    }

    void IObject::LockObject() const
    {
#ifdef ZHU_MODE_THREAD_SAFE
        mObjectMutex.lock();
#endif
    }

    bool IObject::TryLockObject(uint64_t ms) const
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

    void IObject::UnlockObject() const
    {
#ifdef ZHU_MODE_THREAD_SAFE
        mObjectMutex.unlock();
#endif
    }

    void IObject::SetObjectProperty(const std::string& key, const std::string& value)
    {
        LockObject();
        mObjectProperties[key] = value;
        UnlockObject();
    }

    bool IObject::GetObjectProperty(const std::string& key, std::string* value) const
    {
        LockObject();
        auto it = mObjectProperties.find(key);
        if (it == mObjectProperties.end()) {
            UnlockObject();
            return false;
        }

        *value = it->second;
        UnlockObject();
        return true;
    }

    void IObject::SetObjectParent(const std::weak_ptr<IObject>& parent)
    {
        LockObject();
        mObjectParent = parent;
        UnlockObject();
    }

    std::weak_ptr<IObject> IObject::GetObjectParent() const
    {
        return mObjectParent;
    }

    void IObject::AddObjectChild(const std::weak_ptr<IObject>& child)
    {
        LockObject();
        mObjectChildren.push_back(child);
        UnlockObject();
    }

    size_t IObject::ObjectChildCount() const
    {
        return mObjectChildren.size();
    }

    std::weak_ptr<IObject>& IObject::ObjectChildAt(size_t index)
    {
        return mObjectChildren[index];
    }

    const std::weak_ptr<IObject>& IObject::ObjectChildAt(size_t index) const
    {
        return mObjectChildren[index];
    }

    std::weak_ptr<IObject> IObject::FindObject(const std::string& name)
    {
        auto it = smObjects.find(name);
        if (it == smObjects.end())
            return std::weak_ptr<IObject>();
        else
            return it->second;
    }

    void IObject::TraverseObjects(std::function<void(std::weak_ptr<IObject>)> func)
    {
        for (auto it = smObjects.begin(); it != smObjects.end(); it++) {
            func(it->second);
        }
    }

    void IObject::SetObjectTypeIndex(const std::type_index& index)
    {
        mObjectTypeIndex = index;
    }
}