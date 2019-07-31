#pragma once

#include "Engine/Common/Config.h"
#define EIGEN_MATRIX_PLUGIN "Engine/Common/EigenMatrixAddon.h"
#include "Eigen/Eigen"

namespace ZHU
{
    template<typename T, int N>
    using Vector = Eigen::Matrix<T, N, 1>;

    template<typename T>
    using Vector2 = Vector<T, 2>;

    template<typename T>
    using Vector3 = Vector<T, 3>;

    template<typename T>
    using Vector4 = Vector<T, 4>;

    template<typename T, int _NumRows, int _NumCols>
    using Matrix = Eigen::Matrix<T, _NumRows, _NumCols>;
    
    template<typename T>
    using Quaternion = Eigen::Quaternion<T>;
}