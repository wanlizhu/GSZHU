#pragma once

#include "Engine/Common/Config.h"
#include "Eigen/Eigen"

namespace ZHU
{
    template<typename T, int N>
    using Vector = Eigen::Matrix<T, N, 1>;

    template<typename T, int _NumRows, int _NumCols>
    using Matrix = Eigen::Matrix<T, _NumRows, _NumCols>;

    typedef Eigen::Vector2f Vec2f;
    typedef Eigen::Vector2i Vec2i;
    typedef Eigen::Vector2d Vec2d;
    typedef Eigen::Vector3f Vec3f;
    typedef Eigen::Vector3i Vec3i;
    typedef Eigen::Vector3d Vec3d;
    typedef Eigen::Vector4f Vec4f;
    typedef Eigen::Vector4i Vec4i;
    typedef Eigen::Vector4d Vec4d;

    typedef Eigen::Matrix2f Mat2f;
    typedef Eigen::Matrix2i Mat2i;
    typedef Eigen::Matrix2d Mat2d;
    typedef Eigen::Matrix3f Mat3f;
    typedef Eigen::Matrix3i Mat3i;
    typedef Eigen::Matrix3d Mat3d;
    typedef Eigen::Matrix4f Mat4f;
    typedef Eigen::Matrix4i Mat4i;
    typedef Eigen::Matrix4d Mat4d;

    typedef Eigen::Quaternionf Quatf;
    typedef Eigen::Quaterniond Quatd;

    template<typename T, int _NumRows, int _NumCols>
    Matrix<T, _NumRows, _NumCols> SetColumn(Matrix<T, _NumRows, _NumCols>& mat,
                                            int col,
                                            const Vector<T, _NumRows>& vec)
    {
        for (int i = 0; i < _NumRows; i++) {
            mat(i, col) = vec[i];
        }
        return mat;
    }
}