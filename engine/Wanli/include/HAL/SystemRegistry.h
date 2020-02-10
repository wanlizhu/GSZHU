#pragma once

#include "BasicTypes.h"
#include <optional>
#include <variant>

namespace Wanli
{
    enum ERegValueType
    {
        RV_Uint,
        RV_Uint64,
        RV_String,
        RV_ByteArray,
    };

    using RegValue = std::variant<Uint, Uint64, String, ByteArray>;

    class DLLDECL SystemRegistry
    {
    public:
        static std::optional<RegValue> GetValue(const String& key, ERegValueType* type = nullptr);
        static void SetValue(const String& key, const RegValue& value);
    };
}