#pragma once

#include "Config.h"
#define _USE_GAL_DEFINES
#include <cmath>

#define UNUSED(...)         ((void)((void)__VA_ARGS__))

namespace Wanlix
{
    class float2;
    class float3;
    class float4;
    class float4_neon;
    class float4_sse;
    class float3x3;
    class float3x4;
    class float4x4;
    class float4x4_neon;
    class float4x4_sse;

    class line_t;
    class ray_t;
    class segment_t;
    class plane_t;
    class triangle_t;
    class aabb_t;
    class sphere_t;
    class capsule_t;

    FORCE_INLINE constexpr float deg_to_rad(float deg) noexcept { return deg * (M_PI / 180.f); }
    FORCE_INLINE constexpr float rad_to_deg(float rad) noexcept { return rad * (180.f / M_PI); }

    void* aligned_malloc(size_t size, size_t alignment);
    template<typename T>
    T* aligned_new(size_t elements, size_t alignment) {
        return reinterpret_cast<T*>(aligned_malloc(elements * sizeof(T), alignment));
    }
    void aligned_free(void* ptr);

    template<typename T, size_t _Alignment_>
    class aligned_allocator : public std::allocator<T>
    {
    public:
        template<class U>
        struct rebind { 
            typedef aligned_allocator<U, _Alignment_> other;
        };

        typedef std::allocator<T> base;

        typedef typename base::const_pointer const_pointer;
        typedef typename base::pointer pointer;
        typedef typename base::reference reference;
        typedef typename base::const_reference const_reference;
        typedef typename base::value_type value_type;
        typedef typename base::size_type size_type;
        typedef typename base::difference_type difference_type;

        aligned_allocator() {}
        aligned_allocator(const aligned_allocator&) :std::allocator<T>() {}
        template<typename U>
        aligned_allocator(const aligned_allocator<U, _Alignment_>&) {}

        pointer allocate(size_type n, const void* = 0) {
            return (pointer)aligned_malloc(n * sizeof(T), _Alignment_);
        }

        void deallocate(pointer ptr, size_type) {
            aligned_free(ptr);
        }
    };
}