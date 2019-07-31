#pragma once

#include <stdint.h>
#include <cassert>

namespace ZHU
{
    template<bool _ColumnMajor, typename T, int... DIMS>
    class ArrayView {};


    // The array dimensions are known only at run time.
    template<bool _ColumnMajor, typename T>
    class ArrayView<_ColumnMajor, T>
    {
    public:
        inline ArrayView(int rows, int cols, T* matrix)
            : mNumRows(rows)
            , mNumCols(cols)
            , mMatrix(matrix)
        {}

        inline int GetNumRows() const { return mNumRows; }
        inline int GetNumCols() const { return mNumCols; }
        inline       T& operator()(int row, int col) 
        {
            assert(mMatrix != nullptr);
            assert(0 <= row && 0 <= col && (row * col < mNumRows*mNumCols));
            return _ColumnMajor ? mMatrix[row + mNumRows*col] : mMatrix[col + mNumRows * row];
        }
        inline const T& operator()(int row, int col) const 
        {
            assert(mMatrix != nullptr);
            assert(0 <= row && 0 <= col && (row * col < mNumRows * mNumCols));
            return _ColumnMajor ? mMatrix[row + mNumRows * col] : mMatrix[col + mNumRows * row];
        }

    private:
        int mNumRows = 0;
        int mNumCols = 0;
         T* mMatrix  = nullptr;
    };

    // The array dimensions are known at compile time.
    template<bool _ColumnMajor, typename T, int _NumRows, int _NumCols>
    class ArrayView<_ColumnMajor, T, _NumRows, _NumCols>
        : public ArrayView<_ColumnMajor, T>
    {
    public:
        inline ArrayView(T* matrix) 
            : ArrayView<_ColumnMajor, T>(_NumRows, _NumCols, matrix)
        {}
    };
}