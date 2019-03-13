#pragma once

#include <NTGS/Core/Config.h>
#include <NTGS/Math/ArrayArithmetic.h>
#include <NTGS/Math/ArrayComparison.h>
#include <assert.h>
#include <memory>
#include <type_traits>

namespace NTGS {
    template<typename MatrixType>
    class MatrixOperators : public ArrayArithmetic<MatrixType>
                          , public ArrayComparison<MatrixType> {
    public:
        inline void Fill(const typename MatrixType::Vector& Vec) noexcept {
            MatrixType& Me = static_cast<MatrixType&>(*this);
            for (int i = 0; i < MatrixType::MAJOR; i++) {
                Me.mData[i] = Value;
            }
        }

        inline void Copy(const typename MatrixType::Vector* Data) noexcept {
            MatrixType& Me = static_cast<MatrixType&>(*this);
            for (int i = 0; i < MatrixType::MAJOR; i++) {
                Me.mData[i] = Data[i];
            }
        }
    };
}