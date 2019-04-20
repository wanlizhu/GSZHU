#pragma once

#include "math/Vector.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include <type_traits>

namespace NTGS {
    template<typename T, typename U, int Cols, int Rows>
    inline Matrix<T, Cols, Rows> Mul(const Matrix<T, Cols, Rows>& m1, const Matrix<U, Cols, Rows>& m2) {
        using Index = typename Matrix<T, Cols, Rows>::Index;
        Matrix<T, Cols, Rows> result;
        for (int i = 0; i < Cols; i++)
            for (int j = 0; j < Rows; j++)
                result.mArray[Index(i, j)] = m1.mArray[Index(i, j)] * m2.mArray[Index(i, j)];
        return result;
    }

    template<typename T, int N>
    inline T Length(const Vector<T, N>& vec) {
        T sum = 0;
        for (int i = 0; i < N; i++)
            sum += vec[i] * vec[i];
        return std::sqrt(sum);
    }

    template<typename T, int N>
    inline T LengthSquared(const Vector<T, N>& vec) {
        T sum = 0;
        for (int i = 0; i < N; i++)
            sum += vec[i] * vec[i];
        return sum;
    }

    template<typename T, int N>
    inline Vector<T, N> Normalize(const Vector<T, N>& vec) {
        return Vector<T, N>(vec) /= Length(vec);
    }

    template<typename T, int MN,
             typename = typename std::enable_if<(1 < MN)>::type>
    inline bool SetIdentity(Matrix<T, MN, MN>& m) {
        using Index = typename Matrix<T, MN, MN>::Index;
        for (int i = 0; i < MN; i++)
            for (int j = 0; j < MN; j++) {
                m.mArray[Index(i, j)] = (i == j ? 1 : 0);
            }
        return true;
    }

    template<typename T, int MN,
             typename = typename std::enable_if<(1 < MN)>::type>
    inline bool IsIdentity(const Matrix<T, MN, MN>& m) {
        for (int i = 0; i < MN; i++)
            for (int j = 0; j < MN; j++) {
                if (i == j && !FLOAT_EQ(m[i][j], T(1)))
                    return false;
                if (i != j && !FLOAT_EQ(m[i][j], T(0)))
                    return false;
            }
        return true;
    }

    template<typename T, typename U, int N>
    inline T operator*(const Vector<T, N>& v1, 
                       const Vector<U, N>& v2) {
        T sum = 0;
        for (int i = 0; i < N; i++)
            sum += v1[i] * v2[i];
        return sum;
    }

    template<typename T, typename U>
    inline Vector<T, 3> operator^(const Vector<T, 3>& v1,
                                  const Vector<U, 3>& v2) {
        return Vector<T, 3>(
            v1.y * v2.z - v2.y * v1.z,
            v1.z * v2.x - v2.z * v1.x,
            v1.x * v2.y - v2.x * v1.y);
    }

    // If dot(Nref, I) < 0.0 return N, otherwise return -N
    template<typename T, int N>
    inline Vector<T, N> FaceForward(const Vector<T, N>& vecN,
                                    const Vector<T, N>& I, const Vector<T, N>& Nref) {
        if (I * Nref < T(0))
            return vecN;
        else
            return -vecN;
    }

    // For the incident vector I and surface orientation N,
    // compute normalized N(NN), 
    // and return the reflection direction : I - 2 * dot(NN, I) * NN.
    template<typename T, int N>
    inline Vector<T, N> Reflect(const Vector<T, N>& I, const Vector<T, N>& vecN) {
        Vector<T, N> NN = Normalize(vecN);
        return I - 2 * (NN * I) * NN;
    }

    // For the incident vector I and surface normal N, and the ratio of indices of refraction eta, 
    // return the refraction vector.
    template<typename T, int N>
    inline Vector<T, N> Refract(const Vector<T, N>& I, const Vector<T, N>& vecN, T eta) {

    }
}
