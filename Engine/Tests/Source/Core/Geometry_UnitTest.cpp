#pragma once
#include <gtest/gtest.h>
#include <NTGS/Core/Geometry.h>
#include <glm/glm.hpp>

using namespace NTGS;

TEST(Core_Geometry, Vector_Test) {
    Vec3d v1(12.42, 435.2342, -0.0012);
    glm::dvec3 v2(12.42, 435.2342, -0.0012);
    glm::dvec3 v2n = normalize(v2);

    Vec4i v3(v1, 234.f);

    EXPECT_DOUBLE_EQ(Length(v1), glm::length(v2));
    EXPECT_EQ(Normalize(v1), Vec3d(v2n.x, v2n.y, v2n.z));
}

TEST(Core_Geometry, Matrix_Test) {
    Mat4f m;
    EXPECT_EQ(m.IsIdentity(), true);

    m.Set(Mat4f::ColumnType(1));
    EXPECT_EQ(m.IsIdentity(), false);


}