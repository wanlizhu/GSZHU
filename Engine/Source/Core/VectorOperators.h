#pragma once

#include "ArrayAccess.h"

namespace NTGS {
    template<typename Derived, typename T, int Dim>
    struct ArrayAddOperators : public ArrayAccess<Derived, T, Dim> {
    public:
        inline InstanceType operator-() const {
            InstanceType inst(GetInstance());
            for (int i = 0; i < DIMENSION; i++)
                inst[i] = -inst[i];
            return inst;
        }

        inline InstanceType& operator+=(const InstanceType& v) {
            InstanceType& me = GetInstance();
            for (int i = 0; i < DIMENSION; i++)
                me[i] += v[i];
            return me;
        }

        inline InstanceType& operator-=(const InstanceType& v) {
            InstanceType& me = GetInstance();
            for (int i = 0; i < DIMENSION; i++)
                me[i] -= v[i];
            return me;
        }

        inline InstanceType& operator+=(const T& num) {
            return GetInstance() += InstanceType(num);
        }

        inline InstanceType& operator-=(const T& num) {
            return GetInstance() -= InstanceType(num);
        }

        inline InstanceType operator+(const InstanceType& v) const {
            return InstanceType(GetInstance()) += v;
        }

        inline InstanceType operator-(const InstanceType& v) const {
            return InstanceType(GetInstance()) -= v;
        }

        inline InstanceType operator+(const T& num) const {
            return InstanceType(GetInstance()) += InstanceType(num);
        }

        inline InstanceType operator-(const T& num) const {
            return InstanceType(GetInstance()) -= InstanceType(num);
        }

        friend inline InstanceType operator+(const T& num, const InstanceType& v) {
            return InstanceType(num) += v;
        }

        friend inline InstanceType operator-(const T& num, const InstanceType& v) {
            return InstanceType(num) -= v;
        }
    };


    template<typename Derived, typename T, int Dim>
    struct NTGS_API ArrayMulOperators : public ArrayAddOperators<Derived, T, Dim> {
        inline InstanceType& operator*=(const T& num) {
            InstanceType& me(GetInstance());
            for (int i = 0; i < DIMENSION; i++)
                me[i] *= num;
            return me;
        }

        inline InstanceType& operator/=(const T& num) {
            InstanceType& me(GetInstance());
            for (int i = 0; i < DIMENSION; i++)
                me[i] /= num;
            return me;
        }

        inline InstanceType operator*(const T& num) const {
            return InstanceType(GetInstance()) *= num;
        }

        inline InstanceType operator/(const T& num) const {
            return InstanceType(GetInstance()) /= num;
        }

        friend inline InstanceType operator*(const T& num, const InstanceType& v) {
            InstanceType inst(v);
            for (int i = 0; i < DIMENSION; i++)
                inst[i] = num * inst[i];
            return inst;
        }

        friend inline InstanceType operator/(const T& num, const InstanceType& v) {
            InstanceType inst(v);
            for (int i = 0; i < DIMENSION; i++)
                inst[i] = num / inst[i];
            return inst;
        }
    };


    template<typename Derived, typename T, int Dim>
    struct NTGS_API ArrayComparisonOperators : public ArrayMulOperators<Derived, T, Dim> {
        inline bool operator==(const InstanceType& rhs) const {
            const InstanceType& me = GetInstance();
            if (std::is_floating_point<T>::value) {
                for (int i = 0; i < DIMENSION; i++) {
                    if (std::abs(me[i] - rhs[i]) > std::numeric_limits<T>::epsilon())
                        return false;
                }
            }
            else {
                for (int i = 0; i < DIMENSION; i++) {
                    if (!(me[i] == rhs[i]))
                        return false;
                }
            }
            return true;
        }

        inline bool operator!=(const InstanceType& rhs) const {
            return !(GetInstance() == rhs);
        }

        inline bool operator<(const InstanceType& rhs) const {
            for (int i = 0; i < DIMENSION; i++) {
                if (!(me[i] < rhs[i]))
                    return false;
            }
            return true;
        }

        inline bool operator>=(const InstanceType& rhs) const {
            return !(GetInstance() < rhs);
        }

        inline bool operator>(const InstanceType& rhs) const {
            for (int i = 0; i < DIMENSION; i++) {
                if (!(me[i] > rhs[i]))
                    return false;
            }
            return true;
        }

        inline bool operator<=(const InstanceType& rhs) const {
            return !(GetInstance() > rhs);
        }
    };
}