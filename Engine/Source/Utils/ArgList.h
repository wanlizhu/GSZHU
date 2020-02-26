#pragma once

#include "Core/Common.h"

namespace Wanli
{
    class WANLI_API ArgList
    {
    public:
        ArgList();
        ArgList(int argc, char** argv);

        int GetArgCount() const;
        Optional<String> operator[](int index) const;
        Optional<String> operator[](const String& option) const;
        
    protected:
        StringArray mArgs;
        HashMap<String, int> mOptionIndexMap;
    };
}