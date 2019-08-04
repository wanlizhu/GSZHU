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
    
        inline Vector& make_zero();
        inline Vector& make_ones();
        inline Vector& make_unit(int dim); // Component dim is 1, all others are zero.
    };










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
        Vector<T, N, _ColumnMajor>::make_zero()
    {
        for (int i = 0; i < _NumRows * _NumCols; i++) {
            mData[i] = (T)0;
        }
        return *this;
    }


    template<typename T, int N, bool _ColumnMajor>
    inline Vector<T, N, _ColumnMajor>&
        Vector<T, N, _ColumnMajor>::make_ones()
    {
        for (int i = 0; i < _NumRows * _NumCols; i++) {
            mData[i] = (T)1;
        }
        return *this;
    }


    template<typename T, int N, bool _ColumnMajor>
    inline Vector<T, N, _ColumnMajor>&
        Vector<T, N, _ColumnMajor>::make_unit(int dim) // Component dim is 1, all others are zero.
    {
        make_zero();
        mData[dim] = (T)1;
        return *this;
    }

    inline      T      length(bool robust = false) const;
    inline Vector& normalize(bool robust = false);
    inline      T         dot(const Vector& v)     const;
    inline Vector       cross(const Vector& v)     const;
    inline Vector     proj_to(const Vector& v)     const;


    template<typename T, int N, bool _ColumnMajor>
    inline T Vector<T, N, _ColumnMajor>::length(bool robust) const
    {
        if (robust)
        {
            T max_abs_comp = std::abs(mData[0]);
            for (int i = 1; i < N; ++i) {
                T abs_comp = std::abs(v[i]);
                if (abs_comp > max_abs_comp) {
                    max_abs_comp = abs_comp;
                }
            }

            T len;
            if (max_abs_comp > (T)0) {
                Vector scaled = *this / max_abs_comp;
                len = max_abs_comp * std::sqrt(scaled.dot(scaled));
            }
            else {
                len = (T)0;
            }
            return len;
        }
        else
        {
            return std::sqrt(this->dot(*this));
        }
    }


    template<typename T, int N, bool _ColumnMajor>
    inline Vector<T, N, _ColumnMajor>  
        Vector<T, N, _ColumnMajor>::normalized(bool robust) const
    {

    }


    template<typename T, int N, bool _ColumnMajor>
    inline Vector<T, N, _ColumnMajor>& Vector<T, N, _ColumnMajor>::normalize(bool robust = false);


    template<typename T, int N, bool _ColumnMajor>
    inline      T         Vector<T, N, _ColumnMajor>::dot(const Vector& v)     const;


    template<typename T, int N, bool _ColumnMajor>
    inline Vector<T, N, _ColumnMajor>       Vector<T, N, _ColumnMajor>::cross(const Vector& v)     const;


    template<typename T, int N, bool _ColumnMajor>
    inline Vector<T, N, _ColumnMajor>     Vector<T, N, _ColumnMajor>::proj_to(const Vector& v)     const;
}