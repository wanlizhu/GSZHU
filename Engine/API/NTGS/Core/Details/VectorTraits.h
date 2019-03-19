#pragma once

#include <NTGS/Core/Config.h>
#include <type_traits>
#include <numeric>
#include "VectorOperators.h"

namespace NTGS {
    template<typename Derived, typename T, int Dim>
    struct NTGS_API ArrayTraits : public ArrayComparisonOperators<Derived, T, Dim> {
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
    };
}