#pragma once

#include <NTGS/Core/Config.h>
#include <NTGS/Math/ArithmeticOperator.h>
#include <NTGS/Math/ComparisonOperator.h>
#include <assert.h>
#include <memory>

namespace NTGS {

    template<typename T, int _LENGTH>
    class ALIGN16 Array : public ArithmeticOperator<Array, T, _LENGTH>
                        , public ComparisonOperator<Array, T, _LENGTH> {
    public:
        ALIGN16 T mData[_LENGTH] = { 0 };

    public:
        typedef T Scalar;
        static constexpr int LENGTH = _LENGTH;
        static constexpr int BYTES = _LENGTH * sizeof(Scalar);

        Array() = default;
        Array(const Scalar& Value) { for (int i = 0; i < LENGTH; i++) mData[i] = Value; }
        Array(const Scalar* Data) { for (int i = 0; i < LENGTH; i++) mData[i] = Data[i]; }
        Array(const Scalar& Val0, const Scalar& Val1) { mData[0] = Val0; mData[1] = Val1; }
        Array(const Scalar& Val0, const Scalar& Val1, const Scalar& Val2) { mData[0] = Val0; mData[1] = Val1; mData[2] = Val2; }
        Array(const Scalar& Val0, const Scalar& Val1, const Scalar& Val2, const Scalar& Val3) { mData[0] = Val0; mData[1] = Val1; mData[2] = Val2; mData[3] = Val3; }
        Array(const std::initializer_list<Scalar>& List) noexcept {
            assert(List.size() >= LENGTH);
            for (int i = 0; i < LENGTH; i++) {
                mData[i] = *(List.begin() + i);
            }
        }
        template<typename U, int LEN>
        Array(const Array<U, LEN>& Arr) {
            static_assert(LENGTH <= LEN, "");
            for (int i = 0; i < LENGTH; i++) {
                mData[i] = (T)Arr[i];
            }
        }

        inline Scalar& operator[](int i) noexcept { return mData[i]; }
        inline const Scalar& operator[](int i) const noexcept { return mData[i]; }
    };
}