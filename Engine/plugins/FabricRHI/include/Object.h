#pragma once

#include "Config.h"
#include <memory>
#include <string>
#include <optional>
#include <unordered_map>

namespace ZHU
{
    class Object : public std::enable_shared_from_this<Object>
    {
	public:
		using SharedPtr = std::shared_ptr<Object>;
		using SharedConstPtr = std::shared_ptr<const Object>;

		static SharedPtr QueryObject(const std::string& name);

		Object(const std::string& name);
		virtual ~Object();

		void SetObjectName(const std::string& name);
		void SetObjectProperty(const std::string& key, const std::string& value);

		        const std::string& GetObjectName() const;
		std::optional<std::string> GetObjectProperty(const std::string& key) const;

	private:
		std::string mName;
		std::unordered_map<std::string, std::string> mProperties;
		static std::unordered_map<std::string, std::weak_ptr<Object>> smObjects;
    };
}