#pragma once

#include "MatrixBase.h"
#include "Vector.h"

namespace ZHU
{
    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor = true>
    class Matrix : public MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>
    {
    public:
        using Base = MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>;
        using Vector = Vector<T, _ColumnMajor ? _NumRows : _NumCols, _ColumnMajor>;
        static const int IsColumnMajor = _ColumnMajor;

        Matrix() 
            : Base() {}
        template<typename U>
        Matrix(const std::initializer_list<U>& init) 
            : Base(init) {}
        template<typename ... ARGS>
        Matrix(const ARGS& ... args) 
            : Base(std::forward<const ARGS&>(args) ...) {}

    };
}