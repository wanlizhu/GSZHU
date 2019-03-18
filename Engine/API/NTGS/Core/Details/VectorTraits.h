#pragma once

#include <NTGS/Core/Config.h>
#include <type_traits>
#include <numeric>
#include "VectorOperators.h"

namespace NTGS {
    template<typename Derived, typename T, int Dim>
    struct NTGS_API ArrayTraits : public ArrayComparisonOperators<Derived, T, Dim> {
        static inline T GetDistance(const InstanceType& v1, const InstanceType& v2) {
            return (v1 - v2).GetLength();
        }

        template<typename U>
        inline void Set(const U& value) {
            InstanceType& me = GetInstance();
            for (int i = 0; i < Dim; i++)
                me[i] = static_cast<T>(value);
        }

        template<typename U>
        inline void Copy(const U* data) {
            InstanceType& me = GetInstance();
            for (int i = 0; i < Dim; i++)
                me[i] = static_cast<T>(data[i]);
        }

        inline bool IsNormalized() const {
            return std::abs(Length(GetInstance()) - T(1)) <= std::numeric_limits<T>::epsilon();
        }
    };
}