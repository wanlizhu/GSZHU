#pragma once

#include <Eigen/Eigen>
#include <type_traits>
#include <array>

#define ENUM2INT(value) (static_cast<std::underlying_type_t<decltype(value)>>(value))

namespace CGA
{
    using INT2 = std::array<int, 2>;
    using INT3 = std::array<int, 3>;
    using INT4 = std::array<int, 4>;

    using Vec2    = Eigen::Vector2f;
    using Vec3    = Eigen::Vector3f;
    using Vec4    = Eigen::Vector4f;
    using Mat2    = Eigen::Matrix2f;
    using Mat3    = Eigen::Matrix3f;
    using Mat4    = Eigen::Matrix4f;
    using Point   = Vec3;
    using Point2D = Vec2;

    using Quaternion         = Eigen::Quaternionf;
    using AngleAxis          = Eigen::AngleAxisf;
    using Rotation2D         = Eigen::Rotation2Df;
    using Transform          = Eigen::Transform<float, 3, Eigen::Projective>;
    using IsometryTransform  = Eigen::Transform<float, 3, Eigen::Isometry>;

    using AlignedBox         = Eigen::AlignedBox<float, 3>;
    using AlignedBox2D       = Eigen::AlignedBox<float, 2>;
    using Hyperplane         = Eigen::Hyperplane<float, 3>;
    using Hyperplane2D       = Eigen::Hyperplane<float, 2>;
    using ParametrizedLine   = Eigen::ParametrizedLine<float, 3>;
    using ParametrizedLine2D = Eigen::ParametrizedLine<float, 2>;

    template<typename T, int DIM>
    using Vector = Eigen::Matrix<T, DIM, 1>;
}