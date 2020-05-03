#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <optional>
#include "utils/preprocess.h"
#include "utils/strings.h"

namespace djv
{
    class DJV_API ArgList
    {
    public:
        ArgList() = default;
        ArgList(int argc, char** argv);
        ArgList(const std::string& cmdline);

        void parse(const std::string& cmdline);
        bool hasArg(const std::string& key) const;
        void setArg(const std::string& key, const std::optional<std::string>& value);
        void appendArg(const std::string& key, const std::string& value);
        std::optional<std::string> getArg(const std::string& key) const;
        const std::string& operator[](const std::string& key) const;

    private:
        std::unordered_map<std::string, std::string> mArgMap;
    };

    template<typename T>
    std::optional<T> getArgAs(const ArgList& list, const std::string& key)
    {
        auto optVal = list.getarg(key);
        if (!optVal.has_value()) {
            return std::nullopt;
        }

        std::string& value = optVal.value();
        if constexpr (std::is_integral_v<T>) 
        {
            int radix = istrcmp(value, 0, 2, "0x") == 0 ? 16 : 10;
            return static_cast<T>(strtoll(value.c_str(), nullptr, radix));
        } 
        else if constexpr (std::is_floating_point_v<T>) 
        {
            return static_cast<T>(strtod(value.c_str(), nullptr));
        } 
        else if constexpr (std::is_same_v<bool>) 
        {
            if (istrcmp(value, "true") || istrcmp(value, "on") || istrcmp(value, "yes") || istrcmp(value, "1")) {
                return true;
            }
            if (istrcmp(value, "false") || istrcmp(value, "off") || istrcmp(value, "no") || istrcmp(value, "0")) {
                return false;
            }
        }
        else if constexpr (std::is_same_v<std::string>) 
        {
            return value;
        }

        return std::nullopt;
    }
}