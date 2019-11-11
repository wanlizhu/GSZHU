#pragma once

#include "Common.h"

namespace GA2
{
    template<typename _ShapeA_, typename _ShapeB_>
    class DCPQuery
    {
    public:
        static_assert(std::is_same_v<typename _ShapeA_::Scalar, typename _ShapeB_::Scalar>);
        using Scalar = typename _ShapeA_::Scalar;
        using ShapeA = _ShapeA_;
        using ShapeB = _ShapeB_;

        struct Result {};

        Result operator()(const ShapeA& shapeA, const ShapeB& shapeB);
    };
}