#pragma once

#include <chrono>
#include <Eigen/Eigen>

namespace ZHU
{
    using Seconds = std::chrono::seconds;
    using MilliSeconds = std::chrono::milliseconds;
    using MicroSeconds = std::chrono::microseconds;
    using Clock = std::chrono::high_resolution_clock;

    using Vec2i = Eigen::Vector2i;
    using Vec3i = Eigen::Vector3i;
    using Vec4i = Eigen::Vector4i;
    using Vec2f = Eigen::Vector2f;
    using Vec3f = Eigen::Vector3f;
    using Vec4f = Eigen::Vector4f;
    using Vec2d = Eigen::Vector2d;
    using Vec3d = Eigen::Vector3d;
    using Vec4d = Eigen::Vector4d;
    using Mat2i = Eigen::Matrix2i;
    using Mat3i = Eigen::Matrix3i;
    using Mat4i = Eigen::Matrix4i;
    using Mat2f = Eigen::Matrix2f;
    using Mat3f = Eigen::Matrix3f;
    using Mat4f = Eigen::Matrix4f;
    using Mat2d = Eigen::Matrix2d;
    using Mat3d = Eigen::Matrix3d;
    using Mat4d = Eigen::Matrix4d;
    using Quatf = Eigen::Quaternionf;
    using Quatd = Eigen::Quaterniond;


    template<typename T, typename U>
    T Convert(const U& value)
    {
        static_assert(false);
    }
}