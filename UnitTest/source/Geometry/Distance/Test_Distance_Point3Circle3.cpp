#include "gtest/gtest.h"

#if 0
#include "Engine/Geometry/Distance/3D/Distance_Point3Circle3.h"

using namespace ZHU;

TEST(Test_Distance_Point3Circle3, Case1)
{
    DCPQuery<double, Vector3<double>, Circle3<double>> query;
    Vector3<double> p(-1, 1, 0);
    Circle3<double> circle;
    circle.mCenter = Vector3<double>(1, 0, 0);
    circle.mNormal = Vector3<double>(0, 1, 0);
    circle.mRadius = 1;

    auto result = query(p, circle);
    EXPECT_DOUBLE_EQ(result.distance, std::sqrt(2));
    bool bb = result.closest == Vector3<double>(0, 0, 0);
    EXPECT_TRUE(bb);
    EXPECT_FALSE(result.equidistant);
}
#endif

#include "Geometry/Algebra/Matrix.h"

using namespace ZHU;

TEST(XX, YY)
{
    Matrix<float, 2, 3> m0;
    Matrix<float, 2, 3> m1{1, 2, 3, 4, 5, 6};
    Matrix<float, 2, 3, false> m2(1, 2, 3, 4, 5, 6);


    bool b = std::is_base_of_v<MatrixBase<Vector<float, 2, true>, float, 2, 1, false>, Matrix<float, 2, 3, false>::ColumnVector>;
    auto n = typeid(Matrix<float, 2, 3, false>::ColumnVector).name();
    auto nn = typeid(Vector<float, 2, false>).name();
    auto nnn = typeid(Vector<float, 2, false>::Base).name();

}