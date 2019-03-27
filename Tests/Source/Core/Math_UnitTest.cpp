#pragma once
#include <gtest/gtest.h>
#include "Math/Math.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/geometric.hpp>

using namespace NTGS;

#define VEC_EQ(a, b)\
EXPECT_DOUBLE_EQ(a[0], b[0]);\
EXPECT_DOUBLE_EQ(a[1], b[1]);\
EXPECT_DOUBLE_EQ(a[2], b[2]);

#define MAT_EQ(a, b)\
EXPECT_DOUBLE_EQ(a[0][0], b[0][0]);\
EXPECT_DOUBLE_EQ(a[0][1], b[0][1]);\
EXPECT_DOUBLE_EQ(a[0][2], b[0][2]);\
EXPECT_DOUBLE_EQ(a[0][3], b[0][3]);\
EXPECT_DOUBLE_EQ(a[1][0], b[1][0]);\
EXPECT_DOUBLE_EQ(a[1][1], b[1][1]);\
EXPECT_DOUBLE_EQ(a[1][2], b[1][2]);\
EXPECT_DOUBLE_EQ(a[1][3], b[1][3]);\
EXPECT_DOUBLE_EQ(a[2][0], b[2][0]);\
EXPECT_DOUBLE_EQ(a[2][1], b[2][1]);\
EXPECT_DOUBLE_EQ(a[2][2], b[2][2]);\
EXPECT_DOUBLE_EQ(a[2][3], b[2][3]);\
EXPECT_DOUBLE_EQ(a[3][0], b[3][0]);\
EXPECT_DOUBLE_EQ(a[3][1], b[3][1]);\
EXPECT_DOUBLE_EQ(a[3][2], b[3][2]);\
EXPECT_DOUBLE_EQ(a[3][3], b[3][3]);

TEST(NTGS_Common_Math, Vector_Test) {
    Vec3d v1(12.42, 435.2342, -0.0012);
    Vec3d v2(213.12, -83, -0.33);
    glm::dvec3 v22(12.42, 435.2342, -0.0012);
    glm::dvec3 v2n = normalize(v22);
    Vec4i v3(v1, 234);

    EXPECT_DOUBLE_EQ(Length(v1), glm::length(v22));
    EXPECT_TRUE(Normalize(v1) == Vec3d(v2n.x, v2n.y, v2n.z));
    EXPECT_EQ(sizeof(Vec2f), sizeof(float) * 2);
    EXPECT_EQ(sizeof(Vec2d), sizeof(double) * 2);
    EXPECT_EQ(sizeof(Vec3f), sizeof(float) * 3);
    EXPECT_EQ(sizeof(Vec3d), sizeof(double) * 3);
    EXPECT_EQ(sizeof(Vec4f), sizeof(float) * 4);
    EXPECT_EQ(sizeof(Vec4d), sizeof(double) * 4);
    EXPECT_EQ(sizeof(Mat4f), sizeof(float) * 16);
    EXPECT_EQ(sizeof(Mat4d), sizeof(double) * 16);
}

TEST(NTGS_Common_Math, Swizzle_Test) {
    {
        Vec2d v{ 1, 2 };
        Vec2d vv = v.yx;

        EXPECT_DOUBLE_EQ(v[0], vv[1]);
        EXPECT_DOUBLE_EQ(v[1], vv[0]);
    }

    {
        Vec3d v{ 1,2,3 };
        Vec3d vv = v.zyx;
        EXPECT_DOUBLE_EQ(v[0], vv[2]);
        EXPECT_DOUBLE_EQ(v[1], vv[1]);
        EXPECT_DOUBLE_EQ(v[2], vv[0]);
    }
}

