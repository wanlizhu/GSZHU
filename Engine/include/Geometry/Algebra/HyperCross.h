#pragma once

#include "Engine/Common/Math.h"

namespace ZHU
{
    template<typename T>
    Vector4<T> HyperCross(const Vector4<T>& v0,
                          const Vector4<T>& v1,
                          const Vector4<T>& v2)
    {
        T m01 = v0[0] * v1[1] - v0[1] * v1[0];  // x0*y1 - y0*x1
        T m02 = v0[0] * v1[2] - v0[2] * v1[0];  // x0*z1 - z0*x1
        T m03 = v0[0] * v1[3] - v0[3] * v1[0];  // x0*w1 - w0*x1
        T m12 = v0[1] * v1[2] - v0[2] * v1[1];  // y0*z1 - z0*y1
        T m13 = v0[1] * v1[3] - v0[3] * v1[1];  // y0*w1 - w0*y1
        T m23 = v0[2] * v1[3] - v0[3] * v1[2];  // z0*w1 - w0*z1
        
        return Vector4<T>
        {
            +m23 * v2[1] - m13 * v2[2] + m12 * v2[3],  // +m23*y2 - m13*z2 + m12*w2
            -m23 * v2[0] + m03 * v2[2] - m02 * v2[3],  // -m23*x2 + m03*z2 - m02*w2
            +m13 * v2[0] - m03 * v2[1] + m01 * v2[3],  // +m13*x2 - m03*y2 + m01*w2
            -m12 * v2[0] + m02 * v2[1] - m01 * v2[2]   // -m12*x2 + m02*y2 - m01*z2
        };
    }
}