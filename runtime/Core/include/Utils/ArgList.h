#pragma once

#include "Core/Common.h"

namespace Wanlix
{
    class DLL_DECL ArgVal
    {
    public:
        ArgVal(const String& str)
            : mValue(str)
        {}

        bool IsEmpty() const;
        bool ToBool() const;
        Int ToInt() const;
        Uint ToUint() const;
        Uint64 ToUint64() const;
        float ToFloat() const;
        String ToString() const;

    private:
        String mValue;
    };

    class DLL_DECL ArgList
    {
    public:
        void AddArgs(int argc, char** argv);
        void AddArg(const String& key);
        void AddArg(const String& key, const ArgVal& val);
        bool HasArg(const String& key) const;
        Array<ArgVal> GetPosArgs() const;
        Array<ArgVal> GetValues(const String& key) const;
        const ArgVal& operator[](const String& key) const;
        
    private:
        HashMap<String, Array<ArgVal>> mMap;
        Array<ArgVal> mPosArgs;
    };
}