TEST(NTGS_Common_Math, Vector_Operator_Test) {
    Vec3d v1(12.42, -45.2248723342, -0.0000012);
    Vec3d v2(-0.42, 5.229990342, -33.003434512);
    glm::dvec3 g1(12.42, -45.2248723342, -0.0000012);
    glm::dvec3 g2(-0.42, 5.229990342, -33.003434512);

    v1 = -v1;
    v1 += v2;
    v1 = v1 + v2;
    v1 -= v2;
    v1 = v1 - v2;
    v1 = v1 * 234.24;
    v1 = v1 *= -345.34;
    v1 = v1 /= 234.23;
    v1 = v1 / 234.4;
    v1 = 32.423 * v1;
    v1 = 23243 / v1;

    g1 = -g1;
    g1 += g2;
    g1 = g1 + g2;
    g1 -= g2;
    g1 = g1 - g2;
    g1 = g1 * 234.24;
    g1 = g1 *= -345.34;
    g1 = g1 /= 234.23;
    g1 = g1 / 234.4;
    g1 = 32.423 * g1;
    g1 = glm::dvec3(23243) / g1;

    EXPECT_DOUBLE_EQ(v1[0], g1[0]);
    EXPECT_DOUBLE_EQ(v1[1], g1[1]);
    EXPECT_DOUBLE_EQ(v1[2], g1[2]);
}

TEST(Core_Geometry, Matrix_Constructor_Test) {
    int a[2][3];
    int l = sizeof(a) / sizeof(a[0]);
    int ll = sizeof(a[0]) / sizeof(a[0][0]);
    glm::mat3 mmm{ 1,2,3,4,5,6,7,8,9 };
    auto nnnn = mmm[1][2];
    
    EXPECT_EQ(sizeof(Vec4d), sizeof(double)*4);
    EXPECT_EQ(sizeof(Mat4f), sizeof(float) * 16);


    Mat4f m;
    EXPECT_EQ(IsIdentity(m), true);

    m = Mat4f(Vec4f(1));
    EXPECT_EQ(IsIdentity(m), false);

    Mat4f mm = { {1,2,3,4},{1,2,3,4},{1,2,3,4 },{1,2,3,4} };
    Mat4f mm2(Vec4f{ 1,2,3,4 }, Vec4i{ 1,2,3,4 }, Vec4d{ 1,2,3,4 }, Vec4d{ 1,2,3,4 });
    MAT_EQ(mm, mm2);
}

TEST(Core_Geometry, Matrix_Math_Test) {
    Matrix<double, 4, 4> m = 
    { {1.234,2.5422,0.2343,234.234},
      {.2344,5.234,6,0},
      {0,-0.2348,9,3},
      {13., 345.234, 23, 3} };
    double d = Vec4d(1.234, .2344, 0, 13.) * Vec4d(1.234, 2.5422, 0.2343, 234.234);

    auto row = CopyBlock<Matrix<double, 4, 1>>(m);
    auto col = CopyBlock<Matrix<double, 1, 4>>(m);
    double dd =  (row * col)[0];
    EXPECT_DOUBLE_EQ(d, dd);

    {
        Vec3d v{123423.23424, -0.001234, 99.122};
        glm::dvec3 vv(123423.23424, -0.001234, 99.122);
       
        EXPECT_EQ(Length(v), glm::length(vv));
        VEC_EQ(Normalize(v), glm::normalize(vv));
        EXPECT_DOUBLE_EQ(v*v, glm::dot(vv, vv));
        VEC_EQ((v^v), glm::cross(vv, vv));
    }

    {
        Mat4d m{ {1.123,0,3,4},{512,6.131,7.312,8},{143.31,132,0.002313,4},{0,2,-0.2342,1} };
        Mat4d mm{ {-1123,3.12,-3,.0004},{5.12,-6.131,-7.312,0},{1.4331,-132,234,456},{124,-2,-0.2342,1.324} };

        glm::dmat4 m2(1.123, 0, 3, 4, 512, 6.131, 7.312, 8, 143.31, 132, 0.002313, 4, 0, 2, -0.2342, 1);
        glm::dmat4 mm2(-1123, 3.12, -3, .0004, 5.12, -6.131, -7.312, 0, 1.4331, -132, 234, 456, 124, -2, -0.2342, 1.324);

        Mat4d prod = m * mm;
        glm::dmat4 prod2 = m2 * mm2;
        MAT_EQ(prod, prod2);

        Mat4d inv = Inverse(m);
        glm::dmat4 inv2 = glm::inverse(m2);
        MAT_EQ(inv, inv2);

        double det = Determinant(m);
        double det2 = glm::determinant(m2);
        EXPECT_DOUBLE_EQ(det, det2);
         
        Mat4d ts = Transpose(m);
        glm::dmat4 ts2 = glm::transpose(m2);
        MAT_EQ(ts, ts2);
    }
}