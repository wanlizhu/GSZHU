#pragma once

#include <Eigen/Eigen>
#include <type_traits>
#include <limits>
#include <numeric>
#include <algorithm>
#include <array>
#include <functional>
#include <set>
#include <exception>
#include <stdexcept>
#include <optional>
#include "Config.h"

namespace GE2
{
    template<typename T>
    using LIST    = std::vector<T>;
    template<typename K, typename V>
    using MAP     = std::unordered_map<K, V>;
    template<typename T>
    using WPTR    = std::weak_ptr<T>;
    template<typename T>
    using SPTR    = std::shared_ptr<T>;
    template<typename T>
    using UPTR    = std::unique_ptr<T>;

    using CSTR    = const char*;
    using CSTRS   = const char* const*;
    using STR     = std::string;
    using STRLIST = std::vector<std::string>;
    using STRMAP  = std::unordered_map<std::string, std::string>;
    using HANDLE  = void*;

    using INT2 = std::array<int, 2>;
    using INT3 = std::array<int, 3>;
    using INT4 = std::array<int, 4>;

    using FLOAT2 = std::array<float, 2>;
    using FLOAT3 = std::array<float, 3>;
    using FLOAT4 = std::array<float, 4>;

    using Vec2     = Eigen::Vector2d;
    using Vec3     = Eigen::Vector3d;
    using Vec4     = Eigen::Vector4d;
    using Mat2     = Eigen::Matrix2d;
    using Mat3     = Eigen::Matrix3d;
    using Mat4     = Eigen::Matrix4d;
    using Vector   = Vec3;
    using Vector2D = Vec2;
    using Point    = Vec3;
    using Point2D  = Vec2;

    using Quaternion         = Eigen::Quaterniond;
    using AngleAxis          = Eigen::AngleAxisd;
    using Rotation2D         = Eigen::Rotation2Dd;
    using Transform          = Eigen::Transform<double, 3, Eigen::Projective>;
    using IsometryTransform  = Eigen::Transform<double, 3, Eigen::Isometry>;

    using AlignedBox         = Eigen::AlignedBox<double, 3>;
    using AlignedBox2D       = Eigen::AlignedBox<double, 2>;
    using Hyperplane         = Eigen::Hyperplane<double, 3>;
    using Hyperplane2D       = Eigen::Hyperplane<double, 2>;
    using ParametrizedLine   = Eigen::ParametrizedLine<double, 3>;
    using ParametrizedLine2D = Eigen::ParametrizedLine<double, 2>;

    template<int N>
    using PointT = Eigen::Matrix<double, N, 1>;

    template<int N>
    using VectorT = Eigen::Matrix<double, N, 1>;

    struct OFFSET
    {
        int32_t x = 0;
        int32_t y = 0;
    };

    struct EXTENT
    {
        uint32_t width = 0;
        uint32_t height = 0;
    };

    struct RECT
    {
        OFFSET offset;
        EXTENT extent;
    };
}