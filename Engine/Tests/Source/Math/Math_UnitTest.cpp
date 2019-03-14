#include <gtest/gtest.h>
#include <NTGS/Math/LinearAlgebra.h>
#include <glm/glm.hpp>

using namespace NTGS;

TEST(Math, Vector_Constructors) {
    {
        Vector<double, 3> Vec;
        EXPECT_DOUBLE_EQ(Vec[0], 0.0);
        EXPECT_DOUBLE_EQ(Vec[1], 0.0);
        EXPECT_DOUBLE_EQ(Vec[2], 0.0);
    }

    {
        Vector<float, 2> Vec(123.456f);
        EXPECT_FLOAT_EQ(Vec[0], 123.456f);
        EXPECT_FLOAT_EQ(Vec[1], 123.456f);
    }

    {
        float Data[] = { 123.456f, 234.567f, 345.678f };
        Vector<float, 3> Vec(Data);
        EXPECT_FLOAT_EQ(Vec[0], 123.456f);
        EXPECT_FLOAT_EQ(Vec[1], 234.567f);
        EXPECT_FLOAT_EQ(Vec[2], 345.678f);
    }

    {
        Vector<float, 3> Vec(123.456f, 234.567f, 345.678f);
        EXPECT_FLOAT_EQ(Vec[0], 123.456f);
        EXPECT_FLOAT_EQ(Vec[1], 234.567f);
        EXPECT_FLOAT_EQ(Vec[2], 345.678f);
    }

    {
        Vector<float, 3> Vec = { 123.456f, 234.567f, 345.678f };
        EXPECT_FLOAT_EQ(Vec[0], 123.456f);
        EXPECT_FLOAT_EQ(Vec[1], 234.567f);
        EXPECT_FLOAT_EQ(Vec[2], 345.678f);
    }

    {
        Vector<float, 3> Vec = { 123.456f, 234.567f, 345.678f };

        EXPECT_EQ(sizeof(Vector<float, 3>), 3 * sizeof(float));
        EXPECT_EQ(sizeof(Vec), 3 * sizeof(float));
    }

    {
        Vector<float, 3> Vec = { 123.456f, 234.567f, 345.678f };
        Vector<int, 4> Vec2 = { 123, 234, 345, 456 };

        Vec = Vec2;
        EXPECT_FLOAT_EQ(Vec[0], 123.f);
        EXPECT_FLOAT_EQ(Vec[1], 234.f);
        EXPECT_FLOAT_EQ(Vec[2], 345.f);

        Vector<double, 3> Vec3 = { 123.123, 234.234, 345.345 };
        Vec = Vec3;
        EXPECT_FLOAT_EQ(Vec[0], 123.123f);
        EXPECT_FLOAT_EQ(Vec[1], 234.234f);
        EXPECT_FLOAT_EQ(Vec[2], 345.345f);
    }
}

TEST(Math, Vector_Arithmetic_Operators) {
    {
        Vector<float, 3> V1 = { 1.f, 1.777f, 3.f };
        Vector<float, 3> V2 = { 4.f, 5.f, 6.f };
        Vector<float, 3> V3(0, 123.456f, 0.000456f);

        auto V = V1 + V2;
        EXPECT_FLOAT_EQ(V[0], 5.f);
        EXPECT_FLOAT_EQ(V[1], 6.777f);
        EXPECT_FLOAT_EQ(V[2], 9.f);

        V = V2 + V3;
        EXPECT_FLOAT_EQ(V[0], 4.f);
        EXPECT_FLOAT_EQ(V[1], 5.f + 123.456f);
        EXPECT_FLOAT_EQ(V[2], 6.f + 0.000456f);

        V = V2 - V3;
        EXPECT_FLOAT_EQ(V[0], 4.f - 0.f);
        EXPECT_FLOAT_EQ(V[1], 5.f - 123.456f);
        EXPECT_FLOAT_EQ(V[2], 6.f - 0.000456f);

        V = 1.f / V1;
        EXPECT_FLOAT_EQ(V[0], 1.f / 1.f);
        EXPECT_FLOAT_EQ(V[1], 1.f / 1.777f);
        EXPECT_FLOAT_EQ(V[2], 1.f / 3.f);

        V = V1 / -862.456f;
        EXPECT_FLOAT_EQ(V[0], 1.f / -862.456f);
        EXPECT_FLOAT_EQ(V[1], 1.777f / -862.456f);
        EXPECT_FLOAT_EQ(V[2], 3.f / -862.456f);

        V = -V1;
        EXPECT_FLOAT_EQ(V[0], -1.f);
        EXPECT_FLOAT_EQ(V[1], -1.777f);
        EXPECT_FLOAT_EQ(V[2], -3.f);

        V = V3;
        V += V3;
        EXPECT_FLOAT_EQ(V[0], 0.f);
        EXPECT_FLOAT_EQ(V[1], 123.456f + 123.456f);
        EXPECT_FLOAT_EQ(V[2], 0.000456f + 0.000456f);

        V = -456.932f * V3;
        EXPECT_FLOAT_EQ(V[0], -456.932f * 0.f);
        EXPECT_FLOAT_EQ(V[1], -456.932f * 123.456f);
        EXPECT_FLOAT_EQ(V[2], -456.932f * 0.000456f);

        V = V3 * 4.567f;
        EXPECT_FLOAT_EQ(V[0], 0.f * 4.567f);
        EXPECT_FLOAT_EQ(V[1], 123.456f * 4.567f);
        EXPECT_FLOAT_EQ(V[2], 0.000456f * 4.567f);
    }
}

