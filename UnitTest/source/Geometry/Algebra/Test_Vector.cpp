#include "gtest/gtest.h"
#include "Geometry/Algebra/Vector.h"

using namespace ZHU;

#define VEC_EQ(n, v0, v1) \
for (int i = 0; i < n; i++){\
    EXPECT_DOUBLE_EQ(v0[i], v1[i]);\
}

TEST(Test_Vector, Constructors)
{
    Vector<double, 5> v;
    Vector<double, 5> v1(1, 2, 3, 4, 5);
    Vector<double, 5> v2{1, 2, 3, 4, 5};

    EXPECT_TRUE(decltype(v)::IsColumnMajor);
    VEC_EQ(5, v1, v2);
}