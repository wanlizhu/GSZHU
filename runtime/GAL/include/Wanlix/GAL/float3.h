#pragma once

#include "float2.h"

namespace Wanlix
{
    class float3
    {
    public:
        union {
            struct { float x, y, z; };
            struct { float r, g, b; };
        };

        static const float3 zero;
        static const float3 one;
        static const float3 unit_x;
        static const float3 unit_y;
        static const float3 unit_z;
        static const float3 nan;
        static const float3 inf;

        static float3 from_string(const std::string& str);
        static float3 from_spherical_coords(float azimuth, float inclination, float radius = 1.f);
        static float3 from_spherical_coords(const float3& spherical);
        static float3 random(float length = 1.f);

        float3();
        float3(float x, float y, float z);
        explicit float3(float scalar);
        float3(const float2& xy, float z);
        float3(float x, const float2& yz);
        explicit float3(const float* data);
        #ifdef GAL_USE_EIGEN
        float3(const Eigen::Vector3f& v);
        #endif

        FORCE_INLINE void   set(float a, float b, float c) { x = a; y = b; z = c; }
        FORCE_INLINE float* data() { return &x; }
        FORCE_INLINE const float* data() const { return &x; }
        FORCE_INLINE float& operator[](int index) { return at(index); }
        FORCE_INLINE const float& operator[](int index) const { return at(index); }

        float& at(int index);
        const float& at(int index) const;
        std::string to_string() const;
        float3 to_spherical_coords() const;
        float2 to_spherical_coords_normalized() const;
        float4 to_pos4() const;
        float4 to_dir4() const;
        float  length() const;
        float  length_sq() const;
        float  distance(const float3& point) const;
        float  distance_sq(const float3& point) const;
        float  distance_to(const float3& point) const;
        float  distance_to(const line_t& line) const;
        float  distance_to(const ray_t& ray) const;
        float  distance_to(const segment_t& segment) const;
        float  distance_to(const plane_t& plane) const;
        float  distance_to(const triangle_t& triangle) const;
        float  distance_to(const sphere_t& sphere) const;
        float  distance_to(const capsule_t& capsule) const;
        float  scale_to_length(float newlength);
        float3 scaled_to_length(float newlength) const;
        float  normalize();
        float3 normalized() const;
        float3 clamp(const float3& floor, const float3& ceil) const;
        float3 clamp01() const;
        bool   is_normalized(float eps = 1e-6f) const;
        bool   is_zero(float eps = 1e-6f) const;
        bool   is_infinite() const;
        bool   is_perp(const float3& other, float eps = 1e-6f) const;
        bool   equals(const float3& other, float eps = 1e-6f) const;
        bool   equals(float x, float y, float z, float eps = 1e-6f) const;
        bool   bits_equals(const float3& other) const;

        float min_element() const;
        float max_element() const;
        int   min_element_index() const;
        int   max_element_index() const;
        float sum() const;
        float product() const;
        float average() const;

        float    dot(const float3& v) const;
        float3   cross(const float3& v) const;
        float3x3 outer_product(const float3& v) const;
        float3   perp(const float3& hint = float3(0, 1, 0), const float3& hint2 = float3(0, 0, 1)) const;
        float3   another_perp(const float3& hint = float3(0, 1, 0), const float3& hint2 = float3(0, 0, 1)) const;
        void     orthogonal_basis(float3& b, float3& c) const;
        float3   perp_dot(const float3& rhs) const;
        float3   reflect(const float3& normal) const;
        float3   refract(const float3& normal, float exitingIndex, float enteringIndex) const;
        float3   project_to(const float3& direction) const;
        float    angle_to(const float3& other) const;
        void     decompose(const float3& dir, float3& parallel, float3& prep) const;
        float3   lerp(const float3& end, float t) const;

        float3 operator-() const;
        float3 operator+() const;
        float3 operator+(const float3& rhs) const;
        float3 operator-(const float3& rhs) const;
        float3 operator*(float scalar) const;
        float3 operator/(float scalar) const;

        float3& operator+=(const float3& v);
        float3& operator-=(const float3& v);
        float3& operator*=(float scalar);
        float3& operator/=(float scalar);
    };

    std::ostream& operator<<(std::ostream& out, const float3& rhs);
    std::string to_string(const float3& v);
    float3 operator*(float scalar, const float3& rhs);
    float3 clamp(const float3& v, const float3& a, const float3& b);
    float3 clamp01(const float3& v);
    float3 abs(const float3& v);
    float3 neg(const float3& v);
    float3 recip(const float3& v);
    float  length(const float3& v);
    float3 perp(const float3& v);
    float3 min(const float3& a, const float3& b);
    float3 max(const float3& a, const float3& b);
    float  dot(const float3& a, const float3& b);
    float3 cross(const float3& a, const float3& b);
    float  distance(const float3& a, const float3& b);
    float3 lerp(const float3& a, const float3& b, float t);
    bool   are_collinear(const float3& a, const float3& b, const float3& c, float eps = 1e-6f);
    bool   are_orthonormal(const float3& a, const float3& b, const float3& c, float eps = 1e-6f);
    void   orthonormalize(float3& a, float3& b);
    void   orthonormalize(float3& a, float3& b, float3& c);
}