#pragma once

#include "Eigen/Eigen"

namespace ZHU
{
    template<typename T, int N>
    using Vector = Eigen::Matrix<T, N, 1>;

    template<typename T, int _NumRows, int _NumCols>
    using Matrix = Eigen::Matrix<T, _NumRows, _NumCols>;

    template<typename T>
    using Quaternion = Eigen::Quaternion<T>;


}