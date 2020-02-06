#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <optional>
#include "BasicTypes.h"

namespace Wanli
{
    struct CommandLineArgsLayout
    {
        HashMap<String, int> argsNumMap;
        StringArray requiredArgs;
    };

    class DLLDECL CommandLineArgs
    {
    public:
        CommandLineArgs() = default;
        CommandLineArgs(int argc, char** argv, const CommandLineArgsLayout& layout = {});

        bool HasArg(const String& name) const;
        bool IsArgFlag(const String& name) const;
        bool IsArgArray(const String& name) const;

        bool GetArgFlag(const String& name) const;
        const String& GetArg(const String& name) const;
        const StringArray& GetArgArray(const String& name) const;
        const StringArray& GetUnnamedArgs() const;

    private:
        int argc = 0;
        char** argv = nullptr;
        HashMap<String, StringArray> mNamedArgs;
        StringArray mUnnamedArgs;
    };
}