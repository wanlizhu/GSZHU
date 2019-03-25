#pragma once

#include <assert.h>
#include <initializer_list>

namespace NTGS {
    template<typename T, int DIM_MAJOR, int DIM_MINOR> 
    struct Array {
        struct Index {
            inline Index(const int& major, const int& minor) 
                : mMajor(major), mMinor(minor) { assert((int) *this < DIM_MAJOR*DIM_MINOR); }

            inline operator int() const { return DIM_MINOR * mMajor + mMinor; }
            inline T& operator()(T* data) const { return data[(int) *this]; }
            inline const T& operator()(const T* data) const { return data[(int) *this]; }

        private:
            const int& mMajor;
            const int& mMinor;
        };

        static constexpr int LENGTH = DIM_MAJOR * DIM_MINOR;
        T mData[LENGTH];

        // DO NOT DEFINE any kind of constructors or distructors,
        // in order to take Array as a member of the 'union'

        T& operator[](int i) {
            assert(i >= 0 && i < LENGTH);
            return mData[i];
        }

        const T& operator[](int i) const {
            assert(i >= 0 && i < LENGTH);
            return mData[i];
        }

        Array& SetArray(int offset, int size, const T* data) {
            assert(offset + size <= LENGTH);
            for (int i = 0; i < size; i++)
                mData[offset + i] = (data == nullptr ? 0 : data[i]);
            return *this;
        }
    };
}