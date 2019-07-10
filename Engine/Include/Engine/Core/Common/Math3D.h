#pragma once
#include <Engine/Core/Common/BasicPrimitives.h>

namespace ZHU
{
    Mat4f LookAt(const Point& target, const Vector& eye, const Vector& up);
}