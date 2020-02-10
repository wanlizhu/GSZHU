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
    class DLLDECL CommandLineArgs
    {
    public:
        CommandLineArgs() = default;
        CommandLineArgs(int argc, char** argv);

        inline int GetArgCount() const { (int)mArgs.size(); }
        inline String operator[](int index) const { return mArgs[index]; }

        Optional<String> FindArg(const String& name) const;
        Optional<StringArray> FindArgArray(const String& name) const;
        
    private:
        StringArray mArgs;
    };
}