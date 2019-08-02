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




    template<typename T, int N, bool _ColumnMajor>
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

        Vector() : Base() {}
        template<typename U>
        Vector(const std::initializer_list<U>& init) : Base(init) {}
        template<typename ... ARGS>
        Vector(const ARGS& ... args) : Base(std::forward<const ARGS&>(args) ...) {}

    };
}