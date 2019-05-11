#pragma once

#include <GSZHU/ENUM/EValueType.h>

namespace GSZHU {
    struct SBufferFormat {
        EVALUE_TYPE ValueType = VT_UNDEFINED; // Type of components. For a formatted buffer views
        UINT8 NumComponents = 0; // Number of components. Allowed values: 1, 2, 3, 4. 
        bool IsNormalized = false; // For signed and unsigned integer value types 

        SBufferFormat() noexcept {}
        SBufferFormat(EVALUE_TYPE _ValueType, UINT8 _NumComponents) noexcept;

        bool operator==(const SBufferFormat& rhs) const;
    };
}