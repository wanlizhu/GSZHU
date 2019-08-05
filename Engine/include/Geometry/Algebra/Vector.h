#pragma once

#include "MatrixBase.h"

namespace ZHU
{
    template<bool cond, typename T0, typename T1>
    struct enable_if_else
    {};

    template<typename T0, typename T1>
    struct enable_if_else<true, T0, T1>
    { using type = T0; };

    template<typename T0, typename T1>
    struct enable_if_else<false, T0, T1>
    { using type = T1; };




    /* The functions with 'robust' set to 'false' use the
     * standard algorithm for normalizing a vector by computing the length as a
     * square root of the squared length and dividing by it.  The results can be
     * infinite (or NaN) if the length is zero.
     * When 'robust' is set to 'true', the algorithm is designed to avoid floating-point overflow
     * and sets the normalized vector to zero when the length is zero.
     */
    template<typename T, int N, bool _ColumnMajor = true>
    class Vector : public enable_if_else<_ColumnMajor,
                                         MatrixBase<T, N, 1, true>, 
                                         MatrixBase<T, 1, N, false>>::type
    {
    public:
        using Base = typename enable_if_else<_ColumnMajor,
                                             MatrixBase<T, N, 1, true>,
                                             MatrixBase<T, 1, N, false>>::type;
        static const  int Length = N;
        static const bool IsColumnMajor = _ColumnMajor;

        Vector()
            : Base() {}
        template<typename U>
        Vector(const std::initializer_list<U>& init) 
            : Base(init) {}
        template<typename ... ARGS>
        Vector(const ARGS& ... args) 
            : Base(std::forward<const ARGS&>(args) ...) {}

        inline Vector& MakeZero();
        inline Vector& MakeOnes();
        inline Vector& MakeUnit(int dim); // Component dim is 1, all others are zero.
    };




    /* Geometric operations.  
     * The functions with 'robust' set to 'false' use the
     * standard algorithm for normalizing a vector by computing the length as a
     * square root of the squared length and dividing by it.  The results can be
     * infinite (or NaN) if the length is zero.  When 'robust' is set to 'true',
     * the algorithm is designed to avoid floating-point overflow and sets the
     * normalized vector to zero when the length is zero.
     */
    template<typename T, int N, bool CM = true>
    T Dot(const Vector<T, N, CM>& v0, const Vector<T, N, CM>& v1);


    template<typename T, int N, bool CM = true>
    T Length(const Vector<T, N, CM>& v, bool robust = false);


    template<typename T, int N, bool CM = true>
    T Normalize(Vector<T, N, CM>& v, bool robust = false);


    /* Gram-Schmidt orthonormalization 
     * to generate orthonormal vectors from the linearly independent inputs.  
     * The function returns the smallest length of
     * the unnormalized vectors computed during the process.  
     * If this value is nearly zero, it is possible that the inputs are linearly dependent (within
     * numerical round-off errors).  
     * On input, 1 <= numElements <= N and v[0] through v[numElements-1] must be initialized. 
     * On output, the vectors v[0] through v[numElements-1] form an orthonormal set.
     */
    template<typename T, int N, bool CM = true>
    T Orthonormalize(int numElements, Vector<T, N, CM>* v, bool robust = false);


    /* Construct a single vector orthogonal to the nonzero input vector.  
     * If the maximum absolute component occurs at index i, 
     * then the orthogonal vector U has u[i] = v[i+1], u[i+1] = -v[i], 
     * and all other components zero.  The index addition i+1 is computed modulo N.
     */
    template<typename T, int N, bool CM = true>
    Vector<T, N, CM> GetOrtho(const Vector<T, N, CM>& v, bool unitLen);


    /* Compute the axis-aligned bounding box of the vectors.  
     * The return value is 'true' iff the inputs are valid, 
     * in which case vmin and vmax have valid values.
     */
    template<typename T, int N, bool CM = true>
    bool ComputeExtremes(int numVectors, const Vector<T, N, CM>* v,
                         Vector<T, N, CM>& vmin,
                         Vector<T, N, CM>& vmax);















    /*
                        TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                               CLASS : Vector
                           ARGUMENTS : <typename T, int N, bool _ColumnMajor = true>
                         GENERAL IMP : YES
              PARTIAL SPECIALIZATION : NO
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */

    template<typename T, int N, bool _ColumnMajor>
    inline Vector<T, N, _ColumnMajor>& 
        Vector<T, N, _ColumnMajor>::MakeZero()
    {
        for (int i = 0; i < _NumRows * _NumCols; i++) {
            mData[i] = (T)0;
        }
        return *this;
    }


