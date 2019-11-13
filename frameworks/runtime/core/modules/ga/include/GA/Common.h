#pragma once

#include "Config.h"
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

// Constants involving pi.
#define PI           3.1415926535897931
#define HALF_PI      1.5707963267948966
#define QUARTER_PI   0.7853981633974483
#define TWO_PI       6.2831853071795862
#define INV_PI       0.3183098861837907
#define INV_TWO_PI   0.1591549430918953
#define INV_HALF_PI  0.6366197723675813

// Conversions between degrees and radians.
#define DEG_TO_RAD   0.0174532925199433
#define RAD_TO_DEG   57.295779513082321

// Common constants.
#define SQRT_2       1.4142135623730951
#define INV_SQRT_2   0.7071067811865475
#define LN_2         0.6931471805599453
#define INV_LN_2     1.4426950408889634
#define LN_10        2.3025850929940459
#define INV_LN_10    0.43429448190325176

#define FLOAT_EQ(a, b) (std::abs(a - b) <= std::numeric_limits<decltype(a)>::epsilon())

#define DEFINE_GETTERS(name, type)\
    inline       type& Get##name()       { return m##name; }\
    inline const type& Get##name() const { return m##name; }

#define DEFINE_TRIVIAL_COMPARISONS(type)\
    inline bool operator!=(const type& other) const { return !operator==(other); }\
    inline bool operator> (const type& other) const { return !operator<=(other); }\
    inline bool operator>=(const type& other) const { return !operator<(other); }\
    inline bool operator<=(const type& other) const { return operator<(other) || operator==(other); }

namespace GA
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

    using INT2  = std::array<int, 2>;
    using INT3  = std::array<int, 3>;
    using INT4  = std::array<int, 4>;
    using INT9  = std::array<int, 9>;
    using INT16 = std::array<int, 16>;

    using FLT2  = std::array<float, 2>;
    using FLT3  = std::array<float, 3>;
    using FLT4  = std::array<float, 4>;
    using FLT9  = std::array<float, 9>;
    using FLT16 = std::array<float, 16>;

    using DBL2  = std::array<double, 2>;
    using DBL3  = std::array<double, 3>;
    using DBL4  = std::array<double, 4>;
    using DBL9  = std::array<double, 9>;
    using DBL16 = std::array<double, 16>;

    template<typename T, int N>
    using Vector   = Eigen::Matrix<T, N, 1>;

    template<typename T, int NR, int NC>
    using Matrix   = Eigen::Matrix<T, NR, NC>;

    using Vec2     = Eigen::Vector2d;
    using Vec3     = Eigen::Vector3d;
    using Vec4     = Eigen::Vector4d;
    using Mat2     = Eigen::Matrix2d;
    using Mat3     = Eigen::Matrix3d;
    using Mat4     = Eigen::Matrix4d;
    using Quat     = Eigen::Quaterniond;

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

    template<typename T, int N>
    bool operator<(const Vector<T, N>& v0, const Vector<T, N>& v1)
    {
        Vector<T, N> diff = v0 - v1;
        return std::all_of(diff.begin(), diff.end(), [](const T& e){ return e < (T)0; });
    }
}