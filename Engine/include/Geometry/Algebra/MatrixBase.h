#pragma once

#include <array>
#include <type_traits>
#include <cassert>
#include <cmath>

namespace ZHU
{
    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor = true>
    class MatrixBase 
    {
    public:
        using Element = T;
        static const int        NumRows = _NumRows;
        static const int     NumColumns = _NumCols;
        static const bool IsColumnMajor = _ColumnMajor;

        MatrixBase();
        template<typename U>
        MatrixBase(const std::initializer_list<U>& init);
        template<typename... ARGS>
        MatrixBase(const ARGS& ... args);

        inline       T& operator[](int i)       { return mData[i]; }
        inline const T& operator[](int i) const { return mData[i]; }
        inline       T& operator()(int r, int c);
        inline const T& operator()(int r, int c) const;
        inline     bool operator==(const MatrixBase& other) const;
        inline     bool operator!=(const MatrixBase& other) const { return !operator==(other); }
        inline     bool operator< (const MatrixBase& other) const;
        inline     bool operator<=(const MatrixBase& other) const { return  operator< (other) || operator==(other); }
        inline     bool operator> (const MatrixBase& other) const { return !operator<=(other); }
        inline     bool operator>=(const MatrixBase& other) const { return !operator< (other); }

        // Componentwise algebraic operations.
        inline MatrixBase operator-() const;
        inline MatrixBase operator+(const MatrixBase& mat) const;
        inline MatrixBase operator-(const MatrixBase& mat) const;
        inline MatrixBase operator*(const MatrixBase& mat) const;
        inline MatrixBase operator/(const MatrixBase& mat) const;
        inline MatrixBase operator*(const          T& num) const;
        inline MatrixBase operator/(const          T& num) const;
        friend inline MatrixBase operator*(const T& num, const MatrixBase& mat) { return mat * num; }

        inline MatrixBase& operator+=(const MatrixBase& mat);
        inline MatrixBase& operator-=(const MatrixBase& mat);
        inline MatrixBase& operator*=(const MatrixBase& mat);
        inline MatrixBase& operator/=(const MatrixBase& mat);
        inline MatrixBase& operator*=(const          T& num);
        inline MatrixBase& operator/=(const          T& num);

    public:
        T mData[_NumRows*_NumCols];
    };










    /*
                        TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                               CLASS : MatrixBase
                           ARGUMENTS : <typename T, int _NumRows, int _NumCols, bool _ColumnMajor = true>
                         GENERAL IMP : YES
              PARTIAL SPECIALIZATION : NO
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */

    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::MatrixBase() 
    {
        static_assert(_NumRows >= 1 && _NumCols >= 1, "");
        std::memset(mData, 0, sizeof(T) * _NumRows * _NumCols);
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    template<typename U>
    MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::MatrixBase(const std::initializer_list<U>& init)
    {
        static_assert(std::is_constructible_v<T, U>, "");
        assert(init.size() >= _NumRows*_NumCols);
        int i = 0;
        for (auto it = init.begin(); it != init.end(); it++) {
            mData[i++] = (T)*it;
        }
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    template<typename... ARGS>
    MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::MatrixBase(const ARGS& ... args)
    {
        int index = 0;
        auto push_back = [&](const T& num) {
            assert(index < _NumRows * _NumCols);
            mData[index++] = num;
        };

        static_assert((std::is_constructible_v<T, const ARGS&> && ...));
        (push_back((T)std::forward<const ARGS&>(args)), ...);
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline T& MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator()(int r, int c)
    {
        if constexpr (_ColumnMajor) {
            return operator[](r + c * _NumRows);
        }
        else {
            return operator[](c + r * _NumCols);
        }
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline const T& MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator()(int r, int c) const
    {
        if constexpr (_ColumnMajor) {
            return operator[](r + c * _NumRows);
        }
        else {
            return operator[](c + r * _NumCols);
        }
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline bool MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator==(const MatrixBase& other) const
    {
        for (int i = 0; i < _NumRows * _NumCols; i++) {
            if constexpr (std::is_floating_point_v<T>) {
                if (std::abs(mData[i] - other.mData[i]) >= std::numeric_limits<T>::epsilon())
                    return false;
            }
            else {
                if (!(mData[i] == other.mData[i]))
                    return false;
            }
        }
        return true;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline bool MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator< (const MatrixBase& other) const
    {
        for (int i = 0; i < _NumRows * _NumCols; i++) {
            if (!(mData[i] < other.mData[i]))
                return false;
        }
        return true;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>
        MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator-() const
    {
        MatrixBase mat;
        for (int i = 0; i < _NumRows * _NumCols; i++) {
            mat.mData[i] = -mData[i];
        }
        return mat;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline MatrixBase<T, _NumRows, _NumCols, _ColumnMajor> 
        MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator+(const MatrixBase& mat) const
    {
        MatrixBase res;
        for (int i = 0; i < _NumRows * _NumCols; i++) {
            res.mData[i] = mData[i] + mat.mData[i];
        }
        return res;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>
        MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator-(const MatrixBase& mat) const
    {
        MatrixBase res;
        for (int i = 0; i < _NumRows * _NumCols; i++) {
            res.mData[i] = mData[i] - mat.mData[i];
        }
        return res;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>
        MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator*(const MatrixBase& mat) const
    {
        MatrixBase res;
        for (int i = 0; i < _NumRows * _NumCols; i++) {
            res.mData[i] = mData[i] * mat.mData[i];
        }
        return res;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>
        MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator/(const MatrixBase& mat) const
    {
        MatrixBase res;
        for (int i = 0; i < _NumRows * _NumCols; i++) {
            res.mData[i] = mData[i] / mat.mData[i];
        }
        return res;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>
        MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator*(const T& num) const
    {
        MatrixBase res;
        for (int i = 0; i < _NumRows * _NumCols; i++) {
            res.mData[i] = mData[i] * num;
        }
        return res;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline MatrixBase<T, _NumRows, _NumCols, _ColumnMajor> 
        MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator/(const T& num) const
    {
        MatrixBase res;
        for (int i = 0; i < _NumRows * _NumCols; i++) {
            res.mData[i] = mData[i] / num;
        }
        return res;
    }


    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>& 
        MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator+=(const MatrixBase& mat)
    {
        *this = *this + mat;
        return *this;
    }
    

    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>& 
        MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator-=(const MatrixBase& mat)
    {
        *this = *this - mat;
        return *this;
    }
    
    
    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>& 
        MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator*=(const MatrixBase& mat)
    {
        *this = *this * mat;
        return *this;
    }
    
    
    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>&
        MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator/=(const MatrixBase& mat)
    {
        *this = *this / mat;
        return *this;
    }
    
    
    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>&
        MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator*=(const T& num)
    {
        *this = *this * num;
        return *this;
    }
    

    template<typename T, int _NumRows, int _NumCols, bool _ColumnMajor>
    inline MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>&
        MatrixBase<T, _NumRows, _NumCols, _ColumnMajor>::operator/=(const T& num)
    {
        *this = *this / num;
        return *this;
    }
}