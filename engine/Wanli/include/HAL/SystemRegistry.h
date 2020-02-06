#pragma once

#include "BasicTypes.h"
#include <optional>
#include <variant>

namespace Wanli
{
    class DLLDECL SystemRegistry
    {
    public:
        using Value = std::variant<Uint, Uint64, String, ByteArray>;
        enum EValueType
        {
            VT_Uint,
            VT_Uint64,
            VT_String,
            VT_ByteArray,
        };

        static std::optional<Value> GetValue(const String& key, EValueType* type = nullptr);
        static void SetValue(const String& key, const Value& value);
    };
}