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
#define GS_OBJECT 




    class GS_API Object : public std::enable_shared_from_this<Object>
    {
		GS_OBJECT
    public:
        using SharedPtr = std::shared_ptr<Object>;
        using UniquePtr = std::unique_ptr<Object>;
		
		virtual ~Object();

        bool SetObjectName(const std::wstring& name);
        const std::wstring& GetObjectName() const;

        void SetObjectProperty(const std::wstring& key, const std::wstring& value);
        bool GetObjectProperty(const std::wstring& key, std::wstring* value) const;

        static SharedPtr FindObject(const std::wstring& name);
        static void ForEachObject(const std::function<void(SharedPtr)>& func);
        
	protected:
		Object(const std::wstring& name);
		
    private:
        std::wstring mObjectName;
        std::unordered_map<std::wstring, std::wstring> mObjectProperties;

        static std::unordered_map<std::wstring, std::weak_ptr<Object>> smObjects;
    };




	template<typename T, typename... ARGS>
	std::enable_if_t<std::is_base_of_v<GS::Object, T> ||
		             std::is_same_v<GS::Object, T>,
		             typename T::SharedPtr>
	NewObject(ARGS... args)
	{
		return T::Create(std::forward<ARGS>(args)...);
	}
}