#pragma once

#include <NTGS/Core/Config.h>
#include <NTGS/Math/ArrayArithmetic.h>
#include <NTGS/Math/ArrayComparison.h>
#include <NTGS/Math/Vector.h>
#include <assert.h>
#include <memory>
#include <type_traits>

namespace NTGS {
    
    template<typename MatrixType, typename T, int MAJOR, int MINOR>
    class MatrixTraits : public ArrayArithmetic<MatrixType, Vector<T, MINOR>, MAJOR * MINOR> {
    protected:
        template<typename U>
        inline void Fill(const Vector<U, MINOR>& cVec) noexcept {
            MatrixType& Me = static_cast<MatrixType&>(*this);
            for (int i = 0; i < MAJOR; i++) {
                Me[i] = cVec;
            }
        }

        template<typename U>
        inline void Copy(const Vector<U, MINOR>* pData) noexcept {
            MatrixType& Me = static_cast<MatrixType&>(*this);
            for (int i = 0; i < MAJOR; i++) {
                Me[i] = pData[i];
            }
        }
    };
}