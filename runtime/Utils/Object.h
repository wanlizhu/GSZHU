#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <any>
#include <set>
#include <optional>
#include <algorithm>
#include <regex>
#include "Utils/NonCopyable.h"

namespace Wanlix
{
    class Object : public std::enable_shared_from_this<Object>
                 , public NonCopyable
    {
    public:
        inline Object(const std::string& name, Object* parent = nullptr) noexcept
            : mObjectName(name)
            , mParent(parent)
        {}
        inline Object(Object&& obj) noexcept
            : mObjectName(std::move(obj.mObjectName))
            , mParent(obj.mParent)
        {}
        inline Object& operator=(Object&& obj) noexcept
        {
            mObjectName = std::move(obj.mObjectName);
            mParent = obj.mParent;
            mProperties = std::move(obj.mProperties);
            obj.mParent = nullptr;
        }
        virtual ~Object()
        {}

        inline void SetObjectName(const std::string& name) noexcept {
            mObjectName = name;
        }
        inline const std::string& GetObjectName() const noexcept {
            return mObjectName;
        }
        inline void SetParent(Object* parent) noexcept { 
            mParent = parent;
            parent->mChildren.insert(this);
        }
        inline Object* GetParent() const noexcept {
            return mParent; 
        }
        inline void SetProperty(const std::string& name, const std::any& value) noexcept {
            mProperties[name] = value;
        }
        inline std::optional<std::any> GetProperty(const std::string& name) const noexcept { 
            auto it = mProperties.find(name);
            return (it == mProperties.end() ? std::nullopt : std::make_optional(*it));
        }

        inline const std::set<Object*>& Children() const noexcept {
            return mChildren;
        }
        inline Object* FindChild(const std::string& name) const {
            auto it = std::find_if(mChildren.begin(), mChildren.end(),
                                   [&] (Object* child) {
                                       return child->GetObjectName() == name;
                                   });
            return (it == mChildren.end() ? nullptr : *it);
        }
        inline std::vector<Object*> FindChildren(const std::string& regexName) const {
            std::vector<Object*> res;
            std::regex reg(regexName);
            std::smatch match;

            for (auto& item : mChildren) {
                if (std::regex_match(item->GetObjectName(), match, reg)) {
                    res.push_back(item);
                }
            }

            return res;
        }

    private:
        std::string mObjectName;
        Object* mParent = nullptr;
        std::set<Object*> mChildren;
        std::unordered_map<std::string, std::any> mProperties;
    };
}