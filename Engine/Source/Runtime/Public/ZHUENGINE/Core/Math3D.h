#pragma once
#include <ZHUENGINE/Core/BasicPrimitives.h>

namespace ZHU
{
    Mat4f LookAt(const Point& target, const Vector& eye, const Vector& up);
}