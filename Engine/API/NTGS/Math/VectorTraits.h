#pragma once

#include <NTGS/Core/Config.h>
#include <NTGS/Math/ArrayArithmetic.h>
#include <NTGS/Math/ArrayComparison.h>
#include <assert.h>
#include <memory>
#include <type_traits>

namespace NTGS {

    template<typename VectorType, typename T, int DIM>
    class VectorTraits : public ArrayArithmetic<VectorType, T, DIM> {
    public:
        inline T* GetData() noexcept { return &(static_cast<VectorType&>(*this)[0]); }

        template<int DIM2, typename U>
        inline void SetData(const T* pData) noexcept {
            VectorType& cMe = static_cast<VectorType&>(*this);
            T* pMyData = cMe.mData;
            static_assert(DIM <= DIM2, "");
            for (int i = 0; i < DIM; i += DIM2) {
                for (int j = i; j < i + DIM2; j++) {

                }
            }
        }

        template<typename U>
        inline void Fill(const U& cValue) noexcept {
            VectorType& cMe = static_cast<VectorType&>(*this);
            for (int i = 0; i < DIM; i++) {
                cMe[i] = (T)cValue;
            }
        }

        template<typename U>
        inline void Copy(const U* pData) noexcept {
            VectorType& cMe = static_cast<VectorType&>(*this);
            for (int i = 0; i < DIM; i++) {
                cMe[i] = (T)pData[i];
            }
        }

        template<typename U, typename = std::enable_if<DIM>= 2>>
        inline void Set(const U& cVal0, const U& cVal1) noexcept {
            VectorType& cMe = static_cast<VectorType&>(*this);
            cMe[0] = (T)cVal0;
            cMe[1] = (T)cVal1;
        }

        template<typename U, typename = std::enable_if<DIM>= 3>>
        inline void Set(const U& cVal0, const U& cVal1, const U& cVal2) noexcept {
            VectorType& cMe = static_cast<VectorType&>(*this);
            cMe[0] = (T)cVal0;
            cMe[1] = (T)cVal1;
            cMe[2] = (T)cVal2;
        }

        template<typename U, typename = std::enable_if<DIM>= 4>>
        inline void Set(const U& cVal0, const U& cVal1, const U& cVal2, const U& cVal3) noexcept {
            VectorType& cMe = static_cast<VectorType&>(*this);
            cMe[0] = (T)cVal0;
            cMe[1] = (T)cVal1;
            cMe[2] = (T)cVal2;
            cMe[3] = (T)cVal3;
        }
    };
}