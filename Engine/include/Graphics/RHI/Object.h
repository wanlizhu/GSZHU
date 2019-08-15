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
    class ZHU_API Object : public std::enable_shared_from_this<Object>
    {
    public:
        using Pointer = std::shared_ptr<Object>;

        Object(const std::string& name);
        virtual ~Object();

        bool SetObjectName(const std::string& name);
        const std::string& GetObjectName() const;

        void LockObject() const;
        bool TryLockObject(uint64_t ms = 0) const;
        void UnlockObject() const;

        void SetObjectProperty(const std::string& key, const std::string& value);
        bool GetObjectProperty(const std::string& key, std::string* value) const;

        std::weak_ptr<Object> GetObjectParent() const;
        size_t FindObjectChild(const std::string& name) const;
        size_t GetObjectChildCount() const;
        std::weak_ptr<Object> GetObjectChild(int index);

        static std::shared_ptr<Object> FindObject(const std::string& name);
        static size_t GetObjectCount();
        static std::weak_ptr<Object> GetObject(size_t index);

    protected:
        void SetObjectParent(std::weak_ptr<Object> parent);
        void SetObjectTypeIndex(const std::type_index& index);
        void AddObjectChild(std::weak_ptr<Object> child);
        
    protected:
        std::string mObjectName;
        std::type_index mObjectTypeIndex;
        std::unordered_map<std::string, std::string> mObjectProperties;

        std::weak_ptr<Object> mObjectParent;
        std::vector<std::weak_ptr<Object>> mObjectChildren;

        mutable std::recursive_timed_mutex mObjectMutex;
    private:
        static std::unordered_map<std::string, std::weak_ptr<Object>> smObjects;
    };
}