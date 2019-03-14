#pragma once

#include <NTGS/Core/Config.h>
#include <NTGS/Math/ArrayArithmetic.h>
#include <NTGS/Math/ArrayComparison.h>
#include <assert.h>
#include <memory>
#include <type_traits>

namespace NTGS {

    template<typename VectorType, typename T, int DIMENSION>
    class VectorTraits : public ArrayArithmetic<VectorType, T, DIMENSION> {
    protected:
        template<typename U>
        inline void Fill(const U& cValue) noexcept {
            VectorType& cMe = static_cast<VectorType&>(*this);
            for (int i = 0; i < DIMENSION; i++) {
                cMe[i] = (T)cValue;
            }
        }

        template<typename U>
        inline void Copy(const U* pData) noexcept {
            VectorType& cMe = static_cast<VectorType&>(*this);
            for (int i = 0; i < DIMENSION; i++) {
                cMe[i] = (T)pData[i];
            }
        }

        template<typename U, typename = std::enable_if<DIMENSION>= 2>>
        inline void Set(const U& cVal0, const U& cVal1) noexcept {
            VectorType& cMe = static_cast<VectorType&>(*this);
            cMe[0] = (T)cVal0;
            cMe[1] = (T)cVal1;
        }

        template<typename U, typename = std::enable_if<DIMENSION>= 3>>
        inline void Set(const U& cVal0, const U& cVal1, const U& cVal2) noexcept {
            VectorType& cMe = static_cast<VectorType&>(*this);
            cMe[0] = (T)cVal0;
            cMe[1] = (T)cVal1;
            cMe[2] = (T)cVal2;
        }

        template<typename U, typename = std::enable_if<DIMENSION>= 4>>
        inline void Set(const U& cVal0, const U& cVal1, const U& cVal2, const U& cVal3) noexcept {
            VectorType& cMe = static_cast<VectorType&>(*this);
            cMe[0] = (T)cVal0;
            cMe[1] = (T)cVal1;
            cMe[2] = (T)cVal2;
            cMe[3] = (T)cVal3;
        }
    };
}