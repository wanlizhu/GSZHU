#include "gtest/gtest.h"
#include "Engine/Geometry/Algebra/ConvertCoordinates.h"

using namespace ZHU;

TEST(Test_ConvertCoordinates, Linear_Change_Of_Basis)
{
    ConvertCoordinates<double, 3> conv;
    Vector<double, 3> X, Y, P0, P1, diff;
    Matrix<double, 3, 3> U, V, A, B;
    bool isRHU, isRHV;

    SetColumn(U, 0, { 1.0, 0.0, 0.0 });
    SetColumn(U, 1, { 0.0, 1.0, 0.0 });
    SetColumn(U, 2, { 1.0, 0.0, 1.0 });

    SetColumn(V, 0, { 1.0, 0.0, 0.0 });
    SetColumn(V, 1, { 0.0, 0.0, 1.0 });
    SetColumn(V, 2, { 1.0, 1.0, 0.0 });

    conv(U, V);
    isRHU = conv.IsRH_U();
    isRHV = conv.IsRH_V();

    X = { 1.0, 2.0, 3.0 };
    Y = conv.U2V(X); // { 1.0, 3.0, 2.0 }

    P0 = U * X;
    P1 = V * Y;
    diff = P0 - P1; // { 0, 0, 0 }

    Y = { 0.0, 1.0, 2.0 };
    X = conv.V2U(Y);  // { 0.0, 2.0, 1.0 }
    P0 = U*X;
    P1 = V*Y;
    diff = P0 - P1;  // { 0, 0, 0 }

    double c = 0.6, s = 0.8;  // cs*cs + sn*sn = 1
    SetColumn(A, 0, {  c,   s, 0.0});
    SetColumn(A, 1, { -s,   c, 0.0});
    SetColumn(A, 2, {0.0, 0.0, 1.0});
    B = conv.U2V(A);

    SetColumn(B, 0, { c, 0, s});
    SetColumn(B, 1, { 0, 1, 0});
    SetColumn(B, 2, {-s, 0, c});
    X = A*X;  // U is VOR
    Y = B*Y;  // V is VOR
    P0 = U*X;
    P1 = V*Y;
    diff = P0 - P1;  // { 0, 0, 0 }
}