#pragma once

#define GAL_USE_EIGEN
#define GAL_USE_EXCEPTION
//#define GAL_SILENT_ASSUME

// Uncomment to specify the SIMD instruction set level in use.
//#define GAL_AVX
//#define GAL_SSE41
//#define GAL_SSE3
//#define GAL_SSE2
//#define GAL_SSE // SSE1.

#ifdef GAL_NEON
#include <arm_neon.h>
#endif

// GAL_FMA implies GAL_AVX, 
// which implies GAL_SSE41, 
// which implies GAL_SSE3, 
// which implies GAL_SSE2, 
// which implies GAL_SSE.
#ifdef GAL_FMA
#    ifndef GAL_AVX
#        define GAL_AVX
#    endif
#endif

#ifdef GAL_AVX
#    if defined(__GNUC__) || defined(__clang__)
#        include <immintrin.h>
#    endif
#    ifndef GAL_SSE41
#        define GAL_SSE41
#    endif
#endif

#ifdef GAL_SSE41
#    ifndef GAL_SSE3
#        define GAL_SSE3
#    endif
#endif

#ifdef GAL_SSE3
#    ifdef _MSC_VER
#        include <intrin.h>
#    else
#        include <pmmintrin.h>
#    endif
#    ifndef GAL_SSE2
#        define GAL_SSE2
#    endif
#endif

#ifdef GAL_SSE2
#   ifndef GAL_SSE
#       define GAL_SSE
#   endif
#   include <emmintrin.h>
#endif

#ifdef GAL_SSE
#   include <xmmintrin.h>
#endif

#if defined(GAL_SSE) || defined(GAL_NEON)
#   define GAL_SIMD
#   define GAL_AUTOMATIC_SSE // Automatically use the SSE-optimized operations for all code.
#   ifdef GAL_NEON
typedef float32x4_t simd4f;
#   elif defined(GAL_SSE)
typedef __m128 simd4f;
#   endif
#endif

        
#ifdef _DEBUG
#   define FORCE_INLINE inline
#elif defined(_MSC_VER)
#   define FORCE_INLINE __forceinline 
#else
#   define FORCE_INLINE inline __attribute__((always_inline))
#endif

#define IS16ALIGNED(x) ((((uintptr_t)(x)) & 0xF) == 0)
#define IS32ALIGNED(x) ((((uintptr_t)(x)) & 0x1F) == 0)
#define IS64ALIGNED(x) ((((uintptr_t)(x)) & 0x3F) == 0)

#ifdef GAL_SIMD
#   ifdef GAL_AVX
#       define ALIGN_MAT ALIGN32
#       define MAT_ALIGNMENT 32
#       define IS_MAT_ALIGNED(x) IS32ALIGNED(x)
#   else
#       define ALIGN_MAT ALIGN16
#       define MAT_ALIGNMENT 16
#       define IS_MAT_ALIGNED(x) IS16ALIGNED(x)
#   endif
#   ifdef _MSC_VER
#       define ALIGN16 __declspec(align(16))
#       define ALIGN32 __declspec(align(32))
#       define ALIGN64 __declspec(align(64))
#   else
#       define ALIGN16 __attribute__((aligned(16)))
#       define ALIGN32 __attribute__((aligned(32)))
#       define ALIGN64 __attribute__((aligned(64)))
#   endif
#else
#   define ALIGN16
#   define ALIGN32
#   define ALIGN64
#   define ALIGN_MAT
#   define IS_MAT_ALIGNED(x) true
#endif