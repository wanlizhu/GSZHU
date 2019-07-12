#pragma once
#include <Common/Utilities/BasicTypes.h>

namespace ZHU
{
    Mat4f LookAt(const Vec3f& target, const Vec3f& eye, const Vec3f& up);
}