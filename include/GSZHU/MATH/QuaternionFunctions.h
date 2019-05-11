#pragma once

#include "SQuaternion.h"
#include "SMatrix.h"
#include "SVector.h"

namespace GSZHU {
    template<typename T>
    SMatrix<T, 4, 4> ToMatrix(const SQuaternion<T>& Quat) noexcept {

    }

    template<typename T>
    SQuaternion<T> ToQuaternion(const SMatrix<T, 4, 4>& Mat) noexcept {

    }
}