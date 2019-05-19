#include <GSZHU/MATH/MatrixFunctions.h>
#include <GSZHU/MATH/VectorFunctions.h>
#include <GSZHU/MATH/QuaternionFunctions.h>
#include <GSZHU/BasicTypes.h>
#include <catch2/catch.hpp>
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace GSZHU;

#define MAT4_EQUAL(m1, m2) \
REQUIRE(FLT64_EQ(m1[0][0], m2[0][0]));\
REQUIRE(FLT64_EQ(m1[0][1], m2[0][1]));\
REQUIRE(FLT64_EQ(m1[0][2], m2[0][2]));\
REQUIRE(FLT64_EQ(m1[0][3], m2[0][3]));\
REQUIRE(FLT64_EQ(m1[1][0], m2[1][0]));\
REQUIRE(FLT64_EQ(m1[1][1], m2[1][1]));\
REQUIRE(FLT64_EQ(m1[1][2], m2[1][2]));\
REQUIRE(FLT64_EQ(m1[1][3], m2[1][3]));\
REQUIRE(FLT64_EQ(m1[2][0], m2[2][0]));\
REQUIRE(FLT64_EQ(m1[2][1], m2[2][1]));\
REQUIRE(FLT64_EQ(m1[2][2], m2[2][2]));\
REQUIRE(FLT64_EQ(m1[2][3], m2[2][3]));\
REQUIRE(FLT64_EQ(m1[3][0], m2[3][0]));\
REQUIRE(FLT64_EQ(m1[3][1], m2[3][1]));\
REQUIRE(FLT64_EQ(m1[3][2], m2[3][2]));\
REQUIRE(FLT64_EQ(m1[3][3], m2[3][3]));
    
TEST_CASE("Test vector of math library", "[Math/Vector]") {
    SVector<double, 3> V(123.32425342, -0.00001234, 12.123456);
    SVector<double, 3> V2(-234.132423, 34.23425, 6.98765);
    glm::dvec3 v(123.32425342, -0.00001234, 12.123456);
    glm::dvec3 v2(-234.132423, 34.23425, 6.98765);
    REQUIRE(sizeof(V) == sizeof(double)*3);

    auto D = Dot(V, V);
    auto d = glm::dot(v, v);
    REQUIRE(FLT64_EQ(D, d));

    auto V3 = Cross(V, V2);
    auto v3 = glm::cross(v, v2);
    REQUIRE(FLT64_EQ(V3[0], v3[0]));
    REQUIRE(FLT64_EQ(V3[1], v3[1]));
    REQUIRE(FLT64_EQ(V3[2], v3[2]));

    auto L = Length(V);
    auto l = glm::length(v);
    REQUIRE(FLT64_EQ(L, l));

    auto V4 = Normalize(V);
    auto v4 = glm::normalize(v);
    REQUIRE(FLT64_EQ(V4[0], v4[0]));
    REQUIRE(FLT64_EQ(V4[1], v4[1]));
    REQUIRE(FLT64_EQ(V4[2], v4[2]));

    {
        auto Vx = -V;
        auto vx = -v;
        REQUIRE(FLT64_EQ(Vx[0], vx[0]));
        REQUIRE(FLT64_EQ(Vx[1], vx[1]));
        REQUIRE(FLT64_EQ(Vx[2], vx[2]));
    }

    {
        auto Vx = V + V2;
        auto vx = v + v2;
        REQUIRE(FLT64_EQ(Vx[0], vx[0]));
        REQUIRE(FLT64_EQ(Vx[1], vx[1]));
        REQUIRE(FLT64_EQ(Vx[2], vx[2]));
    }

    {
        auto Vx = V - V2;
        auto vx = v - v2;
        REQUIRE(FLT64_EQ(Vx[0], vx[0]));
        REQUIRE(FLT64_EQ(Vx[1], vx[1]));
        REQUIRE(FLT64_EQ(Vx[2], vx[2]));
    }

    {
        auto Vx = V * V2;
        auto vx = v * v2;
        REQUIRE(FLT64_EQ(Vx[0], vx[0]));
        REQUIRE(FLT64_EQ(Vx[1], vx[1]));
        REQUIRE(FLT64_EQ(Vx[2], vx[2]));
    }

    {
        auto Vx = V / V2;
        auto vx = v / v2;
        REQUIRE(FLT64_EQ(Vx[0], vx[0]));
        REQUIRE(FLT64_EQ(Vx[1], vx[1]));
        REQUIRE(FLT64_EQ(Vx[2], vx[2]));
    }

    {
        auto Vx = V * 123.88;
        auto vx = v * 123.88;
        REQUIRE(FLT64_EQ(Vx[0], vx[0]));
        REQUIRE(FLT64_EQ(Vx[1], vx[1]));
        REQUIRE(FLT64_EQ(Vx[2], vx[2]));
    }

    {
        auto Vx = -0.2342242 * V;
        auto vx = -0.2342242 * v;
        REQUIRE(FLT64_EQ(Vx[0], vx[0]));
        REQUIRE(FLT64_EQ(Vx[1], vx[1]));
        REQUIRE(FLT64_EQ(Vx[2], vx[2]));
    }

    {
        auto Vx = V / 123.88;
        auto vx = v / 123.88;
        REQUIRE(FLT64_EQ(Vx[0], vx[0]));
        REQUIRE(FLT64_EQ(Vx[1], vx[1]));
        REQUIRE(FLT64_EQ(Vx[2], vx[2]));
    }

    {
        auto Vx = V;
        Vx *= 123.88;
        auto vx = v;
        vx *= 123.88;
        REQUIRE(FLT64_EQ(Vx[0], vx[0]));
        REQUIRE(FLT64_EQ(Vx[1], vx[1]));
        REQUIRE(FLT64_EQ(Vx[2], vx[2]));
    }

    {
        auto Vx = V;
        Vx /= 123.88;
        auto vx = v;
        vx /= 123.88;
        REQUIRE(FLT64_EQ(Vx[0], vx[0]));
        REQUIRE(FLT64_EQ(Vx[1], vx[1]));
        REQUIRE(FLT64_EQ(Vx[2], vx[2]));
    }
}