    template<typename T, int N, bool _ColumnMajor>
    inline Vector<T, N, _ColumnMajor>&
        Vector<T, N, _ColumnMajor>::MakeOnes()
    {
        for (int i = 0; i < _NumRows * _NumCols; i++) {
            mData[i] = (T)1;
        }
        return *this;
    }


    template<typename T, int N, bool _ColumnMajor>
    inline Vector<T, N, _ColumnMajor>&
        Vector<T, N, _ColumnMajor>::MakeUnit(int dim) // Component dim is 1, all others are zero.
    {
        MakeZero();
        mData[dim] = (T)1;
        return *this;
    }









    template<typename T, int N, bool CM = true>
    T Dot(const Vector<T, N, CM>& v0, const Vector<T, N, CM>& v1)
    {
        T dot = v0[0] * v1[0];
        for (int i = 1; i < N; ++i) {
            dot += v0[i] * v1[i];
        }
        return dot;
    }


    template<typename T, int N, bool CM = true>
    T Length(const Vector<T, N, CM>& v, bool robust)
    {
        if (robust) {
            T maxAbsComp = std::abs(v[0]);
            for (int i = 1; i < N; ++i) {
                T absComp = std::abs(v[i]);
                if (absComp > maxAbsComp) {
                    maxAbsComp = absComp;
                }
            }

            T length;
            if (maxAbsComp > (T)0) {
                Vector<N, T, CM> scaled = v / maxAbsComp;
                length = maxAbsComp * std::sqrt(Dot(scaled, scaled));
            }
            else {
                length = (T)0;
            }
            return length;
        }
        else {
            return std::sqrt(Dot(v, v));
        }
    }


    template<typename T, int N, bool CM = true>
    T Normalize(Vector<T, N, CM>& v, bool robust)
    {
        if (robust) {
            T maxAbsComp = std::abs(v[0]);
            for (int i = 1; i < N; ++i) {
                T absComp = std::abs(v[i]);
                if (absComp > maxAbsComp) {
                    maxAbsComp = absComp;
                }
            }

            T length;
            if (maxAbsComp > (T)0) {
                v /= maxAbsComp;
                length = std::sqrt(Dot(v, v));
                v /= length;
                length *= maxAbsComp;
            }
            else {
                length = (T)0;
                for (int i = 0; i < N; ++i) {
                    v[i] = (T)0;
                }
            }
            return length;
        }
        else {
            T length = std::sqrt(Dot(v, v));
            if (length > (T)0) {
                v /= length;
            }
            else {
                for (int i = 0; i < N; ++i) {
                    v[i] = (T)0;
                }
            }
            return length;
        }
    }


    template<typename T, int N, bool CM = true>
    T Orthonormalize(int numElements, Vector<T, N, CM>* v, bool robust)
    {
        if (v && 1 <= numInputs && numInputs <= N) {
            T minLength = Normalize(v[0], robust);
            for (int i = 1; i < numInputs; ++i) {
                for (int j = 0; j < i; ++j) {
                    T dot = Dot(v[i], v[j]);
                    v[i] -= v[j] * dot;
                }
                T length = Normalize(v[i], robust);
                if (length < minLength) {
                    minLength = length;
                }
            }
            return minLength;
        }

        return (T)0;
    }



    template<typename T, int N, bool CM = true>
    Vector<T, N, CM> GetOrtho(const Vector<T, N, CM>& v, bool unitLen)
    {
        T cmax = std::abs(v[0]);
        int imax = 0;
        for (int i = 1; i < N; ++i) {
            T c = std::abs(v[i]);
            if (c > cmax) {
                cmax = c;
                imax = i;
            }
        }

        Vector<T, N, CM> result;
        result.MakeZero();
        int inext = imax + 1;
        if (inext == N) {
            inext = 0;
        }
        result[imax] = v[inext];
        result[inext] = -v[imax];
        if (unitLen) {
            T sqrDistance = result[imax] * result[imax] + result[inext] * result[inext];
            T invLength = ((T)1) / std::sqrt(sqrDistance);
            result[imax] *= invLength;
            result[inext] *= invLength;
        }
        return result;
    }



    template<typename T, int N, bool CM = true>
    bool ComputeExtremes(int numVectors, const Vector<T, N, CM>* v,
                         Vector<T, N, CM>& vmin,
                         Vector<T, N, CM>& vmax)
    {
        if (v && numVectors > 0) {
            vmin = v[0];
            vmax = vmin;
            for (int j = 1; j < numVectors; ++j) {
                const Vector<T, N, CM>& vec = v[j];
                for (int i = 0; i < N; ++i) {
                    if (vec[i] < vmin[i]) {
                        vmin[i] = vec[i];
                    }
                    else if (vec[i] > vmax[i]) {
                        vmax[i] = vec[i];
                    }
                }
            }
            return true;
        }

        return false;
    }
}