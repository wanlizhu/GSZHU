#include "gtest/gtest.h"
#include "Geometry/Algebra/MatrixAll.h"

using namespace ZHU;

#define MAT_EQ(n, m0, m1)\
for (int i = 0; i < n; i++) {\
    EXPECT_DOUBLE_EQ(m0[i], m1[i]);\
}

TEST(Test_Matrix, Constructors)
{
    Matrix<double, 5, 2> m;
    Matrix<double, 5, 3> m1(0, 1, 2, 3, 4, 
        10, 11, 12, 13, 14,
        20, 21, 22, 23, 24);
    Matrix<double, 5, 3> m2{ 0, 1, 2, 3, 4,
        10, 11, 12, 13, 14,
        20, 21, 22, 23, 24 };

    EXPECT_TRUE(decltype(m)::NumRows == 5);
    EXPECT_TRUE(decltype(m)::NumColumns == 2);
    EXPECT_TRUE(decltype(m)::IsColumnMajor);
    MAT_EQ(15, m1, m2);
}