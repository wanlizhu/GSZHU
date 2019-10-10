#pragma once

#include <Eigen/Eigen>
#include <type_traits>
#include <array>
#include "Config.h"

#define ENUM2INT(value) (static_cast<std::underlying_type_t<decltype(value)>>(value))

namespace CGA
{
    using INT2 = std::array<int, 2>;
    using INT3 = std::array<int, 3>;
    using INT4 = std::array<int, 4>;

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

    template<typename T, int DIM>
    using VectorT = Eigen::Matrix<T, DIM, 1>;
}