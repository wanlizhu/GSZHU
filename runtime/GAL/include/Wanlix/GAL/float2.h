#pragma once

#include "Common.h"
#include <string>
#include <vector>
#include <iostream>
#ifdef GAL_USE_EIGEN
#include <Eigen/Eigen>
#endif

namespace Wanlix
{
    class float2
    {
    public:
        float x;
        float y;

        static const float2 zero;
        static const float2 one;
        static const float2 unit_x;
        static const float2 unit_y;
        static const float2 nan;
        static const float2 inf;

        static float2 from_string(const std::string& str);
        static float2 from_polar_coords(float theta, float length);
        static float2 random(float length = 1.f);

        float2();
        float2(const float2& rhs);
        float2(float x, float y);
        explicit float2(float scalar);
        explicit float2(const float* data);
        #ifdef GAL_USE_EIGEN
        float2(const Eigen::Vector2f& v);
        #endif

        FORCE_INLINE void         set(float x, float y) { this->x = x; this->y = y; }
        FORCE_INLINE float*       data() { return &x; }
        FORCE_INLINE const float* data() const { return &x; }
        FORCE_INLINE float&       operator[](int index) { return at(index); }
        FORCE_INLINE const float& operator[](int index) const { return at(index); }
        
        float&       at(int index);
        const float& at(int index) const;
        std::string to_string() const;
        float2 to_polar_coords() const;
        float  length() const;
        float  length_sq() const;
        float  distance(const float2& point) const;
        float  distance_sq(const float2& point) const;
        float  scale_to_length(float newlength);
        float2 scaled_to_length(float newlength) const;
        float  normalize();
        float2 normalized() const;
        float2 clamp(const float2& floor, const float2& ceil) const;
        float2 clamp01() const;
        bool   is_normalized(float eps = 1e-6f) const;
        bool   is_zero(float eps = 1e-6f) const;
        bool   is_infinite() const;
        bool   is_perp(const float2& other, float eps = 1e-6f) const;
        bool   equals(const float2& other, float eps = 1e-6f) const;
        bool   equals(float x, float y, float eps = 1e-6f) const;
        bool   bits_equals(const float2& other) const;

        float min_element() const;
        float max_element() const;
        int   min_element_index() const;
        int   max_element_index() const;
        float sum() const;
        float product() const;
        float average() const;

        float  dot(const float2& v) const;
        float2 rotate_ccw(float radian) const;
        float2 perp() const;
        float2 perp_dot(const float2& rhs) const;
        float2 reflect(const float2& normal) const;
        float2 refract(const float2& normal, float exitingIndex, float enteringIndex) const;
        float2 project_to(const float2& direction) const;
        float  angle_to(const float2& other) const;
        void   decompose(const float2& dir, float2& parallel, float2& prep) const;
        float2 lerp(const float2& end, float t) const;

        float2 operator-() const;
        float2 operator+() const;
        float2 operator+(const float2& rhs) const;
        float2 operator-(const float2& rhs) const;
        float2 operator*(float scalar) const;
        float2 operator/(float scalar) const;

        float2& operator+=(const float2& v);
        float2& operator-=(const float2& v);
        float2& operator*=(float scalar);
        float2& operator/=(float scalar); 
    };

    std::ostream& operator<<(std::ostream& out, const float2& rhs);
    std::string to_string(const float2& v);
    float2 operator*(float scalar, const float2& rhs);
    float2 clamp(const float2& v, const float2& a, const float2& b);
    float2 clamp01(const float2& v);
    float2 abs(const float2& v);
    float2 neg(const float2& v);
    float2 recip(const float2& v);
    float  length(const float2& v);
    float2 perp(const float2& v);
    float2 rotate_ccw(const float2& v, float radian);
    float2 min(const float2& a, const float2& b);
    float2 max(const float2& a, const float2& b);
    float2 dot(const float2& a, const float2& b);
    float  distance(const float2& a, const float2& b);
    float2 lerp(const float2& a, const float2& b, float t);
    bool   are_orthonormal(const float2& a, const float2& b, float eps = 1e-5f);
    bool   oriented_ccw(const float2& a, const float2& b, const float2& c);
    void   orthonormalize(float2& a, float2& b);
    void   convex_hull(const float2* points, int num, std::vector<float2>& convexhull);
    int    convex_hull_in_place(float* points, int num);
    bool   convex_hull_contains(const float2* convexhull, int num, const float2& point);
    float  min_area_rect(float2* points, int num, float2& center, float2& minU, float2& maxU, float2& minV, float2& maxV);
}