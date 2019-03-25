#pragma once

#include "Common/Math.h"

namespace NTGS {
    template<typename T>
    inline Matrix<T, 4, 4> Translate(const Vector<T, 3>& vec) {
        
    }

    template<typename T>
    inline Matrix<T, 4, 4> Rotate(const T& angle, const Vector<T, 3>& axis) {
    
    }

    template<typename T>
    inline Matrix<T, 4, 4> Scale(const Vector<T, 3>& vec) {
    
    }

    template<typename T>
    inline Matrix<T, 4, 4> LookAt(const Vector<T, 3>& eye, const Vector<T, 3>& center, const Vector<T, 3>& up) {
    
    }

    template<typename T>
    inline Matrix<T, 4, 4> Perspective(T fovy, T aspect, T zNear, T zFar) {
        
    }

    template<typename T>
    inline Matrix<T, 4, 4> Ortho(T left, T right, T bottom, T top) {
    
    }
}