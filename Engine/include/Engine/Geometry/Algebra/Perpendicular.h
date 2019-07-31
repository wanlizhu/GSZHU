#pragma once

#include "Engine/Common/Math.h"

namespace ZHU
{
    template<typename T>
    class Perpendicular
    {
    public:
        Vector2<T> operator()(const Vector2<T>& vec) const
        {
            return Vector2<T>{ vec[1], -vec[0] };
        }
    };
}