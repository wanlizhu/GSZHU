#pragma once

#include <NTGS/Math/ArithmeticOperator.h>
#include <NTGS/Math/ComparisonOperator.h>
#include <assert.h>

namespace NTGS {

    template<typename T, int _ROWS, int _COLS, bool _COLUMN_FIRST = true>
    class Array : public ArithmeticOperator<Array<T, _ROWS, _COLS, _COLUMN_FIRST>>
                , public ComparisonOperator<Array<T, _ROWS, _COLS, _COLUMN_FIRST>> {
    public:
        T mData[_ROWS * _COLS];

    public:
        struct IndexType {
            int mRow = 0, mCol = 0;

            IndexType() = default;
            IndexType(int Row, int Col) : mRow(Row), mCol(Col) {}

            inline IndexType& operator++() { Increase(); return *this; }
            inline IndexType operator++(int) { auto Old = *this; Increase(); return Old; }
            inline operator bool() const noexcept { return (mRow < _ROWS) && (mCol < _COLS); }
            inline int operator*() const noexcept { return this->operator int(); }
            inline operator int() const noexcept { 
                if (_COLUMN_FIRST)
                    return _ROWS * mCol + mRow;
                else 
                    return _COLS * mRow + mCol;
            }
            inline void Increase() noexcept {
                int& Major = _COLUMN_FIRST ? mCol : mRow;
                int MajorMax = _COLUMN_FIRST ? _COLS : _ROWS;
                int& Minor = _COLUMN_FIRST ? mRow : mCol;
                int MinorMax = _COLUMN_FIRST ? _ROWS : _COLS;

                if (Minor < MinorMax)
                    Minor++;
                else {
                    Major++;
                    Minor = 0;
                }
            }
        };

        typedef T Scalar;
        static constexpr int ATTRIBS = _ATTRIBS;
        static constexpr int ROWS = _ROWS;
        static constexpr int COLS = _COLS;
        static constexpr int LENGTH = _ROWS * _COLS;
        static constexpr int BYTES = _ROWS * _COLS * sizeof(Scalar);
        static constexpr bool COLUMN_FIRST = _COLUMN_FIRST;

        Array() = default;
        Array(const Scalar& Value) { IndexType Index;  while (Index) { (*this)[Index++] = Value; } }
        Array(const Scalar* Data) { std::memcpy(mData, Data, BYTES); }
        Array(const Scalar& Val0, const Scalar& Val1) { mData[0] = Val0; mData[1] = Val1; }
        Array(const Scalar& Val0, const Scalar& Val1, const Scalar& Val2) { mData[0] = Val0; mData[1] = Val1; mData[2] = Val2; }
        Array(const Scalar& Val0, const Scalar& Val1, const Scalar& Val2, const Scalar& Val3) { mData[0] = Val0; mData[1] = Val1; mData[2] = Val2; mData[3] = Val3; }
        Array(const std::initializer_list<Scalar>& List) noexcept {
            assert(List.size() >= LENGTH);
            IndexType Index;
            for (auto& It = List.begin(); It != List.end(); It++, Index++) {
                (*this)[Index] = *It;
            }
        }

        inline operator bool() const { return HasNaN(); }
        inline Scalar& operator[](IndexType Index) { return mData[(int)Index]; }
        inline const Scalar& operator[](IndexType Index) const { return mData[(int) Index]; }
    };
}