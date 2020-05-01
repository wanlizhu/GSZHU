#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <optional>
#include "utils/preprocess.h"

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
}