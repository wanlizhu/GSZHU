#pragma once

#include <Eigen/Eigen>
#include <type_traits>

#define ENUM2INT(value) (static_cast<std::underlying_type_t<decltype(value)>>(value))

namespace GML
{
    template<typename T, int N>
    using Vector = Eigen::Matrix<T, N, 1>;

    using Vec2i = Eigen::Vector2i;
    using Vec2f = Eigen::Vector2f;
    using Vec2d = Eigen::Vector2d;
    using Vec3i = Eigen::Vector3i;
    using Vec3f = Eigen::Vector3f;
    using Vec3d = Eigen::Vector3d;
    using Vec4i = Eigen::Vector4i;
    using Vec4f = Eigen::Vector4f;
    using Vec4d = Eigen::Vector4d;

    template<typename T, int Rows, int Cols>
    using Matrix = Eigen::Matrix<T, Rows, Cols>;

    using Mat2i = Eigen::Matrix2i;
    using Mat2f = Eigen::Matrix2f;
    using Mat2d = Eigen::Matrix2d;
    using Mat3i = Eigen::Matrix3i;
    using Mat3f = Eigen::Matrix3f;
    using Mat3d = Eigen::Matrix3d;
    using Mat4i = Eigen::Matrix4i;
    using Mat4f = Eigen::Matrix4f;
    using Mat4d = Eigen::Matrix4d;

    template<typename T>
    using Quaternion = Eigen::Quaternion<T>;

    using Quatf = Eigen::Quaternionf;
    using Quatd = Eigen::Quaterniond;
}