TEST_CASE("Test matrix of math library", "[Math/Matrix]") {
    SMatrix<double, 4, 4> M{ {1.24, 2.234, -0.23423, 0.0}, {5.234, 6.234, 7.2342, 8.45674}, {0.00000009, 1.0, 0.434635411, -0.34512}, {1.3453, -14.345, 13455.0, 1.6} };
    glm::dmat4 m{ {1.24, 2.234, -0.23423, 0.0}, {5.234, 6.234, 7.2342, 8.45674}, {0.00000009, 1.0, 0.434635411, -0.34512}, {1.3453, -14.345, 13455.0, 1.6} };
    REQUIRE(sizeof(M) == sizeof(double)*16);

    auto M2 = M * M;
    auto m2 = m * m;
    MAT4_EQUAL(M2, m2);

    auto M3 = Transpose(M);
    auto m3 = glm::transpose(m);
    MAT4_EQUAL(M3, m3);

    auto M4 = ~M;
    auto m4 = glm::inverse(m);
    MAT4_EQUAL(M4, m4);

    SVector<double, 3> V(123, -0.87834, -0.2523);
    glm::dvec3 v(123, -0.87834, -0.2523);
    auto V2 = M * SVector<double, 4>(V, 1);
    auto v2 = m * glm::dvec4(v, 1);
    REQUIRE(FLT64_EQ(V2[0], v2[0]));
    REQUIRE(FLT64_EQ(V2[1], v2[1]));
    REQUIRE(FLT64_EQ(V2[2], v2[2]));

    {
        auto Mx = -M;
        auto mx = -m;
        MAT4_EQUAL(Mx, mx);
    }

    {
        auto Mx = M + M;
        auto mx = m + m;
        MAT4_EQUAL(Mx, mx);
    }

    {
        auto Mx = M - M;
        auto mx = m - m;
        MAT4_EQUAL(Mx, mx);
    }

    {
        auto Mx = M * M;
        auto mx = m * m;
        MAT4_EQUAL(Mx, mx);
    }


    {
        auto Mx = M * 123.4235;
        auto mx = m * 123.4235;
        MAT4_EQUAL(Mx, mx);
    }

    {
        auto Mx = M / 123.4235;
        auto mx = m / 123.4235;
        MAT4_EQUAL(Mx, mx);
    }
}