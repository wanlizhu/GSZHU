#pragma once

#include "CBuilderBase.h"

namespace GSZHU {
    template<typename T>
    CBuilderBase<T>::CBuilderBase() noexcept
        : mImpl(new T) {
    }

    template<typename T>
    template<typename ... ARGS>
    CBuilderBase<T>::CBuilderBase(ARGS&& ... args) noexcept
        : mImpl(new T(std::forward<ARGS>(args)...)) {
    }

    template<typename T>
    CBuilderBase<T>::~CBuilderBase() noexcept {
        delete mImpl;
    }

    template<typename T>
    CBuilderBase<T>::CBuilderBase(CBuilderBase const& rhs) noexcept
        : mImpl(new T(*rhs.mImpl)) {
    }

    template<typename T>
    CBuilderBase<T>& CBuilderBase<T>::operator=(CBuilderBase<T> const& rhs) noexcept {
        *mImpl = *rhs.mImpl;
        return *this;
    }
}