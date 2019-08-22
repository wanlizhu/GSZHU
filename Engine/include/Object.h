#pragma once

#include "Framework.h"
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <typeindex>
#include <mutex>
#include <functional>

namespace GS
{
    class GS_API Object : public std::enable_shared_from_this<Object>
    {
    public:
        using SharedPtr = std::shared_ptr<Object>;
        using UniquePtr = std::unique_ptr<Object>;
		
		virtual ~Object();

        bool SetObjectName(const std::string& name);
        const std::string& GetObjectName() const;

        void SetObjectProperty(const std::string& key, const std::string& value);
        bool GetObjectProperty(const std::string& key, std::string* value) const;

        static SharedPtr FindObject(const std::string& name);
        static void ForEachObject(const std::function<void(SharedPtr)>& func);
        
	protected:
		Object(const std::string& name);
		
    private:
        std::string mObjectName;
        std::unordered_map<std::string, std::string> mObjectProperties;

        static std::unordered_map<std::string, std::weak_ptr<Object>> smObjects;
    };
}