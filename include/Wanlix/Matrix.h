#pragma once

#include <type_traits>
#include <cstdint>
#include <array>

namespace Wanlix
{
    template<typename T, int _NumRows_, int _NumCols_>
    class Matrix 
    {
    public:
        using Element = T;
        using Column  = Matrix<T, _NumRows_, 1>;
        using Row     = Matrix<T, 1, _NumCols_>;
        const static int kLength = N;
        const static int kNumRows = _NumRows_;
        const static int kNumCols = _NumCols_;

        Matrix();
    };
}