#pragma once

#include "Geometry/Algebra/MatrixBase.h"
#include "Geometry/Algebra/Vector.h"
#include "Geometry/Algebra/Vector2.h"
#include "Geometry/Algebra/Vector3.h"
#include "Geometry/Algebra/Vector4.h"

namespace ZHU
{
    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor = true>
    class Matrix : public MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>
    {
    public:
        using Base = MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>;
        using ColumnVector = Vector<T, _NumRows, true>;
        using    RowVector = Vector<T, _NumCols, false>;

        Matrix() 
            : Base() {}
        template<typename U>
        Matrix(const std::initializer_list<U>& init) 
            : Base(init) {}
        template<typename ... ARGS>
        Matrix(const ARGS& ... args) 
            : Base(std::forward<const ARGS&>(args) ...) {}

        inline void SetRow(int r, const    RowVector& vec);
        inline void SetCol(int c, const ColumnVector& vec);
        inline    RowVector GetRow(int r) const;
        inline ColumnVector GetCol(int c) const;

        inline Matrix& MakeZero();
        inline Matrix& MakeUnit(int r, int c); // Component (r,c) is 1, all others zero.
        inline Matrix& MakeIdentity();
        static Matrix  Zero();
        static Matrix  Unit(int r, int c);
        static Matrix  Identity();
    };















    /*
                        TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                               CLASS : Matrix
                           ARGUMENTS : <typename T, int _NumRows, int _NumCols, bool _ColumnMajor = true>
                         GENERAL IMP : YES
              PARTIAL SPECIALIZATION : NO
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */

    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline void Matrix<T, _NumRows, _NumCols, _ColumnMajor>::SetRow(int r, const RowVector& vec)
    {
        for (int i = 0; i < _NumCols; i++) {
            this->operator()(r, i) = vec[i];
        }
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline void Matrix<T, _NumRows, _NumCols, _ColumnMajor>::SetCol(int c, const ColumnVector& vec)
    {
        for (int i = 0; i < _NumRows; i++) {
            this->operator()(i, c) = vec[i];
        }
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline typename Matrix<T, _NumRows, _NumCols, _ColumnMajor>::RowVector 
        Matrix<T, _NumRows, _NumCols, _ColumnMajor>::GetRow(int r) const
    {
        RowVector vec;
        for (int i = 0; i < RowVector::NumColumns; i++) {
            vec[i] = this->operator()(r, i);
        }
        return vec;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline typename Matrix<T, _NumRows, _NumCols, _ColumnMajor>::ColumnVector
        Matrix<T, _NumRows, _NumCols, _ColumnMajor>::GetCol(int c) const
    {
        ColumnVector vec;
        for (int i = 0; i < RowVector::NumRows; i++) {
            vec[i] = this->operator()(i, c);
        }
        return vec;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline Matrix<T, _NumRows, _NumCols, _ColumnMajor>& 
        Matrix<T, _NumRows, _NumCols, _ColumnMajor>::MakeZero()
    {
        for (int i = 0; i < _NumRows * _NumCols; i++) {
            this->mData[i] = T(0);
        }
        return *this;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline Matrix<T, _NumRows, _NumCols, _ColumnMajor>&
        Matrix<T, _NumRows, _NumCols, _ColumnMajor>::MakeUnit(int r, int c) 
    {
        MakeZero();
        this->operator()(r, c) = T(1);
        return *this;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline Matrix<T, _NumRows, _NumCols, _ColumnMajor>&
        Matrix<T, _NumRows, _NumCols, _ColumnMajor>::MakeIdentity()
    {
        for (int r = 0; r < _NumRows; r++) {
            for (int c = 0; c < _NumCols; c++) {
                this->operator()(r, c) = (r == c ? (T)1 : (T)0);
            }
       }
        return *this;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    static Matrix<T, _NumRows, _NumCols, _ColumnMajor>
        Matrix<T, _NumRows, _NumCols, _ColumnMajor>::Zero()
    {
        static Matrix smMat;
        static bool smInited = false;
        if (!smInited) {
            smInited = true;
            smMat.MakeZero();
        }
        return smMat;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    static Matrix<T, _NumRows, _NumCols, _ColumnMajor> 
        Matrix<T, _NumRows, _NumCols, _ColumnMajor>::Unit(int r, int c)
    {
        static Matrix smMat;
        smMat.MakeUnit(r, c);
        return smMat;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    static Matrix<T, _NumRows, _NumCols, _ColumnMajor> 
        Matrix<T, _NumRows, _NumCols, _ColumnMajor>::Identity()
    {
        static Matrix smMat;
        static bool smInited = false;
        if (!smInited) {
            smInited = true;
            smMat.MakeIdentity();
        }
        return smMat;
    }
}