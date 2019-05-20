#pragma once

#include <GSZHU/ENUM/EValueType.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct GSZHU_API SBufferFormat {
        EVALUE_TYPE ValueType = VT_UNDEFINED; // Type of components. For a formatted buffer views
        uint8_t NumComponents = 0; // Number of components. Allowed values: 1, 2, 3, 4. 
        bool IsNormalized = false; // For signed and unsigned integer value types 

        SBufferFormat() noexcept;
        SBufferFormat(EVALUE_TYPE _ValueType, uint8_t _NumComponents) noexcept;

        bool operator==(const SBufferFormat& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SBufferFormat> {
        size_t operator()(const GSZHU::SBufferFormat& Desc) const {
            return GSZHU::ComputeHash(static_cast<int>(Desc.ValueType),
                                      Desc.NumComponents,
                                      static_cast<int>(Desc.IsNormalized));
        }
    };
}