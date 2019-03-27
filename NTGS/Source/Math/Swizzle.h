#pragma once

#include <stdint.h>

namespace NTGS {
    template<template<typename, int> class _VEC, typename T, int... INDICES>
    class Swizzle {
        using VEC = _VEC<T, sizeof...(INDICES)>;
        
    public:
        VEC& operator=(const VEC& rhs) {
            int indices[] = { INDICES... };
            for (int i = 0; i < sizeof...(INDICES); i++) {
                mData[INDICES[i]] = rhs[i];
            }
            return *reinterpret_cast<VEC*>(this);
        }

        operator VEC() const {
            return VEC(mData[INDICES]...);
        }

    private:
        T mData[sizeof...(INDICES)];
    };
}