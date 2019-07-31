#include "gtest/gtest.h"
#include "Engine/Geometry/Algebra/ConvertCoordinates.h"

using namespace ZHU;

TEST(Test_ConvertCoordinates, Linear_Change_Of_Basis)
{
    ConvertCoordinates<double, 3> conv;
    Vector<double, 3> X, Y, P0, P1, diff;
    Matrix<double, 3, 3> U, V, A, B;
    bool isRHU, isRHV;

    U.col(0) = Vector<double, 3>{ 1.0, 0.0, 0.0 };
    U.col(1) = Vector<double, 3>{ 0.0, 1.0, 0.0 };
    U.col(2) = Vector<double, 3>{ 0.0, 0.0, 1.0 };

    V.col(0) = Vector<double, 3>{ 1.0, 0.0, 0.0 };
    V.col(1) = Vector<double, 3>{ 0.0, 0.0, 1.0 };
    V.col(2) = Vector<double, 3>{ 0.0, 1.0, 0.0 };

    conv(U, V);
    isRHU = conv.IsRH_U();
    isRHV = conv.IsRH_V();
    EXPECT_TRUE(isRHU);
    EXPECT_FALSE(isRHV);

    X = { 1.0, 2.0, 3.0 };
    Y = conv.U2V(X); // { 1.0, 3.0, 2.0 }
    EXPECT_DOUBLE_EQ(Y[0], 1.0);
    EXPECT_DOUBLE_EQ(Y[1], 3.0);
    EXPECT_DOUBLE_EQ(Y[2], 2.0);

    P0 = U * X;
    P1 = V * Y;
    diff = P0 - P1; // { 0, 0, 0 }
    EXPECT_DOUBLE_EQ(diff[0], 0.0);
    EXPECT_DOUBLE_EQ(diff[1], 0.0);
    EXPECT_DOUBLE_EQ(diff[2], 0.0);

    Y = { 0.0, 1.0, 2.0 };
    X = conv.V2U(Y);  // { 0.0, 2.0, 1.0 }
    EXPECT_DOUBLE_EQ(X[0], 0.0);
    EXPECT_DOUBLE_EQ(X[1], 2.0);
    EXPECT_DOUBLE_EQ(X[2], 1.0);

    P0 = U*X;
    P1 = V*Y;
    diff = P0 - P1;  // { 0, 0, 0 }
    EXPECT_DOUBLE_EQ(diff[0], 0.0);
    EXPECT_DOUBLE_EQ(diff[1], 0.0);
    EXPECT_DOUBLE_EQ(diff[2], 0.0);

    double c = 0.6, s = 0.8;  // cs*cs + sn*sn = 1
    A.col(0) = Vector<double, 3>{  c,   s, 0.0};
    A.col(1) = Vector<double, 3>{ -s,   c, 0.0};
    A.col(2) = Vector<double, 3>{0.0, 0.0, 1.0};
    B = conv.U2V(A);

    B.col(0) = Vector<double, 3>{ c, 0, s};
    B.col(1) = Vector<double, 3>{ 0, 1, 0};
    B.col(2) = Vector<double, 3>{-s, 0, c};
    X = A*X;  // U is VOR
    Y = B*Y;  // V is VOR
    P0 = U*X;
    P1 = V*Y;
    diff = P0 - P1;  // { 0, 0, 0 }
    EXPECT_DOUBLE_EQ(diff[0], 0.0);
    EXPECT_DOUBLE_EQ(diff[1], 0.0);
    EXPECT_DOUBLE_EQ(diff[2], 0.0);
}