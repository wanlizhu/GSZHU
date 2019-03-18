#pragma once
#include <NTGS/Core/Config.h>

namespace NTGS {
    template<typename Derived, typename T, int Dim>
    struct NTGS_API ArrayAccess {
        static constexpr int DIMENSION = Dim;
        using ElementType = T;
        using InstanceType = Derived;

        inline T* GetData() { return GetInstance().mData; }
        inline const T* GetData() const { return GetInstance().mData; }
        inline T& operator[](int i) { assert(i >= 0 && i < Dim); return GetData()[i]; }
        inline const T& operator[](int i) const { assert(i >= 0 && i < Dim); return GetData()[i]; }

    protected:
        inline InstanceType& GetInstance() { return static_cast<InstanceType&>(*this); }
        inline const InstanceType& GetInstance() const { return static_cast<const InstanceType&>(*this); }
    };
}