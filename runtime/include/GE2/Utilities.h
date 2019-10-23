#pragma once

#include "BasicTypes.h"

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
#define ENUM2INT(value) (static_cast<std::underlying_type_t<std::remove_reference_t<decltype(value)>>>(value))
#define EBIT_TEST(value, sample) (ENUM2INT(value & sample) != 0)
#define BIT_TEST(value, sample) ((value & sample) != 0)
#define UINT_SETBIT(n)  ((uint32_t)1 << n)
#define FLOAT_EQ(a, b)  (std::abs(a - b) <= std::numeric_limits<decltype(a)>::epsilon())

#define ALL_TRUE(vec)   (std::all_of(vec.begin(), vec.end(), [](bool e){ return e; }))
#define ALL_FALSE(vec)  (std::all_of(vec.begin(), vec.end(), [](bool e){ return !e; }))
#define ONE_TRUE(vec)   (std::any_of(vec.begin(), vec.end(), [](bool e){ return e; }))

// enum class helpers
#define DEFINE_ENUM_OPS(EnumClass)  \
        EnumClass  operator~ (EnumClass  e0)                { return (EnumClass)~ENUM2INT(e0); }\
        EnumClass  operator& (EnumClass  e0, EnumClass e1)  { return (EnumClass)(ENUM2INT(e0) & ENUM2INT(e1)); }\
        EnumClass  operator| (EnumClass  e0, EnumClass e1)  { return (EnumClass)(ENUM2INT(e0) | ENUM2INT(e1)); }\
        EnumClass  operator^ (EnumClass  e0, EnumClass e1)  { return (EnumClass)(ENUM2INT(e0) ^ ENUM2INT(e1)); }\
        EnumClass& operator&=(EnumClass& e0, EnumClass e1)  { e0 = (EnumClass)(ENUM2INT(e0) & ENUM2INT(e1)); return e0; }\
        EnumClass& operator|=(EnumClass& e0, EnumClass e1)  { e0 = (EnumClass)(ENUM2INT(e0) | ENUM2INT(e1)); return e0; }\
        EnumClass& operator^=(EnumClass& e0, EnumClass e1)  { e0 = (EnumClass)(ENUM2INT(e0) ^ ENUM2INT(e1)); return e0; }
        

namespace GE2
{
    GE2_IMPEXP UPTR<CSTR> Unpack(const STRLIST& list);
}