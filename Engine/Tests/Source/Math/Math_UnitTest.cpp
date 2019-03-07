#include <gtest/gtest.h>
#include <NTGS/Math/LinearAlgebra.h>

using namespace NTGS;

TEST(Math, Alignment_Test) {
    int Align = 0;

    Align = std::alignment_of<Array<int, 3>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Vector<double, 2>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Vector<float, 3>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Vector<double, 3>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Vector<float, 6>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Vector<short, 9>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Vector<double, 11>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Vector<double, 13>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Matrix<float, 3, 17>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Matrix<double, 3, 2>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Matrix<bool, 1, 3>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Matrix<short, 3, 4>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Matrix<short, 13, 4>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Matrix<short, 1, 7>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Matrix<short, 7, 1>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Matrix<short, 3, 9>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Matrix<short, 13, 3>>::value;
    EXPECT_EQ(Align, 16);
    Align = std::alignment_of<Matrix<short, 11, 3>>::value;
    EXPECT_EQ(Align, 16);
}

TEST(Math, Array_Constructors) {
    {
        Array<double, 3> Arr;
        EXPECT_DOUBLE_EQ(Arr[0], 0.0);
        EXPECT_DOUBLE_EQ(Arr[1], 0.0);
        EXPECT_DOUBLE_EQ(Arr[2], 0.0);
    }

    {
        Array<float, 2> Arr(123.456f);
        EXPECT_FLOAT_EQ(Arr[0], 123.456f);
        EXPECT_FLOAT_EQ(Arr[1], 123.456f);
    }

    {
        float Data[] = { 123.456f, 234.567f, 345.678f };
        Array<float, 3> Arr(Data);
        EXPECT_FLOAT_EQ(Arr[0], 123.456f);
        EXPECT_FLOAT_EQ(Arr[1], 234.567f);
        EXPECT_FLOAT_EQ(Arr[2], 345.678f);
    }

    {
        Array<float, 3> Arr(123.456f, 234.567f, 345.678f);
        EXPECT_FLOAT_EQ(Arr[0], 123.456f);
        EXPECT_FLOAT_EQ(Arr[1], 234.567f);
        EXPECT_FLOAT_EQ(Arr[2], 345.678f);
    }

    {
        Array<float, 3> Arr = { 123.456f, 234.567f, 345.678f };
        EXPECT_FLOAT_EQ(Arr[0], 123.456f);
        EXPECT_FLOAT_EQ(Arr[1], 234.567f);
        EXPECT_FLOAT_EQ(Arr[2], 345.678f);
    }

    {
        Array<float, 3> Arr = { 123.456f, 234.567f, 345.678f };
        int Bytes = Array<float, 3>::BYTES;
        EXPECT_EQ(Bytes, 3 * sizeof(float));
    }

    {
        Array<float, 3> Arr = { 123.456f, 234.567f, 345.678f };
        Array<int, 4> Arr2 = { 123, 234, 345, 456 };

        Arr = Arr2;
        EXPECT_FLOAT_EQ(Arr[0], 123.f);
        EXPECT_FLOAT_EQ(Arr[1], 234.f);
        EXPECT_FLOAT_EQ(Arr[2], 345.f);

        Array<double, 3> Arr3 = { 123.123, 234.234, 345.345 };
        Arr = Arr3;
        EXPECT_FLOAT_EQ(Arr[0], 123.123f);
        EXPECT_FLOAT_EQ(Arr[1], 234.234f);
        EXPECT_FLOAT_EQ(Arr[2], 345.345f);
    }
}

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
        int Bytes = Array<float, 3>::BYTES;
        EXPECT_EQ(Bytes, 3 * sizeof(float));
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
