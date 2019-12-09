#pragma once

#include "float3.h"
#include "SSEMath.h"

namespace Wanlix
{
    struct float4_storage
    {
        float x, y, z, w;

        float4_storage() = default;
        operator float4() const { 
            return *reinterpret_cast<const float4*>(this);
        }
    };

    class ALIGN16 float4
    {
    public:
        union {
            struct { float x, y, z, w; };
            struct { float r, g, b, a; };
            #ifdef GAL_SIMD
            simd4f v;
            #endif
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
        static float3 random(float w = 0.f, float length = 1.f);

        float4();
        float4(float x, float y, float z, float w);
        float4(const float3& xyz, float w);
        float4(float x, const float3& yzx);
        float4(float x, float y, const float2& zw);
        float4(const float2& xy, float z, float w);
        float4(const float2& xy, const float2& zw);
        explicit float4(const float* data);
        #ifdef GAL_USE_EIGEN
        float4(const Eigen::Vector4f& v);
        #endif

        FORCE_INLINE void   set(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
        FORCE_INLINE float* data() { return &x; }
        FORCE_INLINE const float* data() const { return &x; }
        FORCE_INLINE float& operator[](int index) { return at(index); }
        FORCE_INLINE const float& operator[](int index) const { return at(index); }

        float& at(int index);
        const float& at(int index) const;
        std::string to_string() const;
        float3& as_float3();
        const float3& as_float3() const;
        
        float4 operator-() const;
        float4 operator+() const;
        float4 operator+(const float4& rhs) const;
        float4 operator-(const float4& rhs) const;
        float4 operator*(float scalar) const;
        float4 operator/(float scalar) const;

        float4& operator+=(const float4& v);
        float4& operator-=(const float4& v);
        float4& operator*=(float scalar);
        float4& operator/=(float scalar);
    };
}