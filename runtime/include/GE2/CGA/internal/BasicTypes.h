#pragma once

#include <type_traits>
#include <limits>
#include <numeric>
#include <array>
#include <set>
#include "Config.h"

// Maximum number of iterations for bisection before a subinterval
// degenerates to a single point. TODO: Verify these.  I used the formula:
// 3 + std::numeric_limits<T>::digits - std::numeric_limits<T>::min_exponent.
//   IEEEBinary16:  digits = 11, min_exponent = -13
//   float:         digits = 27, min_exponent = -125
//   double:        digits = 53, min_exponent = -1021
#define MAX_BISECTIONS_FLOAT16    27u
#define MAX_BISECTIONS_FLOAT32    155u
#define MAX_BISECTIONS_FLOAT64    1077u
#define MAX_BISECTIONS_BSNUMBER   0xFFFFFFFFu
#define MAX_BISECTIONS_BSRATIONAL 0xFFFFFFFFu
#define MAX_BISECTIONS_GENERIC    2048u

// Constants involving pi.
#define PI          3.1415926535897931
#define HALF_PI     1.5707963267948966
#define QUARTER_PI  0.7853981633974483
#define TWO_PI      6.2831853071795862
#define INV_PI      0.3183098861837907
#define INV_TWO_PI  0.1591549430918953
#define INV_HALF_PI 0.6366197723675813

// Conversions between degrees and radians.
#define DEG_TO_RAD  0.0174532925199433
#define RAD_TO_DEG  57.295779513082321

// Common constants.
#define SQRT_2 1.4142135623730951
#define INV_SQRT_2 0.7071067811865475
#define LN_2 0.6931471805599453
#define INV_LN_2 1.4426950408889634
#define LN_10 2.3025850929940459
#define INV_LN_10 0.43429448190325176

// Helper macros
#define ENUM2INT(value) (static_cast<std::underlying_type_t<decltype(value)>>(value))
#define FLOAT_EQ(a, b) (std::abs(a - b) <= std::numeric_limits<decltype(a)>::epsilon())

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