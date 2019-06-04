#pragma once

#include "GraphicTypes.h"
#include <stdint.h>

namespace GSZHU {
    class CBufferFormat {
    public:
        CBufferFormat(EScalar scalar, uint8_t num, bool norm = false)
            : mScalar(scalar)
            , mNumComponents(num)
            , mIsNormalized(norm)
        {}

        inline EScalar GetScalarType() const { return mScalar; }
        inline uint8_t GetNumComponents() const { return mNumComponents; }
        inline bool IsNormalized() const { return mIsNormalized; }

    private:
        EScalar mScalar = EScalar::Undefined; // For a formatted buffer views, this value cannot be Undefined
        uint8_t mNumComponents = 0; // Allowed values: 1, 2, 3, 4 
        bool mIsNormalized = false; // For signed and unsigned integer value types 
    };
}