#pragma once
#include <gtest/gtest.h>
#include <NTGS/Core/Geometry.h>
#include <glm/glm.hpp>

using namespace NTGS;

#define MATRIX_EQ(m1, m2)\
    EXPECT_FLOAT_EQ(m1[0][0], m2[0][0]);\
    EXPECT_FLOAT_EQ(m1[0][1], m2[0][1]);\
    EXPECT_FLOAT_EQ(m1[0][2], m2[0][2]);\
    EXPECT_FLOAT_EQ(m1[0][3], m2[0][3]);\
    EXPECT_FLOAT_EQ(m1[1][0], m2[1][0]);\
    EXPECT_FLOAT_EQ(m1[1][1], m2[1][1]);\
    EXPECT_FLOAT_EQ(m1[1][2], m2[1][2]);\
    EXPECT_FLOAT_EQ(m1[1][3], m2[1][3]);\
    EXPECT_FLOAT_EQ(m1[2][0], m2[2][0]);\
    EXPECT_FLOAT_EQ(m1[2][1], m2[2][1]);\
    EXPECT_FLOAT_EQ(m1[2][2], m2[2][2]);\
    EXPECT_FLOAT_EQ(m1[2][3], m2[2][3]);\
    EXPECT_FLOAT_EQ(m1[3][0], m2[3][0]);\
    EXPECT_FLOAT_EQ(m1[3][1], m2[3][1]);\
    EXPECT_FLOAT_EQ(m1[3][2], m2[3][2]);\
    EXPECT_FLOAT_EQ(m1[3][3], m2[3][3]);

TEST(Core_Geometry, Vector_Test) {
    Vec3d v1(12.42, 435.2342, -0.0012);
    glm::dvec3 v2(12.42, 435.2342, -0.0012);
    glm::dvec3 v2n = normalize(v2);

    Vec4i v3(v1, 234.f);
  
    EXPECT_DOUBLE_EQ(Length(v1), glm::length(v2));
    EXPECT_EQ(IsNormalized(Normalize(v1)), true);
    EXPECT_EQ(Normalize(v1), Vec3d(v2n.x, v2n.y, v2n.z));
}

TEST(Core_Geometry, Matrix_Test) {
    Mat4f m;
    SetIdentity(m);
    EXPECT_EQ(IsIdentity(m), true);

    m.Set(Mat4f::ColumnType(1));
    EXPECT_EQ(IsIdentity(m), false);

    Vec2f dd{ 234.4, 3453.0 };

    Mat4f mm = {
        Vec4f{0.0, 234.0, -342.43, 0.0},
    Vec4f{234., -54., 99.0, 234.},
    Vec4f{0.0, -5.67, -4.53, 0.0},
    Vec4f{-23.4, 5.6, 4.5, 7.} };
    EXPECT_FLOAT_EQ(mm[2][3], 0.0);

    Mat4f mm4 = Mul(mm, mm);
    glm::mat4* mm2 = (glm::mat4*)&mm;
    glm::mat4 mm3 = (*mm2) * (*mm2);

    MATRIX_EQ(mm3, mm4);
}