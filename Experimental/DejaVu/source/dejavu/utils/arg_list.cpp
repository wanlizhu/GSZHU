#include "arg_list.h"
#include "strings.h"

namespace djv
{
    ArgList::ArgList(int argc, char** argv)
    {
        std::string cmdline = argv[0];
        for (int i = 1; i < argc; i++) {
            cmdline += std::string(" ") + argv[i];
        }
        parse(cmdline);
    }

    ArgList::ArgList(const std::string& cmdline)
    {
        parse(cmdline);
    }
    
    void ArgList::parse(const std::string& cmdline)
    {
        auto list = split(cmdline, " ", true);
        std::string key = "";
        int pos = 0;

        for (int i = 0; i < (int)list.size(); i++) {
            if (starts_with(list[i], "-")) {
                key = list[i].substr(list[i].find_first_not_of("-"));
                mArgMap[key] = "";
            } else {
                if (!key.empty()) {
                    mArgMap[key] += mArgMap[key].empty() ? list[i] : (std::string(" ") + list[i]);
                } else {
                    mArgMap[std::to_string(pos++)] = list[i];
                }
            }
        }
    }

    bool ArgList::hasArg(const std::string& key) const
    {
        return mArgMap.find(key) != mArgMap.end();
    }

    void ArgList::setArg(const std::string& key, const std::optional<std::string>& value)
    {
        mArgMap[key] = value.has_value() ? value.value() : "";
    }

    void ArgList::appendArg(const std::string& key, const std::string& value)
    {
        mArgMap[key] = hasArg(key) ? mArgMap[key] + " " + value : value;
    }

    std::optional<std::string> ArgList::getArg(const std::string& key) const
    {
        return hasArg(key) ? std::make_optional(mArgMap.at(key)) : std::nullopt;
    }

    const std::string& ArgList::operator[](const std::string& key) const
    {
        return mArgMap.at(key);
    }
}