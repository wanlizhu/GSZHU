#include "gtest/gtest.h"
#include "Geometry/Algebra/Quaternion.h"

using namespace ZHU;

TEST(Test_Quaternion, Constructors)
{
    Quaternion<double> q;
    Quaternion<double> q1(1, 2, 3, 4);
}