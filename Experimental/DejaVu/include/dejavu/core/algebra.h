#pragma once

#include <array>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <Eigen/Eigen>

namespace djv
{
    template<typename T, int N>
    using Vector = Eigen::Matrix<T, N, 1>;

    template<typename T, int RS, int CS>
    using Matrix = Eigen::Matrix<T, RS, CS>;

    template<typename T>
    using Quaternion = Eigen::Quaternion<T>;

    template<typename T, int N>
    using AlignedBox = Eigen::AlignedBox<T, N>;

    template<typename T>
    using AngleAxis = Eigen::AngleAxis<T>;


}