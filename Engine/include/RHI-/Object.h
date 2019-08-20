#pragma once

#include "Graphics/Config.h"
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
    class ZHU_GS_API Object : public std::enable_shared_from_this<Object>
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

        static std::shared_ptr<Object> FindObject(const std::string& name);
        static size_t GetObjectCount();
        static std::weak_ptr<Object> GetObject(size_t index);
        
    protected:
        std::string mObjectName;
        std::unordered_map<std::string, std::string> mObjectProperties;
        mutable std::recursive_timed_mutex mObjectMutex;

    private:
        static std::unordered_map<std::string, std::weak_ptr<Object>> smObjects;
    };
}