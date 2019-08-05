#include "gtest/gtest.h"
#include "Geometry/Algebra/Rotation.h"

using namespace ZHU;

TEST(Test_Rotation, Convert_From_Matrix)
{
    Matrix<double, 3, 3> m;
    Rotation<double, 3> rot(m);
}