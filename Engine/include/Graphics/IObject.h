#pragma once

#include "Common/Config.h"
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <typeindex>
#include <mutex>
#include <functional>

namespace ZHU
{
    class ZHU_API IObject : public std::enable_shared_from_this<IObject>
    {
    public:
        IObject(const std::string& name = "");
        virtual ~IObject();

        bool SetObjectName(const std::string& name);
        const std::string& GetObjectName() const;

        void LockObject() const;
        bool TryLockObject(uint64_t ms = 0) const;
        void UnlockObject() const;

        void SetObjectProperty(const std::string& key, const std::string& value);
        bool GetObjectProperty(const std::string& key, std::string* value) const;

        void SetObjectParent(const std::weak_ptr<IObject>& parent);
        std::weak_ptr<IObject> GetObjectParent() const;

        void   AddObjectChild(const std::weak_ptr<IObject>& child);
        size_t ObjectChildCount() const;
        std::weak_ptr<IObject>& ObjectChildAt(size_t index);
        const std::weak_ptr<IObject>& ObjectChildAt(size_t index) const;

        static std::weak_ptr<IObject> FindObject(const std::string& name);
        static void TraverseObjects(std::function<void(std::weak_ptr<IObject>)> func);

    protected:
        void SetObjectTypeIndex(const std::type_index& index);
        
    protected:
        std::string mObjectName;
        std::type_index mObjectTypeIndex;
        mutable std::recursive_timed_mutex mObjectMutex;
        std::unordered_map<std::string, std::string> mObjectProperties;

        std::weak_ptr<IObject> mObjectParent;
        std::vector<std::weak_ptr<IObject>> mObjectChildren;

    private:
        static std::unordered_map<std::string, std::weak_ptr<IObject>> smObjects;
    };
}