TEST(Math, Vector_Linear_Algebra) {

    glm::vec3 a(-234.634f, 0.00234f, 9123.233f), b(0, 12.9999f, 1.3456f);
    glm::vec3 c = glm::cross(a, b);

    Vector<float, 3> A(&a[0]), B(&b[0]);
    auto C = Cross(A, B);
   
    EXPECT_FLOAT_EQ(c[0], C[0]);
    EXPECT_FLOAT_EQ(c[1], C[1]);
    EXPECT_FLOAT_EQ(c[2], C[2]);

    float d = glm::dot(a, b);
    float D = Dot(A, B);
    EXPECT_FLOAT_EQ(d, D);

    float len = glm::length(a);
    float Len = Length(A);
    EXPECT_FLOAT_EQ(len, Len);
    len = glm::length(b);
    Len = Length(B);
    EXPECT_FLOAT_EQ(len, Len);

    glm::vec3 a1 = glm::normalize(a);
    glm::vec3 b1 = glm::normalize(b);
    auto A1 = Normalize(A);
    auto B1 = Normalize(B);
    EXPECT_FLOAT_EQ(a1[0], A1[0]);
    EXPECT_FLOAT_EQ(a1[1], A1[1]);
    EXPECT_FLOAT_EQ(a1[2], A1[2]);
    EXPECT_FLOAT_EQ(b1[0], B1[0]);
    EXPECT_FLOAT_EQ(b1[1], B1[1]);
    EXPECT_FLOAT_EQ(b1[2], B1[2]);
}

TEST(Math, Matrix_Constructors) {
    glm::mat4 m(1.234f, 2.3423f, -13.123f, 4.0072f,
                52.242f, 6.23423f, 72.234235f, 8.23423f,
                -0.23429f, -0.22f, 36.234f, 0.000001f,
                0, 0.23423f, 4.123f, -985.3423f);
    float* p = &m[0][0];      
    Matrix<float, 4, 4> M((Vector<float, 4>*)p);

    EXPECT_FLOAT_EQ(m[0][0], M[0][0]);
    EXPECT_FLOAT_EQ(m[0][1], M[0][1]);
    EXPECT_FLOAT_EQ(m[0][2], M[0][2]);
    EXPECT_FLOAT_EQ(m[0][3], M[0][3]);
    EXPECT_FLOAT_EQ(m[1][0], M[1][0]);
    EXPECT_FLOAT_EQ(m[1][1], M[1][1]);
    EXPECT_FLOAT_EQ(m[1][2], M[1][2]);
    EXPECT_FLOAT_EQ(m[1][3], M[1][3]);
    EXPECT_FLOAT_EQ(m[2][0], M[2][0]);
    EXPECT_FLOAT_EQ(m[2][1], M[2][1]);
    EXPECT_FLOAT_EQ(m[2][2], M[2][2]);
    EXPECT_FLOAT_EQ(m[2][3], M[2][3]);
    EXPECT_FLOAT_EQ(m[3][0], M[3][0]);
    EXPECT_FLOAT_EQ(m[3][1], M[3][1]);
    EXPECT_FLOAT_EQ(m[3][2], M[3][2]);
    EXPECT_FLOAT_EQ(m[3][3], M[3][3]);  
}

TEST(Matrix, General_Multiply) {
    glm::mat4 m1(1.234f, 2.3423f, -13.123f, 4.0072f,
        52.242f, 6.23423f, 72.234235f, 8.23423f,
        -0.23429f, -0.22f, 36.234f, 0.000001f,
        0, 0.23423f, 4.123f, -985.3423f);
    glm::mat4 m2(1.234f, 2.3423f, -13.123f, -4.0072f,
        -52.242f, 0, -9.2f, 8.23423f,
        -0.23429f, 0.22f, 36.234f, 121.f,
        0, 0.23423f,-213, 985.3423f);
    Matrix4 M1((const Vector4*)&m1[0]);
    Matrix4 M2((const Vector4*)&m2[0]);

    glm::mat4 m3 = m1 * m2;
    Matrix4 M3 = M2 * M1;

    EXPECT_FLOAT_EQ(m3[0][0], M3[0][0]);
    EXPECT_FLOAT_EQ(m3[0][1], M3[0][1]);
    EXPECT_FLOAT_EQ(m3[0][2], M3[0][2]);
    EXPECT_FLOAT_EQ(m3[0][3], M3[0][3]);
    EXPECT_FLOAT_EQ(m3[1][0], M3[1][0]);
    EXPECT_FLOAT_EQ(m3[1][1], M3[1][1]);
    EXPECT_FLOAT_EQ(m3[1][2], M3[1][2]);
    EXPECT_FLOAT_EQ(m3[1][3], M3[1][3]);
    EXPECT_FLOAT_EQ(m3[2][0], M3[2][0]);
    EXPECT_FLOAT_EQ(m3[2][1], M3[2][1]);
    EXPECT_FLOAT_EQ(m3[2][2], M3[2][2]);
    EXPECT_FLOAT_EQ(m3[2][3], M3[2][3]);
    EXPECT_FLOAT_EQ(m3[3][0], M3[3][0]);
    EXPECT_FLOAT_EQ(m3[3][1], M3[3][1]);
    EXPECT_FLOAT_EQ(m3[3][2], M3[3][2]);
    EXPECT_FLOAT_EQ(m3[3][3], M3[3][3]);
}